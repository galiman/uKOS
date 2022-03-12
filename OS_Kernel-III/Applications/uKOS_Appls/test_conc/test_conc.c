/*
; test_conc.
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
;			- P0..3: Call the routine concurrently
;				  Display a string
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

typedef	struct	myPack	myPack_t;

struct	myPack {
			char_t		*oName;											// Ptr on the name
			uint32_t	oCounter;										// Initial counter value
			uint32_t	oTime;											// Waiting time
			uint32_t	oLed;											// LedNumber
		};

static	const	myPack_t	aParameter[6] = {
								{ "Process: Aladin          0 ", 0,         100, 0 },
								{ "Process: Marsupilami     1 ", 10000,     200, 1 },
								{ "Process: Fraggle Rock    2 ", 100000,    300, 2 },
								{ "Process: Lupo de Lupis   3 ", 1000000,   400, 3 },
								{ "Process: Muppet show     4 ", 10000000,  500, 4 },
								{ "Process: Max le voyageur 5 ", 100000000, 600, 5 }
							};

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_conc    uKernel test of the concurrent exec.      (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_conc\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess
 *
 * - Common process code called in a concurrent way
 *   - Do not use static variable. This routine has to be reentrant!!
 *
 * - P0..3: - Call the routine concurrently
 *          - Display the string
 *
 */
static	void	aProcess(const void *argument) {
	const	myPack_t	*pack;
			char_t		*name;
			uint32_t	counter, time, led;

	pack = argument;

	name    = pack->oName;
	counter = pack->oCounter;
	time    = pack->oTime;
	led     = pack->oLed;

	while (TRUE) {
		kern_suspendProcess(time);									// Each process is waiting for its specific time
		misc_toggleLed(led);										// Each process toggle its own LED
		iotx_printf(KSYST, "%s Counter: %d\n", name, counter++);	// Print the task string + the counter
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
	volatile	proc_t 	*process_2, *process_3;
	volatile	proc_t 	*process_4, *process_5;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrIden_2[]) =    "Process_User_2";
	LOC_CONST_STRG(aStrIden_3[]) =    "Process_User_3";
	LOC_CONST_STRG(aStrIden_4[]) =    "Process_User_4";
	LOC_CONST_STRG(aStrIden_5[]) =    "Process_User_5";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_3[]) =    "Process user 3.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_4[]) =    "Process user 4.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_5[]) =    "Process user 5.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess, aStrIden_0, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess, aStrIden_1, KDEF0, (KHIGHPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess, aStrIden_2, KDEF0, (KHIGHPRIORITY+3), 0, KPROCNORMAL);
	PROC_SUPV(3, vSpecification_3, aStrText_3, KSZSTACKMIN, aProcess, aStrIden_3, KDEF0, (KHIGHPRIORITY+4), 0, KPROCNORMAL);
	PROC_SUPV(4, vSpecification_4, aStrText_4, KSZSTACKMIN, aProcess, aStrIden_4, KDEF0, (KHIGHPRIORITY+5), 0, KPROCNORMAL);
	PROC_SUPV(5, vSpecification_5, aStrText_5, KSZSTACKMIN, aProcess, aStrIden_5, KDEF0, (KHIGHPRIORITY+6), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, &aParameter[0], &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, &aParameter[1], &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, &aParameter[2], &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_3, &aParameter[3], &process_3) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_4, &aParameter[4], &process_4) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_5, &aParameter[5], &process_5) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

