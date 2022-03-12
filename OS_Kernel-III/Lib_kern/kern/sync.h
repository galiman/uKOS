/*
; sync.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 168                  $:  Revision of last commit
; $Date:: 2017-07-03 17:28:19#$:  Date of last commit
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

#ifndef	__SYNC__
#define	__SYNC__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	sync_init(void);
extern	int32_t	sync_createEvent(const char_t *identifier, volatile evnt_t **handle);
extern	int32_t	sync_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent);
extern	int32_t	sync_signalEvent(uint32_t bitEvent, uint32_t mode);
extern	int32_t	sync_waitEvent(uint32_t *bitEvent, uint32_t timeout);
extern	int32_t	sync_killEvent(volatile evnt_t *handle);
extern	int32_t	sync_getEventById(const char_t *identifier, volatile evnt_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the event
// ----------------------

struct	evnt {
	const	char_t		*oIdentifier;									// Event identifier
			uint8_t		oState;											// Status
			#define		BEVNTINSTALLED	0								// Event installed

			uint32_t	oBitEvent;										// The mask event
			#define		KNBEVENTS		32								// Number of events (has to correspond to the oEvent type (uintxx_t))
};

#define	KSYNCSWTC	0													// Synchro with the context switching
#define	KSYNCCONT	1													// Synchro without the context switching
#endif
