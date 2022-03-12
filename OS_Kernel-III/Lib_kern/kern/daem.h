/*
; daem.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 178                  $:  Revision of last commit
; $Date:: 2017-07-15 21:15:17#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Kern - Daemon management.
;
;			This module launches all the daemons responsible for managing
;			some uKernel stuff (i.e. idle state, timeout management, etc.).
;
; 			Daemon system calls
; 			-------------------
;
;			void	daem_init(void);
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

#ifndef	__DAEM__
#define	__DAEM__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	daem_init(void);

#ifdef	__cplusplus
}
#endif

// Daemons
// -------

#define	KDAEMON_IDLE			"Daemon_idle"
#define	KDAEMON_TIMEOUT			"Daemon_timeout"
#define	KDAEMON_STACK			"Daemon_stack_integrity"
#define	KDAEMON_SOFTWARE_TIMER	"Daemon_software_timer"
#endif
