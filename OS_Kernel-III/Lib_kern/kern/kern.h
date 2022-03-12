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

#ifndef	__KERN__
#define	__KERN__

/*!
 * \addtogroup Lib_kern
 */
/*@{*/

#include	<conf_kernel.h>

#define	KKERNMAN		(KKERNNUM<<8)
#define	KKERNERR		((KIDKERN<<24) | KKERNMAN)

// Kernel states
// - Not ready
// - Initialized
// - Running

#define	KNOTREADY		0
#define	KINITIALIZED	1
#define	KRUNNING		2

// Idle states
// - Enter in the idle process
// - Output from the idle process

#define	KINIDLE			0
#define	KOUTIDLE		1

// uKernel Low power modes (used by the idle process)
// - CPU in a normal running mode
// - CPU in a deep low power mode
// - CPU in a hibernate low power mode
// - CPU in a stop mode

#define	KCPUPNORMAL		0
#define	KCPUPDEEP		1
#define	KCPUPHIBERNATE	2
#define	KCPUPSTOP		3

// Have to be used with the macros
// GOTO_KERN_I and GOTO_KERN_M

// For the uKernel basic services

#define	KNOPARAM		(0x0000<<16)
#define	KJUMPKERN		(KNOPARAM+0x0000)
#define	KWAITTIME		(KNOPARAM+0x0001)
#define	KWAITEVNT		(KNOPARAM+0x0002)
#define	KWAITSIGN		(KNOPARAM+0x0003)

// For the uKernel semaphore synchronizations

#define	KSEMASYN		(0x0001<<16)

// Kernel types

typedef	struct	obje	obje_t;
typedef	struct	shar	shar_t;
typedef	struct	astp	astp_t;
typedef	struct	spec	spec_t;
typedef	struct	work	work_t;
typedef	struct	evnt	evnt_t;
typedef	struct	sign	sign_t;
typedef	struct	stts	stts_t;
typedef	struct	proc	proc_t;
typedef	struct	list	list_t;
typedef	struct	pack	pack_t;
typedef	struct	mbox	mbox_t;
typedef	struct	sema	sema_t;
typedef	struct	atim	atim_t;
typedef	struct	sync	sync_t;
typedef	struct	stim	stim_t;
typedef	struct	tspc	tspc_t;
typedef	struct	pcnf	pcnf_t;
typedef	struct	pool	pool_t;

#include	"chls.h"
#include	"desc.h"
#include	"mbox.h"
#include	"asso.h"
#include	"buff.h"
#include	"stat.h"
#include	"sche.h"
#include	"sema.h"
#include	"evnt.h"
#include	"sign.h"
#include	"temp.h"
#include	"daem.h"
#include	"debg.h"
#include	"stim.h"
#include	"pool.h"
#include	"stub.h"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	kern_createProcess			desc_createProcess
#define	kern_killProcess			desc_killProcess
#define	kern_setPriority			desc_setPriority
#define	kern_getProcessByNb			desc_getProcessByNb
#define	kern_getProcessById			desc_getProcessById
#define	kern_getProcessRun			desc_getProcessRun
#define	kern_setGlobalPtr			desc_setGlobalPtr
#define	kern_setLocalPtr			desc_setLocalPtr
#define	kern_installCallBack		desc_installCallBack
#define	kern_suspendProcess			temp_suspendProcess
#define	kern_setUnixTime			temp_setUnixTime
#define	kern_getUnixTime			temp_getUnixTime
#define	kern_switchFast				temp_switchFast
#define	kern_getTiccount			temp_getTiccount
#define	kern_waitAtLeast			temp_waitAtLeast
#define	kern_runPrecise				temp_runPrecise	
#define	kern_createSyncSemaphore	sema_createSyncSemaphore
#define	kern_createMutxSemaphore	sema_createMutxSemaphore
#define	kern_signalSemaphore		sema_signalSemaphore
#define	kern_unlockSemaphore		sema_unlockSemaphore
#define	kern_waitSemaphore			sema_waitSemaphore
#define	kern_lockSemaphore			sema_lockSemaphore
#define	kern_killSemaphore			sema_killSemaphore
#define	kern_restartSemaphore		sema_restartSemaphore
#define	kern_getSemaphoreById		sema_getSemaphoreById
#define	kern_createMailbox			mbox_createMailbox
#define	kern_writeMailbox			mbox_writeMailbox
#define	kern_writeQueue				mbox_writeQueue
#define	kern_readMailbox			mbox_readMailbox
#define	kern_readQueue				mbox_readQueue
#define	kern_killMailbox			mbox_killMailbox
#define	kern_getMailboxById			mbox_getMailboxById
#define	kern_createAssociation		asso_createAssociation
#define	kern_publishAssociation		asso_publishAssociation
#define	kern_findAssociation		asso_findAssociation
#define	kern_killAssociation		asso_killAssociation
#define	kern_getAssociationById		asso_getAssociationById
#define	kern_createEvent			evnt_createEvent
#define	kern_getBitEvent			evnt_getBitEvent
#define	kern_signalEvent			evnt_signalEvent
#define	kern_waitEvent				evnt_waitEvent
#define	kern_killEvent				evnt_killEvent
#define	kern_getEventById			evnt_getEventById
#define	kern_createSignal			sign_createSignal
#define	kern_getBitSignal			sign_getBitSignal
#define	kern_signalSignal			sign_signalSignal
#define	kern_waitSignal				sign_waitSignal
#define	kern_killSignal				sign_killSignal
#define	kern_getSignalById			sign_getSignalById
#define	kern_getOverlay				ovly_getOverlay
#define	kern_stopProcess			debg_stopProcess
#define	kern_reactivateProcess		debg_reactivateProcess
#define	kern_createSoftwareTimer	stim_createSoftwareTimer
#define	kern_setSoftwareTimer		stim_setSoftwareTimer
#define	kern_killSoftwareTimer		stim_killSoftwareTimer
#define	kern_getSoftwareTimerById	stim_getSoftwareTimerById
#define	kern_createPool				pool_createPool
#define	kern_setPool				pool_setPool
#define	kern_allocateBlock			pool_allocateBlock
#define	kern_deAllocateBlock		pool_deAllocateBlock
#define	kern_killPool				pool_killPool
#define	kern_getPoolById			pool_getPoolById

/*!
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
extern	int32_t	kern_init(void);

/*!
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
extern	int32_t	kern_runKernel(void);

/*!
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
extern	int32_t	kern_getState(uint8_t *state);

/*!
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
extern	int32_t	kern_getPC(volatile proc_t *handle, uint32_t *pc);

/*!
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
extern	int32_t	kern_getFunctionName(const uint32_t pc, const char_t **function);

/*!
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
extern	int32_t	kern_createProcess(const spec_t *specification, const void *argument, volatile proc_t **handle);

/*!
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
extern	int32_t	kern_killProcess(volatile proc_t *handle);

/*!
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
extern	int32_t	kern_setPriority(volatile proc_t *handle, uint8_t priority);

/*!
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
extern	int32_t	kern_getProcessByNb(uint8_t number, volatile proc_t **handle);

/*!
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
extern	int32_t	kern_getProcessById(const char_t *identifier, volatile proc_t **handle);

/*!
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
extern	int32_t	kern_getProcessRun(volatile proc_t **handle);

/*!
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
extern	int32_t	kern_setGlobalPtr(const void **global);

/*!
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
extern	int32_t	kern_setLocalPtr(const void *local);

/*!
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
extern	int32_t	kern_installCallBack(void (*code)(uint8_t state));

/*!
 * \brief Suspend the process for a time
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KWAIT312MS    312    // Wait for 312-ms

 * int32_t    status;
 *
 *    status = kern_suspendProcess(KWAIT312MS);
 * \endcode
 *
 * - Disconnect a process from the execution list
 * - Connect the process to the time list
 *
 * \param[in]	time		Suspend the process for a time (1-ms of resolution)
 * \param[out]	KKERNNOERR	OK
 *
 */
extern	int32_t	kern_suspendProcess(uint32_t time);

/*!
 * \brief Set the UNIX absolute time
 *
 * Call example in C:
 *
 * \code{.c}
 * atim_t     time;
 * int32_t    status;
 *
 *    time.oYear    = 2016;
 *    time.oMonths  = KJANUARY;
 *    time.oDays    = 1;
 *    time.oHours   = 12;
 *    time.oMinutes = 27;
 *    time.oSeconds = 0;
 *    status = kern_setUnixTime(&time);
 * \endcode
 *
 * - From the absolute calendar time compute the absolute binary 32-bit 
 *   00 .. 59				Seconds
 *   00 .. 59				Minutes
 *   00 .. 23				Hours
 *   00 .. 06				Week days -> 0 = Sunday
 *   01 .. 28, 29, 30, 31	Days
 *   01 .. 12				Months
 *   1970 .. 2106
 *
 * \param[in]	*time		Ptr on the time structure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNTIFMT	Time format error
 *
 */
extern	int32_t	kern_setUnixTime(const atim_t *time);

/*!
 * \brief Get the UNIX absolute time
 *
 * Call example in C:
 *
 * \code{.c}
 * atim_t     time;
 * int32_t    status;
 *
 *    status = kern_getUnixTime(&time);
 *    iotx_printf(KSYST, “YYYY %d, MM %d, DD %d, WD %d, hh %d, mm %d, ss %d UNIX time %8X\n”, \
 *                        time.oYear, time.oMonths, time.oDays, time.oWeekDays, \
 *                        time.oHours, time.oMinutes, time.oSeconds, time.oWeekDays, time.oAbsolute);
 * \endcode
 *
 * - From the absolute binary 32-bit compute the absolute calendar time 
 *   00 .. 59				Seconds
 *   00 .. 59				Minutes
 *   00 .. 23				Hours
 *   00 .. 06				Week days -> 0 = Sunday
 *   01 .. 28, 29, 30, 31	Days
 *   01 .. 12				Months
 *   1970 .. 2106
 *
 * \param[in]	*time		Ptr on the time structure
 * \param[out]	KKERNNOERR	OK
 *
 */
extern	int32_t	kern_getUnixTime(atim_t *time);

/*!
 * \brief Force the context switching
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = kern_switchFast();
 * \endcode
 *
 * - Force a context switch
 *
 * \param[in]	-
 * \param[out]	KKERNNOERR	OK
 *
 */
extern	int32_t	kern_switchFast(void);

/*!
 * \brief Get the ticcount (1-us) from the start of the uKernel
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint64_t    ticcount;
 *
 *    status = temp_getTiccount(&ticcount);
 * \endcode
 *
 * - Return the 1-us ticcount value from the start of the uKernel
 *
 * \param[in]	*ticcount	Ptr on the tic-count
 * \param[out]	KKERNNOERR	OK
 *
 */
extern	int32_t	kern_getTiccount(uint64_t *ticcount);

/*!
 * \brief Waiting for short times (at least ... a certain time)
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KWAITMIN    120    // Wait at least 120-µs
 *
 * int32_t    status;
 *
 *    status = kern_waitAtLeast(KWAITMIN);
 * \endcode
 *
 * - Return when the time is reached. Max. 65536-uS
 *   This call ensure the minimum time but not the maximum
 *
 * \param[in]	time		Suspend the process for a time (1-us of resolution)
 * \param[out]	KKERNNOERR	OK
 *
 */
extern	int32_t	kern_waitAtLeast(uint16_t time);

/*!
 * \brief Execute cyclically a routine with a precise time
 *
 * Call example in C:
 *
 * \code{.c}
 * status = kern_runPrecise(100, (void (*)(void))aStateMachine);
 * int32_t    status;
 *
 *    // My callback routine
 *
 *    static    void    aStateMachine(void) {
 *       static    uint32_t    vNb = 0;
 *       static    enum    { KSTATE1, KSTATE2, KSTATE3 } vState = KSTATE1;
 *    
 *       switch (vState) {
 *           case KSTATE1: {
 *               if (++vNb < 1)  {                            break; }
 *               else            { vNb = 0; vState = KSTATE2; break; }
 *           }
 *           case KSTATE2: {
 *               if (++vNb < 10) {                            break; }
 *               else            { vNb = 0; vState = KSTATE3; break; }
 *           }
 *           case KSTATE3: {
 *               if(++vNb < 5)   {                            break; }
 *               else            { vNb = 0; vState = KSTATE1; break; }
 *           }
 *       }
 *    }
 * \endcode
 *
 * - time = 0, stop the execution
 * - time > 0 and <= 6553- us
 *
 * \param[in]	time		Sampling time
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNSYCNA	System call not available
 *
 */
extern	int32_t	kern_runPrecise(uint16_t time, void (*code)(void));

/*!
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
extern	int32_t	kern_createSyncSemaphore(const char_t *identifier, volatile sema_t **handle);

/*!
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
extern	int32_t	kern_createMutxSemaphore(const char_t *identifier, volatile sema_t **handle);

/*!
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
extern	int32_t	kern_signalSemaphore(volatile sema_t *handle);

/*!
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
extern	int32_t	kern_unlockSemaphore(volatile sema_t *handle);

/*!
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
extern	int32_t	kern_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout);

/*!
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
extern	int32_t	kern_lockSemaphore(volatile sema_t *handle, uint32_t timeout);

/*!
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
extern	int32_t	kern_killSemaphore(volatile sema_t *handle);

/*!
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
extern	int32_t	kern_restartSemaphore(volatile sema_t *handle);

/*!
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
extern	int32_t	kern_getSemaphoreById(const char_t *identifier, volatile sema_t **handle);

/*!
 * \brief Create a mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Mailbox 0”;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_createMailbox(identifier, &mailBox);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the mailbox Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNMBFUL 	No more mailbox
 * \param[out]	KKERNIDMBO	The mailbox Id is already used
 *
 */
extern	int32_t	kern_createMailbox(const char_t *identifier, volatile mbox_t **handle);

/*!
 * \brief Write a message in the mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * #define     KSIZE    5
 * int32_t     status;
 * uint8_t     message[KSIZE] = {1,2,3,4,5};
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_writeMessage(mailBox, message, KSIZE);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*message	Ptr on the message
 * \param[in]	size		Size of the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNTOPAC 	The mailbox is full
 *
 */
extern	int32_t	kern_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size);

/*!
 * \brief Write a message in the queue
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint8_t     message = 0x2345;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_writeQueue(mailBox, message);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	message		Message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNTOPAC 	The mailbox is full
 *
 */
extern	int32_t	kern_writeQueue(volatile mbox_t *handle, uint32_t message);

/*!
 * \brief Read a message from the mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint8_t     size, *message:
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_readMessage(mailBox, &message, &size);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	**message	Ptr on the message
 * \param[in]	*size		Ptr on the	size of the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNNOPAC 	The mailbox is empty
 *
 */
extern	int32_t	kern_readMailbox(volatile mbox_t *handle, uint8_t **message, uint16_t *size);

/*!
 * \brief Read a message from the queue
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    message:
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_readQueue(mailBox, &message);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*message	Ptr on the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNNOPAC 	The mailbox is empty
 *
 */
extern	int32_t	kern_readQueue(volatile mbox_t *handle, uint32_t *message);

/*!
 * \brief Kill the mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_killMailbox(mailBox);
 * \endcode
 *
 * - If (mailbox still contain messages)
 *   Then return error
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNPAPRP 	The mailbox cannot be removed; a message is still inside the mailbox
 *
 */
extern	int32_t	kern_killMailbox(volatile mbox_t *handle);

/*!
 * \brief Get the handle of a mailbox by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Mailbox 0”;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_getMailboxById(identifier, &mailBox);
 * \endcode
 *
 * - This function returns the handle of the mailbox
 *
 * \param[in]	*identifier	Ptr on the mailbox Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 *
 */
extern	int32_t	kern_getMailboxById(const char_t *identifier, volatile mbox_t **handle);

/*!
 * \brief Create an association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Association 0”;
 * volatile    astp_t    *association;
 *
 *    status = kern_createAssociation(identifier, &association);
 * \endcode
 *
 * - Associate a string to a generic pointer
 *
 * \param[in]	*identifier	Ptr on the association Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNASFUL 	No more association
 * \param[out]	KKERNIDASS	The association Id is already used
 *
 */
extern	int32_t	kern_createAssociation(const char_t *identifier, volatile astp_t **handle);

/*!
 * \brief Publish an association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    sharedBuffer[123];
 * shar_t      pack;
 * volatile    astp_t    *association;
 *
 *    pack.oGeneral = (void *)&sharedBuffer[0];
 *    pack.oSize = sizeof(sharedBuffer);
 *    status = kern_publishAssociation(association, &pack);
 * \endcode
 *
 * - Publish the association
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*pack		Ptr on the data pack association
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 * \param[out]	KKERNASNOI 	The association is not initialized
 *
 */
extern	int32_t kern_publishAssociation(volatile astp_t *handle, shar_t *pack);

/*!
 * \brief Find an association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    *counter, size;
 * shar_t      pack;
 * volatile    astp_t    *association;
 *
 *    status = kern_findAssociation(association, &pack);
 *    counter = (uint32_t *)pack->oGeneral;
 *    size = pack->oSize;
 * \endcode
 *
 * - Find an association (check for the string reference)
 * - Return the associated pointer
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	**pack		Ptr on the data pack association
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 * \param[out]	KKERNASNOI 	The association is not initialized
 *
 */
extern	int32_t	kern_findAssociation(volatile astp_t *handle, volatile shar_t **pack);

/*!
 * \brief Kill the association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    astp_t    *association;
 *
 *    status = kern_killAssociation(association);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 * \param[out]	KKERNASNOI 	The association is not initialized
 *
 */
extern	int32_t	kern_killAssociation(volatile astp_t *handle);

/*!
 * \brief Get the handle of an association by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Association 0”;
 * volatile    astp_t    *association;
 *
 *    status = kern_getAssociationById(identifier, &association);
 * \endcode
 *
 * - This function returns the handle of the association
 *
 * \param[in]	*identifier	Ptr on the association Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 *
 */
extern	int32_t	kern_getAssociationById(const char_t *identifier, volatile astp_t **handle);

/*!
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
extern	int32_t	kern_createEvent(const char_t *identifier, volatile evnt_t **handle);

/*!
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
extern	int32_t	kern_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent);

/*!
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
extern	int32_t	kern_signalEvent(uint32_t bitEvent, uint32_t mode);

/*!
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
extern	int32_t	kern_waitEvent(uint32_t *bitEvent, uint32_t timeout);

/*!
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
extern	int32_t	kern_killEvent(volatile evnt_t *handle);

/*!
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
extern	int32_t	kern_getEventById(const char_t *identifier, volatile evnt_t **handle);

/*!
 * \brief Create a signal
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”BTLE message”;
 * volatile    sign_t    *signal;
 *
 *    status = kern_createSignal(identifier, &signal);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the signal Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSIFUL 	No more signal
 * \param[out]	KKERNIDSIO	The signal Id is already used
 *
 */
extern	int32_t	kern_createSignal(const char_t *identifier, volatile sign_t **handle);

/*!
 * \brief Get the bit signal
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    bitSignal;
 * volatile    sign_t    *signal;
 *
 *    status = kern_signalSignal(signal, &bitSignal);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*bitSignal	Ptr on the bitSignal
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 * \param[out]	KKERNSINOI 	The signal is not initialized
 *
 */
extern	int32_t	kern_getBitSignal(volatile sign_t *handle, uint32_t *bitSignal);

/*!
 * \brief Signal signals
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    bitSignal;
 * volatile    sign_t    *signal;
 * volatile    proc_t    *process;
 *
 *    while (kern_getProcessById(“My process”, &process) != KKERNNOERR) {
 *        kern_suspendProcess(1);
 *    }
 *
 * // Mode selective
 *
 *    status = kern_signalSignal(signal, bitSignal, process, KSYNCCONT);
 *
 * // Mode broadcast
 *
 *    status = kern_signalSignal(signal, bitSignal, NULL, KSYNCCONT);
 * \endcode
 *
 * - If (mode == KSIGNSWTC)
 *   - With the context switching
 *
 * - If (mode == KSIGNCONT)
 *   - Without the context switching
 *
 * - Scan all the installed processes
 *   If the process is not suspended
 *   then, save the new signal in the process->oSynchro.oListSignals
 *   else, if the process->oSynchro.oListSignals & signal !=
 *         then, cancel the matched signal and connect the processes to the execution list
 *         else, next 
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	bitSignal	Signal (1 or more)
 * \param[in]	*process	Ptr on the process handle (selective signal)
 * \param[in]				NULL, broadcast to all the installed processes the signals
 * \param[in]	mode		KSIGNSWTC -> (synchro with the context switching)
 * \param[in]				KSIGNCONT -> (synchro without the context switching)
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 * \param[out]	KKERNSINOI 	The signal is not initialized
 * \param[out]	KKERNNOPRO 	The process does not exist
 *
 */
extern	int32_t	kern_signalSignal(volatile sign_t *handle, uint32_t bitSignal, volatile proc_t *process, uint32_t mode);

/*!
 * \brief Wait for signals
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIGNAL_MSG    0x00080000
 *
 * int32_t     status;
 * volatile    sign_t    *signal;
 *
 *    status = kern_getSignalById(“message OK”, &signal);
 *    status = kern_waitSignal(KSIGNAL_MSG, KSIGNCONT);
 * \endcode
 *
 * - Disconnect a process from the execution list
 * - Connect the process to the signal list
 *
 * \param[in]	*bitSignal	Ptr on the signal (1 or more)
 * \param[in]	timeout		Timeout
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSIKIL  The signal has been killed (with processes in its list)
 * \param[out]	KKERNTIMEO 	Timeout
 *
 */
extern	int32_t	kern_waitSignal(uint32_t *bitSignal, uint32_t timeout);

/*!
 * \brief Kill the signal
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sign_t    *signal;
 *
 *    status = kern_killSignal(“message OK”, &signal);
 * \endcode
 *
 * - If (process still connected to the signal list)
 *   - Then return error
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 * \param[out]	KKERNSINOI 	The signal is not initialized
 *
 */
extern	int32_t	kern_killSignal(volatile sign_t *handle);

/*!
 * \brief Get the handle of an signal by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Signal 0”;
 * volatile    sign_t    *signal;
 *
 *    status = kern_getSignalById(identifier, &signal);
 * \endcode
 *
 * - This function returns the handle of the signal
 *
 * \param[in]	*identifier	Ptr on the signal Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 *
 */
extern	int32_t	kern_getSignalById(const char_t *identifier, volatile sign_t **handle);

/*!
 * \brief Load an overlay process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = kern_ loadOverlay(“OV01”);
 * \endcode
 *
 * - This function loads an overlay process into the execution RAM
 *
 * \param[in]	idModule	"XXYY" module Id
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNOVRLY	The overlay process does not exist
 *
 */
extern	int32_t	kern_loadOverlay(uint32_t idModule);

/*!
 * \brief Get the overlay execution RAM address
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * void       *address;
 *
 *    status = kern_ getOverlay(&address);
 * \endcode
 *
 * - This function returns the overlay execution RAM
 *
 * \param[in]	**address	Ptr on the overlay process entry address
 * \param[out]	KKERNNOERR	OK
 *
 */
extern	int32_t	kern_getOverlay(void **address);

/*!
 * \brief Stop a process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_stopProcess(process);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 * \param[out]	KKERNDBGER	The process is already in the debug list
 *
 */
extern	int32_t	kern_stopProcess(volatile proc_t *handle);

/*!
 * \brief Reactivate a process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_reactivateProcess(process);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 * \param[out]	KKERNDBGER	The process is not in the debug list
 *
 */
extern	int32_t	kern_reactivateProcess(volatile proc_t *handle);

/*!
 * \brief Create a software timer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Timer acquisition”;
 * volatile    stim_t    *softwareTimer;
 *
 *    status = kern_createSoftwareTimer(identifier, &softwareTimer);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the software timer
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSTFUL 	No more software timer
 * \param[out]	KKERNIDSTI	The software timer Id is already used
 *
 */
extern	int32_t	kern_createSoftwareTimer(const char_t *identifier, volatile stim_t **handle);

/*!
 * \brief Set a software timer
 *
 * Call example in C:
 *
 * \code{.c}
 * static      void    _changeStateLed(void);
 *
 * int32_t     status;
 * tspc_t      configure;
 * volatile    stim_t    *softwareTimer;
 *
 *    configure.oMode     = KCONTINUE;
 *    configure.oIniTime  = 10000;
 *    configure.oTime     = 1000;
 *    configure.oArgument = NULL;
 *    configure.oCode     = _changeStateLed;
 *
 *    status = kern_setSoftwareTimer(&softwareTimer, &configure);
 *
 *    // My callback routine
 *
 *    static    void    _changeStateLed(void) {
 *
 *        misc_toggleLed(0);
 *    }
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSTI	The software timer does not exist
 * \param[out]	KKERNSTNOI 	The software timer is not initialized
 *
 */
extern	int32_t	kern_setSoftwareTimer(volatile stim_t *handle, tspc_t *configure);

/*!
 * \brief Kill the software timer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    stim_t    *softwareTimer;
 *
 *    status = kern_killSoftwareTimer(softwareTimer);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSTI	The software timer does not exist
 * \param[out]	KKERNSTNOI 	The software timer is not initialized
 *
 */
extern	int32_t	kern_killSoftwareTimer(volatile stim_t *handle);

/*!
 * \brief Get the handle of a software timer by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Timer acquisition”;
 * volatile    stim_t    *softwareTimer;
 *
 *    status = kern_getSoftwareTimerById(identifier, &softwareTimer);
 * \endcode
 *
 * - This function returns the handle of the software timer
 *
 * \param[in]	*identifier	Ptr on the software timer Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSTI	The software timer does not exist
 *
 */
extern	int32_t	kern_getSoftwareTimerById(const char_t *identifier, volatile stim_t **handle);

/*!
 * \brief Create a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Xfer buffer”;
 * volatile    pool_t    *memoryPool;
 *
 *    status = kern_createPool(identifier, &memoryPool);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the memory pool
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNPOFUL 	No more memory pool
 * \param[out]	KKERNIDPOI	The memory pool Id is already used
 *
 */
extern	int32_t	kern_createPool(const char_t *identifier, volatile pool_t **handle);

/*!
 * \brief Set a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * pcnf_t      configure;
 * volatile    pool_t    *memoryPool;
 *
 *    configure.oNbBlocks  = 16;
 *    configure.oBlockSize = 256;
 *    configure.oLocation  = KINTERNAL;
 *
 *    status = kern_setPool(memoryPool, &configure);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 * \param[out]	KKERNSTNOI 	The memory pool configuration is not possible
 *
 */
extern	int32_t	kern_setPool(volatile pool_t *handle, pcnf_t *configure);

/*!
 * \brief Allocate a block in a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    pool_t    *memoryPool;
 * void        *address;
 *
 *    status = kern_allocateBlock(memoryPool, &address);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 * \param[out]	KKERNBKFUL 	No more block
 *
 */
extern	int32_t	kern_allocateBlock(volatile pool_t *handle, void **address);

/*!
 * \brief deallocate a block in a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    pool_t    *memoryPool;
 * void        *address;
 *
 *    status = kern_deAllocateBlock(memoryPool, address);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 * \param[out]	KKERNNOBKI 	The block does not exist
 *
 */
extern	int32_t	kern_deAllocateBlock(volatile pool_t *handle, void *address);

/*!
 * \brief Kill the memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    pool_t    *memoryPool;
 *
 *    status = kern_killSoftwareTimer(memoryPool);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 *
 */
extern	int32_t	kern_killPool(volatile pool_t *handle);

/*!
 * \brief Get the handle of a memory pool by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Xfer buffer”;
 * volatile    pool_t    *memoryPool;
 *
 *    status = kern_getPoolById(identifier, &memoryPool);
 * \endcode
 *
 * - This function returns the handle of the memory pool
 *
 * \param[in]	*identifier	Ptr on the memory pool Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 *
 */
extern	int32_t	kern_getPoolById(const char_t *identifier, volatile pool_t **handle);

#ifdef	__cplusplus
}
#endif

// kern manager errors
// -------------------

#define	KKERNNOERR	0				// No error
#define	KKERNSYCNA	-(KKERNERR+1)	// System call not available
#define	KKERNGEERR	-(KKERNERR+2)	// General error
#define	KKERNTIMEO	-(KKERNERR+3)	// Timeout error

#define	KKERNLIFUL	-(KKERNERR+4)	// No more process
#define	KKERNNOPRO	-(KKERNERR+5)	// The process does not exist
#define	KKERNIDPRO	-(KKERNERR+6)	// The process Id is already used
#define	KKERNREFRS	-(KKERNERR+7)	// The first cannot be removed
#define	KKERNRESDE	-(KKERNERR+8)	// The system daemons cannot be removed
#define	KKERNNOSTK	-(KKERNERR+9)	// No memory for the stack

#define	KKERNASFUL	-(KKERNERR+10)	// No more association
#define	KKERNNOASS	-(KKERNERR+11)	// The association does not exist
#define	KKERNIDASS	-(KKERNERR+12)	// The association Id is already used
#define	KKERNASNOI	-(KKERNERR+13)	// The association is not initialized

#define	KKERNSEFUL	-(KKERNERR+14)	// No more semaphore
#define	KKERNNOSEM	-(KKERNERR+15)	// The semaphore does not exist
#define	KKERNIDSEM	-(KKERNERR+16)	// The semaphore Id is already used
#define	KKERNSENOI	-(KKERNERR+17)	// The semaphore is not initialized
#define	KKERNSETME	-(KKERNERR+18)	// The semaphore counts too many events
#define	KKERNSEKIL	-(KKERNERR+19)	// The semaphore has been killed (with processes in its list)

#define	KKERNMBFUL	-(KKERNERR+20)	// No more mailbox
#define	KKERNNOMBO	-(KKERNERR+21)	// The mailbox does not exist
#define	KKERNIDMBO	-(KKERNERR+22)	// The mailbox Id is already used
#define	KKERNMBNOI	-(KKERNERR+23)	// The mailbox is not initialized
#define	KKERNTOPAC	-(KKERNERR+24)	// The mailbox is full
#define	KKERNNOPAC	-(KKERNERR+25)	// The mailbox is empty
#define	KKERNPAPRP	-(KKERNERR+26)	// The mailbox cannot be removed; a message is still inside the mailbox

#define	KKERNEVFUL	-(KKERNERR+27)	// No more event
#define	KKERNNOEVO	-(KKERNERR+28)	// The event does not exist
#define	KKERNIDEVO	-(KKERNERR+29)	// The event Id is already used
#define	KKERNEVNOI	-(KKERNERR+30)	// The event is not initialized
#define	KKERNEVKIL	-(KKERNERR+31)	// The event has been killed (with processes in its list)

#define	KKERNSIFUL	-(KKERNERR+32)	// No more signal
#define	KKERNNOSIO	-(KKERNERR+33)	// The signal does not exist
#define	KKERNIDSIO	-(KKERNERR+34)	// The signal Id is already used
#define	KKERNSINOI	-(KKERNERR+35)	// The signal is not initialized
#define	KKERNSIKIL	-(KKERNERR+36)	// The signal has been killed (with processes in its list)

#define	KKERNOVRLY	-(KKERNERR+37)	// The overlay process does not exist
#define	KKERNTIFMT	-(KKERNERR+38)	// The absolute time has a wrong format

#define	KKERNDBGER	-(KKERNERR+39)	// The process is not (or already) in the debug list

#define	KKERNSTFUL	-(KKERNERR+40)	// No more software timer
#define	KKERNNOSTI	-(KKERNERR+41)	// The software timer does not exist
#define	KKERNIDSTI	-(KKERNERR+42)	// The software timer Id is already used
#define	KKERNSTNOI	-(KKERNERR+43)	// The software timer is not initialized

#define	KKERNPOFUL	-(KKERNERR+44)	// No more memory pool
#define	KKERNBKFUL	-(KKERNERR+45)	// No more block
#define	KKERNNOPOI	-(KKERNERR+46)	// The memory pool does not exist
#define	KKERNIDPOI	-(KKERNERR+47)	// The memory pool Id is already used
#define	KKERNPOCNF	-(KKERNERR+48)	// The memory pool configuration is not possible
#define	KKERNPONOI	-(KKERNERR+49)	// The memory pool is not initialized
#define	KKERNNOBKI	-(KKERNERR+50)	// The block does not exist

/*@}*/

#endif
