/*
; fill.
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
; Goal:		This tool allows to fill a memory with a pattern.
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

LOC_CONST_STRG(aStrApplication[]) =	"fill         Fill a memory area with a pattern.        (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Fill a memory area with a pattern\n"
									"=================================\n\n"

									"This tool fills a memory area with a\n"
									"pattern.\n\n"

									"Input format:  fill hex_startAdd hex_endAdd data\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Fill, KIDTOOL, KFILLNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint8_t		*firstAddr;
	uint32_t	i, nbBytes;
	int32_t		status, start, end, value;
	bool_t		error = FALSE;

	iotx_printf(KSYST, "Memory fill.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc != 4) {
		error = TRUE;
	}
	else {
		strg_cnvtHexAsciiToBufInt32(&start, argv[1], 1);
		strg_cnvtHexAsciiToBufInt32(&end,   argv[2], 1);
		strg_cnvtHexAsciiToBufInt32(&value, argv[3], 1);

		nbBytes = end - start;
		firstAddr = (uint8_t *)start;
		for (i = 0; i < nbBytes; i++) {
			*firstAddr++ = (uint8_t)value;
		}
	}

	if (!error) { iotx_printf(KSYST, "\n"); 				 status = EXIT_SUCCESS_OS; }
	else        { iotx_printf(KSYST, "Protocol error.\n\n"); status = EXIT_FAILURE;    }
	return (status);
}

