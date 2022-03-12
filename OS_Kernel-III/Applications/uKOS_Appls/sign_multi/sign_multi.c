/*
; sign_multi.
; ===========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 186                  $:  Revision of last commit
; $Date:: 2017-08-17 15:22:27#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 5 processes:
;
;			- P0: Every 2000-ms
;				  Generate a signal for P3
;
;			- P1: Every 4000-ms
;				  Generate a signal for P4
;
;			- P2: Every 9000-ms
;				  Toggle LED 5
;				  Generate a signal for Pn (broadcast)
;
;			- P3: Waiting for the signal from P0 (or broadcast) under timeout
;				  Toggle LED 0
;				  Display a string or a timeout error message
;
;			- P4: Waiting for the signal from P1 (or broadcast) under timeout
;				  Toggle LED 1
;				  Display a string or a timeout error message
;
;			- P5: Waiting for the signal from P2 (or broadcast) under timeout
;				  Toggle LED 2
;				  Display a string or a timeout error message
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

#define	KP0_TO_P3_SIGNAL_00000001	0x00000001
#define	KP1_TO_P4_SIGNAL_04000000	0x04000000
#define	KP2_TO_PN_SIGNAL_00080000	0x00080000
#define	KP2_TO_PN_SIGNAL_04080001	(KP0_TO_P3_SIGNAL_00000001 | KP1_TO_P4_SIGNAL_04000000 | KP2_TO_PN_SIGNAL_00080000)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sign_multi   Example of how to use signals.            (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sign_multi\n"
									"Output format: [result]\n\n";

LOC_CONST_STRG(aStrIden_0[]) 	  = "Process_Synchro 0";
LOC_CONST_STRG(aStrIden_1[]) 	  = "Process_Synchro 1";
LOC_CONST_STRG(aStrIden_2[]) 	  = "Process_Synchro 2";
LOC_CONST_STRG(aStrIden_3[]) 	  = "Process_User_3";
LOC_CONST_STRG(aStrIden_4[]) 	  = "Process_User_4";
LOC_CONST_STRG(aStrIden_5[]) 	  = "Process_User_5";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 * 			- Generate a signal "process 0" for the process P3
 *
 */
static	void	aProcess_0(const void *argument) {
				uint32_t	signalProcess = KP0_TO_P3_SIGNAL_00000001;
	volatile	sign_t		*signal;
	volatile	proc_t 		*process;

	if (kern_createSignal("Process 0", &signal)      != KKERNNOERR) { exit(EXIT_FAILURE);     }
	while (kern_getProcessById(aStrIden_3, &process) != KKERNNOERR) { kern_suspendProcess(1); }

	while (TRUE) {
		kern_suspendProcess(2000);
		kern_signalSignal(signal, signalProcess, process, KSIGNSWTC);
	}
}

/*
 * \brief aProcess 1
 *
 * - P0: - Every 1234-ms
 * 			- Generate a signal "process 1" for the process P4
 *
 */
static	void	aProcess_1(const void *argument) {
				uint32_t	signalProcess = KP1_TO_P4_SIGNAL_04000000;
	volatile	sign_t		*signal;
	volatile	proc_t 		*process;

	if (kern_createSignal("Process 1", &signal)      != KKERNNOERR) { exit(EXIT_FAILURE);     }
	while (kern_getProcessById(aStrIden_4, &process) != KKERNNOERR) { kern_suspendProcess(1); }

	while (TRUE) {
		kern_suspendProcess(4000);
		kern_signalSignal(signal, signalProcess, process, KSIGNSWTC);
	}
}

/*
 * \brief aProcess 2
 *
 * - P0: - Every 9000-ms
 *  		- Toggle LED 5
 * 			- Generate a signal "process 2" for all the process
 *
 */
static	void	aProcess_2(const void *argument) {
				uint32_t	signalProcess = KP2_TO_PN_SIGNAL_04080001;
	volatile	sign_t		*signal;

	if (kern_createSignal("Process 2", &signal) != KKERNNOERR) { exit(EXIT_FAILURE); }

	while (TRUE) {
		kern_suspendProcess(9000);
		kern_signalSignal(signal, signalProcess, NULL, KSIGNSWTC);
		misc_toggleLed(5);
	}
}

/*
 * \brief aProcess 3
 *
 * - P3: - Waiting for the signal "process 0" (or broadcast) under timeout
 *  		- Toggle LED 0
 * 			- Display a string or a timeout error message
 *
 */
static	void	aProcess_3(const void *argument) {
	int32_t		status;
	uint32_t	signalProcess = KP0_TO_P3_SIGNAL_00000001;

// Get the signal

	while (TRUE) {
		status = kern_waitSignal(&signalProcess, 10000);
		misc_toggleLed(0);

		(status == KKERNTIMEO) ? iotx_printf(KSYST, "P3 - Timeout error! %08X\n", signalProcess) : iotx_printf(KSYST, "P3 - Signal %08X\n", signalProcess);
	}
}

/*
 * \brief aProcess 4
 *
 * - P4: - Waiting for the signal "process 1" (or broadcast) under timeout
 *  		- Toggle LED 1
 * 			- Display a string or a timeout error message
 *
 */
static	void	aProcess_4(const void *argument) {
	int32_t		status;
	uint32_t	signalProcess = KP1_TO_P4_SIGNAL_04000000;

	while (TRUE) {
		status = kern_waitSignal(&signalProcess, 10000);
		misc_toggleLed(1);

		(status == KKERNTIMEO) ? iotx_printf(KSYST, "P4 - Timeout error! %08X\n", signalProcess) : iotx_printf(KSYST, "P4 - Signal %08X\n", signalProcess);
	}
}

/*
 * \brief aProcess 5
 *
 * - P5: - Waiting for the signal "process 2" (or broadcast) under timeout
 *  		- Toggle LED 2
 * 			- Display a string or a timeout error message
 *
 */
static	void	aProcess_5(const void *argument) {
	int32_t		status;
	uint32_t	signalProcess = KP2_TO_PN_SIGNAL_00080000;

	while (TRUE) {
		status = kern_waitSignal(&signalProcess, -1);
		misc_toggleLed(2);

		(status == KKERNTIMEO) ? iotx_printf(KSYST, "P5 - Timeout error! %08X\n", signalProcess) : iotx_printf(KSYST, "P5 - Signal %08X\n", signalProcess);
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
	volatile	proc_t 	*process_0, *process_1, *process_2, *process_3, *process_4, *process_5;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrText_0[]) =    "Process Synchro 0.                        (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process Synchro 1.                        (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process Synchro 2.                        (c) EFr-2017";
	LOC_CONST_STRG(aStrText_3[]) =    "Process user 3.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_4[]) =    "Process user 4.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_5[]) =    "Process user 5.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess_2, aStrIden_2, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(3, vSpecification_3, aStrText_3, KSZSTACKMIN, aProcess_3, aStrIden_3, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(4, vSpecification_4, aStrText_4, KSZSTACKMIN, aProcess_4, aStrIden_4, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(5, vSpecification_5, aStrText_5, KSZSTACKMIN, aProcess_5, aStrIden_5, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, NULL, &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_3, NULL, &process_3) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_4, NULL, &process_4) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_5, NULL, &process_5) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

