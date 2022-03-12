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

#ifndef	__DESC__
#define	__DESC__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	desc_init(void);
extern	int32_t	desc_createProcess(const spec_t *specification, const void *argument, volatile proc_t **handle);
extern	int32_t	desc_killProcess(volatile proc_t *handle);
extern	int32_t	desc_setPriority(volatile proc_t *handle, uint8_t priority);
extern	int32_t	desc_getProcessByNb(uint8_t number, volatile proc_t **handle);
extern	int32_t	desc_getProcessById(const char_t *identifier, volatile proc_t **handle);
extern	int32_t	desc_getProcessRun(volatile proc_t **handle);
extern	int32_t	desc_setGlobalPtr(const void **global);
extern	int32_t	desc_setLocalPtr(const void *local);
extern	int32_t	desc_installCallBack(void (*code)(uint8_t state));
extern	int32_t	desc_callBack(void);

#ifdef	__cplusplus
}
#endif

#define	PROC_USER(index, specification, infoStr, lenStack, code, identifier, commManager, priority, overlayPack, kind) \
	static				uint32_t 	vStackSup_##index[KSZSTACKMIN] __attribute__ ((aligned (8))); \
	static				uint32_t 	vStackUsr_##index[lenStack]    __attribute__ ((aligned (8))); \
	static	const		spec_t		specification = { \
										.oIdentifier	= identifier, \
										.oText  		= infoStr, \
										.oCode  		= (void (*)(const void *argument))code, \
										.oStkSupBegin	= &vStackSup_##index[0], \
										.oStkSup 		= &vStackSup_##index[lenStack-8], \
										.oStkUsr 		= &vStackUsr_##index[lenStack-8], \
										.oSzStk 		= lenStack-8, \
										.oStackMode		= KSTKSTATIC, \
										.oPriority 		= priority, \
										.oIdPack 		= overlayPack, \
										.oCommManager 	= commManager, \
										.oKind 			= kind, \
										.oMode 			= KUSER \
									};

#define	PROC_SUPV(index, specification, infoStr, lenStack, code, identifier, commManager, priority, overlayPack, kind) \
	static				uint32_t 	vStackSup_##index[lenStack] __attribute__ ((aligned (8))); \
	static	const		spec_t		specification = { \
										.oIdentifier	= identifier, \
										.oText  		= infoStr, \
										.oCode  		= (void (*)(const void *argument))code, \
										.oStkSupBegin	= &vStackSup_##index[0], \
										.oStkSup 		= &vStackSup_##index[lenStack-8], \
										.oStkUsr 		= NULL, \
										.oSzStk 		= lenStack-8, \
										.oStackMode		= KSTKSTATIC, \
										.oPriority 		= priority, \
										.oIdPack 		= overlayPack, \
										.oCommManager 	= commManager, \
										.oKind 			= kind, \
										.oMode 			= KSUPERVISOR \
									};

#define	PROC_SUPV_STACKMALLOC(index, where, specification, infoStr, lenStack, code, identifier, commManager, priority, overlayPack, kind) \
						uint32_t	*vStackSup_##index = (uint32_t *)((((uint32_t)syos_malloc(where, (lenStack*sizeof(uint32_t))+8U))+7U)&(~0x7U)); \
						spec_t		specification = { \
										.oIdentifier	= identifier, \
										.oText  		= infoStr, \
										.oCode  		= (void (*)(const void *argument))code, \
										.oStkSupBegin	= &vStackSup_##index[0], \
										.oStkSup 		= &vStackSup_##index[lenStack-8], \
										.oStkUsr 		= NULL, \
										.oSzStk 		= lenStack-8, \
										.oStackMode		= KSTKDYNAMIC, \
										.oPriority 		= priority, \
										.oIdPack 		= overlayPack, \
										.oCommManager 	= commManager, \
										.oKind 			= kind, \
										.oMode 			= KSUPERVISOR \
									};

// Structure for the process specifier
// -----------------------------------

struct spec {
	const		char_t		*oIdentifier;								// Process identifier
	const		char_t		*oText;										// Ptr on the process Id text
	
				uint32_t	*oStkSupBegin;								// Ptr on the begin of process stack (supervisor)
				uint32_t	*oStkSup;									// Ptr on the process stack (supervisor)
				uint32_t	*oStkUsr;									// Ptr on the process stack (user)
				uint32_t	oSzStk;										// Size of the process stack (supervisor and user)
				void		(*oCode)(const void *argument);				// Ptr on the process code
				uint8_t		oStackMode;									// Stack mode
				#define		KSTKSTATIC		0							// KSTKSTATIC = static stack allocation
				#define		KSTKDYNAMIC		1							// KSTKDYNAMIC = dynamic stack allocation

				uint8_t		oKind;										// Process kind
				#define		KPROCNORMAL		0							// KPROCNORMAL = normal process
				#define		KPROCDAEMON		1							// KPROCDAEMON = daemon process
				#define		KPROCOVERLAY	2							// KPROCOVERLAY = overlay process

				uint8_t		oMode;										// Process run mode
				#define		KUSER			0							// KUSER = process running in a user space
				#define		KSUPERVISOR		1							// KSUPERVISOR = process running in a supervisor space

				uint8_t		oPriority;									// Process priority
				uint32_t	oCommManager;								// Default I/O channel
				uint32_t	oIdPack;									// Overlay pack of processes
};

// Structure for the uKernel working states
// ----------------------------------------

struct work {
				uint8_t		oState;										// Process state
				#define		BPROCINSTALLED	0							// Process installed
				#define		BPROCRUNNING	1							// Process running
				#define		BPROCSUSPTIME	2							// Process suspended for a time
				#define		BPROCSUSPEVNT	3							// Process suspended for an event
				#define		BPROCSUSPSIGN	4							// Process suspended for an signal
				#define		BPROCSUSPSEMA	5							// Process suspended for a semaphore
				#define		BPROCSUSPDEBG	6							// Process suspended for a debug

				int32_t		oStatus;									// Status
				uint32_t	oTimeout;									// Process timeout	
				uint16_t	oSkip;										// Number of time that the process was skipped
				uint8_t		oDynaPriority;								// Process priority	dynamic

	const		void		*oLocal;									// Ptr on a general local structure
};

// Structure for the uKernel synchros (events and semaphores)
// ----------------------------------------------------------

struct sync {
				uint32_t	oFlags;										// For cmsis compatibility
				uint32_t	oListEvents;								// List of events (1 bit per event)
				uint32_t	oListEventsPending;							// List of pending events (1 bit per event)
				uint32_t	oListSignals;								// List of signals (1 bit per signal)
				uint32_t	oListSignalsPending;						// List of pending signals (1 bit per signal)
				uint32_t	oNbSyncSema;								// Number of synchros for a semaphore
};

// Structure for the statistics
// ----------------------------

struct stts {
				uint64_t	oNbExecutions;								// Number of time that the process has been scheduled
				uint64_t	oNbKernCalls;								// Number of uKernel system calls

				uint32_t	oAvStkSup;									// Available size in the supervisor stack
				uint16_t	oTimePMin;									// Min CPU time used by the Process
				uint16_t	oTimePMax;									// Max CPU time used by the Process
				uint16_t	oTimePAvg;									// Avg CPU time used by the Process
				uint64_t	oTimePCum;									// Cum CPU time used by the Process
				uint16_t	oTimeKMin;									// Min CPU time used by the uKernel
				uint16_t	oTimeKMax;									// Max CPU time used by the uKernel
				uint16_t	oTimeKAvg;									// Avg CPU time used by the uKernel
				uint64_t	oTimeKCum;									// Cum CPU time used by the uKernel
				uint16_t	oTimeEMin;									// Min CPU time used by the Exceptions
				uint16_t	oTimeEMax;									// Max CPU time used by the Exceptions
				uint16_t	oTimeEAvg;									// Avg CPU time used by the Exceptions
				uint64_t	oTimeECum;									// Cum CPU time used by the Exceptions
};

// Structure of the process
// ------------------------

struct proc {
				obje_t		oObject;									// Process object (connectable)
				spec_t		oSpecification;								// Process specification
				work_t		oInternal;									// Process internal stuff
				stts_t		oStatistic;									// uKernel statistic
				sync_t		oSynchro;									// uKernel synchro (event & semaphores)
};
#endif
