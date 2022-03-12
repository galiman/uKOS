/*
; BaNgLa.
; =======

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
; Goal:		Display the purpose of the Bangla team.
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

LOC_CONST_STRG(aStrApplication[]) =	"BaNgLa       The Bangla group.                         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "The Bangla group\n"
									"================\n\n"

									"The purpose of the Bangla group is:\n"
									"   Eating a lot.\n"
									"   Playing the flipper on Amiga.\n"
									"   Having a good time.\n\n"

									"Input format:  BaNgLa\n"
									"Output format: [result]\n\n";

LOC_CONST_STRG(aStrBangla[])      = "Bangla team for ever.\n\n"
							        "- Franzi Edo.\n"
								    "- Tettoni Laurent\n"
								    "- Mondada Francesco\n"
								    "- Cheneval Yves\n\n"
								    "El trona en val culera e el rimbomba em mezz ai ciapp ....\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(BaNgLa, KIDTOOL, KBANGLANUM, prgm, "1.0", ((1<<BEXECONSOLE) | (1<<BCONFIDENTIAL)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	 
	iotx_printf(KSYST, "Bangla team ...\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	iotx_printf(KSYST, "%s", aStrBangla);
	return (EXIT_SUCCESS_OS);
}

