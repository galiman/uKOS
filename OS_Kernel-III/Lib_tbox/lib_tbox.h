/*
; lib_tbox.
; =========

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
; Goal:		lib_tbox system call interface module.
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

#ifndef	__LIBTBOX__
#define	__LIBTBOX__

/*!
 * \defgroup Lib_tbox
 *
 * \brief Tool box system calls
 *
 * This library contains the following managers, the glob, the misc, and the strg.
 * Accordingly to the hardware that μKOS has to support, additional managers can be added.
 * The glob is responsible for handling some generic functionalities.
 * The misc is responsible for handling the extension bus as well as the exception management.
 * The strg is a collection of functions for manipulating formatted strings and chars.
 *
 * @{
 */

#include	<uKOS.h>

#if (defined(__MAN_GLOB__))
#include 	<glob/glob.h>
#endif
#if (defined(__MAN_MISC__))
#include 	<misc/misc.h>
#endif
#if (defined(__MAN_STRG__))
#include 	<strg/strg.h>
#endif

/*!@}*/

#endif
