/*
; test_malloc.
; ============

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
;			Launch 8 processes:
;
;			- P0..P7
;			      Randomly test the malloc
;		    	  Ask for random quantity of memory
;				  Each bloc is filled with a pattern
;				  When the heap is full, check of the coherence of the blocs & release them
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

#define	KNBSAMPLES	10000

typedef	struct		myPack	myPack_t;

static	uint8_t		*aTabPtr0[KNBSAMPLES];
static	uint8_t		*aTabPtr1[KNBSAMPLES];
static	uint8_t		*aTabPtr2[KNBSAMPLES];
static	uint8_t		*aTabPtr3[KNBSAMPLES];
static	uint8_t		*aTabPtr4[KNBSAMPLES];
static	uint8_t		*aTabPtr5[KNBSAMPLES];
static	uint8_t		*aTabPtr6[KNBSAMPLES];
static	uint8_t		*aTabPtr7[KNBSAMPLES];

struct	myPack {
			char_t		*oProcessName;									// Ptr on the process name
			uint8_t		**oTabPtr;										// Ptr on the syos_malloc ptr table
		};

static	const	myPack_t	aParameter[8] = {
								{ "Process: 0 ", aTabPtr0 },
								{ "Process: 1 ", aTabPtr1 },
								{ "Process: 2 ", aTabPtr2 },
								{ "Process: 3 ", aTabPtr3 },
								{ "Process: 4 ", aTabPtr4 },
								{ "Process: 5 ", aTabPtr5 },
								{ "Process: 6 ", aTabPtr6 },
								{ "Process: 7 ", aTabPtr7 }
							};

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_malloc  test of the malloc.                       (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_malloc\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0..7
 *
 * - Px: Randomly test the malloc
 *       Ask for random quantity of memory
 *		 Each bloc is filled with a pattern
 *       When the heap is full, check of the coherence of the blocs & release them
 *
 */
static	void	aProcess(const void *argument) {
			char		*processName;
			uint32_t	i, j, amount;
			uint8_t		**aTabPtr, *length, *counter, cpt;
			bool_t		terminate;
	const	myPack_t	*pack;

	pack = argument;

	processName = pack->oProcessName;
	aTabPtr     = pack->oTabPtr;

// Initialize the ptr table

	for (i = 0; i < KNBSAMPLES; i++) {
		aTabPtr[i] = NULL;
	}

	while (TRUE) {

// Try to reserve as many blocs as possible
// Consider only 5 < blocks < 256

		i = 0; terminate = FALSE;
		do {
			do {
				kern_suspendProcess(1);
				glob_getRND(&amount);
				amount &= 0xFF;
			} while (amount < 5);

			aTabPtr[i] = (uint8_t *)syos_malloc(KEXTERNAL, amount);
			if (aTabPtr[i] == NULL) {
				terminate = TRUE;
			}
			else {
				//        --|-------------------------------------|--
				syos_trace("--> Reserve 1: display the pointer     ", (uint32_t)aTabPtr[i]);

// Fill the area with a counter
// First 4 bytes the counter, then, the counter ...
//		   counter			        (amount - 1)
// 0       1       2       3        xx
// length  00      01      02 ..... nn

				length  = aTabPtr[i];
				*length = (uint8_t)amount - 1;
				counter = aTabPtr[i] + 1;
				cpt     = 0;

				for (j = 0; j < *length; j++) {
					*counter++ = cpt++;
				}
			}
		} while ((++i < KNBSAMPLES) && (!terminate));

// Verify the counter
// 0       1    2   3      xx
// length  00   01  02 ... nn
//
// Release the reserved blocs

		i = 0; terminate = FALSE;
		do {
			if (aTabPtr[i] == NULL) {
				terminate = TRUE;
			}
			else {
				//        --|-------------------------------------|--
				syos_trace("--> Verify 1: display the pointer      ", (uint32_t)aTabPtr[i]);
				length  = aTabPtr[i];
				counter = (uint8_t *)(aTabPtr[i] + 1);
				cpt     = 0;

				for (j = 0; j < *length; j++) {
					if (counter[j] != cpt++) {
						iotx_printf(KSYST, "%s: Problem, counter %d\n", processName, j);
						exit(EXIT_FAILURE);
					}
				}
				//        --|-------------------------------------|--
				syos_trace("--> Verify 2: syos_free                ", (uint32_t)aTabPtr[i]);
				syos_free(aTabPtr[i]);
			}
		} while ((++i < KNBSAMPLES) && (!terminate));

		iotx_printf(KSYST, "%s: Number of blocs = %d\n", processName, i);
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
	volatile	proc_t 	*process_0, *process_1, *process_2, *process_3;
	volatile	proc_t 	*process_4, *process_5, *process_6, *process_7;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_2[]) =    "Process_User_2";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_3[]) =    "Process_User_3";
	LOC_CONST_STRG(aStrText_3[]) =    "Process user 3.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_4[]) =    "Process_User_4";
	LOC_CONST_STRG(aStrText_4[]) =    "Process user 4.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_5[]) =    "Process_User_5";
	LOC_CONST_STRG(aStrText_5[]) =    "Process user 5.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_6[]) =    "Process_User_6";
	LOC_CONST_STRG(aStrText_6[]) =    "Process user 6.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_7[]) =    "Process_User_7";
	LOC_CONST_STRG(aStrText_7[]) =    "Process user 7.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess, aStrIden_0, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess, aStrIden_1, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess, aStrIden_2, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(3, vSpecification_3, aStrText_3, KSZSTACKMIN, aProcess, aStrIden_3, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(4, vSpecification_4, aStrText_4, KSZSTACKMIN, aProcess, aStrIden_4, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(5, vSpecification_5, aStrText_5, KSZSTACKMIN, aProcess, aStrIden_5, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(6, vSpecification_6, aStrText_6, KSZSTACKMIN, aProcess, aStrIden_6, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(7, vSpecification_7, aStrText_7, KSZSTACKMIN, aProcess, aStrIden_7, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, &aParameter[0], &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, &aParameter[1], &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, &aParameter[2], &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_3, &aParameter[3], &process_3) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_4, &aParameter[4], &process_4) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_5, &aParameter[5], &process_5) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_6, &aParameter[6], &process_6) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_7, &aParameter[7], &process_7) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

