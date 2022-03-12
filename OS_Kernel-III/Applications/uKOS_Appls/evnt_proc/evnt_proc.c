/*
; evnt_proc.
; ==========

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
;			Launch 4 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;				  Generate an event P0
;
;			- P1: Every 1234-ms
;				  Toggle LED 1
;				  Generate an event P1
;
;			- P2: Waiting for the event P0 | P1 under timeout
;				  Display a string or a timeout error message
;
;			- P3: Waiting for the event P0 under timeout
;				  Display a string or a timeout error message
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

LOC_CONST_STRG(aStrApplication[]) =	"evnt_proc    Example of how to use events.             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  evnt_proc\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- Toggle LED 0
 * 			- Generate the event "process 0"
 *
 */
static	void	aProcess_0(const void *argument) {
				uint32_t	eventProcess;
	volatile	evnt_t		*event;

	if (kern_createEvent("Process 0", &event)  != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_getBitEvent(event, &eventProcess) != KKERNNOERR) { exit(EXIT_FAILURE); }

	while (TRUE) {
		kern_suspendProcess(1000);
		kern_signalEvent(eventProcess, KEVNTSWTC);
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P0: - Every 1234-ms
 *  		- Toggle LED 1
 * 			- Generate the event "process 1"
 *
 */
static	void	aProcess_1(const void *argument) {
				uint32_t	eventProcess;
	volatile	evnt_t		*event;

	if (kern_createEvent("Process 1", &event)  != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_getBitEvent(event, &eventProcess) != KKERNNOERR) { exit(EXIT_FAILURE); }

	while (TRUE) {
		kern_suspendProcess(1234);
		kern_signalEvent(eventProcess, KEVNTSWTC);
		misc_toggleLed(1);
	}
}

/*
 * \brief aProcess 2
 *
 * - P2: - Waiting for the event "process 0" & "process 1" under timeout
 * 			- Display a string or a timeout error message
 *
 */
static	void	aProcess_2(const void *argument) {
				int32_t		status;
				uint32_t	eventTmp, eventProcess0, eventProcess1;
	volatile	evnt_t		*event;

// Get the events

	while (kern_getEventById  ("Process 0", &event) != KKERNNOERR) { kern_suspendProcess(1); }
	eventProcess0 = event->oBitEvent;

	while (kern_getEventById  ("Process 1", &event) != KKERNNOERR) { kern_suspendProcess(1); }
	eventProcess1 = event->oBitEvent;

	while (TRUE) {
		eventTmp = eventProcess0 | eventProcess1;
		status = kern_waitEvent(&eventTmp, 2000);

		(status == KKERNTIMEO) ? iotx_printf(KSYST, "P2 - Timeout error! %08X\n", eventTmp) : iotx_printf(KSYST, "P2 - Events %08X\n", eventTmp);
	}
}

/*
 * \brief aProcess 3
 *
 * - P3: - Waiting for the event "process 1" under timeout
 * 			- Display a string or a timeout error message
 *
 */
static	void	aProcess_3(const void *argument) {
				int32_t		status;
				uint32_t	eventTmp, eventProcess1;
	volatile	evnt_t		*event;

	while (kern_getEventById  ("Process 1", &event) != KKERNNOERR) { kern_suspendProcess(1); }
	eventProcess1 = event->oBitEvent;

	while (TRUE) {
		eventTmp = eventProcess1;
		status = kern_waitEvent(&eventTmp, 2000);

		(status == KKERNTIMEO) ? iotx_printf(KSYST, "P3 - Timeout error!\n") : iotx_printf(KSYST, "P3 - Events %08X\n", eventTmp);
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
	volatile	proc_t 	*process_0, *process_1, *process_2, *process_3;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_Synchro 0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_Synchro 1";
	LOC_CONST_STRG(aStrIden_2[]) =    "Process_User_2";
	LOC_CONST_STRG(aStrIden_3[]) =    "Process_User_3";
	LOC_CONST_STRG(aStrText_0[]) =    "Process Synchro 0.                        (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process Synchro 1.                        (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_3[]) =    "Process user 3.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess_2, aStrIden_2, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(3, vSpecification_3, aStrText_3, KSZSTACKMIN, aProcess_3, aStrIden_3, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, NULL, &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_3, NULL, &process_3) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

