/*
; evnt.
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
; Goal:		Kern - Event management.
;
;			This module implements the event primitives.
;
; 			Software event system calls
; 			---------------------------
;
;			void	evnt_init(void);
;			int32_t	evnt_createEvent(const char_t *identifier, volatile evnt_t **handle);
;			int32_t	evnt_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent);
;			int32_t	evnt_signalEvent(uint32_t bitEvent, uint32_t mode);
;			int32_t	evnt_waitEvent(uint32_t *bitEvent, uint32_t timeout);
;			int32_t	evnt_killEvent(volatile evnt_t *handle);
;			int32_t	evnt_getEventById(const char_t *identifier, volatile evnt_t **handle);
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
 * - This function initializes the "event" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	evnt_init(void) {
	uint16_t	i;

	TRACE("entry: evnt_init");
	for (i = 0; i < KNBEVENTS; i++) {
		vKern_event[i].oIdentifier = NULL;
		vKern_event[i].oState      = 0;
		vKern_event[i].oBitEvent   = (1<<i);
	}

	chls_initialize(&vKern_listEvnt);
	TRACE("exit: evnt_init OK");
}

/*
 * \brief Create an event
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”BTLE message”;
 * volatile    evnt_t    *event;
 *
 *    status = kern_createEvent(identifier, &event);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the event Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNEVFUL 	No more event
 * \param[out]	KKERNIDEVO	The event Id is already used
 *
 */
int32_t	evnt_createEvent(const char_t *identifier, volatile evnt_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: evnt_createEvent");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBEVENTS; i++) {
		if (buff_cmpStrings(vKern_event[i].oIdentifier, identifier)) { TRACE("exit: evnt_createEvent KO 1"); RETURN_INT_RESTORED(KKERNIDEVO); }
	}

	for (i = 0; i < KNBEVENTS; i++) {
		if (vKern_event[i].oIdentifier == NULL) {
			vKern_event[i].oState      |= (1<<BEVNTINSTALLED);
			vKern_event[i].oIdentifier  = identifier;
			*handle = &vKern_event[i];
			TRACE("exit: evnt_createEvent OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: evnt_createEvent KO 2");
	RETURN_INT_RESTORED(KKERNEVFUL);
}

/*
 * \brief Get the bit event
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    bitEvent;
 * volatile    evnt_t    *event;
 *
 *    status = kern_signalEvent(event, &bitEvent);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*bitEvent	Ptr on the bitEvent
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOEVO 	The event does not exist
 * \param[out]	KKERNEVNOI 	The event is not initialized
 *
 */
int32_t	evnt_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent) {
	volatile	evnt_t		*event;

	INTERRUPTION_OFF;
	TRACE("entry: evnt_getBitEvent");
	vKern_current->oStatistic.oNbKernCalls++;
	event = handle;
	if (event == NULL)							    { TRACE("exit: evnt_getBitEvent KO 1"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if (event->oIdentifier == NULL)				    { TRACE("exit: evnt_getBitEvent KO 2"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if ((event->oState & (1<<BEVNTINSTALLED)) == 0) { TRACE("exit: evnt_getBitEvent KO 3"); RETURN_INT_RESTORED(KKERNEVNOI); }

	*bitEvent = event->oBitEvent;
	TRACE("exit: evnt_getBitEvent OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Signal events
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    bitEvent;
 * volatile    evnt_t    *event;
 *
 *    status = kern_getBitEvent(event, &bitEvent);
 *    status = kern_signalEvent(bitEvent, KSYNCCONT);
 * \endcode
 *
 * - If (mode == KEVNTSWTC)
 *   - With the context switching
 *
 * - If (mode == KEVNTCONT)
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
 * \param[in]	mode		KEVNTSWTC -> (synchro with the context switching)
 * \param[in]				KEVNTCONT -> (synchro without the context switching)
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	evnt_signalEvent(uint32_t bitEvent, uint32_t mode) {
	uint16_t	i;
	uint32_t	theEvent;

	INTERRUPTION_OFF;
	TRACE("entry: evnt_signalEvent");
	vKern_current->oStatistic.oNbKernCalls++;

	for (i = 0; i < KNBPROCESS; i++) {
		if ((vKern_process[i].oInternal.oState & (1<<BPROCINSTALLED)) != 0) {
			vKern_process[i].oSynchro.oListEventsPending |= bitEvent;
			if ((vKern_process[i].oInternal.oState & (1<<BPROCSUSPEVNT)) != 0) {
				theEvent = vKern_process[i].oSynchro.oListEvents & bitEvent;
				if (theEvent != 0) {
					vKern_process[i].oInternal.oState &= ~(1<<BPROCSUSPEVNT);

// NEED to be TESTED
					#if 0
					chls_disconnect(&vKern_listEvnt, &vKern_process[i]);

					#else
					chls_disconnect(vKern_process[i].oObject.oList, &vKern_process[i]);
					#endif

					chls_connect(&vKern_listExec, &vKern_process[i]);
					vKern_process[i].oInternal.oStatus = KKERNNOERR;

// mode == KEVNTSWTC
// Context switching (preemption when "INTERRUPTION_RESTORED")

					if (mode == KEVNTSWTC) {
						PREEMPTION;
					}
				}
			}
		}
	}
	TRACE("exit: evnt_signalEvent OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Wait for events
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    eventTMP, eventMSG, eventXFER;
 * volatile    evnt_t    *event;
 *
 *    status = kern_getEventById(“message OK”, &event);
 *    status = kern_getBitEvent(event, &eventMSG);
 *    status = kern_getEventById(“start”, &event);
 *    status = kern_getBitEvent(event, &eventXFER);
 *    eventTMP = eventMSG | eventXFER;
 *    status = kern_waitEvent(&eventTMP, KSYNCCONT);
 * \endcode
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
int32_t	evnt_waitEvent(uint32_t *bitEvent, uint32_t timeout) {
	uint32_t	theEvent;

	INTERRUPTION_OFF;
	TRACE("entry: evnt_waitEvent");
	vKern_current->oStatistic.oNbKernCalls++;

	theEvent = vKern_current->oSynchro.oListEventsPending & *bitEvent;

// The event already occurred
// Cancel it and return the event

	if (theEvent != 0) {
		vKern_current->oSynchro.oListEventsPending &= ~theEvent;
		*bitEvent = theEvent;
		RETURN_INT_RESTORED(KKERNNOERR);
	}

// Wait for the event
// Add the event in the vKern_current->oSynchro.oListEvents
//
// When the process is scheduled, cancel it and return the event

	else {
		if (timeout > 0) {
			vKern_current->oSynchro.oListEvents |= *bitEvent;
			vKern_current->oInternal.oTimeout = (timeout/KTIMEUNIT);

			GOTO_KERN_I(KWAITEVNT);
			theEvent = vKern_current->oSynchro.oListEventsPending & *bitEvent;
			*bitEvent = theEvent;
			vKern_current->oSynchro.oListEvents        &= ~theEvent;
			vKern_current->oSynchro.oListEventsPending &= ~theEvent;

			TRACE("exit: evnt_waitEvent ->");
			RETURN_INT_RESTORED(vKern_current->oInternal.oStatus);
		}
		else {
			TRACE("exit: evnt_waitEvent KO 1");
			RETURN_INT_RESTORED(KKERNTIMEO);
		}

	}
}

/*
 * \brief Kill the event
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    evnt_t    *event;
 *
 *    status = kern_killEvent(event);
 * \endcode
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
int32_t	evnt_killEvent(volatile evnt_t *handle) {
				uint16_t	i;
	volatile	proc_t		*process;
	volatile	evnt_t		*event;

	INTERRUPTION_OFF;
	TRACE("entry: evnt_killEvent");
	vKern_current->oStatistic.oNbKernCalls++;
	event = handle;
	if (event == NULL)							    { TRACE("exit: evnt_killEvent KO 1"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if (event->oIdentifier == NULL)				    { TRACE("exit: evnt_killEvent KO 2"); RETURN_INT_RESTORED(KKERNNOEVO); }
	if ((event->oState & (1<<BEVNTINSTALLED)) == 0) { TRACE("exit: evnt_killEvent KO 3"); RETURN_INT_RESTORED(KKERNEVNOI); }

	if (vKern_listEvnt.oNbElements > 0) {
		for (i = 0; i < vKern_listEvnt.oNbElements; i++) {
			process = vKern_listEvnt.oFirst;
			process->oSynchro.oListEvents  &= ~(event->oBitEvent);
			process->oSynchro.oListEventsPending &= ~(event->oBitEvent);
			if (process->oSynchro.oListEvents == 0) {
				process->oInternal.oState &= ~(1<<BPROCSUSPEVNT);

// NEED to be TESTED
				#if 0
				chls_disconnect(&vKern_listEvnt, process);

				#else
				chls_disconnect(process->oObject.oList, process);
				#endif

				chls_connect(&vKern_listExec, process);
				process->oInternal.oStatus = KKERNEVKIL;
			}
		}
	}
	event->oIdentifier = NULL;
	event->oState      = 0;
	TRACE("exit: evnt_killEvent OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of an event by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Event 0”;
 * volatile    evnt_t    *event;
 *
 *    status = kern_getEventById(identifier, &event);
 * \endcode
 *
 * - This function returns the handle of the event
 *
 * \param[in]	*identifier	Ptr on the event Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOEVO 	The event does not exist
 *
 */
int32_t	evnt_getEventById(const char_t *identifier, volatile evnt_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: evnt_getEventById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBEVENTS; i++) {
		if (buff_cmpStrings(vKern_event[i].oIdentifier, identifier)) {
			*handle = &vKern_event[i];
			TRACE("exit: evnt_getEventById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: evnt_getEventById KO");
	RETURN_INT_RESTORED(KKERNNOEVO);
}
#endif
