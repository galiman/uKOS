/*
; stub_usb0.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
; Version:	1.1
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 184                  $:  Revision of last commit
; $Date:: 2017-07-17 10:50:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the "usb0" manager module.
;
;   (c) 1992-2017, Franzi Edo.
;   --------------------------
;                                              __ ______  _____
;   Franzi Edo.                         __  __/ //_/ __ \/ ___/
;   5-Route de Cheseaux                / / / / ,< / / / /\__ \
;   CH 1400 Cheseaux-Noréaz           / /_/ / /| / /_/ /___/ /
;                                     \__,_/_/ |_\____//____/
;   edo.franzi@ukos.ch
;
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU Affero General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU Affero General Public License for more details.
;
;   You should have received a copy of the GNU Affero General Public License
;   along with this program. If not, see <http://www.gnu.org/licenses/>.
;
;------------------------------------------------------------------------
*/

#include 	<uKOS.h>

#define	__USB0_SEMA_TX__
#define	__USB0_SEMA_RX__

#define	KLNFT2232FIFO	4096
#define	KUSB0LNRECBUF	128
#define KUSB0NBREADBU	32
#define KMAXWAIT		5000000

static	volatile	bool_t		vSending      = FALSE;
static	volatile	bool_t		vRXSema	      = FALSE;
static	volatile	bool_t		vTXSema	      = FALSE;
static	volatile	bool_t		vRecBufferFul = FALSE;
static	volatile	int32_t		vStatus 	  = KUSB0NOERR;
static	volatile	uint8_t		vRecBuffer[KUSB0LNRECBUF];
static	volatile	uint8_t		*vRRecBuffer;
static	volatile	uint8_t		*vWRecBuffer;

#if (defined(__WITHSEMA__))
static	volatile	sema_t		*vSeHandleRX[1];
static	volatile	sema_t		*vSeHandleTX[1];
#endif

// Module strings
// ==============

#if (defined(__WITHSEMA__))
LOC_CONST_STRG(aStrSemaphoreRX[]) =	KSEMAPHORE_USB0RX;
LOC_CONST_STRG(aStrSemaphoreTX[]) =	KSEMAPHORE_USB0TX;
#endif

// Prototypes
// ==========

extern	void	pbus_read(uint16_t address, uint8_t *data);
extern	void	pbus_write(uint16_t address, uint8_t data);
extern	void	handle_usb0_interruptionRX(void);

/*
 * \brief stub_usb0_init
 *
 * - Initialize the manager
 *
 */
void	stub_usb0_init(void) {
	uint8_t		dummy __attribute__ ((unused));
	uint16_t	i;

// Create the semaphores

	#if (defined(__WITHSEMA__))
	kern_createSyncSemaphore(aStrSemaphoreRX, &vSeHandleRX[0]);
	kern_createSyncSemaphore(aStrSemaphoreTX, &vSeHandleTX[0]);
	#endif

// Initialize the circular buffers (rec)

	vRRecBuffer = vWRecBuffer = vRecBuffer;

// Activate the USB0 (Rx) interruption

	*GPIO_IRQ_CONFIG_2 &= ~(1<<BIRQUSBR);

	*RQCTRL_IRQ_VEC_1      = (uint32_t)(handle_usb0_interruptionRX);
	*RQCTRL_SOURCE_EN_W1S  = (1<<BGPIO0); 								
	*RQCTRL_IRQ_CONFIG_0  |= (KINT_LEVEL_COMM<<BPRIREQ01); 				
	*RQCTRL_IRQ_LEVEL_EN   = KINT_SYS_LEVEL;

// Flush the (RX)

	for (i = 0; i < KLNFT2232FIFO; i++) {
		pbus_read(LOCFTDI, &dummy);
	}

	*GPIO_IRQ0_EN |= (1<<BIRQUSBR);
}

/*
 * \brief stub_usb0_configure
 *
 * - Configure the manager
 *
 */
int32_t	stub_usb0_configure(const cnfUsbx_t *configure) {

	INTERRUPTION_OFF;
	vRXSema = ((configure->oKernSync & (1<<BRXSEMA)) != 0) ? TRUE : FALSE;
	vTXSema = ((configure->oKernSync & (1<<BTXSEMA)) != 0) ? TRUE : FALSE;
	RETURN_INT_RESTORED(KUSB0NOERR);
}

/*
 * \brief stub_usb0_write
 *
 * - Write a buffer
 *
 */
int32_t	stub_usb0_write(const uint8_t *buffer, uint32_t size) {
			uint64_t	time[2];
	static	bool_t		busy = FALSE;

	INTERRUPTION_OFF;
	if (busy)      { RETURN_INT_RESTORED(KUSB0SEPRO); }
	if (size == 0) { RETURN_INT_RESTORED(KUSB0LNBU0); }

	busy = TRUE;
	INTERRUPTION_RESTORED;

// Send the data

	do {
		kern_getTiccount(&time[0]);

		while (*GPIO_DATAIN & (1<<BIRQUSBW)) {
			kern_switchFast();
			kern_getTiccount(&time[1]);
			if ((time[1] - time[0]) >= KMAXWAIT) {
				busy = FALSE;
				return (KUSB0TIMEO);
			}
		} 

		pbus_write(LOCFTDI, *buffer++);
	} while (--size != 0);

// Signal end of the message

	#if (defined(__WITHSEMA__) && defined(__USB0_SEMA_TX__))
	if (vTXSema) { kern_signalSemaphore(vSeHandleTX[0]); }
	#endif

	busy = FALSE;
	return (KUSB0NOERR);
}

/*
 * \brief stub_usb0_read
 *
 * - Read n x bytes
 *   - Try to read a buffer from the buffer usb
 *     In case the buffer is full (with the transfers blocked)
 *     then, it is necessary to read at least KUSB0NBREADBU Bytes before
 *     to enable again the interruptions
 *
 */
int32_t	stub_usb0_read(uint8_t *buffer, uint32_t *size) {
			int32_t		status;
			uint32_t	i;
	static	uint8_t		nbChars = 0;

	INTERRUPTION_OFF;
	if (*size == 0)                     { *size = 0; RETURN_INT_RESTORED(KUSB0NOERR); }
	if (vRRecBuffer == vWRecBuffer)     { *size = 0; RETURN_INT_RESTORED(KUSB0RBUEM); }

	for (i = 0; i < *size; i++) {
		if (vRRecBuffer == vWRecBuffer) { *size = i; RETURN_INT_RESTORED(KUSB0NOERR); }

		*buffer++ = *vRRecBuffer++;
		if (vRRecBuffer == (vRecBuffer + KUSB0LNRECBUF)) {
			vRRecBuffer = vRecBuffer;
		}
		if (vRecBufferFul) {
			if (nbChars++ >= KUSB0NBREADBU) {
				nbChars = 0;
				vRecBufferFul = FALSE;

				*GPIO_IRQ0_EN |= (1<<BIRQUSBR);
			}
		}
	}
	status = vStatus; vStatus = KUSB0NOERR;
	RETURN_INT_RESTORED(status);
}

/*
 * \brief stub_usb0_flush
 *
 * - Flush the usb0 manager
 *
 */
int32_t	stub_usb0_flush(void) {
	volatile	sema_t	*semaphore;

// Initialize the circular buffers (rec) and restart the associated semaphore

	INTERRUPTION_OFF;
	vRRecBuffer = vWRecBuffer = vRecBuffer;
	kern_getSemaphoreById(KSEMAPHORE_USB0RX, &semaphore);
	kern_restartSemaphore(semaphore);
	RETURN_INT_RESTORED(KUSB0NOERR);
}

// Local routines
// ==============

/*
 * \brief stub_usb0_interruptionRX
 *
 * - Interruption function
 *
 *            Case a                  case b
 *
 * \note      +-------+               +-------+ KLN = 7
 * \note      I 6     I               I 6     I
 * \note      +-------+               +-------+
 * \note      I 5     I< R            I 5     I
 * \note      +-------+               +-------+
 * \note      I 4     I               I 4     I< W
 * \note      +-------+               +-------+
 * \note   W >I 3     I               I 3     I
 * \note      +-------+               +-------+
 * \note      I 2     I            R >I 2     I
 * \note      +-------+               +-------+
 * \note      I 1     I               I 1     I
 * \note      +-------+               +-------+
 * \note      I 0     I               I 0     I
 * \note  Buf +-------+           Buf +-------+
 *
 *  - if ((R - W) > 0) nbPlaces = (R - W)                     = (R - W)       // case a
 *  - else             nbPlaces = (Buf + KLN - W) + (R - Buf) = (R - W + KLN) // case b
 *
 */
#pragma	GCC diagnostic	push
#pragma	GCC diagnostic	ignored	"-Wcast-qual"
void	stub_usb0_interruptionRX(void) {
	int32_t		nbPlaces;

	*GPIO_IRQ_STATUS = (1<<BIRQUSBR);

// Check if there is place inside the buffer
// If only 1 position is available, then consider a buffer full condition
// The USB driver of the host computer should stops to send data

	INTERRUPTION_OFF;
	nbPlaces = (int32_t)(vRRecBuffer - vWRecBuffer);
	if (nbPlaces <= 0) nbPlaces += KUSB0LNRECBUF;
	if (nbPlaces == 1) {
		vRecBufferFul = TRUE;
		*GPIO_IRQ0_EN &= ~(1<<BIRQUSBR);
		INTERRUPTION_RESTORED;
		return;
	}

// Read the data and signal the event

	pbus_read(LOCFTDI, (uint8_t *)vWRecBuffer++);

	if (vWRecBuffer == (vRecBuffer + KUSB0LNRECBUF)) vWRecBuffer = vRecBuffer;
	INTERRUPTION_RESTORED;

// Signal an available byte

	#if (defined(__WITHSEMA__) && defined(__USB0_SEMA_RX__))
	if (vRXSema) { kern_signalSemaphore(vSeHandleRX[0]); }
	#endif
}
#pragma	GCC	diagnostic	pop

