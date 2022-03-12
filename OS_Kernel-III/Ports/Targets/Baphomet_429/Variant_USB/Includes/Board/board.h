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
#define	KBOARD			"Baphomet 429 (Variant_USB)"
#define	KTARGET			"Baphomet_429"

#define KCRYSTAL		16000000								// Quartz crystal resonator
#define KFREQUENCY_AHB	168000000								// AHB bus frequency of 168-MHz
#define KFREQUENCY_APB2	84000000								// APB2 periph bus frequency of 84-MHz
#define KFREQUENCY_APB1	42000000								// APB1 periph bus frequency of 42-MHz
#define KFREQUENCY_1	1000000									// Frequency of 1-MHz

// GPIO settings
// -------------

#define KNBLED			8										// Number of LEDs

// PORTs

#define	BLED0			4										// PORT A 4, LED
#define	BLED1			5										// PORT A 5, LED
#define	BOI0			7										// PORT A 7, OI0 extension bus
#define	BIO3			8										// PORT A 8, IO3 extension bus

#define	BSW1			0										// PORT B 0, SW1
#define	BOI1			1										// PORT B 1, OI1 extension bus
#define	BSW3			5										// PORT B 5, SW3
#define	BSELEEPROM		12										// PORT B 12, /CS of the EEPROM

#define	BIO4			1										// PORT C 1, IO4 extension bus
#define	BOI5			3										// PORT C 3, OI5 extension bus
#define	BIRQUSBR		4										// PORT C 4, Fifo RX empty
#define	BIRQUSBW		5										// PORT C 5, Fifo TX not available
#define	BSW0			13										// PORT C 13, SW0

#define	BIO2			6										// PORT D 6, IO2 extension bus
#define	BLED7			11										// PORT D 11, LED
#define	BLED5			12										// PORT D 12, LED
#define	BLED4			13										// PORT D 13, LED

#define	BSELSDCARD		4										// PORT E 4, /CS of the device

#define	BSELADT7320		6										// PORT F 6, /CS of the device
#define	BSW2			10										// PORT F 10, SW2

#define	BLED6			3										// PORT G 3, LED
#define	BTP3			7										// PORT G 7, test point 3
#define	BTP4			9										// PORT G 9, test point 3
#define	BOI6			11										// PORT G 11, OI6 extension bus
#define	BOI7			12										// PORT G 12, OI7 extension bus
#define	BNOSDCARD		14										// PORT G 14, SDCard inside the slot 

#define	BLED2			3										// PORT H 3, LED
#define	BLED3			13										// PORT H 13, LED

#define	BOI8			8										// PORT I 8, OI8 extension bus
#define	BOI9			9										// PORT I 9, OI9 extension bus

// SPI mmx1
// --------

#define	KSZAPPL			0x00040000								// Max size of the application
#define	KSZEEPROMAPPL	0x00040000								// Size of the application segment (EEPROM)
#define	KSZUKOSOS		0x00000000								// Size to skip the necessary place for the uKOS OS
#define	KNBEEPROMAPPL	((KSZEEPROM-KSZUKOSOS)/KSZEEPROMAPPL)	// Max number of EEPROM applications (- ukos)
#define	KNBEEPROMSEC	(KSZEEPROMAPPL/KSZSECTOR)				// Number of sectors per appl

// FTDI interface
// --------------

#define LOCFTDI			(volatile uint8_t *)(0x60000000)		// FTDI bus location
#define KLNFTDI			0x00000001								// FTDI bus length A0

// Bus interface
// -------------

#define LOCBUSINT		(volatile uint8_t *)(0x68000000)		// Extension bus location
#define KLNBUSINT		0x00000100								// Extension bus length A0..A7
#endif
