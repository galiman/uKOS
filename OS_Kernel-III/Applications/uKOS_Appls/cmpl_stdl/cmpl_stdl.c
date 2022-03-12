/*
; cmpl_stdl.
; ==========

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
; Goal:		Demo of a small application using the uKOS uKernel.
;
;			Launch 3 processes:
;
;			- Process: alive
;			- Process: blink
;			- Process: display the system statistic
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

#define	KLEDIDLE	3

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"cmpl_stdl    Stand-alone application using the uKernel (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  cmpl_stdl\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

extern	void	installaProcess_alive(void);
extern	void	installaProcess_blink(void);
extern	void	installaProcess_statistic(void);
static	void	_myRoutine(uint8_t state);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief main
 *
 * - Setup the exceptions
 * - Initialize the hardware
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {

	INTERRUPTION_OFF;
	installaProcess_alive();
	installaProcess_blink();
	installaProcess_statistic();

	kern_installCallBack(_myRoutine);
	kern_runKernel();
	INTERRUPTION_SET;
	kern_switchFast();

	return (EXIT_SUCCESS_OS);
}

// Local routines
// ==============

/*
 * \brief _myRoutine
 * 
 * - Callback routine (called by the idle daemon)
 *
 */
static	void	_myRoutine(uint8_t state) {

	if (state == KINIDLE) { hard_onLed(KLEDIDLE);  }
	else			      { hard_offLed(KLEDIDLE); }
}

