/*
; sema_memo.
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
;			Create the synchro semaphore "Semaphore 0"
;
;			- P0: Waiting for 5000-ms
;				  Every 1000-ms
;				  Waiting for the semaphore "Semaphore 0"
;				  Toggle LED 0
;				  Display the number of synchros
;
;			- P1: 5 times
;				  Signal an event on the semaphore "Semaphore 0"
;				  P1 will commit a suicide
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

LOC_CONST_STRG(aStrApplication[]) =	"sema_memo    Example of how to use semaphores.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sema_memo\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Waiting for 5000-ms
 *       - Every 1000-ms
 * 			- Waiting for the semaphore "Semaphore 0"
 *			- Toggle LED 0
 *			- Display the number of synchros
 *
 */
static	void	aProcess_0(const void *argument) {
				uint8_t		nbSynchros = 0;
	volatile	sema_t		*semaphore;

	if (kern_createSyncSemaphore("Semaphore 0", &semaphore) != KKERNNOERR) {
		exit(EXIT_FAILURE);
	}

	kern_suspendProcess(5000);

	while (TRUE) {
		kern_waitSemaphore(semaphore, 1, -1);
		misc_toggleLed(0);

		iotx_printf(KSYST, "Synchro semaphore %d\n", ++nbSynchros);
		kern_suspendProcess(1000);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: 5 times
 *       - Signal an event on the semaphore "Semaphore 0"
 *       - P1 will commit a suicide
 *
 */
static	void	aProcess_1(const void *argument) {
				uint8_t		i;
	volatile	sema_t		*semaphore;

	while (kern_getSemaphoreById("Semaphore 0", &semaphore) != KKERNNOERR) { kern_switchFast(); }

	for (i = 0; i < 5; i++) {
		kern_signalSemaphore(semaphore);
	}

	exit(EXIT_SUCCESS);
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

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

