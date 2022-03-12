/*
; alive.
; ======

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
; Goal:		alive process; blink the LED 0.
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
#include	"alive.h"

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"alive        Alive process: everything is OK.          (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "alive process\n"
									"=============\n\n"

									"The system is living\n\n";

LOC_CONST_STRG(aStrIden[])        = "Process_alive";
LOC_CONST_STRG(aStrText[])        = "Process alive: the system is living.      (c) EFr-2017";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
extern	void	stub_alive_process(const void *argument);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Alive, KIDPROCESS, KALIVENUM, prgm, "1.0", (1<<BSHOW));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
			volatile	proc_t 		*process;
	static				cnfAlive_t	configure;

// Values set by default
 
	configure.oTime[0] = KTIMEALIVE_ON;
	configure.oTime[1] = KTIMEALIVE_OFF;
	if (argc == 3) {
		strg_cnvtDecAsciiToBufInt32((int32_t *)&configure.oTime[0], argv[1], 1);
		strg_cnvtDecAsciiToBufInt32((int32_t *)&configure.oTime[1], argv[2], 1);
	}

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, stub_alive_process, aStrIden, KDEF0, (KLOWPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, &configure, &process) != KKERNNOERR) { exit(EXIT_PANIC); }

	return (EXIT_SUCCESS_OS);
}

