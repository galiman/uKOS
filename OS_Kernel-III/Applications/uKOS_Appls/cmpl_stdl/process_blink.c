/*
; process_blink.
; ==============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Process: blink.
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
#include	"hard.h"

#define	KTBLINK		500
#define	KLEDBLINK	1

// Prototypes
// ==========

static	void	aProcess(const void *argument);

/*
 * \brief Install & launch the process
 *
 */
void	installaProcess_blink(void) {
	volatile	proc_t 	*process;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden[]) =      "User_Process_blink";
	LOC_CONST_STRG(aStrText[]) =      "Process blink.                            (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, aProcess, aStrIden, KDEF0, (KLOWPRIORITY+7), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR) { exit(EXIT_FAILURE); }
}

/*
 * \brief aProcess
 *
 */
static	void	aProcess(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(KTBLINK);
		hard_toggleLed(KLEDBLINK);
	}
}

