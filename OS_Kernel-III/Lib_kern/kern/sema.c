/*
; sema.
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
; Goal:		Kern - Semaphore management.
;
;			This module implements the semaphore primitives.
;
; 			Semaphore system calls
; 			----------------------
;
;			void	sema_init(void);
;			int32_t	sema_createSyncSemaphore(const char_t identifier, volatile sema_t **handle);
;			int32_t	sema_createMutxSemaphore(const char_t identifier, volatile sema_t **handle);
;			int32_t	sema_signalSemaphore(volatile sema_t *handle);
;			int32_t	sema_unlockSemaphore(volatile sema_t *handle);
;			int32_t	sema_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout);
;			int32_t	sema_lockSemaphore(volatile sema_t *handle, uint32_t timeout);
;			int32_t	sema_killSemaphore(volatile sema_t *handle);
;			int32_t	sema_restartSemaphore(volatile sema_t *handle);
;			int32_t	sema_getSemaphoreById(const char_t identifier, volatile sema_t **handle);
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

#if (defined(__WITHSEMA__))
extern	volatile	list_t		vKern_listExec;							// Execution list for all the priorities
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	sema_t		vKern_semaphore[KNBSEMAPHO];			// Semaphores

// Prototypes
// ==========

static	int32_t	_createSemaphore(const char_t *identifier, uint8_t bitMode, volatile sema_t **handle);
static	int32_t	_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout);
static	int32_t	_signalSemaphore(volatile sema_t *handle);

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "sema" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	sema_init(void) {
	uint16_t	i;

	TRACE("entry: sema_init");
	for (i = 0; i < KNBSEMAPHO; i++) {
		chls_initialize(&vKern_semaphore[i].oList);
		vKern_semaphore[i].oIdentifier = NULL;
		vKern_semaphore[i].oOwner	   = NULL;
		vKern_semaphore[i].oState      = 0;
		vKern_semaphore[i].oCounter    = 0;
	}
	TRACE("exit: sema_init OK");
}

/*
 * \brief Create a sync semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Semaphore 0”;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_createSyncSemaphore(identifier, &semaphore);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the semaphore Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSEFUL 	No more semaphore
 * \param[out]	KKERNIDSEM	The semaphore Id is already used
 *
 */
int32_t	sema_createSyncSemaphore(const char_t *identifier, volatile sema_t **handle) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _createSemaphore(identifier, BSEMASYNCHRO, handle);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Create a mutx semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Semaphore 0”;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_createMutxSemaphore(identifier, &semaphore);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the semaphore Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSEFUL 	No more semaphore
 * \param[out]	KKERNIDSEM	The semaphore Id is already used
 *
 */
int32_t	sema_createMutxSemaphore(const char_t *identifier, volatile sema_t **handle) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _createSemaphore(identifier, BSEMAMUTEX, handle);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Signal a semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_signalSemaphore(semaphore);
 * \endcode
 *
 * - counter++
 * - If (counter <= 0)
 *   - Then disconnect the first process from the semaphore list (accordigly to the number of events)
 *   - Connect the process to the execution list
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 * \param[out]	KKERNSETME 	The semaphore counts too many events
 *
 */
int32_t	sema_signalSemaphore(volatile sema_t *handle) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _signalSemaphore(handle);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Unlock a semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_lockSemaphore(semaphore);
 *    // Atomic access code portion
 *    status = kern_unlockSemaphore(semaphore);
 * \endcode
 *
 * - counter++
 * - If (counter <= 0)
 *   - Then disconnect the first process from the semaphore list (accordigly to the number of events)
 *   - Connect the process to the execution list
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 * \param[out]	KKERNSETME 	The semaphore counts too many events
 *
 */
int32_t	sema_unlockSemaphore(volatile sema_t *handle) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _signalSemaphore(handle);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Wait for a semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_waitSemaphore(semaphore, 1, timeout);
 * \endcode
 *
 * - counter -= nbEvents
 * - If (counter < 0)
 *   - Then suspend the process
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	nbEvents	Number of events
 * \param[in]	timeout		Timeout
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 * \param[out]	KKERNSETME 	The semaphore counts too many events
 * \param[out]	KKERNSEKIL 	The semaphore has been killed (with processes in its list)
 * \param[out]	KKERNTIMEO 	Timeout
 *
 */
int32_t	sema_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _waitSemaphore(handle, nbEvents, timeout);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief lock a semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_lockSemaphore(semaphore);
 *    // Atomic access code portion
 *    status = kern_unlockSemaphore(semaphore);
 * \endcode
 *
 * - counter -= 1
 * - If (counter < 0)
 *   - Then suspend the process
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	timeout		Timeout
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 * \param[out]	KKERNSETME 	The semaphore counts too many events
 * \param[out]	KKERNSEKIL 	The semaphore has been killed (with processes in its list)
 * \param[out]	KKERNTIMEO 	Timeout
 *
 */
int32_t	sema_lockSemaphore(volatile sema_t *handle, uint32_t timeout) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _waitSemaphore(handle, 1, timeout);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Kill the semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_killSemaphore(semaphore);
 * \endcode
 *
 * - If (process still connected to the semaphore list)
 *   - Then return error
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 *
 */
int32_t	sema_killSemaphore(volatile sema_t *handle) {
				uint16_t	i;
	volatile	proc_t		*process;
	volatile	sema_t		*semaphore;

	INTERRUPTION_OFF;
	TRACE("entry: sema_killSemaphore");
	vKern_current->oStatistic.oNbKernCalls++;
	semaphore = handle;
	if (semaphore == NULL) 				   			    { TRACE("exit: sema_killSemaphore KO 1"); RETURN_INT_RESTORED(KKERNNOSEM); }
	if (semaphore->oIdentifier == NULL)	   			    { TRACE("exit: sema_killSemaphore KO 2"); RETURN_INT_RESTORED(KKERNNOSEM); }
	if ((semaphore->oState & (1<<BSEMAINSTALLED)) == 0) { TRACE("exit: sema_killSemaphore KO 3"); RETURN_INT_RESTORED(KKERNSENOI); }

	if (semaphore->oList.oNbElements > 0) {
		for (i = 0; i < semaphore->oList.oNbElements; i++) {
			process = semaphore->oList.oFirst;
			process->oInternal.oState &= ~(1<<BPROCSUSPSEMA);
			chls_disconnect(&semaphore->oList, process);
			chls_connect(&vKern_listExec, process);
			process->oInternal.oStatus = KKERNSEKIL;
		}
	}
	semaphore->oIdentifier = NULL;
	semaphore->oOwner	   = NULL;
	semaphore->oCounter    = 0;
	semaphore->oState      = 0;
	TRACE("exit: sema_killSemaphore OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Restart the semaphore
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_restartSemaphore(semaphore);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 *
 */
int32_t	sema_restartSemaphore(volatile sema_t *handle) {
				uint16_t	i;
	volatile	proc_t		*process;
	volatile	sema_t		*semaphore;

	INTERRUPTION_OFF;
	TRACE("entry: sema_restartSemaphore");
	vKern_current->oStatistic.oNbKernCalls++;
	semaphore = handle;
	if (semaphore == NULL) 				   			    { TRACE("exit: sema_restartSemaphore KO 1"); RETURN_INT_RESTORED(KKERNNOSEM); }
	if (semaphore->oIdentifier == NULL)	   			    { TRACE("exit: sema_restartSemaphore KO 2"); RETURN_INT_RESTORED(KKERNNOSEM); }
	if ((semaphore->oState & (1<<BSEMAINSTALLED)) == 0) { TRACE("exit: sema_restartSemaphore KO 3"); RETURN_INT_RESTORED(KKERNSENOI); }

	if (semaphore->oList.oNbElements > 0) {
		for (i = 0; i < semaphore->oList.oNbElements; i++) {
			process = semaphore->oList.oFirst;
			process->oInternal.oState &= ~(1<<BPROCSUSPSEMA);
			chls_disconnect(&semaphore->oList, process);
			chls_connect(&vKern_listExec, process);
			process->oInternal.oStatus = KKERNNOERR;
		}
	}
	semaphore->oCounter = 0;
	semaphore->oOwner	= NULL;
	TRACE("exit: sema_restartSemaphore OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of a semaphore by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Semaphore 0”;
 * volatile    sema_t    *semaphore;
 *
 *    status = kern_getSemaphoreById(identifier, &semaphore);
 * \endcode
 *
 * - This function returns the handle of the semaphore
 *
 * \param[in]	*identifier	Ptr on the semaphore Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 *
 */
int32_t	sema_getSemaphoreById(const char_t *identifier, volatile sema_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: sema_getSemaphoreById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBSEMAPHO; i++) {
		if (buff_cmpStrings(vKern_semaphore[i].oIdentifier, identifier)) {
			*handle = &vKern_semaphore[i];
			TRACE("exit: sema_getSemaphoreById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: sema_getSemaphoreById KO");
	RETURN_INT_RESTORED(KKERNNOSEM);
}

// Local routines
// ==============

/*
 * \brief _createSemaphore
 *
 * \param[in]	*identifier	Ptr on the semaphore Id
 * \param[in]	bitMode		BSEMASYNC = Synchro, BSEMAMUTEX = Mutex
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSEFUL 	No more semaphore
 * \param[out]	KKERNIDSEM	The semaphore Id is already used
 *
 */
static	int32_t	_createSemaphore(const char_t *identifier, uint8_t bitMode, volatile sema_t **handle) {
	uint16_t	i;

	TRACE("entry: sema_createSemaphore");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBSEMAPHO; i++) {
		if (buff_cmpStrings(vKern_semaphore[i].oIdentifier, identifier)) { TRACE("exit: sema_createSemaphore KO 1"); return (KKERNIDSEM); }
	}

	for (i = 0; i < KNBSEMAPHO; i++) {
		if (vKern_semaphore[i].oIdentifier == NULL) {
			vKern_semaphore[i].oState      |= (1<<BSEMAINSTALLED) | (1<<bitMode);
			vKern_semaphore[i].oIdentifier  = identifier;
			vKern_semaphore[i].oCounter     = (bitMode == BSEMAMUTEX) ? (1) : (0);
			*handle = &vKern_semaphore[i];
			TRACE("exit: sema_createSemaphore OK");
			return (KKERNNOERR);
		}

	}
	TRACE("exit: sema_createSemaphore KO 2");
	return (KKERNSEFUL);
}

/*
 * \brief _waitSemaphore
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	nbEvents	Number of events
 * \param[in]	timeout		Timeout
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 * \param[out]	KKERNSETME 	The semaphore counts too many events
 * \param[out]	KKERNSEKIL 	The semaphore has been killed (with processes in its list)
 * \param[out]	KKERNTIMEO 	Timeout
 *
 */
static	int32_t	_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout) {
				uint16_t	i;
				uint32_t	synchro;
				int32_t		events;
	volatile	sema_t		*semaphore;

	TRACE("entry: sema_waitSemaphore");
	vKern_current->oStatistic.oNbKernCalls++;
	semaphore = handle;
	if (nbEvents == 0)					   			    { TRACE("exit: sema_waitSemaphore OK");   return (KKERNNOERR); }
	if (semaphore == NULL) 				   			    { TRACE("exit: sema_waitSemaphore KO 1"); return (KKERNNOSEM); }
	if (semaphore->oIdentifier == NULL)	   			    { TRACE("exit: sema_waitSemaphore KO 2"); return (KKERNNOSEM); }
	if ((semaphore->oState & (1<<BSEMAINSTALLED)) == 0) { TRACE("exit: sema_waitSemaphore KO 3"); return (KKERNSENOI); }
	if (semaphore->oCounter == KMINSEMCPT) 			    { TRACE("exit: sema_waitSemaphore KO 4"); return (KKERNSETME); }

	for (i = 0; i < KNBSEMAPHO; i++) {
		if (&vKern_semaphore[i] == handle) {
			semaphore->oCounter -= nbEvents;
			if (semaphore->oCounter >= 0) {
				if ((semaphore->oState & (1<<BSEMAMUTEX)) != 0) {
					semaphore->oOwner = vKern_current;
				}
				TRACE("exit: sema_waitSemaphore OK");
				return (KKERNNOERR);
			}

			if ((semaphore->oCounter + (int32_t)nbEvents) > 0) { events = (int32_t)nbEvents + semaphore->oCounter; }
			else									           { events = (int32_t)nbEvents;                       }

			if (timeout > 0) {
				vKern_current->oInternal.oTimeout   = (timeout/KTIMEUNIT);
				vKern_current->oSynchro.oNbSyncSema = (uint32_t)events;
				synchro = KSEMASYN + ((uint32_t)i & 0x0000FFFF);

				GOTO_KERN_M(synchro);
				TRACE("exit: sema_waitSemaphore ->");
				return (vKern_current->oInternal.oStatus);
			}
			else {
				TRACE("exit: sema_waitSemaphore KO 5");
				return (KKERNTIMEO);
			}

		}
	}
	TRACE("exit: sema_waitSemaphore KO 6");
	return (KKERNNOSEM);
}

/*
 * \brief _signalSemaphore
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSEM 	The semaphore does not exist
 * \param[out]	KKERNSENOI 	The semaphore is not initialized
 * \param[out]	KKERNSETME 	The semaphore counts too many events
 *
 */
static	int32_t	_signalSemaphore(volatile sema_t *handle) {
	volatile	proc_t		*process;
	volatile	sema_t		*semaphore;

	TRACE("entry: sema_signalSemaphore");
	vKern_current->oStatistic.oNbKernCalls++;
	semaphore = handle;
	if (semaphore == NULL) 				   			    { TRACE("exit: sema_signalSemaphore KO 1"); return (KKERNNOSEM); }
	if (semaphore->oIdentifier == NULL)	   			    { TRACE("exit: sema_signalSemaphore KO 2"); return (KKERNNOSEM); }
	if ((semaphore->oState & (1<<BSEMAINSTALLED)) == 0) { TRACE("exit: sema_signalSemaphore KO 3"); return (KKERNSENOI); }
	if (semaphore->oCounter == KMAXSEMCPT) 			    { TRACE("exit: sema_signalSemaphore KO 4"); return (KKERNSETME); }

	semaphore->oCounter++;
	if (semaphore->oList.oNbElements == 0)				{ TRACE("exit: sema_signalSemaphore OK");   return (KKERNNOERR); }

// More wait calls than signal ones

	process = semaphore->oList.oFirst;
	process->oSynchro.oNbSyncSema--;
	if (process->oSynchro.oNbSyncSema == 0) {
		process->oInternal.oState &= ~(1<<BPROCSUSPSEMA);
		chls_disconnect(&semaphore->oList, process);
		chls_connect(&vKern_listExec, process);
		process->oInternal.oStatus = KKERNNOERR;

		if ((semaphore->oState & (1<<BSEMAMUTEX)) != 0) {
			semaphore->oOwner = process;
		}
	}
	TRACE("exit: sema_signalSemaphore OK");
	return (KKERNNOERR);
}
#endif
