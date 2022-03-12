/*
; sema_driv.
; ==========

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
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			Create the synchro semaphore "urt0 - RX char"
;			Configure the urt0 manager
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Waiting for the semaphore "urt0 - RX char" (5 chars)
;				  Display a string on the urt0
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

LOC_CONST_STRG(aStrApplication[]) =	"sema_driv    Example of how to use semaphores.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sema_driv\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);

		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Waiting for the semaphore "urt0 - RX char" (5 events)
 *   		- Display a string or a timeout error message
 *
 */
static	void	aProcess_1(const void *argument) {
				int32_t		status;
				uint32_t	size;
				uint8_t		buffer[5+1];
	volatile	sema_t		*semaphore;

	while (kern_getSemaphoreById(KSEMAPHORE_URT0RX, &semaphore) != KKERNNOERR) { kern_switchFast(); }

	while (TRUE) {
		status = kern_waitSemaphore(semaphore, 5, -1);
		if (status != KKERNNOERR) {
			iotx_printf(KSYST,"Error P1\n");
		}
		else {
			size = 5;
			buffer[5] = '\0';
			status = urt0_read(&buffer[0], &size);
			if ((status != KURT0NOERR) || (size != 5)) {
				iotx_printf(KSYST,"Problem! %d status = %X\n", size, -(status & 0x0F));
			}
			iotx_printf(KSYST,"\nP1: %d Buffer read %s\n", size, &buffer[0]);
			kern_switchFast();
		}
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
int		main(void) {
				cnfUrtx_t	configureURTx;
	volatile	proc_t 		*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),  0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KURT0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBDDEFAULT;
	configureURTx.oKernSync = (1<<BRXSEMA);
	urt0_configure(&configureURTx);
	urt0_flush();

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }

// Do not return with "EXIT_SUCCESS_OS" because the "console" uses the KURT0

	return (EXIT_SUCCESS);
}

