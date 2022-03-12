/*
; asso_sema.
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
;			Create the synchro semaphore "urt0 - RX char"
;			Create the mutex semaphore "Critical"
;			Create the association "share counters"
;			Configure the urt0 manager
;
;			- P0: Publish an association named "share counters" with the counters
;				  Every 10-ms
;				  Toggle LED 0
;				  Generate a set of 8-bytes (counters)
;
;			- P1: Waiting for the semaphore "urt0 - RX char"
;				  If the touch Y is pushed, display the value of the counters
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

#define	KNBCOUNTERS		8

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"asso_sema    Example of how to use associations.       (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  asso_sema\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));	

/*
 * \brief aProcess 0
 *
 * - P0: - Every 10-ms
 *  		- Toggle LED 0
 *			- Generate a set of 8-bytes (counters)
 *
 */
static	void	aProcess_0(const void *argument) {
				uint8_t		i, counter[KNBCOUNTERS];
				shar_t		pack;
	volatile	sema_t		*semaphore;
	volatile	astp_t		*association;

// Create the association "share counters" -> counter
// Create the semaphore "Critical"
// Get the semaphore handle
// Initialize the counter record

	pack.oGeneral = &counter[0];
	pack.oSize = sizeof(counter);

	if (kern_createAssociation("share counters", &association) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_publishAssociation(association, &pack) 		   != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createMutxSemaphore("Critical", &semaphore) 	   != KKERNNOERR) { exit(EXIT_FAILURE); }

	for (i = 0; i < KNBCOUNTERS; i++) {
		counter[i] = i;
	}

	while (TRUE) {
		kern_suspendProcess(10);

// Asking for the semaphore, waiting if busy
// If the semaphore is not busy (reserved for me), then increment the counters
// Release the semaphore

		kern_lockSemaphore(semaphore, -1);
		for (i = 0; i < KNBCOUNTERS; i++) {
			counter[i]++;
		}
		kern_unlockSemaphore(semaphore);

		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Waiting for the semaphore "urt0 - RX char" (to avoid to use the cpu)
 *   		- Display the counters
 *
 */
static	void	aProcess_1(const void *argument) {
				uint8_t		i, received, *counter, localCounter[KNBCOUNTERS];
				uint32_t 	size;
				int32_t		status;
	volatile	shar_t		*pack;
	volatile	sema_t		*semaphoreMUTX, *semaphoreURT0;
	volatile	astp_t		*association;

	urt0_flush();

// Get the semaphore handle (UART0)
// Get the semaphore handle (MUTEX)
// Get the association handle & find it

	while (kern_getSemaphoreById(KSEMAPHORE_URT0RX, &semaphoreURT0) != KKERNNOERR) { kern_switchFast(); }
	while (kern_getSemaphoreById("Critical", &semaphoreMUTX)        != KKERNNOERR) { kern_switchFast(); }
	while (kern_getAssociationById("share counters", &association)  != KKERNNOERR) { kern_switchFast(); }
	while (kern_findAssociation(association, &pack)      			!= KKERNNOERR) { kern_switchFast(); }

	counter = (uint8_t *)pack->oGeneral;

	while (TRUE) {

// Waiting for a character on the UART0 (without timeout)
// Read 1 byte & verify if it is = "Y"

		do { status = kern_waitSemaphore(semaphoreURT0, 1, -1); } while (status != KKERNNOERR);

		size = 1;
		status = urt0_read(&received, &size);
		if ((status == KURT0NOERR) && (size == 1) && (received == 'Y')) {

// Asking for the semaphore, waiting if busy
// If the semaphore is not busy (reserved for me), then copy the counters
// Release the semaphore

			kern_lockSemaphore(semaphoreMUTX, -1);
			for (i = 0; i < KNBCOUNTERS; i++) {
				localCounter[i] = *(counter + i);
			}
			kern_unlockSemaphore(semaphoreMUTX);

			iotx_printf(KSYST, "%d %d %d %d %d %d %d %d\n", localCounter[0], localCounter[1],
															localCounter[2], localCounter[3],
															localCounter[4], localCounter[5],
															localCounter[6], localCounter[7]);
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBDDEFAULT;
	configureURTx.oKernSync = (1<<BRXSEMA);
	urt0_configure(&configureURTx);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }

// Do not return with "EXIT_SUCCESS_OS" because the "console" uses the KURT0

	return (EXIT_SUCCESS);
}

