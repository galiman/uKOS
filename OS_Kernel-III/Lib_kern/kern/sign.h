/*
; sign.
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
; Goal:		Kern - Signal management.
;
;			This module implements the signals primitives.
;
; 			Software signal system calls
; 			----------------------------
;
;			void	sign_init(void);
;			int32_t	sign_createSignal(const char_t *identifier, volatile sign_t **handle);
;			int32_t	sign_getBitSignal(volatile sign_t *handle, uint32_t *bitSignal);
;			int32_t	sign_signalSignal(volatile sign_t *handle, uint32_t bitSignal, volatile proc_t *process, uint32_t mode);
;			int32_t	sign_waitSignal(uint32_t *bitSignal, uint32_t timeout);
;			int32_t	sign_killSignal(volatile sign_t *handle);
;			int32_t	sign_getSignalById(const char_t *identifier, volatile sign_t **handle);
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

#ifndef	__SIGN__
#define	__SIGN__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	sign_init(void);
extern	int32_t	sign_createSignal(const char_t *identifier, volatile sign_t **handle);
extern	int32_t	sign_getBitSignal(volatile sign_t *handle, uint32_t *bitSignal);
extern	int32_t	sign_signalSignal(volatile sign_t *handle, uint32_t bitSignal, volatile proc_t *process, uint32_t mode);
extern	int32_t	sign_waitSignal(uint32_t *bitSignal, uint32_t timeout);
extern	int32_t	sign_killSignal(volatile sign_t *handle);
extern	int32_t	sign_getSignalById(const char_t *identifier, volatile sign_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the signal
// -----------------------

struct	sign {
	const	char_t		*oIdentifier;									// Signal identifier
			uint8_t		oState;											// State
			#define		BSIGNINSTALLED	0								// Signal installed

			uint32_t	oBitSignal;										// The mask signal
};

#define	KSIGNSWTC	0													// Signal with the context switching
#define	KSIGNCONT	1													// Signal without the context switching
#endif
