/*
; misc_txrx.
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
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Send/receive messages on urt4
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

LOC_CONST_STRG(aStrApplication[]) =	"misc_txrx    Example of how to use the urt4.          (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_txrx\n"
									"Output format: [result]\n\n";

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

	while (TRUE) {
		kern_suspendProcess(1000);

		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Send/receive a message
 *
 */
static	void	aProcess_1(const void *argument) {
			#define		KSZBUFFER	(50+1)

			int32_t		status;
			uint32_t	i, size, read, posi, cpt = 0;
			cnfUrtx_t	configureURTx;
			uint8_t		recBuffer[KSZBUFFER];
			enum		{ KERRNO, KERRUART, KERRMSG } error = KERRNO;
//                                              1        10        20        30        40        50
//                                              I        I         I         I         I         I
	const	uint8_t		sndBuffer[KSZBUFFER] = "This is a long message to send by the urt4 channel";

// Configurations of the communication channel

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBD460800;
	configureURTx.oKernSync = 0;
	urt4_configure(&configureURTx);
	urt4_flush();

	kern_suspendProcess(10000);

	while (TRUE) {

// Send the message
// 51 chars & 460800 should take ~(1000000/460800) x 52 = 1-ms

		do {
			kern_suspendProcess(1);
			status = urt4_write(&sndBuffer[0], KSZBUFFER);
		} while (status != KURT4NOERR);

// Read the message (by packets of data)

		posi = 0;
		read = 0;
		size = KSZBUFFER;
		do {
			kern_suspendProcess(1);
			posi = posi + read;
			read = size;
			status = urt4_read(&recBuffer[posi], &read);
			if ((read != 0) && (status != KURT4NOERR)) {
				error = KERRUART;
			}
			
			size = size - read;
		} while ((size > 0) || (error != KERRNO));

// Test the message

		for (i = 0; i < KSZBUFFER; i++) {
			if (recBuffer[i] != sndBuffer[i]) {
				error = KERRMSG;
			}
		}
			
		switch (error) {
			case KERRNO:   { iotx_printf(KSYST,"%s  %d\n", &recBuffer[0], cpt++); break;              }
			case KERRUART: { iotx_printf(KSYST,"Error = %0d\n", -(status & 0xF)); exit(EXIT_FAILURE); }
			case KERRMSG:  { iotx_printf(KSYST,"Message error\n");                exit(EXIT_FAILURE); }
			default : {
				break;
			}
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
	volatile	proc_t	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),  0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

