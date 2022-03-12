/*
; man.
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
; Goal:		Show the help of the module.
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

LOC_CONST_STRG(aStrApplication[]) =	"man          Show the help of the module.              (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Show the help of the module\n"
									"===========================\n\n"

									"This tool displays the help for a\n"
									"selected module.\n\n"

									"Input format:  man {moduleName}\n"
									"Output format: [result]\n\n";

LOC_CONST_STRG(aStrLogo[])        = KSTRLOGO_HELP
LOC_CONST_STRG(aStrCopyright[])   =	KCOPYRIGHT_FRANZI

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Man, KIDTOOL, KMANNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
			uint16_t	index;
			int32_t		status;
			uint32_t	idModule;
			bool_t		error = FALSE;
	const	module_t	*module;

	iotx_printf(KSYST, "Module help.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc == 2) {

// man with parameters

		if (syos_getNameModule(KMEM0, argv[1], &index, &module) != KSYOSNOERR) {
			if (syos_getNameModule(KMEM1, argv[1], &index, &module) != KSYOSNOERR) {
				error = TRUE;
			}
		}
		if (!error) {
			iotx_printf(KSYST, "%s%s", module->oStrHelp, aStrCopyright);
		}
	}

// man without parameter
// Display the logo and the tool list

	else {
		iotx_printf(KSYST, "%s", aStrLogo);

		index = 0;
		while (syos_getFamilyModule(KMEM0, KIDTOOL, &idModule, &index, &module) == KSYOSNOERR) {
			if (module->oFlag & (1<<BSHOW)) {
				iotx_printf(KSYST, "%s\n", module->oStrApplication);
			}
			index++;
		}
	}

	if (!error) { iotx_printf(KSYST, "\n");				 	 status = EXIT_SUCCESS_OS; }
	else        { iotx_printf(KSYST, "Protocol error.\n\n"); status = EXIT_FAILURE;    }
	return (status);
}

