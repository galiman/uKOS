/*
; cycle.
; ======

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
; Goal:		cycle tool; launch a program cyclically.
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

static				uint32_t	vTime;
static				int32_t		(*vTool)(uint32_t argc, const char_t *argv[]);
static	volatile	uint32_t	vCommManager;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"cycle        Cycle function.                           (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "cycle\n"
									"=====\n\n"

									"This tool launches cyclically a tool/program\n"
									"or a protocol.\n\n"

									"Input format:  cycle prgmName time {commManager, urt0, ...}\n"
									"Output format: [result]\n\n";

LOC_CONST_STRG(aStrIden[])        = "Process_Cycle";
LOC_CONST_STRG(aStrText[])        = "Process cycle.                            (c) EFr-2017";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_process(const void *argument);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Cycle, KIDTOOL, KCYCLENUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				uint16_t	index;
				int32_t		status;
				enum 		{ KERRNOT, KERRINA, KERRPNE, KERRUMB } error = KERRNOT;
	const		module_t	*module;
	volatile	proc_t 		*process;

	iotx_printf(KSYST, "Run cyclically.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc < 3) {
		error = KERRINA;
	}
	else {
		strg_cnvtDecAsciiToBufInt32((int32_t *)&vTime, argv[2], 1);

		vCommManager = KDEF0;
		if (argc == 4) { vCommManager = GET_PTR_32(argv[3]); }

		if (syos_getNameModule(KMEM0, argv[1], &index, &module) != KSYOSNOERR) {
			if (syos_getNameModule(KMEM1, argv[1], &index, &module) != KSYOSNOERR) {
				error = KERRPNE;
			}
		}
	}
	
	switch (error) {
		case KERRNOT: {
			vTool = module->oExecution;
			PROC_SUPV_STACKMALLOC(0, KINTERNAL, vSpecification, aStrText, KSZSTACKMIN, _process, aStrIden, KDEF0, (KNORMALPRIORITY+1), 0, KPROCNORMAL);

			if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR)                 { status = EXIT_FAILURE;    break; }
			else	  { iotx_printf(KSYST, "Process launched.\n\n");				    			 status = EXIT_SUCCESS_OS; break; }
		}
		case KERRINA: { iotx_printf(KSYST, "Incorrect arguments.\n\n"); 							 status = EXIT_FAILURE;    break; }
		case KERRPNE: { iotx_printf(KSYST, "The selected program does not exist.\n\n");				 status = EXIT_FAILURE;    break; }
		case KERRUMB: { iotx_printf(KSYST, "The user memory is busy by a running application.\n\n"); status = EXIT_FAILURE;    break; }
		default:	  {																				 status = EXIT_FAILURE;    break; }
	}
	return (status);
}

/*
 * \brief _process
 *
 */
static	void	_process(const void *argument) {
	static		int32_t		(*code)(uint32_t argc, const char_t *argv[]);
	volatile	proc_t 		*process;

	INTERRUPTION_OFF;
	kern_getProcessRun(&process);
	process->oSpecification.oCommManager = vCommManager;
	INTERRUPTION_RESTORED;

	code = vTool;
	while (TRUE) {
		kern_suspendProcess(vTime);
		(*code)(1, NULL);
	}
}

