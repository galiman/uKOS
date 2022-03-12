/*
; sync.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 178                  $:  Revision of last commit
; $Date:: 2017-07-15 21:15:17#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Kern - Synchro event management.
;
;			This module implements the software synchro primitives.
;
; 			Software event system calls
; 			---------------------------
;
;			void	sync_init(void);
;			int32_t	sync_createEvent(const char_t *identifier, volatile evnt_t **handle);
;			int32_t	sync_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent);
;			int32_t	sync_signalEvent(uint32_t bitEvent, uint32_t mode);
;			int32_t	sync_waitEvent(uint32_t *bitEvent, uint32_t timeout);
;			int32_t	sync_killEvent(volatile evnt_t *handle);
;			int32_t	sync_getEventById(const char_t *identifier, volatile evnt_t **handle);
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

#if (defined(__WITHEVNT__))
extern	volatile	proc_t		vKern_process[KNBPROCESS];				// User processes, Id = 1..2^31; Id = IDLE if 1
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
extern	volatile	list_t		vKern_listExec;							// Execution list for all the priorities
		volatile	list_t		vKern_listEvnt;							// Event list (processes waiting for a software event)
		volatile	evnt_t		vKern_event[KNBEVENTS];					// Events

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "sync" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	sync_init(void) {
	uint16_t	i;

	TRACE("entry: sync_init");
	for (i = 0; i < KNBEVENTS; i++) {
		vKern_event[i].oIdentifier = NULL;
		vKern_event[i].oState      = 0;
		vKern_event[i].oBitEvent   = (1<<i);
	}

	chls_initialize(&vKern_listEvnt);
	TRACE("exit: sync_init OK");
}

/*
 * \brief Create an event
 *
 * \param[in]	*identifier	Ptr on the event Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNEVFUL 	No more event
 * \param[out]	KKERNIDEVO	The event Id is already used
 *
 */
int32_t	sync_createEvent(const char_t *identifier, volatile evnt_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: sync_createEvent");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBEVENTS; i++) {
		if (buff_cmpStrings(vKern_event[i].oIdentifier, identifier)) { TRACE("exit: sync_createEvent KO 1"); RETURN_INT_RESTORED(KKERNIDEVO); }
	}

	for (i = 0; i < KNBEVENTS; i++) {
		if (vKern_event[i].oIdentifier == NULL) {
			vKern_event[i].oState      |= (1<<BEVNTINSTALLED);
			vKern_event[i].oIdentifier  = identifier;
			*handle = &vKern_event[i];
			TRACE("exit: sync_createEvent OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}
	}
	TRACE("exit: sync_createEvent KO 2");
	RETURN_INT_RESTORED(KKERNEVFUL);
}

/*
 * \brief Get the bit event
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*bitEvent	Ptr on the bitEvent
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOEVO 	The event does not exist
 * \param[out]	KKERNEVNOI 	The event is not initialized
 *
 */
int32_t	sync_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent) {
	volatile	evnt_t		*event;

	INTERRUPTION_OFF;
	TRACE("entry: sync_getBitEvent");
	vKern_current->oStatistic.oNbKernCalls++;
	event = handle;
	if (event == NULL)							    { TRACE("exit: sync_getBitEvent KO 1"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if (event->oIdentifier == NULL)				    { TRACE("exit: sync_getBitEvent KO 2"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if ((event->oState & (1<<BEVNTINSTALLED)) == 0) { TRACE("exit: sync_getBitEvent KO 3"); RETURN_INT_RESTORED(KKERNEVNOI); }

	*bitEvent = event->oBitEvent;
	TRACE("exit: sync_getBitEvent OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Signal events
 *
 * - If (mode == KSYNCSWTC)
 *   - With the context switching
 *
 * - If (mode == KSYNCCONT)
 *   - Without the context switching
 *
 * - Scan all the installed processes
 *   If the process is not suspended
 *   then, save the new event in the process->oSynchro.oListEvents
 *   else, if the process->oSynchro.oListEvents & event !=
 *         then, cancel the matched event and connect the processes to the execution list
 *         else, next 
 *
 * \param[in]	bitEvent	Event (1 or more)
 * \param[in]	mode		KSYNCSWTC -> (synchro with the context switching)
 * \param[in]				KSYNCCONT -> (synchro without the context switching)
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	sync_signalEvent(uint32_t bitEvent, uint32_t mode) {
	uint16_t	i;
	uint32_t	theEvent;

	INTERRUPTION_OFF;
	TRACE("entry: sync_signalEvent");
	vKern_current->oStatistic.oNbKernCalls++;

	for (i = 0; i < KNBPROCESS; i++) {
		if ((vKern_process[i].oInternal.oState & (1<<BPROCINSTALLED)) != 0) {
			vKern_process[i].oSynchro.oListPending |= bitEvent;
			if ((vKern_process[i].oInternal.oState & (1<<BSUSPEVNT)) != 0) {
				theEvent = vKern_process[i].oSynchro.oListEvents & bitEvent;
				if (theEvent != 0) {
					vKern_process[i].oInternal.oState &= ~(1<<BSUSPEVNT);
					chls_disconnect(&vKern_listEvnt, &vKern_process[i]);
					chls_connect(&vKern_listExec, &vKern_process[i]);
					vKern_process[i].oInternal.oStatus = KKERNNOERR;

// mode == KSYNCSWTC
// Context switching (preemption when "INTERRUPTION_RESTORED")

					if (mode == KSYNCSWTC) {
						PREEMPTION;
					}
				}
			}
		}
	}
	TRACE("exit: sync_signalEvent OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Waiting for events
 *
 * - Disconnect a process from the execution list
 * - Connect the process to the event list
 *
 * \param[in]	*bitEvent	Ptr on the event (1 or more)
 * \param[in]	timeout		Timeout
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNEVKIL  The event has been killed (with processes in its list)
 * \param[out]	KKERNTIMEO 	Timeout
 *
 */
int32_t	sync_waitEvent(uint32_t *bitEvent, uint32_t timeout) {
	uint32_t	theEvent;

	INTERRUPTION_OFF;
	TRACE("entry: sync_waitEvent");
	vKern_current->oStatistic.oNbKernCalls++;

	theEvent = vKern_current->oSynchro.oListPending & *bitEvent;

// The event already occurred
// Cancel it and return the event

	if (theEvent != 0) {
		vKern_current->oSynchro.oListPending &= ~theEvent;
		*bitEvent = theEvent;
		RETURN_INT_RESTORED(KKERNNOERR);
	}

// Waiting for the event
// Add the event in the vKern_current->oSynchro.oListEvents
//
// When the process is scheduled, cancel it and return the event

	else {
		if (timeout > 0) {
			vKern_current->oSynchro.oListEvents |= *bitEvent;
			vKern_current->oInternal.oTimeout = (timeout/KTIMEUNIT);

			GOTO_KERN_I(KWAITEVNT);
			theEvent = vKern_current->oSynchro.oListPending & *bitEvent;
			*bitEvent = theEvent;
			vKern_current->oSynchro.oListEvents  &= ~theEvent;
			vKern_current->oSynchro.oListPending &= ~theEvent;

			TRACE("exit: sync_waitEvent ->");
			RETURN_INT_RESTORED(vKern_current->oInternal.oStatus);
		}
		else {
			TRACE("exit: sync_waitEvent KO 1");
			RETURN_INT_RESTORED(KKERNTIMEO);
		}

	}
}

/*
 * \brief Kill the event
 *
 * - If (process still connected to the event list)
 *   - Then return error
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOEVO 	The event does not exist
 * \param[out]	KKERNEVNOI 	The event is not initialized
 *
 */
int32_t	sync_killEvent(volatile evnt_t *handle) {
				uint16_t	i;
	volatile	proc_t		*process;
	volatile	evnt_t		*event;

	INTERRUPTION_OFF;
	TRACE("entry: sync_killEvent");
	vKern_current->oStatistic.oNbKernCalls++;
	event = handle;
	if (event == NULL)							    { TRACE("exit: sync_killEvent KO 1"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if (event->oIdentifier == NULL)				    { TRACE("exit: sync_killEvent KO 2"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if ((event->oState & (1<<BEVNTINSTALLED)) == 0) { TRACE("exit: sync_killEvent KO 3"); RETURN_INT_RESTORED(KKERNEVNOI); }

	if (vKern_listEvnt.oNbElements > 0) {
		for (i = 0; i < vKern_listEvnt.oNbElements; i++) {
			process = vKern_listEvnt.oFirst;
			process->oSynchro.oListEvents  &= ~(event->oBitEvent);
			process->oSynchro.oListPending &= ~(event->oBitEvent);
			if (process->oSynchro.oListEvents == 0) {
				process->oInternal.oState &= ~(1<<BSUSPEVNT);
				chls_disconnect(&vKern_listEvnt, process);
				chls_connect(&vKern_listExec, process);
				process->oInternal.oStatus = KKERNEVKIL;
			}
		}
	}
	event->oIdentifier = NULL;
	event->oState      = 0;
	TRACE("exit: sync_killEvent OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of an event by its Id
 *
 * - This function returns the handle of the event
 *
 * \param[in]	*identifier	Ptr on the event Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOEVO 	The event does not exist
 *
 */
int32_t	sync_getEventById(const char_t *identifier, volatile evnt_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: sync_getEventById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBEVENTS; i++) {
		if (buff_cmpStrings(vKern_event[i].oIdentifier, identifier)) {
			*handle = &vKern_event[i];
			TRACE("exit: sync_getEventById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: sync_getEventById KO");
	RETURN_INT_RESTORED(KKERNNOEVO);
}
#endif

