/*
; kill.
; =====

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
; Goal:		Kill a running process.
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

LOC_CONST_STRG(aStrApplication[]) =	"kill         Kill a running process.                   (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Kill a running process\n"
									"======================\n\n"

									"This tool kills a running process.\n\n"

									"Input format:  kill process_name\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Kill, KIDTOOL, KKILLNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				int32_t		status;
				enum 		{ KERRNOT, KERRPRO, KERRPNK, KERRPNE } error = KERRNOT;
	volatile	proc_t 		*process;

	iotx_printf(KSYST, "Process kill.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc != 2) {
		error = KERRPRO;
	}
	else {
		if (kern_getProcessById(argv[1], &process) != KKERNNOERR) {
			error = KERRPNE;
		}
		if (kern_killProcess(process) != KKERNNOERR) {
			error = KERRPNK;
		}
	}
	
	switch (error) {
		case KERRNOT: { 	 													 status = EXIT_SUCCESS_OS; break; }
		case KERRPNE: { iotx_printf(KSYST, "The process does not exist.\n\n");   status = EXIT_FAILURE;    break; }
		case KERRPNK: { iotx_printf(KSYST, "The process cannot be killed.\n\n"); status = EXIT_FAILURE;    break; }
		case KERRPRO: { iotx_printf(KSYST, "Protocol error.\n\n");	             status = EXIT_FAILURE;    break; }
		default:	  {														     status = EXIT_FAILURE;    break; }
	}
	return (status);
}

