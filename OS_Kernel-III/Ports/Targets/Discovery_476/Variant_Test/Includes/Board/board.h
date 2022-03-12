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

#define	KCPU			"ARM-Cortex m4 L"
#define	KCONTROLLER		"STM32L476"
#define	KBOARD			"Discovery 476 (Variant_Test)"
#define	KTARGET			"Discovery_476"

// Main system clocks
// ------------------

#define KCRYSTAL		16000000								// Quartz crystal resonator
#define KFREQUENCY_AHB	80000000								// AHB bus frequency of 80-MHz
#define KFREQUENCY_APB2	80000000								// APB2 periph bus frequency of 80-MHz
#define KFREQUENCY_APB1	80000000								// APB1 periph bus frequency of 80-MHz
#define KFREQUENCY_1	1000000									// Frequency of 1-MHz

// GPIO settings
// -------------

#define KNBLED			2										// Number of LEDs

// PORTs

#define	BTEST			11										// PORT E 11, test analyser

#define	BLED0			2										// PORT B 2, LED red
#define	BLEDRED			BLED0									// PORT B 2, LED red

#define	BLED1			8										// PORT E 8, LED green
#define	BLEDGREEN		BLED1									// PORT E 8, LED green
#endif
