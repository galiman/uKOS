/*
; bench.
; ======

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
;			Launch 1 processes:
;
;			- P0: bench performance
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

LOC_CONST_STRG(aStrApplication[]) =	"bench        M4 - M7 performance benches.              (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "The M4 - M7 benches\n"
									"===================\n\n"

									"This tool performs some benches to figure-out the real\n"
									"cpu performances\n\n"

									"Input format:  bench\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
extern	bool_t	bench_00(void);
extern	bool_t	bench_01(void);
extern	bool_t	bench_02(void);
extern	bool_t	bench_03(void);
extern	bool_t	bench_04(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Bench, KIDTOOL, KBENCHNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {

	iotx_printf(KSYST, "System bench.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (!bench_00()) { iotx_printf(KSYST, "Not enough memory.\n"); return (EXIT_FAILURE); }
	if (!bench_01()) { iotx_printf(KSYST, "Not enough memory.\n"); return (EXIT_FAILURE); }
	if (!bench_02()) { iotx_printf(KSYST, "Not enough memory.\n"); return (EXIT_FAILURE); }
	if (!bench_03()) { iotx_printf(KSYST, "Not enough memory.\n"); return (EXIT_FAILURE); }
	if (!bench_04()) { iotx_printf(KSYST, "Not enough memory.\n"); return (EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

