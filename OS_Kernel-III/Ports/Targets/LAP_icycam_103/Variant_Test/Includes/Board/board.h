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

// System identifiers
// ------------------

#define	KCPU			"ARM-Cortex m3"
#define	KCONTROLLER		"STM32F103"
#define	KBOARD			"LAP (epfl) icycam (Variant_Test)"
#define	KTARGET			"LAP_icycam_103"

// Main system clocks
// ------------------

#define KCRYSTAL		8000000									// Quartz crystal resonator
#define KFREQUENCY_AHB	72000000								// AHB bus frequency of 72-MHz
#define KFREQUENCY_APB2	84000000								// APB2 periph bus frequency of 84-MHz
#define KFREQUENCY_APB1	42000000								// APB1 periph bus frequency of 42-MHz
#define KFREQUENCY_1	1000000									// Frequency of 1-MHz

// CPLD mapping for the pseudo bus
// -------------------------------		

#define	LOCLEDS			0x00									// (RW) LED location
#define	LOCSWITCHES		0x01									// (R-) Switch location
#define	LOCFTDI			0x02									// (RW) FTDI location
#define	LOCCIE			0x03									// (-W) Interruption Enable location
#define	LOCPPI			0x04									// (RW) PPI location
#define	LOCHEXSWITCH	0x05									// (R-) Hexadecimal rotary switch location
#define	LOCGPIOS		0x06									// (RW) 4 GPIOs
#define	LOCICYINT		0x07									// (W) 	Request an interrupt to the icycam Soc

#define KNBLED			8										// Number of LEDs

// RTC (init @ 00h 00m 00s)
// ------------------------

#define	KTIME			((0*24*3600) + (0*3600) + (0*60) + 0)	// Initial time in seconds
#define	KRTCCLK			(32768-1)								// Crystal clock (corrected)
#define	KRTC_PERIOD		1000									// 1000-ms
#endif
