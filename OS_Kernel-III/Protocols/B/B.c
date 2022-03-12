/*
; B.
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
; Goal:		Read the software version.
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

#define	KLNASCBUF	10
#define	KIDCSTUP	((KIDPROCESS<<24) | (KSTARTUPNUM<<8) | '_')
#define	KIDKERNEL	((KIDKERN<<24)    | (KKERNNUM<<8)    | '_')

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"B            Read the software versions.               (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Read the software versions\n"
									"==========================\n\n"

									"This protocol module gives the versions of the\n"
									"softwares present in the ROM.\n\n"

									"Input format:  B\n"
									"Output format: b,vers_TBox,vers_Kernel,vers_Start-Up\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(B, KIDPROTOCOL, KBNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
			uint16_t	index;
	const	module_t	*module_0, *module_1;
	const	char_t		*revTBox;

// The TBox revision
// Find the module "kern"
// Find the module "Start-Up"

	glob_getRevision(&revTBox);
	syos_getIdModule(KMEM0, KIDKERNEL, &index, &module_0);
	syos_getIdModule(KMEM0, KIDCSTUP,  &index, &module_1);

	iotx_printf(KSYST, "b,%s,%s,%s\n", revTBox, module_0->oStrRevision, module_1->oStrRevision);
	return (EXIT_SUCCESS_OS);
}

