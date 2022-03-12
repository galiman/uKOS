/*
; debg.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 161                  $:  Revision of last commit
; $Date:: 2017-06-15 15:01:25#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Kern - Debug list management.
;
;			This module implements the debug list primitives.
;
; 			Software debug system calls
; 			---------------------------
;
;			void	debg_init(void);
;			int32_t	debg_stopProcess(volatile proc_t *handle);
;			int32_t	debg_reactivateProcess(volatile proc_t *handle);
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

#ifndef	__DEBG__
#define	__DEBG__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	debg_init(void);
extern	int32_t	debg_stopProcess(volatile proc_t *handle);
extern	int32_t	debg_reactivateProcess(volatile proc_t *handle);

#ifdef	__cplusplus
}
#endif
#endif
