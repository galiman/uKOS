/*
; uart_xchange.
; =============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			- P0: Waiting for the event KP1READY
;				  Toggle LED 0
;				  Send a buffer on the serial channel
;
;			- P1: As soon as possible
;				  Toggle LED 1
;				  Read & test the message
;				  Generate the event KP1READY
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

#include	<uKOS.h>

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"uart_xchange Example of an uart communication.         (c) EFr-2016";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  uart_xchange\n"
									"Output format: [result]\n\n";

const	uint8_t	aSndMessage[]     = "The quick brown fox jumps over the lazy dog before to be killed by the hunter\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Waiting for the event "process 1"
 * 			- Send a buffer on the serial channel
 *  		- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
				int32_t		status;
				uint32_t	eventTmp, eventProcess1, lnMessage;
	volatile	evnt_t		*event;

	while (kern_getEventById  ("Process 1", &event) != KKERNNOERR) { kern_suspendProcess(1); }
	eventProcess1 = event->oBitEvent;

	lnMessage = sizeof(aSndMessage);

	while (TRUE) {

// Waiting for the event KMSGREAD

		eventTmp = eventProcess1;
		kern_waitEvent(&eventTmp, -1);

// Send the full buffer

		do {
			status = urt4_write(&aSndMessage[0], lnMessage);
			switch (status) {
				case KURT4SEPRO: { iotx_printf(KSYST, "The sender is busy\n");  		 exit(EXIT_FAILURE); }
				case KURT4LNBUB: { iotx_printf(KSYST, "The buffer length is too big\n"); exit(EXIT_FAILURE); }
				case KURT4LNBU0: { iotx_printf(KSYST, "The buffer length is = 0\n"); 	 exit(EXIT_FAILURE); }
				default: {
					break;
				}
			}
			kern_switchFast();
		} while (status != KURT4NOERR);

		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P0: - As soon as possible
 * 			- Read & test the message
 * 			- Generate the event "process 1"
 *  		- Toggle LED 1
 *
 */
static	void	aProcess_1(const void *argument) {
				int32_t		status;
				uint32_t	size, nb, lnMessage;
				uint8_t		*recBuffer;
				uint32_t	eventProcess1;
	volatile	evnt_t		*event;

	if (kern_createEvent("Process 1", &event)   != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_getBitEvent(event, &eventProcess1) != KKERNNOERR) { exit(EXIT_FAILURE); }

	lnMessage = sizeof(aSndMessage);
	recBuffer = (uint8_t *)syos_malloc(KINTERNAL, lnMessage*sizeof(uint8_t));

// Be sure that P0 is scheduled and it is waiting for the event "process 1"
// Generate the first event "process 1" to initiate the transfer

	kern_suspendProcess(100);
	kern_signalEvent(eventProcess1, KEVNTSWTC);

	while (TRUE) {
		size = lnMessage; nb = 0;
		
		do {
			size = lnMessage - nb;
			do {
				status = urt4_read(&recBuffer[nb], &size);
				switch (status) {
					case KCOMMRBFUL: { iotx_printf(KSYST, "The receiver buffer is full\n"); exit(EXIT_FAILURE); }
					case KURT4EROVR: { iotx_printf(KSYST, "Overrun error\n"); 				exit(EXIT_FAILURE); }
					case KURT4ERNOI: { iotx_printf(KSYST, "Noise error\n"); 				exit(EXIT_FAILURE); }
					case KURT4ERFRA: { iotx_printf(KSYST, "Framing error\n"); 				exit(EXIT_FAILURE); }
					case KURT4ERPAR: { iotx_printf(KSYST, "Parity error\n"); 				exit(EXIT_FAILURE); }
					default: {
						break;
					}
				}
				kern_switchFast();
			} while (status == KURT3RBUEM);

			nb = nb + size;
		} while (nb < lnMessage);

		iotx_printf(KSYST,"%s", &recBuffer[0]);

// Message read
// Generate the event "process 1" to start the next transfer

		kern_signalEvent(eventProcess1, KEVNTSWTC);
		misc_toggleLed(1);
	}
}

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int	main(void) {
				cnfUrtx_t	configureURTx;
	volatile	proc_t 		*process_0, *process_1;

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBD921600;
	configureURTx.oKernSync = 0;
	urt4_configure(&configureURTx);

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_Xchange 0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_Xchange 1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process Xchange 0.                        (c) EFr-2016";
	LOC_CONST_STRG(aStrText_1[]) =    "Process Xchange 1.                        (c) EFr-2016";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1),   0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) exit(EXIT_FAILURE);
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) exit(EXIT_FAILURE);
	return (EXIT_SUCCESS_OS);
}

