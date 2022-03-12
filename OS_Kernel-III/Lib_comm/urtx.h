/*
; urtx.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		urtx manager.
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

#ifndef	__URTX__
#define	__URTX__

// Configuration structure
// -----------------------

typedef	struct	cnfUrtx	cnfUrtx_t;

struct cnfUrtx {
			uint8_t		oNBits;											// Number of bits
			#define		K7BIT		0									// K7BIT = word of 7-bits
			#define		K8BIT		1									// K8BIT = word of 8-bits

			uint8_t		oStopBits;										// Number of stop bits
			#define		K1STBIT		0									// K1STBIT = 1 stop bits
			#define		K2STBIT		1									// K2STBIT = 2 stop bits

			uint8_t		oParity;										// Parity
			#define		KNONE		0									// KEVEN = Parity even
			#define		KEVEN		1									// KODD = Parity odd
			#define		KODD		2									// KNONE = No Parity

			uint8_t		oBaudRate;										// Baudrate

// Mandatory baudrates: supported by all the targets
									
			#define		KBDDEFAULT	0									// KBDDEFAULT = default baudrate
			#define		KBD2400		1									// KBD2400 = 2400-bits/s
			#define		KBD4800		2									// KBD4800 = 4800-bits/s
			#define		KBD9600		3									// KBD9600 = 9600-bits/s
			#define		KBD19200	4									// KBD19200 = 19200-bits/s
			#define		KBD38400	5									// KBD38400 = 38400-bits/s
			#define		KBD57600	6									// KBD57600 = 57600-bits/s
			#define		KBD115200	7									// KBD115200 = 115200-bits/s
			#define		KBD230400	8									// KBD230400 = 230400-bits/s

// Optional baudrates: supported only by some specific hardware
// If not supported, the baudrate is not changed (previous configuration)

			#define		KBD460800	9									// KBD460800 = 460800-bits/s
			#define		KBD500000	10									// KBD500000 = 500000-bits/s
			#define		KBD921600	11									// KBD921600 = 921600-bits/s
			#define		KBD1000000	12									// KBD1000000 = 1000000-bits/s
			#define		KBD1500000	13									// KBD1500000 = 1500000-bits/s
			#define		KBD1843200	14									// KBD1843200 = 1843200-bits/s
			#define		KBD2000000	15									// KBD2000000 = 2000000-bits/s
			#define		KBD2500000	16									// KBD2500000 = 2500000-bits/s
			#define		KBD3000000	17									// KBD3000000 = 3000000-bits/s

			uint8_t		oKernSync;										// uKernel synchro
			#define		BRXSEMA		0									// RX semaphore
			#define		BTXSEMA		1									// TX semaphore
};

// Semaphores
// ----------

#define	KSEMARX			0												// Semaphore RX
#define	KSEMATX			1												// Semaphore TX
#endif
