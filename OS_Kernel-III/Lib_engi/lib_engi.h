/*
; lib_engi.
; =========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		lib_engi system call interface module.
;			uKOS interface for Sysquake Embedded (Calerga Sarl, www.calerga.com).
;
;   (c) 1992-2017, Franzi Edo.
;   --------------------------
;
;   Developed for use with Sysquake Embedded.
;   Sysquake is a trademark of Calerga Sarl, used with permission. 
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

#ifndef	__LIBENGI__
#define	__LIBENGI__

/*!
 * \defgroup Lib_engi
 *
 * \brief Engine manager system calls
 *
 * This library is intended to control third party complex engines to extend the μKOS capability.
 * The idea is to give the possibility to an μKOS process to exchange and to compute complex data
 * via the capabilities of the supported engine.
 *
 * @{
 */

#include	<uKOS.h>

#if (defined(__MAN_SQEE__))
#include 	<sqee/sqee.h>
#endif
#if (defined(__MAN_MPYT__))
#include 	<mpyt/mpyt.h>
#endif

/*!@}*/

#endif
