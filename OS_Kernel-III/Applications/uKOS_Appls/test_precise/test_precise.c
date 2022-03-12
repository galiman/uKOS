/*
; test_precise.
; =============

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
;			Launch 1 processes:
;
;			- P0: Initialize a state machine
;				  Every 1000-ms
;				  Display the timer generated by the state machine
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

static	volatile	uint32_t	vCounter = 0;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_precise test of a process with a state machine.   (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_precise\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aStateMachine
 *
 * - Increment a counter every 100-us
 *
 */
static	void	aStateMachine(void) {
	static	uint32_t	vNb = 0;
	static	enum 		{ KSTATE1, KSTATE2, KSTATE3 } vState = KSTATE1;

// State machine
// -------------

// Every 100-us * (1 + 10 + 15) increment the counter
// ---> every 26 * 100 = 2.6-ms increment the counter, so, ~384 increment/s

	switch (vState) {

// State 1 --> State 2

		case KSTATE1: {
			if (++vNb < 1) {                             break; }
			else           { vNb = 0;  vState = KSTATE2; break; }
		}

// State 2 --> State 3

		case KSTATE2: {
			if (++vNb < 10) {                            break; }
			else            { vNb = 0; vState = KSTATE3; break; }
		}

// State 3 --> State 1

		case KSTATE3: {
			if (++vNb < 15) {                                                           break; }
			else            { vNb = 0; vState = KSTATE1; vCounter++; misc_toggleLed(2); break; }
		}
		default: {
			break;
		}
	}
}

/*
 * \brief aProcess 0
 *
 * - P0: Initialize a state machine
 *       Every 1000-ms
 *       Display the timer generated by the state machine
 *
 */
static	void	aProcess_0(const void *argument) {

	kern_runPrecise(100, (void (*)(void))aStateMachine);

	while (TRUE) {
		kern_suspendProcess(1000);

		iotx_printf(KSYST, "%d\n", vCounter);
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
	volatile	proc_t 	*process_0;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}
