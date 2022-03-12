/*
; lib_comm.
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
; Goal:		lib_comm system call interface module.
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

#ifndef	__LIBCOMM__
#define	__LIBCOMM__

/*!
 * \defgroup Lib_comm
 *
 * \brief Communication manager system calls
 *
 * This library controls all the communication I/O of the system.
 * The user can directly use a specific manager like “sci0”, “urt0”, “usb0”, etc..
 * The generic “comm” manager allows to redirect the calls to the appropriate communication
 * manager according to the value of the commManager parameter.
 *
 * @{
 */

#include	<uKOS.h>

#if (defined(__MAN_COMM__))
#include 	<comm/comm.h>
#endif
#if (defined(__MAN_URT0__))
#include 	<urt0/urt0.h>
#endif
#if (defined(__MAN_URT1__))
#include 	<urt1/urt1.h>
#endif
#if (defined(__MAN_URT2__))
#include 	<urt2/urt2.h>
#endif
#if (defined(__MAN_URT3__))
#include 	<urt3/urt3.h>
#endif
#if (defined(__MAN_URT4__))
#include 	<urt4/urt4.h>
#endif
#if (defined(__MAN_BLT0__))
#include 	<blt0/blt0.h>
#endif
#if (defined(__MAN_USB0__))
#include 	<usb0/usb0.h>
#endif
#if (defined(__MAN_USB1__))
#include 	<usb1/usb1.h>
#endif

/*!@}*/

#endif
