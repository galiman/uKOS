/*
; types_unix.
; ===========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
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

#ifndef	__TYPES_UNIX__
#define	__TYPES_UNIX__

#include	<stdint.h>

// uKOS program returns / exits
//
// return (code)
//         code = EXIT_SUCCESS			--->		Success, kill the console process. Only the running processes 
//         code = EXIT_SUCCESS_OS		--->		Success; return to the OS (console) with the running processes
//         code = EXIT_FAILURE			--->		Failure; return to the OS (console) with an error message
//         code = EXIT_FAILURE_CRT0		--->		Failure; return to the OS (console). Reload the latest OS
//         code = EXIT_PANIC			--->		Failure; panic. System is stopped
//         code = default				--->		Failure; panic. System is stopped
//
// exit (code)
//         code = EXIT_SUCCESS			--->		Success, commit a suicide
//         code = EXIT_FAILURE			--->		Failure; commit a suicide
//         code = EXIT_PANIC			--->		Failure; panic. System is stopped
//         code = default				--->		Failure; panic. System is stopped

#ifndef	EXIT_SUCCESS
#define	EXIT_SUCCESS			0
#endif

#ifndef	EXIT_SUCCESS_OS
#define	EXIT_SUCCESS_OS			1
#endif

#ifndef	EXIT_FAILURE
#define	EXIT_FAILURE			(-1)
#endif

#ifndef	EXIT_FAILURE_CRT0
#define	EXIT_FAILURE_CRT0		(-2)
#endif

#ifndef	EXIT_PANIC
#define	EXIT_PANIC				(-3)
#endif

// uKOS misc

#ifndef	NULL
#define	NULL					((void *)0)
#endif

#ifndef	FALSE
#define	FALSE					0
#endif

#ifndef	TRUE
#define	TRUE					1
#endif

typedef unsigned char			bool_t;
typedef	char					char_t;
typedef float					float32_t;
typedef double					float64_t;
#endif
