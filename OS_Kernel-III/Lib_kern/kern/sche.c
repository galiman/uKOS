/*
; sche.
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
; Goal:		Kern - Scheduler management.
;
;			This module is responsible for controlling the logic of the uKernel.
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

enum	{
		KFORCENORM,														// Normal case of context switch
		KFORCELOCK,														// Normal case of context switch (but locked)
		KNOFORNORM,														// Normal process timeout
		KNOFORLOCK														// Normal process timeout (but locked)
		};

extern	const		void		**vKern_ptrGlobal;						// Ptr on a global variable
extern				uint32_t	vKern_stackProcess;						// Process stack (entry point by Trap)
extern	volatile	sema_t		vKern_semaphore[KNBSEMAPHO];			// Semaphores
extern	volatile	list_t		vKern_listWait;							// Wait list (processes waiting for a time)
extern	volatile	list_t		vKern_listExec;							// Execution list for all the priorities
extern	volatile	list_t		vKern_listEvnt;							// Event list (processes waiting for a software event)
extern	volatile	list_t		vKern_listSign;							// Signal list (processes waiting for a software signal)
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
extern	volatile	proc_t		*vKern_backwardProcess;					// Ptr on the backward process

// Prototypes
// ==========

static				void	_nextAction(uint8_t action, volatile list_t *list, uint8_t bitNb);
static	volatile	proc_t	*_getNextProcess(void);

/*
 * \brief Connect the current process to a list and change the context
 *
 * \param[in]	force	FALSE process timeout
 * \param[in]	force	TRUE force to change the context
 * \param[in]	*list	Ptr on the list
 * \param[in]	bitNb	bit number (flag for suspending)
 * \param[out]	-
 *
 */
void	sche_changeContext(bool_t force, volatile list_t *list, uint8_t bitNb) {

	vKern_backwardProcess = vKern_current;
	vKern_current->oInternal.oState &= ~(1<<BPROCRUNNING);

// force == TRUE,  Context switching due to a specific demand
// force == FALSE, Context switching due to an event

	if (force) { _nextAction(KFORCENORM, list, bitNb); }
	else       { _nextAction(KNOFORNORM, list, bitNb); }

// Reload the global pointer (i.e. for the newlib multi-thread)
// Load the new stack

	if (vKern_current->oInternal.oLocal != NULL) {
		*vKern_ptrGlobal = vKern_current->oInternal.oLocal;
	}
	vKern_stackProcess = (uint32_t)vKern_current->oSpecification.oStkSup;

// If the process is an overlay, load it

	#if (defined(__WITHOVLY__))
	if (vKern_current->oSpecification.oKind == KPROCOVERLAY) {
		ovly_loadOverlay(vKern_current->oSpecification.oIdPack);
	}
	#endif

// New time for this process
// Save the scheduled time

	vKern_current->oInternal.oState |= (1<<BPROCRUNNING);
	stub_kern_timeNext();
}

/*
 * \brief _nextAction
 *
 * The next action could be:
 * - KFORCENORM: Context switching due to a specific demand
 * - KNOFORNORM: Context switching due to an event
 *
 */
static	void	_nextAction(uint8_t action, volatile list_t *list, uint8_t bitNb) {
	volatile	proc_t		*process;

	switch (action) {

// Context switching due to a specific demand
// - Save the stack of the current process
// - Schedule the most priority process

		case KFORCENORM: {
			process = vKern_current;
			process->oSpecification.oStkSup = (uint32_t *)vKern_stackProcess;
			vKern_current = _getNextProcess();
			break;
		}

// Context switching due to an event
// - Save the stack of the current process
// - Disconnect the current process from the execution list
// - Connect the current process to the "list" list
// - Schedule the most priority process

		case KNOFORNORM: {
			process = vKern_current;
			process->oInternal.oState |= (1<<bitNb);
			process->oSpecification.oStkSup = (uint32_t *)vKern_stackProcess;

// NEED to be TESTED
			#if 0
			chls_disconnect(&vKern_listExec, process);

			#else
			chls_disconnect(process->oObject.oList, process);
			#endif

			chls_connect(list, process);
			vKern_current = _getNextProcess();
			break;
		}
		default: {
			break;
		}
	}
}

/*
 * \brief Determine the next process to run f(priority)
 *
 *  Process     	Pri.	Pri.	Pri.	Pri.	Pri.	Pri
 *  P1 256 (Idle)   256		256		256		256		256		256
 *  P2 5       		4       3       2		1		5		4
 *  P5 4   			3       2       4		3		2		4
 *  P6 6         	2       6       5		4		3		2
 *  P8 3         	3       2       1		3		2		1
 *
 *  Run     		P6      P5      P8		P2		P5		P8 ..
 *
 * - Scan the "execution" list and search for the process having the highest dynamic priority (smallest value)
 * - If more processes have the same priority, select the process that was waiting for the longer time (Skip counter)
 *   - Increment the counter Skip for all the processes in competition
 *
 * - Decrement the dynamic priority of all the processes of the list
 * - For the selected process, set the dynamic priority with the static one
 *
 */
static	volatile proc_t	*_getNextProcess(void) {
				uint16_t	i, j;
				enum		{ KBIGGER, KLOWER, KEQUAL } action;
	volatile	proc_t		*process, *priorityProcess;

	j = vKern_listExec.oNbElements;
	if (j == 1) {
		return (vKern_listExec.oFirst);
	}

	process         = vKern_listExec.oFirst;
	priorityProcess = vKern_listExec.oFirst;

	for (i = 0; i < (j-1); i++) {

// Determine the next action

		if (priorityProcess->oInternal.oDynaPriority > process->oObject.oForward->oInternal.oDynaPriority)      { action = KBIGGER; }
		else if (priorityProcess->oInternal.oDynaPriority < process->oObject.oForward->oInternal.oDynaPriority) { action = KLOWER;  }
		 	 else 																		  				        { action = KEQUAL;  }

		switch (action) {

// Case where the P(x) > P(i)
// i = 0 means idle process; do not decrement the dynamic priority

			case KBIGGER: {
				if ((priorityProcess->oInternal.oDynaPriority > 0) && (i > 0)) {
					 priorityProcess->oInternal.oDynaPriority--;
				}
				priorityProcess = process->oObject.oForward;
				break;
			}

// Case where the P(x) < P(i)
// i = 0 means idle process; do not decrement the dynamic priority

			case KLOWER: {
				if (process->oObject.oForward->oInternal.oDynaPriority > 0) {
					process->oObject.oForward->oInternal.oDynaPriority--;
				}
				break;
			}

// Case where the P(x) = P(i)
// i = 0 means idle process; do not decrement the dynamic priority

			case KEQUAL: {
				if (priorityProcess->oInternal.oSkip < process->oObject.oForward->oInternal.oSkip) {
					if ((priorityProcess->oInternal.oDynaPriority > 0) && (i > 0)) {
						 priorityProcess->oInternal.oDynaPriority--;
					}
					priorityProcess->oInternal.oSkip++;
					priorityProcess = process->oObject.oForward;
				}
				else {
					if (process->oObject.oForward->oInternal.oDynaPriority > 0) {
						process->oObject.oForward->oInternal.oDynaPriority--;
					}
					process->oObject.oForward->oInternal.oSkip++;
				}
				break;
			}
			default: {
				break;
			}
		}
		process = process->oObject.oForward;
	}
	priorityProcess->oInternal.oSkip = 0;
	priorityProcess->oInternal.oDynaPriority = priorityProcess->oSpecification.oPriority;
	return (priorityProcess);
}

// The callback routines
// =====================

/*
 * \brief sche_callBackFast
 *
 * - Verify the timeout condition of the suspended processes
 *
 */
void	sche_callBackFast(void) {

	temp_testEOTime();
}

/*
 * \brief sche_callBackSlow
 *
 * - Change the context and prepare the next process
 * - INT acknowledge and new time for the next process
 *
 */
void	sche_callBackSlow(void) {

	INTERRUPTION_SET_PERIPH;
	sche_changeContext(TRUE, NULL, 0);
}

/*
 * \brief sche_callBackTrap
 *
 * - Analysis of the message
 *
 * - Basic services: KNOPARAM
 *	 - KJUMPKERN	force the process switching
 *	 - KWAITTIME	waiting for a time
 *	 - KWAITEVNT	waiting for an event
 *	 - KWAITSIGN	waiting for a signal
 *
 * - Semaphore synchronizations: KSEMASYN
 *	 - KSEMASYN		waiting for a semaphore synchro
 *
 */
void	sche_callBackTrap(uint32_t message) {

	switch (message & 0xFFFF0000) {

// Messages without parameters
// - Basic services

		case KNOPARAM: {
			switch (message) {
				case KJUMPKERN: {

					INTERRUPTION_SET_PERIPH;
					sche_changeContext(TRUE, NULL, 0);
					break;
				}
				case KWAITTIME: {

					INTERRUPTION_SET_PERIPH;
					sche_changeContext(FALSE, &vKern_listWait, BPROCSUSPTIME);
					break;
				}

				#if (defined(__WITHEVNT__))
				case KWAITEVNT: {
					sche_changeContext(FALSE, &vKern_listEvnt, BPROCSUSPEVNT);
					break;
				}
				#endif

				#if (defined(__WITHSIGN__))
				case KWAITSIGN: {
					sche_changeContext(FALSE, &vKern_listSign, BPROCSUSPSIGN);
					break;
				}
				#endif

				default: {
					break;
				}
			}
			break;
		}

// Messages with parameters
// - Semaphore synchronizations

		case KSEMASYN:
			sche_changeContext(FALSE, &vKern_semaphore[(message & 0x0000FFFF)].oList, BPROCSUSPSEMA);
			break;

		default: {

			INTERRUPTION_SET_PERIPH;
			sche_changeContext(TRUE, NULL, 0);
			break;
		}
	}
}
