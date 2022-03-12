/*
; daem.
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
; Goal:		Kern - Daemon management.
;
;			This module launches all the daemons responsible for managing
;			some uKernel stuff (i.e. idle state, timeout management, etc.).
;
; 			Daemon system calls
; 			-------------------
;
;			void	daem_init(void);
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

#include 	<uKOS.h>

extern				void		(*vKern_codeRoutine)(uint8_t state);	// Ptr on the Code for the call-back routine
extern	volatile	proc_t		vKern_process[KNBPROCESS];				// User processes, Id = 1..2^31; Id = IDLE if 1
extern	volatile	stim_t		vKern_softTimer[KNBSTIM];				// Software timers
extern	volatile	list_t		vKern_listExec;							// Execution list for all the priorities

// Prototypes
// ==========

static	void	_install(void);

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "deam" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	daem_init(void) {

	TRACE("entry: daem_init");
	_install();
	TRACE("exit: daem_init OK");
}

// Local routines
// ==============

/*
 * \brief _process_idle
 *
 * - This daemon is scheduled only when the execution list is empty.
 *   This occurs when all the system processes are in the waiting state
 *   (waiting for a time, for a sync, for a semaphore, etc.).
 *
 */
static	void	_process_idle(const void *argument) {
	void	(*code)(uint8_t state);

	TRACE("entry: idle daemon");

	while (TRUE) {
		code = vKern_codeRoutine;

// Signal the entry of the idle process
// CPU in a stop mode
// Signal the output of the idle process

		code(KINIDLE);
		stub_kern_setLowPower(KCPUPSTOP);
		code(KOUTIDLE);
		kern_switchFast();
	}
}

/*
 * \brief _process_timeout
 *
 * - This daemon is scheduled cyclically in order to scan the lists
 *   containing suspended processes for verifying the timeout.
 *   As soon as a timeout is reached, the corresponding process is
 *   moved into the execution list.
 *
 */
static	void	_process_timeout(const void *argument) {
	uint16_t	i;

	TRACE("entry: timeout management daemon");

	while (TRUE) {
		kern_suspendProcess(KTIMETIMEOUT);

		for (i = 0; i < KNBPROCESS; i++) {

			INTERRUPTION_OFF;
			if ((vKern_process[i].oInternal.oState & ((1<<BPROCSUSPEVNT) | (1<<BPROCSUSPSIGN) | (1<<BPROCSUSPSEMA))) != 0) {
				if (vKern_process[i].oInternal.oTimeout != 0xFFFFFFFF) {
					if (vKern_process[i].oInternal.oTimeout == 0) {
						chls_disconnect(vKern_process[i].oObject.oList, &vKern_process[i]);
						chls_connect(&vKern_listExec, &vKern_process[i]);
						vKern_process[i].oInternal.oState &= ~((1<<BPROCSUSPEVNT) | (1<<BPROCSUSPSIGN) | (1<<BPROCSUSPSEMA));
						vKern_process[i].oInternal.oStatus = KKERNTIMEO;
					}
					else {
						vKern_process[i].oInternal.oTimeout = (vKern_process[i].oInternal.oTimeout <= (KTIMETIMEOUT/KTIMEUNIT)) ? 0 : vKern_process[i].oInternal.oTimeout - (KTIMETIMEOUT/KTIMEUNIT);
					}
				}
			}
			INTERRUPTION_RESTORED;

		}
	}
}

/*
 * \brief _process_stack
 *
 * - This daemon is scheduled cyclically for verifying the stack
 *   integrity (if there is enough place).
 *
 */
static	void	_process_stack(const void *argument) {
				bool_t		terminate;
				uint16_t	i;
	volatile	uint32_t	*stkSup, szStkSup, j;

	TRACE("entry: stack integrity daemon");

	while (TRUE) {
		kern_suspendProcess(KTIMESTACKIN);

		for (i = 0; i < KNBPROCESS; i++) {
			if ((vKern_process[i].oInternal.oState & (1<<BPROCINSTALLED)) != 0) {

				stkSup   = vKern_process[i].oSpecification.oStkSupBegin + vKern_process[i].oSpecification.oSzStk;
				szStkSup = vKern_process[i].oSpecification.oSzStk;

// Compute the length of the available process stack in bytes

				j = 0; terminate = FALSE;
				do {
					if (*(stkSup-1-j) == (('u'<<24) | ('K'<<16) | ('O'<<8) | 'S')) {
						vKern_process[i].oStatistic.oAvStkSup = (szStkSup-j) * 4;
						terminate = TRUE;
					}
				} while ((++j < szStkSup) && (!terminate));
			}
		}
	}
}

#if (defined(__WITHSTIM__))
/*
 * \brief _process_software_timer
 *
 * - This daemon is scheduled cyclically for the management of
 *   the software timers.
 *
 */
static	void	_process_software_timer(const void *argument) {
	uint16_t	i;
	void		(*code)(const void *argument);

	TRACE("entry: software timer daemon");

	while (TRUE) {
		kern_suspendProcess(KTIMESOFTTIMER);

		for (i = 0; i < KNBSTIM; i++) {
			if ( ((vKern_softTimer[i].oState & (1<<BSTIMINSTALLED)) != 0) && ((vKern_softTimer[i].oState & (1<<BSTIMCONFIGURED)) != 0)) {

// Execute only if mode = Continue
// or, if mode = SingleShot but never executed

				if (( vKern_softTimer[i].oTimerSpec.oMode == KCONTINUE) ||
				    ((vKern_softTimer[i].oTimerSpec.oMode == KSINGLESHOT) && ((vKern_softTimer[i].oState & (1<<BSTIMEXECUTED)) == 0))) {

// Verify the initial time
// Verify the time

					if (vKern_softTimer[i].oInitCounter != 0) {
						vKern_softTimer[i].oInitCounter--;
					}
					else {
						if (vKern_softTimer[i].oCounter-- == 0) {
							vKern_softTimer[i].oCounter = vKern_softTimer[i].oTimerSpec.oTime;
							code = vKern_softTimer[i].oTimerSpec.oCode;
							code(vKern_softTimer[i].oTimerSpec.oArgument);
							
							vKern_softTimer[i].oState &= (vKern_softTimer[i].oTimerSpec.oMode == KSINGLESHOT) ? (~(1<<BSTIMRUNNING)) : (~(0x00));
							vKern_softTimer[i].oState |= (1<<BSTIMEXECUTED);
						}
					}
				}
			}
		}
	}
}
#endif

/*
 * \brief _install
 *
 * - This function installs all the system daemons
 *
 * \warning the first process or daemon installed has to be the "idle"
 *
 */
static	void	_install(void) {
	volatile	proc_t 	*process_idle, *process_timeout, *process_stack;

	#if (defined(__WITHSTIM__))
	volatile	proc_t 	*process_software_timer;
	#endif

// ---------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_idle[])           = KDAEMON_IDLE;
	LOC_CONST_STRG(aStrIden_timeout[])        = KDAEMON_TIMEOUT;
	LOC_CONST_STRG(aStrIden_stack[])          = KDAEMON_STACK;
	LOC_CONST_STRG(aStrText_idle[])           = "Daemon, idle process.                     (c) EFr-2017";
	LOC_CONST_STRG(aStrText_timeout[])        = "Daemon, supervise the process timeout.    (c) EFr-2017";
	LOC_CONST_STRG(aStrText_stack[])          = "Daemon, verify the stack integrity.       (c) EFr-2017";

	#if (defined(__WITHSTIM__))
	LOC_CONST_STRG(aStrIden_software_timer[]) = KDAEMON_SOFTWARE_TIMER;
	LOC_CONST_STRG(aStrText_software_timer[]) = "Daemon, software timer management.        (c) EFr-2017";
	#endif

// Specifications for the processes

	PROC_SUPV(0, vSpecification_idle,           aStrText_idle,           KSZSTACKMIN, _process_idle,           aStrIden_idle,           KDEF0, (KLOWPRIORITY+7), 0, KPROCDAEMON);
	PROC_SUPV(1, vSpecification_timeout,        aStrText_timeout,        KSZSTACKMIN, _process_timeout,        aStrIden_timeout,        KDEF0, (KLOWPRIORITY+1), 0, KPROCDAEMON);
	PROC_SUPV(2, vSpecification_stack,          aStrText_stack,          KSZSTACKMIN, _process_stack,          aStrIden_stack,          KDEF0, (KLOWPRIORITY+1), 0, KPROCDAEMON);

	#if (defined(__WITHSTIM__))
	PROC_SUPV(3, vSpecification_software_timer, aStrText_software_timer, KSZSTACKMIN, _process_software_timer, aStrIden_software_timer, KDEF0, (KLOWPRIORITY+1), 0, KPROCDAEMON);
	#endif

	if (kern_createProcess(&vSpecification_idle,           NULL, &process_idle)           != KKERNNOERR) { exit(EXIT_PANIC); }
	if (kern_createProcess(&vSpecification_timeout,        NULL, &process_timeout)        != KKERNNOERR) { exit(EXIT_PANIC); }
	if (kern_createProcess(&vSpecification_stack,          NULL, &process_stack)          != KKERNNOERR) { exit(EXIT_PANIC); }

	#if (defined(__WITHSTIM__))
	if (kern_createProcess(&vSpecification_software_timer, NULL, &process_software_timer) != KKERNNOERR) { exit(EXIT_PANIC); }
	#endif
}
