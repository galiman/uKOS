/*
; sqe_multi.
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
; 			- P0: - Launch the Sysquake Embedded
;		 		  - A = zero(5,4)
;		 		  - Every 500-ms
;					 - A(1,1) = A(1,1) + 1;
;					 - A(5,4) = A(5,4) + 2;
; 					 - Toggle LED 0
;
; 			- P1: - Launch the Sysquake Embedded
;		 		  - A = zero(5,4)
;		 		  - Every 500-ms
;					 - A(1,5) = A(1,5) + 1;
;					 - A(3,1) = A(3,1) + 2;
; 					 - Toggle LED 1
;
;   (c) 1992-2017, Franzi Edo.
;   --------------------------
;
;   Developed for use with Sysquake Embedded.
;   Sysquake is a trademark of Calerga Sarl, used with permission. 
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

LOC_CONST_STRG(aStrApplication[]) =	"sqe_multi    Example of how to use sqe.                (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sqe_multi\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Launch the Sysquake Embedded
 *		 - A = zero(5,4)
 *		 - Every 500-ms
 *			- A(1,1) = A(1,1) + 1;
 *			- A(5,4) = A(5,4) + 2;
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
	uint32_t	size;
	uint8_t		*memory, row, col;
	float64_t	result[5][4];
	cnfSqee_t	configure;
	ioSqee_t	data, *ref;

// Try to reserve the SQE memory segment

	size   = 40000;
	memory = (uint8_t *)syos_malloc(KINTERNAL, size);
	if (memory == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	configure.oSize   = size;
	configure.oMemory = memory;
	sqee_configure((cnfSqee_t *)&configure);

	ref = &data;
	ref->oSqeeRef = configure.oSqeeRef;

// Generate a (5 x 4) 0 matrix

	SQEE_COMPUTE(ref, "A = zeros(5,4);");

	while (TRUE) {
		kern_suspendProcess(500);

// Add 1 & 2 to the elements A(1,1) & A(5,4)
//  rows = 5
//  cols = 4
//  z    = 0

		SQEE_COMPUTE(ref, "A(1,1) = A(1,1) + 1;\n"
						  "A(5,4) = A(5,4) + 2;");
		SQEE_GET(ref, K2DARRAY, 0, 4, 5, &result[0][0], "xdata(A);");

// Display the results in integer (result * 1000)

		for (row = 0; row < 5; row++) {
			for (col = 0; col < 4; col++) {
				iotx_printf(KSYST, "%8d ", (int32_t)(result[row][col]*1000));
			}
			iotx_printf(KSYST, "\n");
		}
		iotx_printf(KSYST, "\n");
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P0: - Launch the Sysquake Embedded
 *		 - A = zero(3,5)
 *		 - Every 500-ms
 *			- A(1,5) = A(1,5) + 1;
 *			- A(3,1) = A(3,1) + 2;
 * 			- Toggle LED 1
 *
 */
static	void	aProcess_1(const void *argument) {
	uint32_t	size;
	uint8_t		*memory, row, col;
	float64_t	result[3][5];
	cnfSqee_t	configure;
	ioSqee_t	data, *ref;

// Try to reserve the SQE memory segment

	size   = 40000;
	memory = (uint8_t *)syos_malloc(KINTERNAL, size);
	if (memory == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	configure.oSize   = size;
	configure.oMemory = memory;
	sqee_configure((cnfSqee_t *)&configure);

	ref = &data;
	ref->oSqeeRef = configure.oSqeeRef;

// Generate a (3 x 5) 0 matrix

	SQEE_COMPUTE(ref, "B = zeros(3,5);");

	while (TRUE) {
		kern_suspendProcess(500);

// Add 1 & 2 to the elements A(1,5) & A(3,1)
//  rows = 3
//  cols = 5
//  z    = 0

		SQEE_COMPUTE(ref, "B(1,5) = B(1,5) + 1;\n"
						  "B(3,1) = B(3,1) + 2;");
		SQEE_GET(ref, K2DARRAY, 0, 5, 3, &result[0][0], "xdata(B);");

// Display the results in integer (result * 1000)

		for (row = 0; row < 3; row++) {
			for (col = 0; col < 5; col++) {
				iotx_printf(KSYST, "%8d ", (int32_t)(result[row][col]*1000));
			}
			iotx_printf(KSYST, "\n");
		}
		iotx_printf(KSYST, "\n");
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKSQE, aProcess_0, aStrIden_0, KURT0, (KLOWPRIORITY+7), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKSQE, aProcess_1, aStrIden_1, KURT1, (KLOWPRIORITY+7), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

