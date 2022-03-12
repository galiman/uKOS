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

#define	KCPU			"icyflex-1"
#define	KCONTROLLER		"icycam"
#define	KBOARD			"icyboard (Variant_Test)"
#define	KTARGET			"CSEM_icycam"

// Main system clocks
// ------------------

#define KFREQUENCY_33	33000000								// System bus frequency of 33-MHz

// CPLD mapping for the pseudo bus
// -------------------------------

#define	KNBLED			8										// Number of LEDs

#define	KPULLUPIN		0xFFE0									// Pull-ups on the inputs

#define	KDIRIN			0x001F									// IN  - Data[15..8], IN - BRTS X IRQ, OUT - CS R/W A2 A1 A0
#define	KDIROUT			0xFF1F									// OUT - Data[15..8], IN - BRTS X IRQ, OUT - CS R/W A2 A1 A0

#define	LOCLEDS			0x00									// (RW) LED location
#define	LOCSWITCHES		0x01									// (R-) Switch location
#define	LOCFTDI			0x02									// (RW) FTDI location
#define	LOCTESTS		0x03									// (RW) 8 pins for tests
#define	LOCPPI			0x04									// (RW) PPI location
#define	LOCHEXSWITCH	0x05									// (R-) Hexadecimal rotary switch location

#define	BADD0			0										// Pseudo address 0
#define	BADD1			1										// Pseudo address 1
#define	BADD2			2										// Pseudo address 2
#define	BRW				3										// Pseudo address R/W
#define	BCS				4										// Pseudo address CS
#define	BIRQUSBR		5										// Fifo RX empty
#define	BIRQUSBW		6										// Fifo TX not available

#define	BRTS			7										// For RTS bluetooth

// SPI mmx1
// --------

#define	KSZAPPL			0x00008000								// Max size of the application
#define	KSZEEPROMAPPL	0x00010000								// Size of the application segment (EEPROM)
#define	KSZUKOSOS		0x00020000								// Size to skip the necessary place for the uKOS OS (+ 1 app for MINIPACK)
#define	KNBEEPROMAPPL	((KSZEEPROM-KSZUKOSOS)/KSZEEPROMAPPL)	// Max number of EEPROM applications (- ukos)
#define	KNBEEPROMSEC	(KSZEEPROMAPPL/KSZSECTOR)				// Number of sectors per appl

// Location of the bitmaps used by the VS
// --------------------------------------

#define	LOCIMG0			(volatile uint8_t *)(SDRAM_BASE_ADDRESS)				// Image 0 (320 x 240 x 2)
#define	LOCIMG1			(volatile uint8_t *)(SDRAM_BASE_ADDRESS+(320*240*2))	// Image 1 (320 x 240 x 2)
#endif
