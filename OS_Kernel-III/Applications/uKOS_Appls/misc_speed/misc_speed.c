/*
; misc_speed.
; ===========

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
;			- P0: Every 1000-ms
;				  Toggle LED 0
;				  Stop the time
;				  time = stop - start
;				  Display the speed of the test (time)
;				  synchro
;
;			- P1: Waiting for the P0 synchro
;				  Start the time
;				  Increment a counter
;
;			Some results: (bigger values are better)
;			Compiler used: Cortex gcc-4.8.4, icyflex1 gcc-4.6.4
;
;			Baphomet_746	216-MHz		External ram (SDRAM)	174062 (with instruction cache)
;			Discovery_407	160-MHz		Internal ram			111846
;			Discovery_429	180-MHz		External ram (SDRAM)	29740
;			Baphomet_407	160-MHz		External ram (PSRAM)	10496
;			Baphomet_429	180-MHz		External ram (SDRAM)	29740
;			VIP_429			180-MHz		External ram (SDRAM)	28244
;			CSEM_icyamr		33/66-MHz	Internal ram			43579
;			LAP_icycam_103	72-MHz		External ram (PSRAM)	4174
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

static				uint64_t	vTime[2];
static	volatile	uint64_t	vCounter;
static	volatile	bool_t		vEnd = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"userapp      Test of the CPU/memory performance.       (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  userapp\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *			- Toggle LED 0
 *			- Stop the time
 *			- time = stop - start
 *			- Display the speed of the test (time)
 *			- Generate an event "process 0"
 *
 */
static	void	aProcess_0(const void *argument) {
				uint64_t	counter, time;
				uint32_t	eventProcess0;
	volatile	evnt_t		*event;

	if (kern_createEvent("Process 0", &event)   != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_getBitEvent(event, &eventProcess0) != KKERNNOERR) { exit(EXIT_FAILURE); }

	iotx_printf(KSYST, "Target: "KTARGET"\n");

	while (TRUE) {
		kern_suspendProcess(1000);
		misc_toggleLed(0);

// Prepare to synchronize the process 1

		vEnd = FALSE;
		kern_getTiccount(&vTime[0]);
		kern_signalEvent(eventProcess0, KEVNTSWTC);

// Run the process 1
// Run the process 1
// Run the process 1
// Run the process 1
// ...

		kern_getTiccount(&vTime[1]);

		counter = vCounter;
		time    = vTime[1]-vTime[0];
		vEnd    = TRUE;

		iotx_printf(KSYST, "Counter = %ld, Time = %ld\n", counter, time);
		kern_switchFast();

// Run the process 1
// ...
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Waiting for the "process 0" event
 *       	- Start the time
 *			- Increment a counter
 *
 */
static	void	aProcess_1(const void *argument) {
				uint32_t	eventTmp, eventProcess0;
	volatile	evnt_t		*event;

	while (kern_getEventById  ("Process 0", &event) != KKERNNOERR) { kern_suspendProcess(1); }
	eventProcess0 = event->oBitEvent;

	while (TRUE) {
		eventTmp = eventProcess0;
		kern_waitEvent(&eventTmp, -1);

		vCounter = 0;
		do { vCounter++; } while (!vEnd);
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
	volatile	proc_t 	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_Synchro";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process_Synchro.                          (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KHIGHPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

