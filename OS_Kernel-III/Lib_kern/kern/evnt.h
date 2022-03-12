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

#ifndef	__EVNT__
#define	__EVNT__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	evnt_init(void);
extern	int32_t	evnt_createEvent(const char_t *identifier, volatile evnt_t **handle);
extern	int32_t	evnt_getBitEvent(volatile evnt_t *handle, uint32_t *bitEvent);
extern	int32_t	evnt_signalEvent(uint32_t bitEvent, uint32_t mode);
extern	int32_t	evnt_waitEvent(uint32_t *bitEvent, uint32_t timeout);
extern	int32_t	evnt_killEvent(volatile evnt_t *handle);
extern	int32_t	evnt_getEventById(const char_t *identifier, volatile evnt_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the event
// ----------------------

struct	evnt {
	const	char_t		*oIdentifier;									// Event identifier
			uint8_t		oState;											// State
			#define		BEVNTINSTALLED	0								// Event installed

			uint32_t	oBitEvent;										// The mask event
			#define		KNBEVENTS		32								// Number of events (always 32)
};

#define	KEVNTSWTC	0													// Event with the context switching
#define	KEVNTCONT	1													// Event without the context switching
#endif
