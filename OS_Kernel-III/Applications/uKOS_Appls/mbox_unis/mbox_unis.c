/*
; mbox_unis.
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
;			Launch 1 process:
;
;			- P0: Create a mailbox "Mailbox 0"
;				  Every 100-ms
;				  Reserve a memory buffer
;				  Write into the mailbox
;				  Read from the mailbox
;				  Display the information
;				  Toggle LED 0
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

LOC_CONST_STRG(aStrApplication[]) =	"mbox_unis    Example of how to use mail boxes.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  mbox_unis\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Create a mailbox "Mailbox 0"
 *       - Every 100-ms
 *			- Reserve a memory buffer
 *			- Write into the mailbox
 *			- Read from the mailbox
 *			- Display the information
 *			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
				uint8_t		i;
				uint16_t	sizeSnd, sizeRec;
				int32_t		status;
				uint8_t		*bufSnd, *bufRec;
	volatile	mbox_t		*mailBox;

// Create a mailbox 0

	status = kern_createMailbox("Mailbox 0", &mailBox);
	if (status != KKERNNOERR) {
		iotx_printf(KSYST, "No mailbox\n");
		exit(EXIT_FAILURE);
	}

	while (TRUE) {
		kern_suspendProcess(100);

// Send the message

		sizeSnd = sizeof(uint8_t)*KSZMESSAGE;
		bufSnd  = (uint8_t *)syos_malloc(KINTERNAL, sizeSnd);
		if (bufSnd == NULL) {
			iotx_printf(KSYST, "Error malloc\n");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < (sizeSnd-1); i++) {
			bufSnd[i] = (' ' + i);
		}
		bufSnd[sizeSnd-1] = '\0';

// Send a the message (wait until the FIFO is not full)
// Receive the message (wait until the FIFO is not empty)

		while (kern_writeMailbox(mailBox, bufSnd, sizeSnd)  == KKERNTOPAC) { kern_switchFast(); }
		while (kern_readMailbox(mailBox, &bufRec, &sizeRec) == KKERNNOPAC) { kern_switchFast(); }

		iotx_printf(KSYST, "%s\n", bufRec);
		syos_free((void *)bufRec);

		misc_toggleLed(0);
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

