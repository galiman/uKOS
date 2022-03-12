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

#include	<M25P10/M25P10.h>

// System identifiers
// ------------------

#define	KCPU			"ARM-Cortex m4 F"
#define	KCONTROLLER		"STM32F429"
#define	KBOARD			"VIP 429 (Variant_USB)"
#define	KTARGET			"CSEM_VIP_429"

// Main system clocks
// ------------------

#define KCRYSTAL		16000000								// Quartz crystal resonator
#define KFREQUENCY_AHB	168000000								// AHB bus frequency of 168-MHz
#define KFREQUENCY_APB2	84000000								// APB2 periph bus frequency of 84-MHz
#define KFREQUENCY_APB1	42000000								// APB1 periph bus frequency of 42-MHz
#define KFREQUENCY_1	1000000									// Frequency of 1-MHz

// GPIO settings
// -------------

#define KNBLED			2										// Number of LEDs

// PORTs

#define	BIO0			8										// PORT A 8, IO0 extension bus

#define	BOI2			0										// PORT B 0, OI2 extension bus
#define	BOI3			1										// PORT B 1, OI3 extension bus
#define	BSELEEPROM		12										// PORT B 12, /CS of the EEPROM

#define	BOI4			5										// PORT C 5, OI4 extension bus

#define	BIO4			11										// PORT D 11, IO4 extension bus
#define	BLED1			12										// PORT D 12, BLED1 extension bus
#define	BLED0			13										// PORT D 13, BLED0 extension bus

#define	BSELSPI4		4										// PORT E 4, /CS of the device

#define	BSELMAGNETO		6										// PORT F 6, /CS magnetometer

#define	BIO3			3										// PORT G 3, IO3 extension bus
#define	BIO1			6										// PORT G 6, IO1 extension bus
#define	BIO2			7										// PORT G 7, IO2 extension bus
#define	BINTIMU			9										// PORT G 9, /int IMU
#define	BSELACCELERO	14										// PORT G 14, /CS accelerometer/gyroscope

#define	BSTANDBY		2										// PORT H 2, Standby imager
#define	BEXPOSUR		3										// PORT H 3, Exposure imager

#define	BCLK_EN			10										// PORT I 10, Enable the Aptina clock
#define	BDEN_AG			11										// PORT I 11, BDEN_AG IMU

// SPI mmx1
// --------

#define	KSZAPPL			0x00020000								// Max size of the application
#define	KSZEEPROMAPPL	0x00020000								// Size of the application segment (EEPROM)
#define	KSZUKOSOS		0x00000000								// Size to skip the necessary place for the uKOS OS
#define	KNBEEPROMAPPL	((KSZEEPROM-KSZUKOSOS)/KSZEEPROMAPPL)	// Max number of EEPROM applications (- ukos)
#define	KNBEEPROMSEC	(KSZEEPROMAPPL/KSZSECTOR)				// Number of sectors per appl
#endif
