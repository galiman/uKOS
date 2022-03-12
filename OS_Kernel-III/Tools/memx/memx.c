/*
; memx.
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
; Goal:		Management of the mass storage unit.
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

LOC_CONST_STRG(aStrApplication[]) =	"memx         Management of the mass storage unit.      (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Management of the mass storage unit\n"
									"===================================\n\n"

									"This tool allows to erase/write programs inside the\n"
									"mass storage unit.\n\n"

									"Input format:  memx -mem1 [-E | -W | -A] index\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Memx, KIDTOOL, KMEMXNUM, prgm, "1.2", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint16_t	index;
	uint32_t	mms;
	int32_t		status, value;
	enum		{ KWRITE, KERASE, KERASEALL } action;
	enum		{ KERRNOERR, KERRWRARG, KERRBLERP, KERRBLWRP } error = KERRNOERR;
	bool_t		equals;

	iotx_printf(KSYST, "MMX management.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// memx -mem1
// memx -mem1 [-W, -E] 0..X
// memx -mem1 -A
// memx -mem1 -A n to X

	switch (argc) {
		case 3: {
			value = 0;
			text_checkAsciiBuffer(argv[2], "-A", &equals);    if (equals) { action = KERASEALL; break; }
			error = KERRWRARG;
			break;
		}
		case 4: {
			strg_cnvtDecAsciiToBufInt32(&value, argv[3], 1);
			text_checkAsciiBuffer(argv[1], "-mem1", &equals); if (equals) { mms    = KMEM1;            }
														      else        { error  = KERRWRARG; break; }

			text_checkAsciiBuffer(argv[2], "-W", &equals);    if (equals) { action = KWRITE;    break; }
			text_checkAsciiBuffer(argv[2], "-E", &equals);    if (equals) { action = KERASE;    break; }
			text_checkAsciiBuffer(argv[2], "-A", &equals);    if (equals) { action = KERASEALL; break; }
			error = KERRWRARG;
			break;
		}
		default: {
			error = KERRWRARG;
			break;
		}
	}

// The possible actions

	if (!error) {

		switch (action) {
			case KWRITE: {
				index = (uint16_t)value;
				status = syos_eraseBlock(mms, &index);
				if (status != KSYOSNOERR) {
					error = KERRBLERP;
				}
				status = syos_writeBlock(mms, &index);
				if (status != KSYOSNOERR) {
					error = KERRBLWRP;
				}
				break;
			}
			case KERASE: {
				index = (uint16_t)value;
				status = syos_eraseBlock(mms, &index);
				if (status != KSYOSNOERR) {
					error = KERRBLERP;
				}
				break;
			}
			case KERASEALL: {
				if (value < KNBEEPROMAPPL) {
					for (index = (uint16_t)value; index < KNBEEPROMAPPL; index++) {
						status = syos_eraseBlock(mms, &index);
						if (status != KSYOSNOERR) {
							error = KERRBLERP;
							break;
						}
					}
					break;
				}
				else {
					error = KERRBLERP;
					break;
				}
			}
			default: {
				break;
			}
		}
	}

	switch (error) {
		case KERRNOERR: { iotx_printf(KSYST, "Terminated.\n\n");          status = EXIT_SUCCESS_OS; break; }
		case KERRWRARG: { iotx_printf(KSYST, "Incorrect arguments!\n\n"); status = EXIT_FAILURE;    break; }
		case KERRBLERP: { iotx_printf(KSYST, "Block erase problem!\n\n"); status = EXIT_FAILURE;    break; }
		case KERRBLWRP: { iotx_printf(KSYST, "Block write problem!\n\n"); status = EXIT_FAILURE;    break; }
		default:	    {												  status = EXIT_FAILURE;    break; }
	}
	return (status);
}

