/*
; mbox_bidm.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 168                  $:  Revision of last commit
; $Date:: 2017-07-03 17:28:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			- P0: Create a mailbox "Mailbox 0 to 1"
;				  Waiting for the creation of the "Mailbox 1 to 0"
;				  Every 2-ms
;				  Reserve a memory buffer
;				  Write into the mailbox "Mailbox 0 to 1"
;				  Read from the mailbox "Mailbox 1 to 0"
;				  Display the information
;				  Release the memory buffer
;				  Toggle LED 0
;
;			- P1: Create a mailbox "Mailbox 1 to 0"
;				  Waiting for the creation of the "Mailbox 0 to 1"
;				  Reserve a memory buffer
;				  Read from the mailbox "Mailbox 0 to 1"
;				  Write into the mailbox "Mailbox 1 to 0"
;				  Release the memory buffer
;				  Toggle LED 1
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

#define	KSZMESSAGE	30

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"mbox_bidm    Example of how to use mail boxes.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  mbox_bidm\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Create a mailbox "Mailbox 0 to 1"
 *       - Waiting for the creation of the "Mailbox 1 to 0"
 *       - Every 2-ms
 *			- Reserve a memory buffer
 * 			- Write into the mailbox "Mailbox 0 to 1"
 * 			- Read from the mailbox "Mailbox 1 to 0"
 *			- Display the information
 *			- Release the memory buffer
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
				uint16_t	sizeSnd, sizeRec;
				int32_t		status;
				uint8_t		*bufSnd, *bufRec, nbMessages, i;
	volatile	mbox_t		*mailBox_01, *mailBox_10;

// Create the "Mailbox 0 to 1"

	status = kern_createMailbox("Mailbox 0 to 1", &mailBox_01);
	if (status != KKERNNOERR) {
		iotx_printf(KSYST, "No mailbox 0 to 1\n");
		exit(EXIT_FAILURE);
	}

// Waiting for the "Mailbox 1 to 0"

	while (kern_getMailboxById("Mailbox 1 to 0", &mailBox_10) != KKERNNOERR) { kern_switchFast(); }
	nbMessages = ' ';

	while (TRUE) {
		kern_suspendProcess(2);

// Send the message

		sizeSnd = sizeof(uint8_t)*KSZMESSAGE;
		bufSnd  = (uint8_t *)syos_malloc(KINTERNAL, sizeSnd);
		if (bufSnd == NULL) {
			iotx_printf(KSYST, "Error malloc 0\n");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < (sizeSnd-2); i++) {
			bufSnd[i] = (' ' + i);
		}
		bufSnd[sizeSnd-2] = nbMessages;
		bufSnd[sizeSnd-1] = '\0';

// Send a the message (wait until the FIFO is not full)

		while (kern_writeMailbox(mailBox_01, bufSnd, sizeSnd) == KKERNTOPAC) { kern_switchFast(); }

// Receive the message (wait until the FIFO is not empty)

		while (kern_readMailbox(mailBox_10, &bufRec, &sizeRec) == KKERNNOPAC) { kern_switchFast(); }

		iotx_printf(KSYST, "%s\n", bufRec);
		syos_free((void *)bufRec);

		nbMessages = (nbMessages < '~') ? (nbMessages + 1) : ' ';

		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Create a mailbox "Mailbox 1 to 0"
 *       - Waiting for the creation of the "Mailbox 0 to 1"
 * 			- Reserve a memory buffer
 * 			- Read from the mailbox "Mailbox 0 to 1"
 *			- Write into the mailbox "Mailbox 1 to 0"
 * 			- Release the memory buffer
 *			- Toggle LED 1
 *
 */
static	void	aProcess_1(const void *argument) {
				uint16_t	i, sizeSnd, sizeRec;
				int32_t		status;
				uint8_t		*bufSnd, *bufRec;
	volatile	mbox_t		*mailBox_01, *mailBox_10;

// Create the "Mailbox 1 to 0"

	status = kern_createMailbox("Mailbox 1 to 0", &mailBox_10);
	if (status != KKERNNOERR) {
		iotx_printf(KSYST, "No mailbox 1 to 0\n");
		exit(EXIT_FAILURE);
	}

// Waiting for the "Mailbox 0 to 1"

	while (kern_getMailboxById("Mailbox 0 to 1", &mailBox_01) != KKERNNOERR) { kern_switchFast(); }

	while (TRUE) {

// Receive the message (wait until the FIFO is not empty)

		while (kern_readMailbox(mailBox_01, &bufRec, &sizeRec) == KKERNNOPAC) { kern_switchFast(); }

		sizeSnd = sizeRec;
		bufSnd  = (uint8_t *)syos_malloc(KINTERNAL, sizeSnd);
		if (bufSnd == NULL) {
			iotx_printf(KSYST, "Error malloc 1\n");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < sizeRec; i++) {
			*(bufSnd + i) = *(bufRec + i);
		}
		syos_free((void *)bufRec);

// Send a the message (wait until the FIFO is not full)

		while (kern_writeMailbox(mailBox_10, bufSnd, sizeSnd) == KKERNTOPAC) { kern_switchFast(); }

		misc_toggleLed(1);
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

