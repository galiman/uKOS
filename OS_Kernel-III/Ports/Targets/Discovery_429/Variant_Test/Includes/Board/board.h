/*
; board.
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
; Goal:		Board mapping.
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

#ifndef	__BOARD__
#define	__BOARD__

#include	<M25P16/M25P16.h>

// System identifiers
// ------------------

#define	KCPU			"ARM-Cortex m4 F"
#define	KCONTROLLER		"STM32F429"
#define	KBOARD			"Discovery 429 (Variant_Test)"
#define	KTARGET			"Discovery_429"

// Main system clocks
// ------------------

#define KCRYSTAL		8000000									// Quartz crystal resonator
#define KFREQUENCY_AHB	180000000								// AHB bus frequency of 180-MHz
#define KFREQUENCY_APB2	90000000								// APB2 periph bus frequency of 90-MHz
#define KFREQUENCY_APB1	45000000								// APB1 periph bus frequency of 45-MHz
#define KFREQUENCY_1	1000000									// Frequency of 1-MHz

// GPIO settings
// -------------

#define KNBLED			(2+8)									// Number of LEDs

// PORTs

#define	BSWITCH			5										// PORT A 5, Switch
#define	BNOSDCARD		5										// PORT A 5, SDCard inside the slot 

#define	BLED2			4										// PORT B 4, LED
#define	BLED1			7										// PORT B 7, LED

#define	BLED7			3										// PORT C 3, LED
#define	BLED5			8										// PORT C 8, LED
#define	BSELSDCARD		13										// PORT C 13, /CS of the SDCard

#define	BLED4			4										// PORT D 4, LED
#define	BLED3			5										// PORT D 5, LED
#define	BTEST_SCOPE		7										// PORT D 7, For logic analyzer tests

#define	BLED0			3										// PORT E 3, LED
#define	BSELEEPROM		4										// PORT E 4, /CS of the EEPROM

#define	BLED6			6										// PORT F 6, LED

#define	BSW1			2										// PORT G 2, SW1
#define	BSW0			3										// PORT G 3, SW0
#define	BLEDGREEN		13										// PORT G 13, Green LED
#define	BLEDRED			14										// PORT G 14, Red LED

// SPI mmx1
// --------

#define	KSZAPPL			0x00016000								// Max size of the application
#define	KSZEEPROMAPPL	0x00020000								// Size of the application segment (EEPROM)
#define	KSZUKOSOS		0x00000000								// Size to skip the necessary place for the uKOS OS
#define	KNBEEPROMAPPL	((KSZEEPROM-KSZUKOSOS)/KSZEEPROMAPPL)	// Max number of EEPROM applications (- ukos)
#define	KNBEEPROMSEC	(KSZEEPROMAPPL/KSZSECTOR)				// Number of sectors per appl
#endif
