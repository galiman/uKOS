/*
; evnt_cale.
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
;			Launch 2 processes:
;
;			- P0: Every 100-ms
;				  mSecond++
;				  if mSecond == 10 Generate an event
;
;			- P1: Display the absolute calendar and time
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

static	atim_t		vAbsoluteTime;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"evnt_cale    Example of how to use events.             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  evnt_cal\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 100-ms
 *  		- mSecond++
 *  		- if mSecond == 10 Generate an event
 *
 */
static	void	aProcess_0(const void *argument) {
				uint8_t		mSeconds = 0;
				uint32_t	eventCalendar;
	volatile	evnt_t		*event;

	if (kern_createEvent("Calendar", &event)    != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_getBitEvent(event, &eventCalendar) != KKERNNOERR) { exit(EXIT_FAILURE); }

	vAbsoluteTime.oSeconds  = 0;
	vAbsoluteTime.oMinutes  = 0;
	vAbsoluteTime.oHours    = 0;
	vAbsoluteTime.oDays     = 2;
	vAbsoluteTime.oWeekDays = KMONDAY;
	vAbsoluteTime.oMonths   = KJANUARY;
	vAbsoluteTime.oYears    = 2017;
	kern_setUnixTime(&vAbsoluteTime);

	while (TRUE) {
		kern_suspendProcess(100);
		if (mSeconds++ >= 9) {
			mSeconds = 0;
			vAbsoluteTime.oAbsolute++;
			kern_signalEvent(eventCalendar, KEVNTSWTC);
		}
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - On event from P0
 *       - Compute and display the absolute time
 *
 */
static	void	aProcess_1(const void *argument) {
				uint32_t	eventTmp, eventCalendar;
	volatile	evnt_t		*event;
	volatile	proc_t 		*process;

// Get the process & the synchro handles

	while (kern_getProcessById("Process_Synchro", &process) != KKERNNOERR) { kern_suspendProcess(1); }
	while (kern_getEventById  ("Calendar",        &event)   != KKERNNOERR) { kern_suspendProcess(1); }
	eventCalendar = event->oBitEvent;

	while (TRUE) {
		eventTmp = eventCalendar;
		kern_waitEvent(&eventTmp, -1);

		kern_getUnixTime(&vAbsoluteTime);

		iotx_printf(KSYST, "Events: %08X, Absolute: %08X YY:%4d MM:%4d DD:%4d WD:%4d HH:%4d mm:%4d ss:%4d\n",
			eventTmp,
			vAbsoluteTime.oAbsolute,
			vAbsoluteTime.oYears, vAbsoluteTime.oMonths, vAbsoluteTime.oDays, vAbsoluteTime.oWeekDays,
			vAbsoluteTime.oHours, vAbsoluteTime.oMinutes, vAbsoluteTime.oSeconds);
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
	LOC_CONST_STRG(aStrText_0[]) =    "Process Synchro.                          (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

