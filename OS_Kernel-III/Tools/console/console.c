/*
; console.
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
; Goal:		Open a console prompt (multi-user).
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

#define	KLNINPBUF	128

static				char_t		vCommand[KLNINPBUF];
static	volatile	uint32_t	vCommManager;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"console      Console prompt.                           (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Console\n"
									"=======\n\n"

									"This function module controls the system with a\n"
									"standard ASCII protocol. Different function modules\n"
									"as well as different tool modules can be executed.\n\n"

									"Input format:  console commManager\n"
									"Output format: uKOS >\n\n"

									"Protocol mode\n"
									"   Input format:  ID_ORDER,parm1,parm2,..,parmN\n"
									"   Output format: id_order,parm1,parm2,..,parmN\n\n"

									"Tool mode\n"
									"   Input format:  name [parms ..]\n"
									"   Output format: [result]\n\n";

#define	KPOS0DEV	16			// __________________¡____
LOC_STATI_STRG(vStrText_0[])      = "Process console def0.                     (c) EFr-2017";
LOC_STATI_STRG(vStrText_1[])      = "Process console def0.                     (c) EFr-2017";
LOC_STATI_STRG(vStrText_2[])      = "Process console def0.                     (c) EFr-2017";
LOC_STATI_STRG(vStrText_3[])      = "Process console def0.                     (c) EFr-2017";

#define	KPOS1DEV	8			// __________¡____________
LOC_STATI_STRG(vStrIden_0[])      = "Console_def0";
LOC_STATI_STRG(vStrIden_1[])      = "Console_def0";
LOC_STATI_STRG(vStrIden_2[])      = "Console_def0";
LOC_STATI_STRG(vStrIden_3[])      = "Console_def0";
LOC_STATI_STRG(vDevice[]) 	      = "def0"; 

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_copyDev(char_t *destination, const char_t *source);
static	int32_t	_execute(uint32_t argc, char_t *argv[]);
static	void	_process(const void *argument);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Console, KIDTOOL, KCONSOLENUM, prgm, "1.2", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				enum 		{ KERRNOT, KERRINA, KERRNCO } error = KERRNOT;
				int32_t		status;
				uint16_t	i;
				uint32_t	*work;
	volatile	proc_t 		*process;
	static		uint8_t		vNbConsole = 0;

	if (vNbConsole == 3) {
		error = KERRNCO;
	}
	if (argc < 2) {
		error = KERRINA;
	}
	else {

		vCommManager = GET_PTR_32(argv[1]);		

		work  = (void *)&vDevice[0];
		*work = vCommManager;

		switch (argc) {
			case 3:  {
				text_copyAsciiBufferZ(vCommand, argv[2]);
				break;
			}
			default: {
				for (i = 0; i < KLNINPBUF; i++) {
					vCommand[i] = '\0';
				}
				break;
			}
		}
	}

	switch (error) {
		case KERRNOT: {

			switch (vNbConsole) {
				case 0: {
					_copyDev(&vStrText_0[KPOS0DEV], vDevice);
					_copyDev(&vStrIden_0[KPOS1DEV], vDevice);
					PROC_SUPV_STACKMALLOC(0, KINTERNAL, vSpecification_0, vStrText_0, KSZSTACKMIN, _process, vStrIden_0, KDEF0, (KNORMALPRIORITY+1), 0, KPROCNORMAL);

					if (kern_createProcess(&vSpecification_0, NULL, &process) != KKERNNOERR) {               status = EXIT_FAILURE;    }
					else                                                              		 { vNbConsole++; status = EXIT_SUCCESS_OS; }
					break;
				}
				case 1: {
					_copyDev(&vStrText_1[KPOS0DEV], vDevice);
					_copyDev(&vStrIden_1[KPOS1DEV], vDevice);
					PROC_SUPV_STACKMALLOC(1, KINTERNAL, vSpecification_1, vStrText_1, KSZSTACKMIN, _process, vStrIden_1, KDEF0, (KNORMALPRIORITY+1), 0, KPROCNORMAL);

					if (kern_createProcess(&vSpecification_1, NULL, &process) != KKERNNOERR) {               status = EXIT_FAILURE;    }
					else                                                              		 { vNbConsole++; status = EXIT_SUCCESS_OS; }
					break;
				}
				case 2: {
					_copyDev(&vStrText_2[KPOS0DEV], vDevice);
					_copyDev(&vStrIden_2[KPOS1DEV], vDevice);
					PROC_SUPV_STACKMALLOC(2, KINTERNAL, vSpecification_2, vStrText_2, KSZSTACKMIN, _process, vStrIden_2, KDEF0, (KNORMALPRIORITY+1), 0, KPROCNORMAL);

					if (kern_createProcess(&vSpecification_2, NULL, &process) != KKERNNOERR) {               status = EXIT_FAILURE;    }
					else                                                               		 { vNbConsole++; status = EXIT_SUCCESS_OS; }
					break;
				}
				case 3: {
					_copyDev(&vStrText_3[KPOS0DEV], vDevice);
					_copyDev(&vStrIden_3[KPOS1DEV], vDevice);
					PROC_SUPV_STACKMALLOC(3, KINTERNAL, vSpecification_3, vStrText_3, KSZSTACKMIN, _process, vStrIden_3, KDEF0, (KNORMALPRIORITY+1), 0, KPROCNORMAL);

					if (kern_createProcess(&vSpecification_3, NULL, &process) != KKERNNOERR) {               status = EXIT_FAILURE;    }
					else                                                              		 { vNbConsole++; status = EXIT_SUCCESS_OS; }
					break;
				}
				default: {
					status = EXIT_FAILURE;
					break;
				}
			}
			break;
		}
		case KERRNCO: { iotx_printf(KSYST, "Too many open consoles.\n\n"); status = EXIT_FAILURE; break; }
		case KERRINA: { iotx_printf(KSYST, "Incorrect arguments.\n\n");	   status = EXIT_FAILURE; break; }
		default:	  {													   status = EXIT_FAILURE; break; }
	}
	return (status);
}

// Local routines
// ==============

/*
 * \brief _process
 *
 */
static	void	_process(const void *argument) {
				char_t		command[KLNINPBUF], *argv[KLNINPBUF];
				uint16_t	i;
				uint32_t	argc;
				int32_t		status;
	volatile	proc_t		*process;

	INTERRUPTION_OFF;
	kern_getProcessRun(&process);
	process->oSpecification.oCommManager = vCommManager;

	for (i = 0; i < KLNINPBUF; i++) {
		command[i] = '\0';
	}
	text_copyAsciiBufferZ(command, (const char_t *)vCommand);
	INTERRUPTION_RESTORED;

	iotx_printf(KSYST, "Console.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	text_getArgs(command, KLNINPBUF, argv, &argc);
	if (argc != 0) {
		_execute(argc, argv);
	}

	while (TRUE) {
		iotx_printf(KSYST, "uKOS > ");

// Waiting for a command line
// Extract the parameters
// Execute the command

		iotx_waitString(KSYST, command, KLNINPBUF);
		text_getArgs(command, KLNINPBUF, argv, &argc);

		status = _execute(argc, argv);
		switch (status) {
			case EXIT_SUCCESS: {
				exit(EXIT_SUCCESS);
				break;
			}
			case EXIT_SUCCESS_OS: {
				break;
			}
			case EXIT_FAILURE: {
				break;
			}
			case EXIT_FAILURE_CRT0: {
				iotx_printf(KSYST, "Incompatible OS!!!\nReload the latest OS inside the target.\n");
				break;
			}
			default:
			case EXIT_PANIC: {
				exit(EXIT_PANIC);
				break;
			}
		}
	}
}

// Local routines
// ==============

/*
 * \brief _execute
 *
 * - Execute a module
 *
 */
static	int32_t	_execute(uint32_t argc, char_t *argv[]) {
			uint16_t	index;
			bool_t		error = FALSE;
			int32_t		status;
	const	module_t	*module;

// Look for the selected module; if it exists, execute it
// The system has an MMX1 memory

	#if (defined(__MEM_MMX1__))
	if (syos_getNameModule(KMEM0, argv[0], &index, &module) != KSYOSNOERR) {
		if (syos_getNameModule(KMEM1, argv[0], &index, &module) != KSYOSNOERR) {
			error = TRUE;
		}
	}
	
// The system has not an MMX1 memory

	#else
	if (syos_getNameModule(KMEM0, argv[0], &index, &module) != KSYOSNOERR) {
		error = TRUE;
	}
	#endif

	if (error || !(module->oFlag & (1<<BEXECONSOLE))) {
		error = TRUE;
		iotx_printf(KSYST, "Module not found or user memory busy by a running application.\n\n");
	}

	if (error) {
		status = EXIT_FAILURE;
	}
	else {
		status = module->oExecution(argc, (void *)argv);
	}
	return (status);
}

/*
 * \brief _copyDev
 *
 * - Copy the device name
 *
 */
static	void	_copyDev(char_t *destination, const char_t *source) {
	uint8_t		i;

   	#if (defined(__LITTLE_ENDIAN__))
   	for (i = 0; i < 4; i++) { destination[i] = *(source + 3 - i); }

	#else
	for (i = 0; i < 4; i++) { destination[i] = *(source + i); }
   	#endif
}

