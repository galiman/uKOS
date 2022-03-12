/*
; conf_system.
; ============

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
; Goal:		System configuration.
;
;			The system configuration recognizes these flags (#define only the active ones)
;
;			__CHECK_OS_VERSION__	With the check of the OS version (for the downloaded applications)
;
;			__MEM_MMXx__			With the mass memory  x [1..n]; 1 =spi mmx1, 2 = SDCard
;
;			__RECTRACE__			With the record of the last 3 entries in the uKernel routines
;
;			__MAN_COMM__			With the COMM communication manager
;			__MAN_URTx__			With the URTx communication manager x [0..n]
;			__MAN_BLTx__			With the BLTx communication manager x [0..n]
;			__MAN_USBx__			With the USBx communication manager x [0..n]
;
;			__MAN_IOTX__			With the IOTX manager
;			__MAN_SYOS__			With the SYOS manager
;			__MAN_TEXT__			With the TEXT manager
;			__MAN_XFER__			With the XFER manager
;			__MAN_USDC__			With the USDC manager
;			__MAN_JPEG__			With the JPEG manager
;
;			__FIFO_TRACE__			With the fifo trace capability (for debugging with the syos_xxx)
;
;			__MAN_KERN__			With the KERN manager
;
;			__MAN_I2Cx__			With the I2Cx manager x [0..n]
;			__MAN_IMGx__			With the IMGx manager x [0..n]
;			__MAN_TMPx__			With the TMPx manager x [0..n]
;			__MAN_VISx__			With the VISx manager x [0..n]
;			__MAN_IMUx__			With the IMUx manager x [0..n]
;			__MAN_ADCx__			With the ADCx manager x [0..n]
;			__MAN_IMSx__			With the IMSx manager x [0..n]
;			__MAN_OLED__			With the OLED manager x [0..n]
;
;			__MAN_GLOB__			With the GLOB manager
;			__MAN_MISC__			With the MISC manager
;			__MAN_STRG__			With the STRG manager
;
;			__STRG_FULL__			With the full system calls
;
;			__MAN_SQEE__			With the SQEE manager (Synsquake Embedded Engine)
;			__MAN_MPYT__			With the MPYT manager (Micro-Python Engine)
;
;			__MAN_NLIB__			With the NLIB manager (newlib support)
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

#ifndef	__CONF_SYSTEM__
#define	__CONF_SYSTEM__

//  Default baudrate
// -----------------

#define	KDEFAULT_BAUDRATE	460800

// System version check
// --------------------

#define	__CHECK_OS_VERSION__	// With the check of the OS version

// Memory
// ------

// Trace for debugging the uKernel
// -------------------------------

// COMM Manager configuration
// --------------------------

#define	__MAN_COMM__			// With the COMM communication manager
#define	__MAN_URT0__			// With the URT0 communication manager

// GENE Manager configuration
// --------------------------

#define	__MAN_IOTX__			// With the IOTX manager
#define	__MAN_SYOS__			// With the SYOS manager
#define	__MAN_TEXT__			// With the TEXT manager

// syos

#define	__FIFO_TRACE__			// With the fifo trace capability

// KERN Manager configuration
// --------------------------

#define	__MAN_KERN__			// With the KERN manager

// PERI Manager configuration
// --------------------------

// TBOX Manager configuration
// --------------------------

#define	__MAN_GLOB__			// With the GLOB manager
#define	__MAN_MISC__			// With the MISC manager
#define	__MAN_STRG__			// With the STRG manager

// strg

#define	__STRG_FULL__			// With the full system calls

// ENGI Manager configuration
// --------------------------

// NLIB Manager configuration
// --------------------------

#define	__MAN_NLIB__			// With the NLIB manager
#endif
