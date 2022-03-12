/*
; misc_dsp.
; =========

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
;			Launch 3 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Compute the vertical-horizontal sums of a matrix
;			      Every 1000-ms
;
;			- P2: Compute the sum of the 2 vector product
;			      Every 1000-ms
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

LOC_CONST_STRG(aStrApplication[]) =	"misc_dsp     Example of how to use the DSP.            (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_dsp\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

__attribute__(( always_inline )) static __inline void _cumulate32(int32_t *value, int16_t a, int16_t b);
__attribute__(( always_inline )) static __inline void _cumulate64(int64_t *value, int32_t a, int32_t b);

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
 * - P1: - Every 1000-ms
 *			- Compute the vertical-horizontal sums of a matrix
 *
 */
static	void	aProcess_1(const void *argument) {
			uint8_t		i, j;
	const	int16_t		image[10][10] = {
							{  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 },
							{  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 },
							{  2,  3,  4,  5,  6,  7,  8,  9, 10, 11 },
							{  3,  4,  5,  6,  7,  8,  9, 10, 11, 12 },
							{  4,  5,  6,  7,  8,  9, 10, 11, 12, 13 },
							{  5,  6,  7,  8,  9, 10, 11, 12, 13, 14 },
							{  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
							{  7,  8,  9, 10, 11, 12, 13, 14, 15, 16 },
							{  8,  9, 10, 11, 12, 13, 14, 15, 16, 17 },
							{ 10, 10, 11, 12, 13, 14, 15, 16, 17, 18 }
						};

	static	int32_t		projections_x[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static	int32_t		projections_y[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	while (TRUE) {
		kern_suspendProcess(1000);

// Compute the vector rows & cols

		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++) {
				_cumulate32(&projections_x[i], image[i][j], 1);
				_cumulate32(&projections_y[i], image[j][i], 1);
			}

		iotx_printf(KSYST, "\nRows\n\n");
		for (i = 0; i < 10; i++) {
			iotx_printf(KSYST, "%d\n", projections_x[i]);
		}

		iotx_printf(KSYST, "\nCols\n\n");
		for (i = 0; i < 10; i++) {
			iotx_printf(KSYST, "%d\n", projections_y[i]);
		}

	}
}

/*
 * \brief aProcess 2
 *
 * - P2: - Every 1000-ms
 *			- Compute the sum of the 2 vector product
 *
 */
static	void	aProcess_2(const void *argument) {
			uint8_t		i;
			int64_t		result;
	static	int32_t		vector_1[10] = { 1, -2, 3, 4,  5, 6, 7, 8, 9,  10};
	static	int32_t		vector_2[10] = { 1,  2, 3, 4, -5, 6, 7, 8, 9, -10};

	while (TRUE) {
		kern_suspendProcess(1000);

// Compute the vector product

		result = 0;
		for (i = 0; i < 10; i++) {
			_cumulate64(&result, vector_1[i], vector_2[i]);
		}

		iotx_printf(KSYST, "Result = %ld\n", result);
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
	volatile	proc_t 	*process_0, *process_1, *process_2;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrIden_2[]) =    "Process_User_2";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess_2, aStrIden_2, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, NULL, &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

// Local routines
// ==============

/*
 * \brief _cumulate32
 *
 * - Compute the sum of the elements of a vector
 *   - value.32 = value.32 + (a.16 * b.16)
 *
 */
__attribute__(( always_inline )) static __inline void _cumulate32(int32_t *value, int16_t a, int16_t b) {

	__asm__ volatile ("smlabb %0, %1, %2, %3" : "=r" (*value) : "r" (a), "r" (b), "r" (*value));
}

/*
 * \brief _cumulate64
 *
 * - Compute the sum of the elements of a vector
 *   - value.64 = value.64 + (a.32 * b.32)
 *
 */
__attribute__(( always_inline )) static __inline void _cumulate64(int64_t *value, int32_t a, int32_t b) {
	uint32_t	h, l;

	h = (uint32_t)(*value>>32); l = (uint32_t)(*value & 0xFFFFFFFFUL);
	__asm__ volatile ("smlal %0, %1, %2, %3" : "=r" (l), "=r" (h) : "r" (a), "r" (b), "0" (l), "1" (h));
	*value = (((uint64_t)h<<32) | l);
}

