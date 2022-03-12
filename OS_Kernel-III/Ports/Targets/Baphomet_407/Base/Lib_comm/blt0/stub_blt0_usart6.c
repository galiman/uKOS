/*
; stub_blt0_usart6.
; =================

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the connection of the "blt0" manager to the usart6 device.
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

#define	__BLT0_SEMA_RX__
#define	__BLT0_SEMA_TX__

#define	KFREQUENCY_USART6		KFREQUENCY_APB2

#define	KBLT0LNSNDBUF			64
#define	KBLT0LNRECBUF			128

#define	DIV(ck, baudrate)		((uint32_t)(ck/(baudrate * 16)))
#define	REM(ck, baudrate)		((uint32_t)((((ck/(baudrate * 16.0)) - DIV(ck, baudrate)) * 16.0) + 0.5))
#define	BAUDRATE(ck, baudrate)	(DIV(ck, baudrate)<<4 | REM(ck, baudrate))

extern				void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
extern	volatile	uint16_t	vTimeException;
static	volatile	bool_t		vSending = FALSE;
static	volatile	bool_t		vRXSema  = FALSE;
static	volatile	bool_t		vTXSema  = FALSE;
static	volatile	uint8_t		vSndBuffer[KBLT0LNSNDBUF];
static	volatile	uint8_t		vRecBuffer[KBLT0LNRECBUF];
static	volatile	uint8_t		*vRRecBuffer;
static	volatile	uint8_t		*vWRecBuffer;
static	volatile	uint8_t		*vRSndBuffer;
static	volatile	uint8_t		*vWSndBuffer;
static	volatile	int32_t		vStatus = KBLT0NOERR;

#if (defined(__WITHSEMA__))
static	volatile	sema_t		*vSeHandleRX[1];
static	volatile	sema_t		*vSeHandleTX[1];
#endif

#if (defined(__WITHSEMA__))
LOC_CONST_STRG(aStrSemaphoreRX[]) =	KSEMAPHORE_BLT0RX;
LOC_CONST_STRG(aStrSemaphoreTX[]) =	KSEMAPHORE_BLT0TX;
#endif

// Prototypes
// ==========

static	void	aUSART6_IRQHandler(void);
static	void	_initPan1322(void);

/*
 * \brief stub_blt0_init
 *
 * - Initialize the manager
 *
 */
void	stub_blt0_init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

// Create the semaphores

	#if (defined(__WITHSEMA__))
	kern_createSyncSemaphore(aStrSemaphoreRX, &vSeHandleRX[0]);
	kern_createSyncSemaphore(aStrSemaphoreTX, &vSeHandleTX[0]);
	#endif

// Initialize the circular buffers (snd) and (rec)

	vRSndBuffer = vWSndBuffer = vSndBuffer;
	vRRecBuffer = vWRecBuffer = vRecBuffer;

// Initialize the UART (Rx and Tx) and set it to 8-bits, 1-stop-bits, default baudrate and no parity

	vExce_indIntVectors[USART6_IRQn] = aUSART6_IRQHandler;

	NVIC->IP[USART6_IRQn] = (KINT_LEVEL_COMM<<4);
	NVIC->ISER[2]        |= (1<<(USART6_IRQn & 0x1F));

    USART6->BRR	 = BAUDRATE(KFREQUENCY_USART6, 115200);
	USART6->CR1	 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
	USART6->SR  &= ~USART_SR_RXNE;
	USART6->SR  &= ~USART_SR_TXE;
	USART6->CR1 |= USART_CR1_RXNEIE;
}

/*
 * \brief stub_blt0_configure
 *
 * - Configure the manager
 *
 */
int32_t	stub_blt0_configure(const cnfUrtx_t *configure) {

	INTERRUPTION_OFF;
	switch (configure->oNBits) {
		case K7BIT:
		default:
		case K8BIT: { USART6->CR1 &= ~USART_CR1_M; break; }
	}

	USART6->CR1 &= ~(USART_CR1_PCE | USART_CR1_PS);
	switch (configure->oParity) {
		case KEVEN: { USART6->CR1 |=  USART_CR1_PCE;                break; }
		case KODD:  { USART6->CR1 |=  USART_CR1_PCE | USART_CR1_PS; break; }
		default:
		case KNONE: { 											    break; }
	}

	switch (configure->oBaudRate) {
		case KBDDEFAULT:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 115200);  break; }
		case KBD2400:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 2400);    break; }
		case KBD4800:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 4800);    break; }
		case KBD9600:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 9600);    break; }
		case KBD19200:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 19200);   break; }
		case KBD38400:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 38400);   break; }
		case KBD57600:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 57600);   break; }
		case KBD115200:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 115200);  break; }
		case KBD230400:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 230400);  break; }
		case KBD460800:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 460800);  break; }
		case KBD500000:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 500000);  break; }
		case KBD921600:		{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 921600);  break; }
		case KBD1000000:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 1000000); break; }
		case KBD1500000:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 1500000); break; }
		case KBD1843200:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 1843200); break; }
		case KBD2000000:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 2000000); break; }
		case KBD2500000:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 2500000); break; }
		case KBD3000000:	{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 3000000); break; }
		default:			{ USART6->BRR = BAUDRATE(KFREQUENCY_USART6, 115200);  break; }
	}

	vRXSema = ((configure->oKernSync & (1<<BRXSEMA)) != 0) ? TRUE : FALSE;
	vTXSema = ((configure->oKernSync & (1<<BTXSEMA)) != 0) ? TRUE : FALSE;
	INTERRUPTION_RESTORED;

	_initPan1322();
	return (KBLT0NOERR);
}

/*
 * \brief stub_blt0_write
 *
 * - Write a buffer
 *
 */
int32_t	stub_blt0_write(const uint8_t *buffer, uint32_t size) {
			uint32_t	i, newSize;
	static	bool_t		terminate = FALSE, busy = FALSE;

	INTERRUPTION_OFF;
	if (GPIOC->IDR & (1<<BCTS)) { RETURN_INT_RESTORED(KBLT0SEPRO); }
	if ((vSending) || (busy))   { RETURN_INT_RESTORED(KBLT0SEPRO); }
	if (size == 0) 			    { RETURN_INT_RESTORED(KBLT0LNBU0); }

	busy = TRUE;
	INTERRUPTION_RESTORED;

	while (!terminate) {
		while (vSending) kern_switchFast();

		if (size > KBLT0LNSNDBUF) { newSize = KBLT0LNSNDBUF; size -= KBLT0LNSNDBUF; }
		else					  { newSize = size; terminate = TRUE; 				}

// Copy the input buffer into the send one and start the transfer
// Enable the TX interruption

		vSending = TRUE;
		for (i = 0; i < newSize; i++) {
			vSndBuffer[i] = buffer[i];
		}

		vWSndBuffer = vSndBuffer + newSize;
		vRSndBuffer = vSndBuffer;
		buffer += newSize;

		USART6->CR1 |= USART_CR1_TXEIE;
	}
	busy = FALSE;
	terminate = FALSE;
	return (KBLT0NOERR);
}

/*
 * \brief stub_blt0_read
 *
 * - Read n x bytes
 *
 */
int32_t	stub_blt0_read(uint8_t *buffer, uint32_t *size) {
	int32_t		status;
	uint32_t	i;

	INTERRUPTION_OFF;
	status  = vStatus;
	vStatus = KBLT0NOERR;

// Interruption problem: buffer full, Framing error, Overrun error, ..
// In this case, we return 0-bytes read with the corresponding error.
// The Error is then cancelled for the next time.

	if (status != KBLT0NOERR)			{ *size = 0; RETURN_INT_RESTORED(status); 	  }
	if (*size == 0) 					{ *size = 0; RETURN_INT_RESTORED(KBLT0NOERR); }
	if (vRRecBuffer == vWRecBuffer)     { *size = 0; RETURN_INT_RESTORED(KBLT0RBUEM); }

	for (i = 0; i < *size; i++) {
		if (vRRecBuffer == vWRecBuffer) { *size = i; RETURN_INT_RESTORED(KBLT0NOERR); }

		*buffer++ = *vRRecBuffer++;
		if (vRRecBuffer == (vRecBuffer + KBLT0LNRECBUF)) {
			vRRecBuffer = vRecBuffer;
		}
	}
	RETURN_INT_RESTORED(status);
}

/*
 * \brief stub_blt0_flush
 *
 * - Flush the blt0 manager
 *
 */
int32_t	stub_blt0_flush(void) {
	volatile	sema_t	*semaphore;

// Initialize the circular buffers (rec) and restart the associated semaphore

	INTERRUPTION_OFF;
	vRRecBuffer = vWRecBuffer = vRecBuffer;
	kern_getSemaphoreById(KSEMAPHORE_BLT0RX, &semaphore);
	kern_restartSemaphore(semaphore);
	RETURN_INT_RESTORED(KBLT0NOERR);
}

// Local routines
// ==============

/*
 * \brief aUSART6_IRQHandler
 *
 * - Interruption RX-TX
 *
 */
static	void	aUSART6_IRQHandler(void) {
	volatile	uint32_t	IIR;

	IIR = USART6->SR;
    if ((IIR & USART_SR_RXNE) != 0) {

// RX interruption

		USART6->SR &= ~USART_SR_RXNE;
		if ((IIR & USART_SR_ORE) != 0) { vStatus = KBLT0EROVR; }
		if ((IIR & USART_SR_FE)  != 0) { vStatus = KBLT0ERFRA; }
		if ((IIR & USART_SR_PE)  != 0) { vStatus = KBLT0ERFRA; }

// Read the data and signal the event

		*vWRecBuffer++ = (uint8_t)USART6->DR;
		if (vWRecBuffer == vRRecBuffer) {
			vStatus = KBLT0SBFUL;
		}
		if (vWRecBuffer == (vRecBuffer + KBLT0LNRECBUF)) {
			vWRecBuffer = vRecBuffer;
		}

		#if (defined(__WITHSEMA__) && defined(__BLT0_SEMA_RX__))
		if (vRXSema) kern_signalSemaphore(vSeHandleRX[0]);
		#endif
	}

    if (((IIR & USART_SR_TXE) && (USART6->CR1 & USART_CR1_TXEIE)) != 0) {

// TX interruption

		USART6->SR &= ~USART_SR_TXE;
		USART6->DR = (uint16_t)*vRSndBuffer++;
		if (vRSndBuffer == vWSndBuffer) {

// Terminated; disable the Tx interruption

			USART6->CR1 &= ~USART_CR1_TXEIE;
			vSending = FALSE;

			#if (defined(__WITHSEMA__) && defined(__BLT0_SEMA_TX__))
			if (vTXSema) { kern_signalSemaphore(vSeHandleTX[0]); }
			#endif
		}

// Continue to send data

	}
}

/*
 * \brief _initPan1322
 *
 * - Initialize the PAN1322 Bluetooth chip
 *
 */
static	void	_recAnswer(char_t *answer) {
	char_t		recBuffer[40];
	uint32_t	i, size, nbElements;

	strg_getBufferSize((char_t *)answer, &size);
	for (i = 0; i < size; i++) {										
		do {															
			kern_suspendProcess(10);
			nbElements = 1;
			blt0_read((uint8_t *)&recBuffer[i], &nbElements);
		} while (nbElements != 1);
	}
}

static	void	_sndCommand(char_t *command, uint32_t wait) {
	uint32_t	size;

	strg_getBufferSize((char_t *)command, &size);
	blt0_write((uint8_t *)command, size);
	kern_suspendProcess(wait);
}

static	void	_initPan1322(void) {

	_sndCommand("AT+JSEC=4,1,04,1111,0,0\r\n",					100);	//
	_recAnswer("OK\r\n");												// Enable security
	_sndCommand("AT+JDIS=3\r\n",								100);	//
	_recAnswer("OK\r\n");												// Discoverable
	_sndCommand("AT+JRLS=04,08,1101,Baphomet,01,000000\r\n",	100);	//
	_recAnswer("OK\r\n");												// Register Local Service
	_sndCommand("AT+JAAC=1\r\n",								100);	//
	_recAnswer("OK\r\n");												// Auto Accept Connection requests
	_sndCommand("AT+JSLN=04,uKOS\r\n",							100);	//
	_recAnswer("OK\r\n");												// Set Local device friendly Name
	_sndCommand("AT+JSCR\r\n",									100);	//
	_recAnswer("OK\r\n");												// Stream connection

	_recAnswer("+RSLE\r\n");											// Waiting for the pair
	_recAnswer("+RCOI=109ADDB8CE30\r\n");								//
	_recAnswer("+RCCRCNF=500,1101,0\r\n");								// Waiting for the terminal connection

	kern_suspendProcess(5000);
}

