/*
; binfill.
; ========

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
; Goal:		Raw binary loader.
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

#define	KLED1		1
#define	KNBCARBLK	20
#define	KCOMMTO5S	5000000
#define	KCOMMTO10S	10000000

extern	uint32_t	_stUMemo;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"binfill      Raw binary loader.                        (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Raw binary loader\n"
									"=================\n\n"

									"This tool downloads a raw binary data.\n\n"

									"Input format:  binfill\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	bool_t	_getByte(uint32_t *checksum, uint8_t *buffer, uint8_t *blink, uint64_t timeout);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Binfill, KIDTOOL, KBINFILLNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint8_t		*address, byte, cptBlinkLED = 0;
	uint32_t	checksum = 0;
	bool_t		error = FALSE, terminate = FALSE;

	RESERVE_COMM(KSYST, KDEVREAD);
	iotx_printf(KSYST, "Raw binary format loader mode; waiting for the code.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	comm_flush(KSYST);

// Waiting under timeout of 10 s the first Byte

	address = (uint8_t *)&_stUMemo;
	if (!_getByte(&checksum, &byte, &cptBlinkLED, KCOMMTO10S)) {
		error = TRUE;
	}
	else {
		*address++ = byte;

// Now under timeout of 5s the other Bytes

		do {
			if (!_getByte(&checksum, &byte, &cptBlinkLED, KCOMMTO5S)) {
				terminate = TRUE;
			}
			else {
				*address++ = byte;
			}
		} while (!terminate);
	}

	misc_offLed(KLED1);

	comm_flush(KSYST);
	RELEASE_COMM(KSYST, KDEVREAD);

	if (!error) { iotx_printf(KSYST, "\nBIN: download terminated checksum = 0x%X\n\n", checksum); return (EXIT_SUCCESS_OS); }
	else        { iotx_printf(KSYST, "\nBIN: timeout error.\n\n"); 							      return (EXIT_FAILURE);    }
}

// Local routines
// ==============

/*
 * \brief _getByte
 *
 * - Read a Byte under timeout
 *
 */
static	bool_t	_getByte(uint32_t *checksum, uint8_t *buffer, uint8_t *blink, uint64_t timeout) {
	int32_t		status;
	uint32_t	size;
	uint64_t	time[2];

	kern_getTiccount(&time[0]);

	do {
		kern_getTiccount(&time[1]);
		if ((time[1] - time[0]) >= timeout) {
			return (FALSE);
		}

		kern_switchFast();
		size = 1;
		status = comm_read(KSYST, buffer, &size);
	} while (status != KCOMMNOERR);

	*checksum += *buffer;
	if (*blink++ >= KNBCARBLK) {
		*blink = 0;
		misc_toggleLed(KLED1);
	}
	return (TRUE);
}

