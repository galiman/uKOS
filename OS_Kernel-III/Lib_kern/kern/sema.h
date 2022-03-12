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

#ifndef	__SEMA__
#define	__SEMA__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	sema_init(void);
extern	int32_t	sema_createSyncSemaphore(const char_t *identifier, volatile sema_t **handle);
extern	int32_t	sema_createMutxSemaphore(const char_t *identifier, volatile sema_t **handle);
extern	int32_t	sema_signalSemaphore(volatile sema_t *handle);
extern	int32_t	sema_unlockSemaphore(volatile sema_t *handle);
extern	int32_t	sema_waitSemaphore(volatile sema_t *handle, uint32_t nbEvents, uint32_t timeout);
extern	int32_t	sema_lockSemaphore(volatile sema_t *handle, uint32_t timeout);
extern	int32_t	sema_killSemaphore(volatile sema_t *handle);
extern	int32_t	sema_restartSemaphore(volatile sema_t *handle);
extern	int32_t	sema_getSemaphoreById(const char_t *identifier, volatile sema_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the semaphore
// --------------------------

struct	sema {
	const		char_t		*oIdentifier;								// Semaphore identifier
				uint8_t		oState;										// State
				#define		BSEMAINSTALLED	0							// Semaphore installed
				#define		BSEMASYNCHRO	1							// Synchro semaphore
				#define		BSEMAMUTEX		2							// Mutex semaphore

	volatile	proc_t		*oOwner;									// Ptr on the process owner of the semaphore
	volatile	list_t		oList;										// The semaphore list
				int32_t		oCounter;									// Counter for the semaphore
				#define		KMAXSEMCPT		INT32_MAX					//
				#define		KMINSEMCPT		INT32_MIN					//
};
#endif
