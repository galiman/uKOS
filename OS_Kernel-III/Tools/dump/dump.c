/*
; dump.
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
; Goal:		This tool allows to dump a memory area.
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

#define	KLNASCBUF	96

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"dump         Dump a memory area.                       (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Dump a memory area\n"
									"==================\n\n"

									"This tool displays a memory area. The result\n"
									"is displayed in hexadecimal format.\n\n"

									"Input format:  dump hex_startAdd hex_endAdd\n"
									"Output format: add  hexadecimal field\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_printLine(char_t *ascii, uint8_t *firstAddr, uint32_t nbBytes);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Dump, KIDTOOL, KDUMPNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	char_t		*ascii;
	uint8_t		*firstAddr;
	uint32_t	nbBytes;
	int32_t		status, start, end;
	bool_t		error = FALSE;

	iotx_printf(KSYST, "Memory dump.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	ascii = (char_t *)syos_malloc(KINTERNAL, KLNASCBUF);
	if (ascii == NULL) { iotx_printf(KSYST, "Not enough memory.\n\n"); return (EXIT_FAILURE); }

	if (argc != 3) {
		error = TRUE;
	}
	else {
		strg_cnvtHexAsciiToBufInt32(&start, argv[1], 1);
		strg_cnvtHexAsciiToBufInt32(&end,   argv[2], 1);

		nbBytes = end - start;
		firstAddr = (uint8_t *)start;
		_printLine(ascii, firstAddr, nbBytes);
	}

	syos_free(ascii);
	if (!error) { iotx_printf(KSYST, "\n"); 				 status = EXIT_SUCCESS_OS; }
	else        { iotx_printf(KSYST, "Protocol error.\n\n"); status = EXIT_FAILURE;    }
	return (status);
}

// Local routines
// ==============

/*
 * \brief _printLine
 *
 * - Print a 16-Bytes line
 * - 0x12345678: 00 33 34 02 03 04 05 06 07 34 41 BB 62 DD EE FF .34......4A.b..
 *
 */
static	void	_printLine(char_t *ascii, uint8_t *firstAddr, uint32_t nbBytes) {
	uint8_t		j;
	uint32_t	i;
	int32_t		param;
	char_t		*wkAscii, element;

	for (i = 0; i < (nbBytes+16)/16; i++) {
		wkAscii = ascii;
		param = (int32_t)(firstAddr + (i*16));
		strg_cnvtBufInt8ToHexAscii(wkAscii, (int8_t *)(firstAddr + (i*16)), 16);

// The ASCII part of the line

		wkAscii = ascii;
		while (*wkAscii != '\n') wkAscii++;

		*wkAscii++ = ' ';
		*wkAscii++ = ' ';
		for (j = 0; j < 16; j++) {
			element = (char_t)*(firstAddr + (i*16) + j);
			if ((*(firstAddr + (i*16) + j) < 0x20) || (*(firstAddr + (i*16) + j) > 0x7F)) {
				element = '.';
			}
			*wkAscii++ = element;
		}

		iotx_printf(KSYST, "0x%08X: %s\n", param, ascii);
	}
}

