/*
; queue_unis.
; ===========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 176                  $:  Revision of last commit
; $Date:: 2017-07-07 12:58:34#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			- P0: Create a mailbox "Queue 1-to-0"
;				     Read and display the messages coming from the queue 1-to-0
;				     Toggle LED 0
;
;			- P1: Waiting for a mailbox "Queue 1-to-0"
;				     Write a message into the queue queue 1-to-0
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
#include 	<string.h>

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"queue_unis   Example of how to use queue.              (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  queue_unis\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Create a mailbox "Queue 1-to-0"
 * 			- Read and display the messages coming from the queue
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
				uint32_t	message_1_to_0, expected_1_to_0 = 0;
	volatile	mbox_t		*queue_1_to_0;

// Create the queue 1-to-0

	if (kern_createMailbox("Queue 1-to-0", &queue_1_to_0) != KKERNNOERR) { iotx_printf(KSYST, "Error Queue 1-to-0\n"); exit(EXIT_FAILURE); }

	while (TRUE) {

// Receive the message (if FIFO is not empty)
// Display the message

		while (kern_readQueue(queue_1_to_0, &message_1_to_0) == KKERNNOPAC) { kern_suspendProcess(10); }

		if (message_1_to_0 != expected_1_to_0) {
			iotx_printf(KSYST, "Data lost!! Received = %d, expected = %d\n", message_1_to_0, expected_1_to_0);
			exit(EXIT_FAILURE);
		}
		expected_1_to_0++;
		iotx_printf(KSYST, "Message = %d\n", message_1_to_0);
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Waiting for a mailbox "Queue 1-to-0"
 *			- Write a message into the queue queue 1-to-0
 *
 */
static	void	aProcess_1(const void *argument) {
				uint32_t	message_1_to_0 = 0;
	volatile	mbox_t		*queue_1_to_0;

// Waiting for the queue 1-to-0

	while (kern_getMailboxById("Queue 1-to-0", &queue_1_to_0) != KKERNNOERR) { kern_suspendProcess(1); }

	while (TRUE) {
		kern_suspendProcess(100);

// Send a the message (if FIFO is not full)

		while (kern_writeQueue(queue_1_to_0, message_1_to_0) == KKERNTOPAC) { kern_suspendProcess(1); }
		message_1_to_0++;
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

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

