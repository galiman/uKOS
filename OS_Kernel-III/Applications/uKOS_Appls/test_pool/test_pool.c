/*
; test_pool.
; ==========

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
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;				  Fill the allocated blocks with counters
;
;			- P1: Every 200-ms
;				  Read allocated blocks and test the counters
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

static				pcnf_t		vConfigure;
static	volatile	pool_t		*vMemoryPool;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_pool    uKernel test of the memory pool.          (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_pool\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess_0
 *
 * - P0: - Fill the allocated blocks with counters
 *
 */
static	void	aProcess_0(const void *argument) {
				uint16_t	i, nbElements, *array_0, *array_1, *array_2;
				int32_t		status;
	volatile	pool_t		*memoryPool;

	status = pool_getPoolById("Memory pool", &memoryPool);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem P0!!\n"); exit(EXIT_FAILURE); }

// Recover the memory arrays

	array_0 = memoryPool->oUsedBlocks[0];
	if (array_0 == NULL) {
		exit(EXIT_FAILURE);
	}
	array_1 = memoryPool->oUsedBlocks[1];
	if (array_1 == NULL) {
		exit(EXIT_FAILURE);
	}
	array_2 = memoryPool->oUsedBlocks[2];
	if (array_2 == NULL) {
		exit(EXIT_FAILURE);
	}

	nbElements = memoryPool->oBlockSize / sizeof(uint16_t);

	while (TRUE) {
		kern_suspendProcess(1000);

// Fill the 3  pools

		for (i = 0; i < nbElements; i++) { array_0[i] = i + 0; }
		for (i = 0; i < nbElements; i++) { array_1[i] = i + 1; }
		for (i = 0; i < nbElements; i++) { array_2[i] = i + 2; }
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess_1
 *
 * - P0: - Read allocated blocks and test the counters
 *
 */
static	void	aProcess_1(const void *argument) {
				uint16_t	i, nbElements, *array_0, *array_1, *array_2;
				int32_t		status;
	volatile	pool_t		*memoryPool;

// Waiting until P0 has filled at least once the 3 arrays
// Get the pool information

	kern_suspendProcess(2000);

	status = pool_getPoolById("Memory pool", &memoryPool);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem P1!!\n"); exit(EXIT_FAILURE); }

// Recover the memory arrays

	array_0 = memoryPool->oUsedBlocks[0];
	if (array_0 == NULL) {
		exit(EXIT_FAILURE);
	}
	array_1 = memoryPool->oUsedBlocks[1];
	if (array_1 == NULL) {
		exit(EXIT_FAILURE);
	}
	array_2 = memoryPool->oUsedBlocks[2];
	if (array_2 == NULL) {
		exit(EXIT_FAILURE);
	}

	nbElements = memoryPool->oBlockSize / sizeof(uint16_t);

	while (TRUE) {
		kern_suspendProcess(200);

// Test the 3  pools

		for (i = 0; i < nbElements; i++) {
			if (array_0[i] != (i + 0)) {
				iotx_printf(KSYST, "Problem array_0!! Expected %d Read %d\n", i, array_0[i]);
				exit(EXIT_FAILURE);
			}
		}
		for (i = 0; i < nbElements; i++) {
			if (array_1[i] != (i + 1)) {
				iotx_printf(KSYST, "Problem array_1!! Expected %d Read %d\n", i, array_1[i]);
				exit(EXIT_FAILURE);
			}
		}
		for (i = 0; i < nbElements; i++) {
			if (array_2[i] != (i + 2)) {
				iotx_printf(KSYST, "Problem array_2!! Expected %d Read %d\n", i, array_2[i]);
				exit(EXIT_FAILURE);
			}
		}
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
				int32_t		status;
				void		*memory;
	volatile	proc_t 		*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

	vConfigure.oNbBlocks  = 3;
	vConfigure.oBlockSize = 1000*sizeof(uint16_t);
	vConfigure.oLocation  = KINTERNAL;
	status = kern_createPool("Memory pool", &vMemoryPool);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem !!\n"); exit(EXIT_FAILURE); }

	status = kern_setPool(vMemoryPool, &vConfigure);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem !!\n"); exit(EXIT_FAILURE); }

	status = kern_allocateBlock(vMemoryPool, &memory);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem !!\n"); exit(EXIT_FAILURE); }

	status = kern_allocateBlock(vMemoryPool, &memory);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem !!\n"); exit(EXIT_FAILURE); }

	status = kern_allocateBlock(vMemoryPool, &memory);
	if (status != KKERNNOERR) { iotx_printf(KSYST, "Problem !!\n"); exit(EXIT_FAILURE); }

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}
