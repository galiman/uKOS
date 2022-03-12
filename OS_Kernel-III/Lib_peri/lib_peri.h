/*
; lib_peri.
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
; Goal:		lib_peri system call interface module.
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

#ifndef	__LIBPERI__
#define	__LIBPERI__

/*!
 * \defgroup Lib_peri
 *
 * \brief Peripherals system calls
 *
 * This library contains the following managers, imgx, ims2, visx, tmpx, imu0, i2cx, and the adcx one.
 * Accordingly to the hardware that μKOS has to support, additional managers can be implemented.
 * All the manages that control similar hardware should have have the same set system calls and the same I/O parameters (i.e. vis0, vis1, img0, img1, etc.).
 * The configuration is done via a the manager specific structures.
 *
 * @{
 */

#include	<uKOS.h>

#if (defined(__MAN_ADC0__))
#include 	<adc0/adc0.h>
#endif
#if (defined(__MAN_I2C0__))
#include 	<i2c0/i2c0.h>
#endif
#if (defined(__MAN_I2C1__))
#include 	<i2c1/i2c1.h>
#endif
#if (defined(__MAN_IMG0__))
#include 	<img0/img0.h>
#endif
#if (defined(__MAN_IMS2__))
#include 	<ims2/ims2.h>
#endif
#if (defined(__MAN_IMU0__))
#include 	<imu0/imu0.h>
#endif
#if (defined(__MAN_OLE0__))
#include 	<ole0/ole0.h>
#endif
#if (defined(__MAN_PRT0__))
#include 	<prt0/prt0.h>
#endif
#if (defined(__MAN_TMP0__))
#include 	<tmp0/tmp0.h>
#endif
#if (defined(__MAN_VIS0__))
#include 	<vis0/vis0.h>
#endif

/*!@}*/

#endif
