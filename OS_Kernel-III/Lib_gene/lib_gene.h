/*
; lib_gene.
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
; Goal:		lib_gene system call interface module.
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

#ifndef	__LIBGENE__
#define	__LIBGENE__

/*!
 * \defgroup Lib_gene
 *
 * \brief Generic manager system calls
 *
 * This library contains four managers; the text, the iotx, the syos, the usdc and the xfer one.
 * The text is responsible for some text buffer manipulations such as buffer copy, or buffer compare.
 * The iotx is responsible for handling some high level I/O functionalities such as the buffer printing
 * on the standard output communication manager or waiting for an order coming from the standard
 * input communication manager.
 * The syos provides some high level functionalities of the system such as the module finding inside
 * the concatenated list of modules of the μKOS system available inside the memory. The usdc is responsible
 * to manage the Micro SDCard interface (V2).
 *
 * @{
 */

#include	<uKOS.h>

#if (defined(__MAN_IOTX__))
#include 	<iotx/iotx.h>
#endif
#if (defined(__MAN_SYOS__))
#include 	<syos/syos.h>
#endif
#if (defined(__MAN_TEXT__))
#include 	<text/text.h>
#endif
#if (defined(__MAN_USDC__))
#include 	<usdc/usdc.h>
#endif
#if (defined(__MAN_XFER__))
#include 	<xfer/xfer.h>
#endif
#if (defined(__MAN_JPEG__))
#include 	<jpeg/jpeg.h>
#endif

/*!@}*/

#endif
