/*
; run.
; ====

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
; Goal:		Launch a function module.
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

LOC_CONST_STRG(aStrApplication[]) =	"run          Run a downloaded code.                    (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Launch a function module\n"
									"========================\n\n"

									"This tool runs a downloaded code.\n\n"

									"Input format:  run\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

#define	KIDUSER	((KIDAPPLICATION<<24) | (KAPPLICATIONNUM<<8) | '_')

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Run, KIDTOOL, KRUNNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	int32_t		status, (*code)(uint32_t argc, const char_t *argv[]);

	iotx_printf(KSYST, "Execute the downloaded application.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	syos_getDoLoCode((void *)&code);
	if (code == NULL) { iotx_printf(KSYST, "No application in the memory!\n\n");     status = EXIT_FAILURE;    }
	else              { iotx_printf(KSYST, "Run the downloaded application...\n\n"); status = EXIT_SUCCESS_OS; }

	syos_setDoLoCode(NULL);

	if (status == EXIT_SUCCESS_OS) {
		status = (*code)(argc, argv);
	}
	return (status);
}

