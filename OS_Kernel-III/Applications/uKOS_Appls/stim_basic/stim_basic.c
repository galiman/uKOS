/*
; stim_basic.
; ===========

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
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 1 processes:
;
;			- P0: Every 1000-ms
;				  Print a message
;				  Install the software timers
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

LOC_CONST_STRG(aStrApplication[]) =	"stim_basic    Example of how to use a software timet.  (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  stim_basic\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	void	_changeStateLed(const void *argument);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Install the software timers
 *		 - Every 1000-ms
 *  		- Print a message
 *
 */
static	void	aProcess_0(const void *argument) {
				tspc_t 		configure_0, configure_1;
	volatile	stim_t		*softwareTimer_0, *softwareTimer_1;
	const		uint8_t		argument_0[1] = { 5 }, argument_1[1] = { 2 };

	configure_0.oMode     = KCONTINUE;
	configure_0.oIniTime  = 10000;
	configure_0.oTime     = 500;
	configure_0.oCode     = _changeStateLed;
	configure_0.oArgument = &argument_0[0];

	configure_1.oMode     = KCONTINUE;
	configure_1.oIniTime  = 5000;
	configure_1.oTime     = 250;
	configure_1.oCode     = _changeStateLed;
	configure_1.oArgument = &argument_1[0];

	if (kern_createSoftwareTimer("Blink Led 0", &softwareTimer_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_setSoftwareTimer(softwareTimer_0, &configure_0)  	  != KKERNNOERR) { exit(EXIT_FAILURE); }

	if (kern_createSoftwareTimer("Blink Led 1", &softwareTimer_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_setSoftwareTimer(softwareTimer_1, &configure_1)  	  != KKERNNOERR) { exit(EXIT_FAILURE); }

	while (TRUE) {
		kern_suspendProcess(1000);
		iotx_printf(KSYST, "1st software timer test\n");
	}
}

// Routine cyclically called by a software timers

static	void	_changeStateLed(const void *argument) {
	const	uint8_t		*led;
	
	led = argument;
	misc_toggleLed(*led);
}

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {
	volatile	proc_t 	*process_0;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_Software_Timer";
	LOC_CONST_STRG(aStrText_0[]) =    "Process Software Timer.                   (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

