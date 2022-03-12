/*
; flash.
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
; Goal:		Reading flash protection.
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

LOC_CONST_STRG(aStrApplication[]) =	"flash        Flash code protection.                    (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Flash protection\n"
									"================\n\n"

									"This tool disable (for ever) the flash reading accesses.\n"
									"The security level 0, 1 or 2 can be set. \n"
									"For the level 2, no extra possibilities to erase or\n"
									"to modify the flash after this command.\n\n"

									"Input format:  flash {-help | -level0 | -level1 | -level2}\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
		void	flash_protect(uint8_t level);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Flash, KIDTOOL, KFLASHNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	int32_t		status = EXIT_FAILURE;
	bool_t		equals;
	enum		{ KHELP, KLEVEL0, KLEVEL1, KLEVEL2 } action;
	enum		{ KFLASHNOERR, KFLASHERPAR } error = KFLASHNOERR;

	iotx_printf(KSYST, "Flash management.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// flash
// flash -level0
// flash -level1
// flash -level2
// flash -help

	switch (argc) {
		case 1: {
			action = KHELP;
			break;
		}
		case 2: {
			text_checkAsciiBuffer(argv[1], "-level0", &equals); if (equals) { action = KLEVEL0; break; }
			text_checkAsciiBuffer(argv[1], "-level1", &equals); if (equals) { action = KLEVEL1; break; }
			text_checkAsciiBuffer(argv[1], "-level2", &equals); if (equals) { action = KLEVEL2; break; }
			text_checkAsciiBuffer(argv[1], "-help",   &equals); if (equals) { action = KHELP;   break; }
	        error = KFLASHERPAR;
	        break;
		}
		default: {
			error = KFLASHERPAR;
			break;
		}
	}

	switch (error) {
		case KFLASHNOERR: {
			status = EXIT_SUCCESS_OS;
			switch (action) {
				case KHELP:   {                   iotx_printf(KSYST, "\n%s", aStrHelp);                                           break; }
				case KLEVEL0: { flash_protect(0); iotx_printf(KSYST, "!!! Flash protected (level 0). Switch-off the board.\n\n"); break; }
				case KLEVEL1: { flash_protect(1); iotx_printf(KSYST, "!!! Flash protected (level 1). Switch-off the board.\n\n"); break; }
				case KLEVEL2: { flash_protect(2); iotx_printf(KSYST, "!!! Flash protected (level 2). Switch-off the board.\n\n"); break; }
				default: {
					break;
				}
			}
			break;
		}
		case KFLASHERPAR: {						 iotx_printf(KSYST, "\n%s", aStrHelp);								              break; }
		default: {
			break;
		}
	}
	return (status);
}

#include 	<model_flash_protection.cm>

