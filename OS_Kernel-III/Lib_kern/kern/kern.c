/*
; kern.
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
; Goal:		kern manager (uKernel core).
;
;			The KERN manager operates on a series of linked lists
;			where the processes are located.
;
; 			Service system calls
; 			--------------------
;
;			int32_t		kern_init(void);
;			int32_t		kern_runKernel(void);
;			int32_t		kern_getState(uint8_t *state);
;			int32_t		kern_getPC(volatile proc_t *handle, uint32_t *pc);
;			int32_t		kern_getFunctionName(const uint32_t pc, const char_t **function);
;			int32_t		kern_createProcess(const spec_t *specification, const void *argument, volatile proc_t **handle);
;			int32_t		kern_killProcess(volatile proc_t *handle);
;			int32_t		kern_setPriority(volatile proc_t *handle, uint8_t priority);
;			int32_t		kern_getProcessByNb(uint8_t number, volatile proc_t **handle);
;			int32_t		kern_getProcessById(const char_t *identifier, volatile proc_t **handle);
;			int32_t		kern_getProcessRun(volatile proc_t **handle);
;			int32_t		kern_setGlobalPtr(const void **global);
;			int32_t		kern_setLocalPtr(const void *local);
;			int32_t		kern_installCallBack(void (*code)(uint8_t state));
;
;	 		Temporal system calls
; 			---------------------
;
;			int32_t		kern_suspendProcess(uint32_t time);
;			int32_t		kern_setUnixTime(const atim_t *time);
;			int32_t		kern_getUnixTime(atim_t *time);
;			int32_t		kern_switchFast(void);
;			int32_t		kern_getTiccount(uint64_t *ticcount);
;			int32_t		kern_waitAtLeast(uint16_t time);
;			int32_t		kern_runPrecise(uint16_t time, void (*code)(void));
;
; 			Semaphore system calls
; 			----------------------
;
;			int32_t		kern_createSyncSemaphore(const char_t identifier, volatile sema_t **handle);
;			int32_t		kern_createMutxSemaphore(const char_t identifier, volatile sema_t **handle);
;			int32_t		kern_signalSemaphore(volatile sema_t *handle);
;			int32_t		kern_unlockSemaphore(volatile sema_t *handle);
;			int32_t		kern_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout);
;			int32_t		kern_lockSemaphore(volatile sema_t *handle, uint32_t timeout);
;			int32_t		kern_killSemaphore(volatile sema_t *handle);
;			int32_t		kern_restartSemaphore(volatile sema_t *handle);
;			int32_t		kern_getSemaphoreById(const char_t identifier, volatile sema_t **handle);
;
; 			Mailbox system calls
; 			--------------------
;
;			int32_t		kern_createMailbox(const char_t *identifier, volatile mbox_t **handle);
;			int32_t		kern_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size);
;			int32_t		kern_writeQueue(volatile mbox_t *handle, uint32_t message);
;			int32_t		kern_readMailbox(volatile mbox_t *handle, uint8_t **message, uint16_t *size);
;			int32_t		kern_readQueue(volatile mbox_t *handle, uint32_t *message);
;			int32_t		kern_killMailbox(volatile mbox_t *handle);
;			int32_t		kern_getMailboxById(const char_t *identifier, volatile mbox_t **handle);
;
; 			Association system calls
; 			------------------------
;
;			int32_t		kern_createAssociation(const char_t *identifier, volatile astp_t **handle);
;			int32_t		kern_publishAssociation(volatile astp_t *handle, shar_t *pack);
; 			int32_t		kern_findAssociation(volatile astp_t *handle, volatile shar_t **pack);
; 			int32_t		kern_killAssociation(volatile astp_t *handle);
; 			int32_t		kern_getAssociationById(const char_t *identifier, volatile astp_t **handle);
;
; 			Software event system calls
; 			---------------------------
;
;			int32_t		kern_createEvent(const char_t *identifier, volatile evnt_t **handle);
;			int32_t		kern_getBitEvent(evnt_t *handle, uint32_t *bitEvent);
;			int32_t		kern_signalEvent(uint32_t bitEvent, uint32_t mode);
;			int32_t		kern_waitEvent(uint32_t *bitEvent, uint32_t timeout);
;			int32_t		kern_killEvent(volatile evnt_t *handle);
;			int32_t		kern_getEventById(const char_t *identifier, volatile evnt_t **handle);
;
; 			Software signal system calls
; 			----------------------------
;
;			void		kern_init(void);
;			int32_t		kern_createSignal(const char_t *identifier, volatile sign_t **handle);
;			int32_t		kern_getBitSignal(volatile sign_t *handle, uint32_t *bitSignal);
;			int32_t		kern_signalSignal(volatile sign_t *handle, uint32_t bitSignal, volatile proc_t *process, uint32_t mode);
;			int32_t		kern_waitSignal(uint32_t *bitSignal, uint32_t timeout);
;			int32_t		kern_killSignal(volatile sign_t *handle);
;			int32_t		kern_getSignalById(const char_t *identifier, volatile sign_t **handle);
;
; 			Software timer system calls
; 			---------------------------
;
;			int32_t		kern_createSoftwareTimer(const char_t *identifier, volatile stim_t **handle);
;			int32_t		kern_setSoftwareTimer(volatile stim_t *handle, tspc_t *configure);
;			int32_t		kern_killSoftwareTimer(volatile stim_t *handle);
;			int32_t		kern_getSoftwareTimerById(const char_t *identifier, volatile stim_t **handle);
;
; 			Memory pool system calls
; 			------------------------
;
;			int32_t		kern_createPool(const char_t *identifier, volatile pool_t **handle);
;			int32_t		kern_setPool(volatile pool_t *handle, pcnf_t *configure);
;			int32_t		kern_allocateBlock(volatile pool_t *handle, void **address);
;			int32_t		kern_deAllocateBlock(volatile pool_t *handle, void *address);
;			int32_t		kern_killPool(volatile pool_t *handle);
;			int32_t		kern_getPoolById(const char_t *identifier, volatile pool_t **handle);
;
; 			Overlay system calls
; 			--------------------
;
;			int32_t		kern_loadOverlay(uint32_t idModule);
;			int32_t		kern_getOverlay(void **address);
;
; 			Software debug system calls
; 			---------------------------
;
;			int32_t		kern_stopProcess(volatile proc_t *handle);
;			int32_t		kern_reactivateProcess(volatile proc_t *handle);
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

#if (defined(KKERNUSERVERSION))
#define	KKERNVERSION			KKERNUSERVERSION
#else
#define	KKERNVERSION			"11.0"
#endif

volatile	proc_t		*vKern_current;									// Ptr on the current process
static		uint8_t		vKernelState = KNOTREADY;						// The uKernel state

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"kern         kern manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "kern manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Kern, KIDKERN, KKERNNUM, NULL, KKERNVERSION, (1<<BSHOW));

/*
 * \brief Initialize the manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * 
 *    status = kern_init();
 *    installMyProcesses();
 *    status = kern_runKernel();
 *    INTERRUPTION_SET;
 *
 *    while (TRUE);
 * \endcode
 *
 * - This function initializes the "kern" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	kern_init(void) {
	static	bool_t	init = FALSE;

	TRACE("entry: kern_init");
	if (!init) {
		init = TRUE;

		stub_kern_init();
		desc_init();
		temp_init();
		daem_init();

		#if (defined(__WITHASSO__))
		asso_init();
		#endif

		#if (defined(__WITHSEMA__))
		sema_init();
		#endif

		#if (defined(__WITHEVNT__))
		evnt_init();
		#endif

		#if (defined(__WITHSIGN__))
		sign_init();
		#endif

		#if (defined(__WITHMBOX__))
		mbox_init();
		#endif

		#if (defined(__WITHOVLY__))
		ovly_init();
		#endif

		#if (defined(__WITHDEBG__))
		debg_init();
		#endif

		#if (defined(__WITHSTIM__))
		stim_init();
		#endif

		#if (defined(__WITHPOOL__))
		pool_init();
		#endif

		vKernelState = KINITIALIZED;
	}
	TRACE("exit: kern_init OK");
	return (KKERNNOERR);
}

/*
 * \brief Start the multi-process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * 
 *    status = kern_init();
 *    installMyProcesses();
 *    status = kern_runKernel();
 *    INTERRUPTION_SET;
 *
 *    while (TRUE);
 * \endcode
 *
 * - Initialize all the timers (1-ms & 20-ms)
 * - Enable the time sharing and all the interruptions
 *
 * \param[in]	-
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	kern_runKernel(void) {
	static	bool_t	run = FALSE;

	TRACE("entry: kern_runKernel");
	if (!run) {
		run = TRUE;

		vKern_current->oStatistic.oNbKernCalls++;
		stub_kern_runKernel();
		vKernelState = KRUNNING;
	}
	TRACE("exit: kern_runKernel OK");
	return (KKERNNOERR);
}

/*
 * \brief Get the uKernel state
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t   state;
 * 
 *    status = kern_getState(&state);
 *    switch (state) {
 *        case: KNOTREADY    { … break; }
 *        case: KINITIALIZED { … break; }
 *        case: KINITIALIZED { … break; }
 *
 * \endcode
 *
 * - This function return the state of the uKernel
 *
 * \param[in]	*state		Ptr on the uKernel state
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	kern_getState(uint8_t *state) {

	TRACE("entry: kern_getState");
	vKern_current->oStatistic.oNbKernCalls++;
	*state = vKernelState;
	TRACE("exit: kern_getState OK");
	return (KKERNNOERR);
}

/*
 * \brief Get the PC of a process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 * uint32_t    pc;
 * 
 *    status = kern_getPC(process, &pc);
 * \endcode
 *
 * - This function return the PC of a process
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*pc			Ptr on the pc
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	kern_getPC(volatile proc_t *handle, uint32_t *pc) {

	TRACE("entry: kern_getPC");
	vKern_current->oStatistic.oNbKernCalls++;
	stub_kern_getPC(handle->oSpecification.oStkSup, pc);
	TRACE("exit: kern_getPC OK");
	return (KKERNNOERR);
}

/*
 * \brief Get the function name
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    pc;
 * const       char_t    *functionName
 * 
 *    status = kern_getFunctionName(pc, &functionName);
 * \endcode
 *
 * - This function return the name of a function
 *
 * \param[in]	pc			The pc
 * \param[in]	**function	Ptr on the function name
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNSYCNA	System call not available
 *
 */
int32_t	kern_getFunctionName(const uint32_t pc, const char_t **function) {

	TRACE("entry: kern_getFunctionName");
	vKern_current->oStatistic.oNbKernCalls++;

	#if (defined(__ICYFLEX1__))
	*function = NULL;
	return (KKERNSYCNA);

	#else
	stub_kern_getFunctionName(pc, function);
	TRACE("exit: kern_getFunctionName OK");
	return (KKERNNOERR);
	#endif
}
