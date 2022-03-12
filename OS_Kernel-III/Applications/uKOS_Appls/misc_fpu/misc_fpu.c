/*
; misc_fpu.
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
;			Launch 2 process:
;
;			- P0: Every 1000-ms
;				  printf on the KURT0 manager of a function
;
;			- P1: Every 1000-ms
;				  printf on the KURTx manager of Pi (Lambert algorithm)
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
#include 	<math.h>

#define	__TEST_1__				// Compute the Pi value by the Spigot algorithm
#undef	__TEST_2__				// Compute the ln(1-x) by a Mclaurin series

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_fpu     Example of how to use the FPU.            (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_fpu\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

#if (defined(__TEST_1__))
/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- printf on the KURT0 manager of Pi
 *			- Spigot algorithm:
 *			- Pi = Sum 1/16^^n * (4/(8n + 1) - 2/(8n + 4) - 1/(8n + 5) - 1/(8n + 6))
 *
 */
#define	KNBITERATIONS	10000

static	void	aProcess_0(const void *argument) {
	volatile	float32_t	n = 0.0F, Pi = 0.0F;
	uint64_t	time[2];
	uint32_t	delta;

	iotx_printf(KSYST, "\n\n");
	while (TRUE) {
		kern_suspendProcess(1000);

		INTERRUPTION_OFF;
		kern_getTiccount(&time[0]);
		Pi = Pi + (1/powf(16, n)) *
			 	 ((4/(8*n + 1))  -
	          	  (2/(8*n + 4))  -
	          	  (1/(8*n + 5))  -
			   	  (1/(8*n + 6)));

		kern_getTiccount(&time[1]);
		delta = (uint32_t)(time[1] - time[0]);
		INTERRUPTION_RESTORED;

		n = n + 1;
		iotx_printf(KSYST, "n = %d, Pi = %d.%05d Time = %d\n", (uint32_t)n, FLOAT_5(Pi), delta);
		misc_toggleLed(0);
	}
}
#endif

#if (defined(__TEST_2__))
/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- printf on the KURT0 manager of the ln(1-x)
 *			- Mclaurin algorithm:
 *			- ln(1-x) = - (x + 1/2 * X^2 + 1/3 * X^3 + 1/4 * X^4 ....)
 *
 */
static	inline float32_t	element(float32_t x, float32_t n) {
	static	float32_t	s = 1.0;

	s = s * x;
	return ((1.0 / n) * s);
}

static	void	aProcess_0(const void *argument) {
	float32_t	n, x, result;
	uint64_t	time[2];
	uint32_t	delta;

	iotx_printf(KSYST, "\n\n");
	while (TRUE) {

		INTERRUPTION_OFF;
		kern_getTiccount(&time[0]);
		result = 0.0; x = 0.314159265358979323846;
		for (n = 1; n < 100000.0; n++) {
			result += element(x, n);
		}
	
		result = -1.0 * result;
		kern_getTiccount(&time[1]);
		delta = (uint32_t)(time[1] - time[0]);
		INTERRUPTION_RESTORED;
	
		iotx_printf(KSYST, "log(1-x) = %d.%05d, for x = %d.%05d, Time = %d\n", FLOAT_5(result), FLOAT_5(x), delta);
	}
}
#endif

/*
 * \brief aProcess 1
 *
 * - P1: - Every 1000-ms
 * 			- printf on the KURT1 manager of Pi
 *			- Lambert algorithm:
 *			- Pi = sqrt(6 * (1/1^^2 + 1/2^^2 + 1/3^^2 +...)
 *
 */
void	aProcess_1(const void *argument) {
	volatile	float32_t	n = 1.0F, v = 0.0F, Pi;

	iotx_printf(KSYST, "\n\n");
	while (TRUE) {
		kern_suspendProcess(1000);

		v = v + (1/powf(n, 2));
		Pi = sqrt(v * 6);

		iotx_printf(KSYST, "n = %d, Pi = %d.%05d\n", (uint32_t)n, FLOAT_5(Pi));

		n = n + 1;
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKNLB, aProcess_0, aStrIden_0, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKNLB, aProcess_1, aStrIden_1, KURT1, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }	
	return (EXIT_SUCCESS_OS);
}

