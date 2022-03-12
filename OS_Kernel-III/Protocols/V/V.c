/*
; V.
; ==

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 184                  $:  Revision of last commit
; $Date:: 2017-07-17 10:50:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Read a register from the imager.
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

#define	KNBPARIN	1

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"V            Read an imager register.                  (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Read an imager register\n"
									"=======================\n\n"

									"This protocol module read a value from\n"
									"the imager register.\n\n"
									
									"Input format:  V,registerNb\n"
									"Output format: v,value\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(V, KIDPROTOCOL, KVNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	char_t		*myArgv;
	uint32_t	len;
	uint8_t		registerNb;
	uint16_t	value;
	int32_t		parameter[KNBPARIN];
	enum 		{ KERRNOT, KERRPRO, KERRMEM } error = KERRNOT;

	strg_getBufferSize(argv[0], &len);
	myArgv = (char_t *)syos_malloc(KINTERNAL, sizeof(uint8_t)*len);
	if (myArgv != NULL) {
		text_copyAsciiBufferZ(myArgv, argv[0]);

		if ((strg_skipProtocolParam(&myArgv[0], 1) != KSTRGNOERR) || (strg_cnvtDecAsciiToBufInt32(parameter, &myArgv[0], KNBPARIN) != KSTRGNOERR)) {
			error = KERRPRO;
		}
		else {
			registerNb = (uint8_t)(parameter[0] & 0xFF);
			if (img0_getRegister(registerNb, &value) != KIMG0NOERR) {
				error = KERRPRO;
			}
		}
		syos_free(myArgv);
	}
	else {
		error = KERRMEM;
	}
	
	switch (error) {
		case KERRNOT: { iotx_printf(KSYST, "v,%d\n", value); 	      return (EXIT_SUCCESS_OS); }
		case KERRPRO: { iotx_printf(KSYST, "z,Protocol error.\n");    return (EXIT_FAILURE);    }
		case KERRMEM: { iotx_printf(KSYST, "z,Not enough memory.\n"); return (EXIT_FAILURE);    }
		default:	  {											      return (EXIT_FAILURE);    }
	}
}

