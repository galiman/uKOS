/*
; types.
; ======

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
; Goal:		Basic predefined types (normally machine independent).
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

#ifndef	__TYPES__
#define	__TYPES__

#ifndef	EXIT_FAILURE
#define	EXIT_FAILURE	(-1)
#endif

#ifndef	EXIT_SUCCESS
#define	EXIT_SUCCESS	0
#endif

#ifndef	NULL
#define	NULL			0
#endif

#ifndef	FALSE
#define	FALSE			0
#endif

#ifndef	TRUE
#define	TRUE			1
#endif

typedef unsigned char		bool_t;		// Boolean type
typedef	char				char_t;		// 8-bit char type
typedef signed char			int8_t;		// 8-bit wide signed type
typedef unsigned char		uint8_t;	// 8-bit wide unsigned type
typedef signed short		int16_t;	// 16-bit wide signed type
typedef unsigned short		uint16_t;	// 16-bit wide unsigned type
typedef signed long			int32_t;	// 32-bit wide signed type
typedef unsigned long		uint32_t;	// 32-bit wide unsigned type
typedef float				float32_t;	// 32-bit wide float
#endif
