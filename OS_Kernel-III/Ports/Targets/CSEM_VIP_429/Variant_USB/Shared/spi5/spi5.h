/*
; spi5.
; =====

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
; Goal:		For the spi5 "shared" manager module.
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

#ifndef	__SPI5__
#define	__SPI5__

// Served managers
// ---------------

#define	KNOMANAGER		0			// No manager
#define	KIMU0			1			// imu0 manager
#define	KIMUA			2			// Accelerometer imu0 manager
#define	KIMUM			3			// Magnetometer imu0 manager
#define	KTMP0			4			// tmp0 manager

// Peripheral macro
// ----------------

#define	RESERVE_SHARED_SPI5(manager)	while (shared_spi5_reserve(manager) == KSPI5CHBSY) kern_switchFast();
#define	RELEASE_SHARED_SPI5				shared_spi5_release();

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	int32_t	shared_spi5_reserve(uint32_t manager);
extern	int32_t	shared_spi5_release(void);
extern	int32_t	shared_spi5_select(uint32_t manager);
extern	int32_t	shared_spi5_deselect(uint32_t manager);
extern	int32_t	shared_spi5_writeRead(uint8_t *data);

#ifdef	__cplusplus
}
#endif

// Shared spi5 device errors & status
// ----------------------------------

#define	KSPI5NOERR	0				// No error
#define	KSPI5CHBSY	-1				// The spi5 device is busy
#endif
