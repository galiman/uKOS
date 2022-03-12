/*
; tmp100.
; =======

;------------------------------------------------------------------------
; Author:	Lorenzin Florence.	The 2017-04-13
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		TI tmp100 equates.
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

#ifndef	__TMP100__
#define	__TMP100__

// I2c interface

#define	KADDTMP100				0x4E									// I2C tmp100 address 

// Registers

#define	REGPTR					0x00									// Register pointer
#define	REGTMP					0x00									// Register temperature
#define	REGCNF					0x01									// Register configuration
#define	REGLOW					0x02									// Register low temperature
#define	REGHI					0x03									// Register hi temperature

// Configuration register

#define	BSD						0										// Shutdown Mode SD
#define	BTM						1										// Thermostat Mode TM
#define	BPOL					2										// Polarity POL
#define	BF0						3										// Fault Queue F0
#define	BF1						4										// Fault Queue F1
#define	BR0						5										// Resolution R0
#define	BR1						6										// Resolution R1
#define	BALERT					7										// OS/ALERT	alert
#endif
