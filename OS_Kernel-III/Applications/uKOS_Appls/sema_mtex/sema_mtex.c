/*
; sema_mtex.
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
;			Launch 3 processes:
;
;			Create the synchro semaphore "Semaphore 0"
;
;			- P0: Toggle LED 0
;				  lock a semaphore
;				  Waiting for 10-ms
;				  Generate a data structure A and print it
;				  Release a semaphore
;
;			- P1: Toggle LED 1
;				  lock a semaphore
;				  Waiting for 100-ms
;				  Generate a data structure A and print it
;				  Release a semaphore
;
;			- P2: Toggle LED 2
;				  lock a semaphore
;				  Waiting for 1000-ms
;				  Generate a data structure C and print it
;				  Release a semaphore
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

typedef	struct	strt	strt_t;

struct	strt {
			char_t		oProcess;
			uint32_t	oField;
			uint32_t	oCounter;
		};

static	volatile	strt_t	vData;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sema_mtex    Example of how to use semaphores.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sema_mtex\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	void	_printStruct(strt_t data);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Toggle LED 0
 *		 - lock a semaphore
 *		 - Waiting for 10-ms
 *		 - Generate a data structure and print it
 *		 - Release a semaphore
 *
 */
static	void	aProcess_0(const void *argument) {
	volatile	sema_t	*semaphore;

// Get the semaphore handle

	while (kern_getSemaphoreById("Critical", &semaphore) != KKERNNOERR) { kern_switchFast(); }

	while (TRUE) {
		misc_toggleLed(0);
		kern_lockSemaphore(semaphore, -1);
		kern_suspendProcess(10);

		vData.oProcess = '0';
		kern_switchFast();
		vData.oField = 0x0000;
		kern_switchFast();
		vData.oCounter++;
		kern_switchFast();

		_printStruct(vData);
		kern_unlockSemaphore(semaphore);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Toggle LED 1
 *	     - lock a semaphore
 *		 - Waiting for 100-ms
 *		 - Generate a data structure and print it
 *		 - Release a semaphore
 *
 */
static	void	aProcess_1(const void *argument) {
	volatile	sema_t	*semaphore;

// Get the semaphore handle

	while (kern_getSemaphoreById("Critical", &semaphore) != KKERNNOERR) { kern_switchFast(); }

	while (TRUE) {
		misc_toggleLed(1);
		kern_lockSemaphore(semaphore, -1);
		kern_suspendProcess(100);

		vData.oProcess = '1';
		kern_switchFast();
		vData.oField = 0x1111;
		kern_switchFast();
		vData.oCounter++;
		kern_switchFast();

		_printStruct(vData);
		kern_unlockSemaphore(semaphore);
	}
}

/*
 * \brief aProcess 2
 *
 * - P2: - Toggle LED 2
 *		 - lock a semaphore
 *		 - Waiting for 1000-ms
 *		 - Generate a data structure and print it
 *		 - Release a semaphore
 *
 */
static	void	aProcess_2(const void *argument) {
	volatile	sema_t	*semaphore;

// Get the semaphore handle

	while (kern_getSemaphoreById("Critical", &semaphore) != KKERNNOERR) { kern_switchFast(); }

	while (TRUE) {
		misc_toggleLed(2);
		kern_lockSemaphore(semaphore, -1);
		kern_suspendProcess(1000);

		vData.oProcess = '2';
		kern_switchFast();
		vData.oField = 0x2222;
		kern_switchFast();
		vData.oCounter++;
		kern_switchFast();

		_printStruct(vData);
		kern_unlockSemaphore(semaphore);
	}
}

/*
 * \brief _printStruct
 *
 */
static	void	_printStruct(strt_t data) {

	switch (data.oProcess) {
		case '0': {
			if (data.oField != 0x0000) {
				exit(EXIT_FAILURE);
			}
			break;
		}
		case '1': {
			if (data.oField != 0x1111) {
				exit(EXIT_FAILURE);
			}
			break;
		}
		case '2': {
			if (data.oField != 0x2222) { 
				exit(EXIT_FAILURE);
			}
			break;
		}
		default: {
			exit(EXIT_FAILURE);
			break;
		}
	}

	iotx_printf(KSYST, "Process %c, Field %4X, Counter %d\n", data.oProcess, data.oField, data.oCounter);
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
	volatile	sema_t	*semaphore;
	volatile	proc_t 	*process_0, *process_1, *process_2;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "LProcess_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "LProcess_User_1";
	LOC_CONST_STRG(aStrIden_2[]) =    "LProcess_User_2";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),  0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess_2, aStrIden_2, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createMutxSemaphore("Critical", &semaphore)  != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, NULL, &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

