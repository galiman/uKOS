/*
; stub_i2c1_si2c1.
; ================

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
; Goal:		stub for the connection of the "i2c1" manager to the si2c1 device.
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

#include 	<uKOS.h>

// Connect the physical device to the logical manager
// --------------------------------------------------

#define	__SI2C1_SEMA_TX__
#define	__SI2C1_SEMA_RX__

#define	cnfSi2c1_t				cnfI2cx_t

#define	si2c1_init				stub_i2c1_init
#define	si2c1_configure			stub_i2c1_configure
#define	si2c1_write				stub_i2c1_write
#define	si2c1_read				stub_i2c1_read
#define	si2c1_flush				stub_i2c1_flush

#define	KSI2C1NOERR				KI2C1NOERR
#define	KSI2C1CHBSY				KI2C1CHBSY
#define	KSI2C1TIMEO				KI2C1TIMEO
#define	KSI2C1SENOE				KI2C1SENOE

#define	KSEMAPHORE_SI2C1RX		KSEMAPHORE_I2C1RX
#define	KSEMAPHORE_SI2C1TX		KSEMAPHORE_I2C1TX

#define	KFREQUENCY_INTERFACE	KFREQUENCY_APB1
#define	KFREQUENCY_1000000		KFREQUENCY_1

#include 	<model_si2c1.cm>

