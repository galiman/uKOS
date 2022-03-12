/*
; nlib.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
; Version:	1.2
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		newLib interface for gcc C compiler (reentrant version).
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

#ifndef	__INT_NLIB__
#define	__INT_NLIB__

/*!
 * \addtogroup Lib_nlib
 */
/*@{*/

#include	<uKOS.h>

// newlib macros
// -------------

#define	NLIB_PROCESS_INIT	static	struct 	_reent	impure_data __attribute__ ((aligned (8))); \
							_impure_ptr = &impure_data; \
							_REENT_INIT_PTR(_impure_ptr) \
							kern_setLocalPtr((void *)_impure_ptr);

#define	NLIB_INIT			kern_setGlobalPtr((void *)&_impure_ptr);

/*@}*/

#endif
