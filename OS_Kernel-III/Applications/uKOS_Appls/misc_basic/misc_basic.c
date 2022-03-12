/*
; misc_basic.
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
;			Launch 2 process:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Every 1000-ms
;				  print a message
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

#define	__VERBOSE__

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_basic   Example of how to create a process.       (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_basic\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	void	aProcess_1(const void *argument);

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
	static const  uint32_t	paramenters_1[] = { 0x1234, 0x5678 };	
	volatile		proc_t 	*process_1;

	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

	while (TRUE) {
		misc_toggleLed(0);

		PROC_SUPV_STACKMALLOC(1, KINTERNAL, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+3), 0, KPROCNORMAL);

		if (kern_createProcess(&vSpecification_1, paramenters_1, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
		kern_suspendProcess(5000);

		kern_killProcess(process_1);
		kern_suspendProcess(5000);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Every 1000-ms
 *  		- print a message
 *
 */
static	void	aProcess_1(const void *argument) {
				uint32_t	counter = 0;
	const		uint32_t	*paramenters;

	paramenters = argument;
	while (TRUE) {
		kern_suspendProcess(1000);
		iotx_printf(KSYST, "Counter = %d, I am the process 1 %08X  %08X\n", counter++, *(paramenters + 0), *(paramenters + 1));
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

	#if (defined(__VERBOSE__))

// Process 0: normal process with static stack allocation
// ------------------------------------------------------

	static			uint32_t 	vStackSup_0[KSZSTACKMIN] __attribute__ ((aligned (8)));

	static	const	spec_t		vSpecification_0 = {
		.oIdentifier	= "Process_User_0",
		.oText  		= "Process user 0.                           (c) EFr-2017",
		.oCode  		= (void (*)(const void *argument))aProcess_0,
		.oStkSupBegin	= &vStackSup_0[0],
		.oStkSup 		= &vStackSup_0[KSZSTACKMIN-8],
		.oSzStk 		= KSZSTACKMIN-8,
		.oStkUsr 		= NULL,
		.oStackMode		= KSTKSTATIC,
		.oPriority 		= 3,
		.oIdPack 		= 0,
		.oCommManager 	= KDEF0,
		.oKind 			= KPROCNORMAL,
		.oMode 			= KSUPERVISOR
	};
	#else

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+3), 0, KPROCNORMAL);
	#endif

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

