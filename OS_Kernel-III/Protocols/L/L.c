/*
; L.
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
; Goal:		Control the LED states.
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

#define	KNBPARIN	2

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"L            Control the LED states.                   (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Control the LED states\n"
									"======================\n\n"

									"This protocol module performs an action on one LED.\n"
									"Possible actions are:\n"
									"   0: turn OFF\n"
									"   1: turn ON\n"
									"   2: toggle state\n\n"

									"Input format:  L,led_number,action\n"
									"Output format: l\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(L, KIDPROTOCOL, KLNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	char_t		*myArgv;
	uint32_t	len;
	int32_t		parameter[KNBPARIN];
	enum 		{ KERRNOT, KERRPRO, KERRLED, KERRACT, KERRMEM } error = KERRNOT;

	strg_getBufferSize(argv[0], &len);
	myArgv = (char_t *)syos_malloc(KINTERNAL, sizeof(uint8_t)*len);
	if (myArgv != NULL) {
		text_copyAsciiBufferZ(myArgv, argv[0]);

		if ((strg_skipProtocolParam(&myArgv[0], 1) != KSTRGNOERR) || (strg_cnvtDecAsciiToBufInt32(parameter, &myArgv[0], KNBPARIN) != KSTRGNOERR)) {
			error = KERRPRO;
		}
		else {

			switch (parameter[1]) {
				case 0:  { if (misc_offLed(parameter[0])    == KMISCNODEV) error = KERRLED; break; }
				case 1:  { if (misc_onLed(parameter[0])     == KMISCNODEV) error = KERRLED; break; }
				case 2:  { if (misc_toggleLed(parameter[0]) == KMISCNODEV) error = KERRLED; break; }
				default: { 												   error = KERRACT; break; }
			}
		}
		syos_free(myArgv);
	}
	else {
		error = KERRMEM;
	}

	switch (error) {
		case KERRNOT: { iotx_printf(KSYST, "l\n"); 									return (EXIT_SUCCESS_OS); }
		case KERRPRO: { iotx_printf(KSYST, "z,Protocol error.\n"); 					return (EXIT_FAILURE);    }
		case KERRLED: { iotx_printf(KSYST, "z,This LED does not exist.\n");			return (EXIT_FAILURE);    }
		case KERRACT: { iotx_printf(KSYST, "z,This LED action is not possible.\n");	return (EXIT_FAILURE);    }
		case KERRMEM: { iotx_printf(KSYST, "z,Not enough memory.\n");				return (EXIT_FAILURE);    }
		default:	  {																return (EXIT_FAILURE);    }
	}
}

