/*
; ADT7320.
; ========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2013-03-08
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Analog Devices ADT7320 equates.
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

#ifndef	__ADT7320__
#define	__ADT7320__

#define	KNBREGIST				0x07									// Nb. of registers
#define	KWRITE					0x00									// Write bit
#define	KREAD					0x40									// Read bit

#define	REGSTATUS				(0x0<<3)								// Register status (R) 8
#define	REGCONFIG				(0x1<<3)								// Register configuration (W) 8
#define	REGTEMPER				(0x2<<3)								// Register temperature (R) 16
#define	REGID					(0x3<<3)								// Register ID (R) 8
#define	REGTCRSET				(0x4<<3)								// Register T Crit set point (W) 16
#define	REGTHYSET				(0x5<<3)								// Register T Hysteresis set point (W) 8
#define	REGTHISET				(0x6<<3)								// Register T High set point (W) 16
#define	REGTLOSET				(0x7<<3)								// Register T Low set point (W) 16
#endif
