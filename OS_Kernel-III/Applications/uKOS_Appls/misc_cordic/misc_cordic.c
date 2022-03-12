/*
; misc_cordic.
; ============

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
;			- P1: Compute the atan2 with the cordic algo
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
#include 	<stdio.h>
#include 	<string.h>
#include 	<math.h>

#define	KMAXITER	24
#define KPI			(3.14159265358979)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_cordic  Example of the cordic implementation.     (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_cordic\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	float64_t	_atan2(int32_t y, int32_t x);

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
		kern_suspendProcess(1);
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Every 1000-ms
 *			- Compute the atan2
 *
 */
static	void	aProcess_1(const void *argument) {
	uint64_t	time[2];
	uint32_t	delta = 0;
	float64_t	angle;

	while (TRUE) {
		kern_suspendProcess(200);

// Cordic implementation

		kern_getTiccount(&time[0]);
		angle = _atan2(123456789, 23456789);
		kern_getTiccount(&time[1]);
		delta = (uint32_t)(time[1] - time[0]);

		iotx_printf(KSYST, "Cordic angle %d.%07d, time %d\n", FLOAT_7(angle), delta);

// Newlib implementation

		kern_getTiccount(&time[0]);
		angle = atan2(123456789, 23456789);
		kern_getTiccount(&time[1]);
		delta = (uint32_t)(time[1] - time[0]);

		iotx_printf(KSYST, "newlib angle %d.%07d, time %d\n", FLOAT_7(angle), delta);
	}
}

static	float64_t _atan2(int32_t y, int32_t x) {
	register	int32_t		yi, i;
	register	int32_t		lx, ly;
	register	float64_t	angle = 0.0;
	register	float64_t	*tabAtan;
	static		float64_t	aTabAtan[] = {
								0.785398163397448000000000000000, 0.463647609000806000000000000000,
								0.244978663126864000000000000000, 0.124354994546761000000000000000,
								0.062418809995957400000000000000, 0.031239833430268300000000000000,
								0.015623728620476800000000000000, 0.007812341060101110000000000000,
								0.003906230131966970000000000000, 0.001953122516478820000000000000,
								0.000976562189559319000000000000, 0.000488281211194898000000000000,
								0.000244140620149362000000000000, 0.000122070311893670000000000000,
								0.000061035156174208800000000000, 0.000030517578115526100000000000,
								0.000015258789061315800000000000, 0.000007629394531101970000000000,
								0.000003814697265606500000000000, 0.000001907348632810190000000000,
								0.000000953674316405961000000000, 0.000000476837158203089000000000,
								0.000000238418579101558000000000, 0.000000119209289550781000000000,
								0.000000059604644775390600000000, 0.000000029802322387695300000000,
								0.000000014901161193847700000000, 0.000000007450580596923830000000,
								0.000000003725290298461910000000, 0.000000001862645149230960000000,
								0.000000000931322574615479000000, 0.000000000465661287307739000000
							};

	if ((x == 0) && (y == 0)) {
		return (0.0);
	}

	lx = x; ly = y;

// Verify the right plane

	if (lx < 0) { lx = -lx; ly = -ly; angle = KPI;    }
    if (ly > 0) {                     angle = -angle; }
    
    tabAtan = &aTabAtan[0];

// Rotate the vector and compute the angle

	for (i = 0; i < KMAXITER; i++) {
		if (ly < 0) {
			yi = ly + (lx >> i);
			lx = lx - (ly >> i);
			ly = yi;
			angle -= tabAtan[i];
		}
		else {
			yi = ly - (lx >> i);
			lx = lx + (ly >> i);
			ly = yi;
			angle += tabAtan[i];
		}
    }
    return (angle);
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),  0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

