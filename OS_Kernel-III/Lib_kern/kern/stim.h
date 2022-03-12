/*
; stim.
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
; Goal:		Kern - Software timers.
;
;			This module implements the software primitives.
;
; 			Software timer system calls
; 			---------------------------
;
;			void	stim_init(void);
;			int32_t	stim_createSoftwareTimer(const char_t *identifier, volatile stim_t **handle);
;			int32_t	stim_setSoftwareTimer(volatile stim_t *handle, tspc_t *configure);
;			int32_t	stim_killSoftwareTimer(volatile stim_t *handle);
;			int32_t	stim_getSoftwareTimerById(const char_t *identifier, volatile stim_t **handle);
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

#ifndef	__STIM__
#define	__STIM__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	stim_init(void);
extern	int32_t	stim_createTimer(const char_t *identifier, volatile stim_t **handle);
extern	int32_t	stim_setSoftwareTimer(volatile stim_t *handle, tspc_t *configure);
extern	int32_t	stim_killTimer(volatile stim_t *handle);
extern	int32_t	stim_getTimerById(const char_t *identifier, volatile stim_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the software timer & the specification
// ---------------------------------------------------

struct	tspc {
			uint8_t		oMode;											// Mode
			#define		KSTOP			0								// Stop the execution of the timer
			#define		KSINGLESHOT		1								// Single shot software timer (with start)
			#define		KCONTINUE		2								// Continue software timer (with start)

			uint32_t	oIniTime;										// Initial time
			uint32_t	oTime;											// Continuous time
	const	void		*oArgument;										// Ptr on the software timer argument
			void		(*oCode)(const void *argument);					// Ptr on the code
};

struct	stim {
	const	char_t		*oIdentifier;									// Software timer identifier
			uint8_t		oState;											// Status
			#define		BSTIMINSTALLED	0								// Software timer installed
			#define		BSTIMCONFIGURED	1								// Software timer configured
			#define		BSTIMRUNNING	2								// Software timer is running
			#define		BSTIMEXECUTED	3								// Software timer executed (once)

			uint32_t	oInitCounter;									// Initial time (decremented)
			uint32_t	oCounter;										// Continuous time (decremented)
			tspc_t		oTimerSpec;										// Software timer specifications
};
#endif
