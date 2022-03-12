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
#define	KCONTROLLER		"STM32F407"
#define	KBOARD			"Discovery 407 (Variant_Test)"
#define	KTARGET			"Discovery_407"

// Main system clocks
// ------------------

#define KCRYSTAL		8000000									// Quartz crystal resonator
#define KFREQUENCY_AHB	168000000								// AHB bus frequency of 168-MHz
#define KFREQUENCY_APB2	84000000								// APB2 periph bus frequency of 84-MHz
#define KFREQUENCY_APB1	42000000								// APB1 periph bus frequency of 42-MHz
#define KFREQUENCY_1	1000000									// Frequency of 1-MHz

// GPIO settings
// -------------

#define KNBLED			(4+8)									// Number of LEDs

// PORTs

#define	BSWITCH			0										// PORT A 0, Switch
#define	BTEST_SCOPE		1										// PORT A 1, For logic analyzer tests

#define	BSELEEPROM		12										// PORT B 12, /CS of the EEPROM

#define	BLED0			1										// PORT C 1, LED
#define	BLED1			2										// PORT C 2, LED
#define	BSELSDCARD		9										// PORT C 9, /CS of the device
#define	BLED3			13										// PORT C 13, LED

#define	BSW0			0										// PORT D 0, SW0
#define	BSW1			1										// PORT D 1, SW1
#define	BSW2			3										// PORT D 3, SW2
#define	BSW3			4										// PORT D 4, SW3
#define	BLED5			6										// PORT D 6, LED
#define	BLED4			7										// PORT D 7, LED
#define	BLED6			10										// PORT D 10, LED
#define	BLED7			11										// PORT D 11, LED
#define	BLEDGREEN		12										// PORT D 12, Green LED
#define	BLEDORANGE		13										// PORT D 13, Orange LED
#define	BLEDRED			14										// PORT D 14, Red LED
#define	BLEDBLUE		15										// PORT D 15, Blue LED

#define	BNOSDCARD		2										// PORT E 2, SDCard inside the slot 
#define	BLED2			6										// PORT E 6, LED

// SPI mmx1
// --------

#define	KSZAPPL			0x00016000								// Max size of the application
#define	KSZEEPROMAPPL	0x00020000								// Size of the application segment (EEPROM)
#define	KSZUKOSOS		0x00000000								// Size to skip the necessary place for the uKOS OS
#define	KNBEEPROMAPPL	((KSZEEPROM-KSZUKOSOS)/KSZEEPROMAPPL)	// Max number of EEPROM applications (- ukos)
#define	KNBEEPROMSEC	(KSZEEPROMAPPL/KSZSECTOR)				// Number of sectors per appl
#endif
