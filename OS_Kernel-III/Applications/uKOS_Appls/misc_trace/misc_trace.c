/*
; misc_trace.
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
;			On terminal type:
;			uKOSLog -d2x DCWZ1FL8 460800 /tmp/appl.log
;
;			Launch 3 processes:
;
;			Create the synchro semaphore "urt0 - RX char"
;			Configure the urt0 manager
;			Trace the program events
;
;			- P0: Every 1000-ms
;				  TRACE the process
;				  Toggle LED 0
;
;			- P1: Waiting for the semaphore "urt0 - RX char"
;				  TRACE the process
;				  Display a string or a timeout error message
;
;			- P2: Just use the CPU
;				  TRACE the process
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

LOC_CONST_STRG(aStrApplication[]) =	"misc_trace   Example of how to use semaphores.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_trace\n"
									"Output format: [result]\n\n";

// I/O redirection for the trace

#define	KURT1		(('u'<<24) | ('r'<<16) | ('t'<<8) | '1')
#define	KURT2		(('u'<<24) | ('r'<<16) | ('t'<<8) | '2')
#define	KUSB0		(('u'<<24) | ('s'<<16) | ('b'<<8) | '0')

const	uint32_t	aTabIO[] = {
						KURT1,			// Target = Discovery_407			CPU = ARM Cortex M4
						KURT1,			// Target = Discovery_429			CPU = ARM Cortex M4
						0,				// Target = Discovery_476			CPU = ARM Cortex M4
						KURT1,			// Target = Discovery_746			CPU = ARM Cortex M7
						KUSB0,			// Target = Baphomet_407			CPU = ARM Cortex M4
						KUSB0,			// Target = Baphomet_429			CPU = ARM Cortex M4
						KUSB0,			// Target = Baphomet_746			CPU = ARM Cortex M7
						0,				// Target = VIP_407					CPU = ARM Cortex M4
						KURT1,			// Target = VIP_429					CPU = ARM Cortex M4
						KURT1,			// Target = VIP_746					CPU = ARM Cortex M7
						KURT1,			// Target = VIP_777					CPU = ARM Cortex M7
						0,				// Target = LAP_icycam_103			CPU = ARM Cortex M3
						0,				// Target = CSEM_icycam				CPU = icyflex 1
						0,				// Target = CSEM_icyamr				CPU = icyflex 1
						0,				// Target = CSEM_silicon			CPU = icyflex 1
						0,				// Target = CSEM_Suntracker_407		CPU = ARM Cortex M4
						0,				// Target = CSEM_Posbone_WL_407		CPU = ARM Cortex M4
						0				// Target = CSEM_Bio_479			CPU = ARM Cortex M4
					};

static	uint32_t	vCommManager;

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

#pragma GCC diagnostic	push
#pragma GCC diagnostic	ignored	"-Wcast-align"

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		TAG_LOG(vCommManager, -1, 'S','Y','S',' ')
		kern_suspendProcess(1000);
		TAG_LOG(vCommManager, 0, 't','o','p',' ')
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Waiting for the semaphore "urt0 - RX char"
 * 			- Display a string or a timeout error message
 *
 */
static	void	aProcess_1(const void *argument) {
				int32_t		status;
	volatile	sema_t		*semaphore;

	kern_getSemaphoreById(KSEMAPHORE_URT0RX, &semaphore);

	while (TRUE) {
		TAG_LOG(vCommManager, -1, 'S','Y','S',' ')
		status = kern_waitSemaphore(semaphore, 1, 2000);

		if (status == KKERNTIMEO) {
			TAG_LOG(vCommManager, 1, 'e','r','r',' ')
			iotx_printf(KSYST, "Timeout Error process 1\n");
		}
		else {
			TAG_LOG(vCommManager, 1, 'o','k',' ',' ')
			iotx_printf(KSYST, "KSYST - RX char\n");
		}
	}
}

/*
 * \brief aProcess 2
 *
 * - P2: - Every 10-ms
 * 			- Just use the CPU
 *
 */
static	void	aProcess_2(const void *argument) {

	while (TRUE) {
		TAG_LOG(vCommManager, -1, 'S','Y','S',' ')
		kern_suspendProcess(10);
		TAG_LOG(vCommManager, 2, 't','o','p',' ')
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
				uint32_t	family;
	volatile	proc_t 		*process_0, *process_1, *process_2;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrIden_2[]) =    "Process_User_2";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";

	glob_getFamily(&family);
	vCommManager = aTabIO[family];
	iotx_printf(KSYST, "KSYST = %c%c%c%c\n", (uint8_t)(vCommManager>>24), (uint8_t)(vCommManager>>16), (uint8_t)(vCommManager>>8), (uint8_t)(vCommManager));

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess_2, aStrIden_2, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, NULL, &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

#pragma GCC	diagnostic	pop
