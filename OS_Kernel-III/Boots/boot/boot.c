/*
; boot.
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
; Goal:		Bootstrap of the system
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

#define	KIDALIVE	((KIDPROCESS<<24) | (KALIVENUM<<8) | '_')

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"boot         Install all the system processes.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Boot\n"
									"====\n\n"

									"This code install all the available system processes.\n\n";

LOC_CONST_STRG(aStrIden[])        = "Process_boot";
LOC_CONST_STRG(aStrText[])        = "Process boot: launch all the processes.   (c) EFr-2017";

// Prototypes
// ==========

static	void	_process(const void *argument);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Boot, KIDSTARTUP, KBOOTNUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Main entry point
 *
 * - Create the boot process
 * - Launch all the system processes
 * - Process boot commits a suicide (by the exit)
 *
 */
int32_t		boot(void) {
	volatile	proc_t 	*process;

	comm_setCommDev(KDEF0);

// Initialize the kernel
// Install the process boot

	INTERRUPTION_OFF;
	kern_init();

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, _process, aStrIden, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR) { exit(EXIT_PANIC); }

// Start the time sharing
// Set the system interruptions
// Leave the "first" process

	kern_runKernel();
	INTERRUPTION_SET;

	kern_switchFast();
	return (EXIT_SUCCESS_OS);
}

static	void	_process(const void *argument) {
					uint16_t	index = 0;
					uint32_t	idModule, argc = 3;
			const	module_t	*module;
	static	const	char_t		*argv_alive[] = { "alive", "50", "950" };

// Install all the processes (but not the ALIVE)

	while (syos_getFamilyModule(KMEM0, KIDPROCESS, &idModule, &index, &module) == KSYOSNOERR) {
		if (idModule != KIDALIVE) {
			module->oExecution(0, NULL);
		}
		index++;
	}

// Install the process ALIVE
// Process boot commits a suicide (by the exit)

	if (syos_getIdModule(KMEM0, KIDALIVE, &index, &module) == KSYOSNOERR) {
		module->oExecution(argc, argv_alive);
	}
	exit(EXIT_SUCCESS);
}

