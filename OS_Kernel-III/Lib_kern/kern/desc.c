/*
; desc.
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
; Goal:		Kern - Process management.
;
;			This module manages the processes.
;
; 			Service system calls
; 			--------------------
;
;			void	desc_init(void);
;			int32_t	desc_createProcess(const spec_t *specification, const void *argument, volatile proc_t **handle);
;			int32_t	desc_killProcess(volatile proc_t *handle);
;			int32_t	desc_setPriority(volatile proc_t *handle, uint8_t priority);
;			int32_t	desc_getProcessByNb(uint8_t number, volatile proc_t **handle);
;			int32_t	desc_getProcessById(const char_t *identifier, volatile proc_t **handle);
;			int32_t	desc_getProcessRun(volatile proc_t **handle);
;			int32_t	desc_setGlobalPtr(const void **global);
;			int32_t	desc_setLocalPtr(const void *local);
;			int32_t	desc_installCallBack(void (*code)(uint8_t state));
;			int32_t	desc_callBack(void);
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

					void		(*vKern_codeRoutine)(uint8_t state);	// Ptr on the Code for the call-back routine
		const		void		**vKern_ptrGlobal;						// Save the Ptr on a global variable
static				uint32_t	vDummy;									// A dummy variable
static	const		void		*vPtrDummy;								// A dummy Ptr
		volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	proc_t		*vKern_backwardProcess;					// Ptr on the backward process
		volatile	proc_t		vKern_frstProcess;						// First process,  Id = 0
		volatile	proc_t		vKern_process[KNBPROCESS];				// User processes, Id = 1..2^31; Id = IDLE if 1
		volatile	list_t		vKern_listFree;							// Empty list (processes reserve)
		volatile	list_t		vKern_listExec;							// Execution list for all the priorities

// Module strings
// ==============

LOC_CONST_STRG(aStrFirst[])        = "Process_First";
LOC_CONST_STRG(aStrIden_idle[])    = KDAEMON_IDLE;
LOC_CONST_STRG(aStrIden_timeout[]) = KDAEMON_TIMEOUT;
LOC_CONST_STRG(aStrIden_stack[])   = KDAEMON_STACK;

// Prototypes
// ==========

static	void	_initialize(volatile proc_t *handle);
static	void	_setupDescriptor(volatile proc_t *handle, const spec_t *specification, uint8_t state);
static	void	_noAction(uint8_t state);

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "desc" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	desc_init(void) {
	uint16_t	i;

	TRACE("entry: desc_init");

// Initialise & connect the descriptors to the lists

	_initialize(&vKern_frstProcess);
	for (i = 0; i < KNBPROCESS; i++) {
		_initialize(&vKern_process[i]);
	}

// Initialise the lists and connect all the descriptors to the free list

	chls_initialize(&vKern_listFree);
	chls_initialize(&vKern_listExec);
	for (i = 0; i < KNBPROCESS; i++) {
		chls_connect(&vKern_listFree, &vKern_process[i]);
	}

	vKern_current		  = &vKern_frstProcess;
	vKern_backwardProcess = &vKern_frstProcess;
	vKern_codeRoutine     = _noAction;

	vPtrDummy       = &vDummy;
	vKern_ptrGlobal = &vPtrDummy;
	TRACE("exit: desc_init OK");
}

/*
 * \brief Create and install a process
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    512
 *
 * int32_t    status;
 * static     uint32_t    vStack_0[KSZSTACK];
 * static     volatile    proc_t    *vProcess_0, *vProcess_1;
 *
 * const    spec_t    vSpecs_0 = {
 *                        .oIdentifier  = "User Process 0”,
 *                        .oText        = ”Process to do something (static mode)”,
 *                        .oCode        = (const void (*)(const void *argument))process_0,
 *                        .oStkSupBegin = &vStack_0[0],
 *                        .oStkSup      = &vStack_0[KSZSTACK-8],
 *                        .oSzStk       = KSZSTACK-8,
 *                        .oStkUsr      = NULL,
 *                        .oStackMode   = KSTKSTATIC,
 *                        .oPriority    = 5,
 *                        .oIdPack      = 0
 *                        .oCommManager = KDEF0,
 *                        .oKind        = KPROCNORMAL;
 *                        .oMode        = KUSER;
 *                    };
 *
 *    uint32_t    *vStack_1= (uint32_t *)((((uint32_t)syos_malloc(KINTERNAL,  \
 *                                        (KSZSTACK*sizeof(uint32_t))+8U))+7U)&(~0x7U));
 * const    spec_t    vSpecs_0 = {
 *                        .oIdentifier  = "User Process 1”,
 *                        .oText        = ”Process to do something (dynamic mode)”,
 *                        .oCode        = (const void (*)(const void *argument))process_1,
 *                        .oStkSupBegin = &vStack_0[0],
 *                        .oStkSup      = &vStack_1[KSZSTACK-8],
 *                        .oSzStk       = KSZSTACK-8,
 *                        .oStkUsr      = NULL,
 *                        .oStackMode   = KSTKDYNAMIC,
 *                        .oPriority    = 5,
 *                        .oIdPack      = 0
 *                        .oCommManager = KDEF0,
 *                        .oKind        = KPROCNORMAL;
 *                        .oMode        = KUSER;
 *                    };
 *
 * status = kern_createProcess(&vSpecs_0, NULL, &vProcess_0);
 * status = kern_createProcess(&vSpecs_1, NULL, &vProcess_1);
 *
 * To simplify the way to write this initialization, a couple of macros are available;
 * so, the previous example can be written in this way:
 *
 * #define    KSIZE    512
 *
 * int32_t    status;
 * static     char_t      aStrProcess_0[] = "Process to do something 0r\n";
 * static     char_t      aStrProcess_1[] = "Process to do something 1\r\n";
 * static     volatile    proc_t    *vProcess_0, *vProcess_1;;
 *
 * PROC_USER(0, vSpecs_0, aStrProcess_0, KSIZE, process_0, "User Process 0", KDEF0, 5, 0, KPROCNORMAL)
 * PROC_USER(1, vSpecs_1, aStrProcess_1, KSIZE, process_1, "User Process 1", KUSB0, 5, 0, KPROCNORMAL)
 *
 * status = kern_createProcess(&vSpecs_0, NULL, &vProcess_0);
 * status = kern_createProcess(&vSpecs_1, NULL, &vProcess_1);
 * \endcode
 *
 * - Disconnect a process from the empty list
 * - Initialize the stack-frame
 * - Connect the process to the execution list
 *
 * \param[in]	*specification				Ptr on the process specification
 * \param[in]	specification->oIdentifier	Ptr on the process Id ["xyzt"]
 * \param[in]	specification->oText		Ptr on the process text
 * \param[in]	specification->oCode		Ptr on the process code
 * \param[in]	specification->oStackMode	Stack mode
 * \param[in]	specification->oStkSupBegin	Ptr on the begin of process stack (supervisor)
 * \param[in]	specification->oStkSup		Ptr on the process Supervisor stack (the last value)
 * \param[in]	specification->oSzStk		Size of the process stack
 * \param[in]	specification->oStkUsr		Ptr on the process User stack (the last value)
 * \param[in]	specification->oCommManager	Default communication manager
 * \param[in]	specification->oIdPack		Process pack if overlay
 * \param[in]	specification->oPriority	Process priority
 * \param[in]	specification->oKind		Process kind
 * \param[in]	specification->oMode		Process running mode
 * \param[in]	**handle					Ptr on the handle
 * \param[out]	KKERNNOERR					OK
 * \param[out]	KKERNLIFUL					No more process
 * \param[out]	KKERNIDPRO					The process Id is already used
 * \param[out]	KKERNNOSTK					No memory for the stack
 *
 */
int32_t	desc_createProcess(const spec_t *specification, const void *argument, volatile proc_t **handle) {
				uint16_t	i;
	volatile	proc_t		*process;

	INTERRUPTION_OFF;
	TRACE("entry: desc_createProcess");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;

	if (specification->oStkSupBegin == NULL) 											     			  { TRACE("exit: desc_createProcess KO 1"); RETURN_INT_RESTORED(KKERNNOSTK); }
	if (vKern_listFree.oNbElements == 0) 											     				  { TRACE("exit: desc_createProcess KO 2"); RETURN_INT_RESTORED(KKERNLIFUL); }
	for (i = 0; i < KNBPROCESS; i++) {
		if ((vKern_process[i].oInternal.oState & (1<<BPROCINSTALLED)) != 0) {
			if (buff_cmpStrings(vKern_process[i].oSpecification.oIdentifier, specification->oIdentifier)) { TRACE("exit: desc_createProcess KO 3"); RETURN_INT_RESTORED(KKERNIDPRO); }
		}
	}

// Prepare the process

	process = vKern_listFree.oFirst;
	chls_disconnect(&vKern_listFree, process);
	chls_connect(&vKern_listExec, process);
	_setupDescriptor(process, specification, (1<<BPROCINSTALLED));

// Prepare the stack frame

	stub_kern_initStackFrame(process, argument);
	*handle = process;
	TRACE("exit: desc_createProcess OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Kill the process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_killProcess();
 * \endcode
 *
 * - Disconnect a process from the list to which it is connected
 * - Connect the process to the empty list
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNREFRS 	The first process cannot be removed
 * \param[out]	KKERNRESDE 	The system daemons cannot be removed
 * \param[out]	KKERNNOPRO 	The process does not exist
 *
 */
int32_t	desc_killProcess(volatile proc_t *handle) {
				int32_t		status = KKERNNOERR;
				enum		{ KNOTPRESENT, KSUICIDE, KNORMAL } action;
	volatile	proc_t		*process;

	INTERRUPTION_OFF;
	TRACE("entry: desc_killProcess");
	vKern_current->oStatistic.oNbKernCalls++;
	process = handle;
	if (process == NULL)                           			 				    { TRACE("exit: desc_killProcess KO 1"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((process->oInternal.oState & (1<<BPROCINSTALLED)) == 0)    			    { TRACE("exit: desc_killProcess KO 2"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if (buff_cmpStrings(process->oSpecification.oIdentifier, aStrFirst))	    { TRACE("exit: desc_killProcess KO 3"); RETURN_INT_RESTORED(KKERNREFRS); }
	if (buff_cmpStrings(process->oSpecification.oIdentifier, aStrIden_idle))    { TRACE("exit: desc_killProcess KO 4"); RETURN_INT_RESTORED(KKERNRESDE); }

	#if (defined(__DAEMKILL__))

	#else
	if (buff_cmpStrings(process->oSpecification.oIdentifier, aStrIden_timeout)) { TRACE("exit: desc_killProcess KO 5"); RETURN_INT_RESTORED(KKERNRESDE); }
	if (buff_cmpStrings(process->oSpecification.oIdentifier, aStrIden_stack))   { TRACE("exit: desc_killProcess KO 6"); RETURN_INT_RESTORED(KKERNRESDE); }
	#endif

// Determine the next action

	if ((process->oInternal.oState & (1<<BPROCINSTALLED)) == 0) 			            		    		  { action = KNOTPRESENT; }
	else if (buff_cmpStrings(process->oSpecification.oIdentifier, vKern_current->oSpecification.oIdentifier)) { action = KSUICIDE;    }
		 else                                      				    				    		  			  { action = KNORMAL;     }

	switch (action) {

// The process is not present: it is free for the user

		case KNOTPRESENT: {
			status = KKERNNOPRO;
			break;
		}

// The process wants to commit a suicide. If it was locked, it
// unlocks the semaphore

		case KSUICIDE: {
			chls_disconnect(process->oObject.oList, process);
			chls_connect(&vKern_listFree, process);
			if (process->oSpecification.oStackMode == KSTKDYNAMIC) {
				syos_free(process->oSpecification.oStkSupBegin);
			}
			_initialize(process);

			GOTO_KERN_I(KJUMPKERN);
			break;
		}

// Remove a normal process

		case KNORMAL: {
			chls_disconnect(process->oObject.oList, process);
			chls_connect(&vKern_listFree, process);
			if (process->oSpecification.oStackMode == KSTKDYNAMIC) {
				syos_free(process->oSpecification.oStkSupBegin);
			}
			_initialize(process);
			break;
		}
		default: {
			break;
		}
	}
	TRACE("exit: desc_killProcess ->");
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Set a new static priority for a process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_setPriority(process, 23);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	priority	Process priority
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 *
 */
int32_t	desc_setPriority(volatile proc_t *handle, uint8_t priority) {
	volatile	proc_t	*process;

	INTERRUPTION_OFF;
	TRACE("entry: desc_setPriority");
	vKern_current->oStatistic.oNbKernCalls++;
	process = handle;
	if (process == NULL)						   			    { TRACE("exit: desc_setPriority KO 1"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((process->oInternal.oState & (1<<BPROCINSTALLED)) == 0) { TRACE("exit: desc_setPriority KO 2"); RETURN_INT_RESTORED(KKERNNOPRO); }

	priority = (priority == 0)             ? 1                : priority;
	priority = (priority >= KNBPRIORITIES) ? (KLOWPRIORITY+7) : priority;

	process->oSpecification.oPriority = priority;
	process->oInternal.oDynaPriority  = priority;
	TRACE("exit: desc_setPriority OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of a process by its number
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = status = kern_getProcessByNb(12, &process);
 * \endcode
 *
 * - This function returns the handle of the process
 *
 * \param[in]	number		The process number
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 *
 */
int32_t	desc_getProcessByNb(uint8_t number, volatile proc_t **handle) {

	INTERRUPTION_OFF;
	TRACE("entry: desc_getProcessByNb");
	vKern_current->oStatistic.oNbKernCalls++;
	if (number >= KNBPROCESS)                          		    		     { TRACE("exit: desc_getProcessByNb KO 1"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((vKern_process[number].oInternal.oState & (1<<BPROCINSTALLED)) == 0) { TRACE("exit: desc_getProcessByNb KO 2"); RETURN_INT_RESTORED(KKERNNOPRO); }
	*handle = &vKern_process[number];
	TRACE("exit: desc_getProcessByNb OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of a process by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Process 0 - read a sensors”;
 * volatile    proc_t    *process;
 *
 *    status = kern_getProcessById(identifier, &process);
 * \endcode
 *
 * - This function returns the handle of the process
 *
 * \param[in]	*identifier	Ptr on the process Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 *
 */
int32_t	desc_getProcessById(const char_t *identifier, volatile proc_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: desc_getProcessById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBPROCESS; i++) {
		if (buff_cmpStrings(vKern_process[i].oSpecification.oIdentifier, identifier)) {
			*handle = &vKern_process[i];
			TRACE("exit: desc_getProcessById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: desc_getProcessById KO");
	RETURN_INT_RESTORED(KKERNNOPRO);
}

/*
 * \brief Get the handle of a process (the running one)
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_getProcessRun(&process);
 * \endcode
 *
 * - This function returns the handle of the current process
 *
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	desc_getProcessRun(volatile proc_t **handle) {

	INTERRUPTION_OFF;
	TRACE("entry: desc_getProcessRun");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = vKern_current;
	TRACE("exit: desc_getProcessRun OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Set a global pointer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * void       *global;
 *
 *    status = kern_setGlobalPtr(&global);
 * \endcode
 *
 * - This function sets a global pointer. When the context change
 *   occurs, the local process pointer is copied to this global one
 *   Typical usage: _impure_ptr management for the reentrant
 *   support of the newlib
 *
 * \param[in]	**global	Ptr global
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	desc_setGlobalPtr(const void **global) {

	INTERRUPTION_OFF;
	TRACE("entry: desc_setGlobalPtr");
	vKern_current->oStatistic.oNbKernCalls++;
	vKern_ptrGlobal = global;
	TRACE("exit: desc_setGlobalPtr OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Set a local pointer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * void       *local;
 *
 *    status = kern_setLocalPtr(&local);
 * \endcode
 *
 * - This function sets a local pointer. When the context change
 *   occurs, the local process pointer is copied to this global one
 *   Typical usage: _impure_ptr management for the reentrant
 *   support of the newlib
 *
 * \param[in]	*local		Ptr local
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	desc_setLocalPtr(const void *local) {

	INTERRUPTION_OFF;
	TRACE("entry: desc_setLocalPtr");
	vKern_current->oStatistic.oNbKernCalls++;
	vKern_current->oInternal.oLocal = local;
	TRACE("exit: desc_setLocalPtr OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Install a calls back routine
 *
 * Call example in C:
 *
 * \code{.c}
 * static     void    _myRoutine(uint8_t state);
 *
 * int32_t    status;
 *
 *    status = kern_setLocalPtr(&local);
 *
 *    // My callback routine
 *
 *    static    void    _myRoutine(uint8_t state) {
 *
 *        if (state == KINIDLE) misc_onLed(KLEDIDLE);
 *        else                  misc_offLed(KLEDIDLE);
 *    }
 * \endcode
 *
 * \param[in]	*code		Ptr on the routine code
 * \param[out]	KKERNNOERR  OK
 *
 */
int32_t	desc_installCallBack(void (*code)(uint8_t state)) {

	INTERRUPTION_OFF;
	TRACE("entry: desc_installCallBack");
	vKern_current->oStatistic.oNbKernCalls++;
	vKern_codeRoutine = code;
	TRACE("exit: desc_installCallBack OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

static	void	_noAction(uint8_t state) {

	TRACE("entry: _noAction");
}

// Local routines
// ==============

/*
 * \brief _initialize
 *
 */
static	void	_initialize(volatile proc_t *handle) {
	const	spec_t	specification = {
						.oIdentifier  = NULL,
						.oText		  = NULL,
						.oCode		  = NULL,
						.oStkSupBegin  = NULL,
						.oStkSup      = NULL,
						.oSzStk		  = 0,
						.oStkUsr	  = NULL,
						.oStackMode	  = 0,
						.oCommManager = 0,
						.oIdPack	  = 0,
						.oPriority	  = 0,
						.oKind		  = 0,
						.oMode 		  = 0
					};

	_setupDescriptor(handle, &specification, 0);
	handle->oObject.oList	 = NULL;
	handle->oObject.oBack    = NULL;
	handle->oObject.oForward = NULL;
}

/*
 * \brief _setupDescriptor
 *
 */
static	void	_setupDescriptor(volatile proc_t *handle, const spec_t *specification, uint8_t state) {
	uint8_t		priority;

	priority = (specification->oPriority == 0) ? 1                : specification->oPriority;
	priority = (priority >= KNBPRIORITIES)     ? (KLOWPRIORITY+7) : priority;

// The process specifications

	handle->oSpecification.oIdentifier	 = specification->oIdentifier;
	handle->oSpecification.oText		 = specification->oText;
	handle->oSpecification.oCode		 = specification->oCode;
	handle->oSpecification.oStkSupBegin	 = specification->oStkSupBegin;
	handle->oSpecification.oStkSup		 = specification->oStkSup;
	handle->oSpecification.oSzStk		 = specification->oSzStk;
	handle->oSpecification.oStkUsr 		 = specification->oStkUsr;
	handle->oSpecification.oStackMode    = specification->oStackMode;
	handle->oSpecification.oCommManager  = specification->oCommManager;
	handle->oSpecification.oIdPack		 = specification->oIdPack;
	handle->oSpecification.oKind		 = specification->oKind;
	handle->oSpecification.oMode		 = specification->oMode;
	handle->oSpecification.oPriority	 = priority;

// The process work

	handle->oInternal.oStatus			 = 0;
	handle->oInternal.oTimeout			 = 0;
	handle->oInternal.oLocal			 = NULL;
	handle->oInternal.oState			 = state;
	handle->oInternal.oDynaPriority		 = priority;
	handle->oInternal.oSkip				 = 0;

// The process synchros (events, signals & semaphores)

	handle->oSynchro.oFlags				 = 0;
	handle->oSynchro.oListEvents		 = 0;
	handle->oSynchro.oListEventsPending	 = 0;
	handle->oSynchro.oListSignals		 = 0;
	handle->oSynchro.oListSignalsPending = 0;
	handle->oSynchro.oNbSyncSema		 = 0;

// The process statistic

	handle->oStatistic.oNbExecutions	 = 0;
	handle->oStatistic.oNbKernCalls		 = 0;
	handle->oStatistic.oAvStkSup		 = 0;
	handle->oStatistic.oTimePMin		 = 0xFFFF;
	handle->oStatistic.oTimePMax		 = 0;
	handle->oStatistic.oTimePAvg		 = 0;
	handle->oStatistic.oTimePCum		 = 0;
	handle->oStatistic.oTimeKMin		 = 0xFFFF;
	handle->oStatistic.oTimeKMax		 = 0;
	handle->oStatistic.oTimeKAvg		 = 0;
	handle->oStatistic.oTimeKCum		 = 0;
	handle->oStatistic.oTimeEMin		 = 0xFFFF;
	handle->oStatistic.oTimeEMax		 = 0;
	handle->oStatistic.oTimeEAvg		 = 0;
	handle->oStatistic.oTimeECum		 = 0;
}
