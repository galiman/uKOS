/*
; start-up.
; =========

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
; Goal:		start_up process; execute some important initializations
;			before jumping to the selected function.
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

extern	const	char_t	aStrHelp[]	__attribute__ ((aligned (4)));

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"start_up     Start_up process of the system.           (c) EFr-2017";
LOC_CONST_STRG(aStrIden[])        = "Process_Startup";
LOC_CONST_STRG(aStrText[])        = "Process start_up of the system.           (c) EFr-2017";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_process(const void *argument);
extern	void	stub_start_up_launch(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Start_up, KIDPROCESS, KSTARTUPNUM, prgm, "2.0", (1<<BSHOW));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	volatile	proc_t 	*process;

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, _process, aStrIden, KDEF0, (KLOWPRIORITY+7), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR) { exit(EXIT_PANIC); }
	return (EXIT_SUCCESS_OS);
}

/*
 * \brief _process
 *
 */
static	void	_process(const void *argument) {

	stub_start_up_launch();
}

