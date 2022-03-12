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
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
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

#define	KFX240FIFO		512
#define KMAXWAIT		5000000

static	volatile	bool_t		vSending = FALSE;
static	volatile	bool_t		vTXSema	 = FALSE;

#if (defined(__WITHSEMA__))
static	volatile	sema_t		*vSeHandleTX[1];
#endif

// Module strings
// ==============

#if (defined(__WITHSEMA__))
LOC_CONST_STRG(aStrSemaphoreTX[]) =	KSEMAPHORE_USB0TX;
#endif

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
	kern_createSyncSemaphore(aStrSemaphoreTX, &vSeHandleTX[0]);
	#endif

// Flush the (RX)

	for (i = 0; i < KFX240FIFO; i++) {
		dummy = *LOCFTDI;
	}
}

/*
 * \brief stub_usb0_configure
 *
 * - Configure the manager
 *
 */
int32_t	stub_usb0_configure(const cnfUsbx_t *configure) {

	INTERRUPTION_OFF;
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

		while (GPIOC->IDR & (1<<BIRQUSBW)) {
			kern_switchFast();
			kern_getTiccount(&time[1]);
			if ((time[1] - time[0]) >= KMAXWAIT) {
				busy = FALSE;
				return (KUSB0TIMEO);
			}
		} 
		*LOCFTDI = *buffer++;
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
	uint32_t	i;

	if (*size == 0)                     { *size = 0; return (KUSB0NOERR); }
	if (GPIOC->IDR & (1<<BIRQUSBR))    	{ *size = 0; return (KUSB0RBUEM); }

// Read the data

	for (i = 0; i < *size; i++) {
		if (GPIOC->IDR & (1<<BIRQUSBR)) { *size = i; return (KUSB0NOERR); }
		*buffer++ = *LOCFTDI;
	}
	return (KUSB0NOERR);
}

/*
 * \brief stub_usb0_flush
 *
 * - Flush the usb0 manager
 *
 */
int32_t	stub_usb0_flush(void) {

	return (KUSB0NOERR);
}

