/*
; spi4.
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
; Goal:		For the spi4 "shared" manager module.
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

#ifndef	__SPI4__
#define	__SPI4__

// Served managers
// ---------------

#define	KNOMANAGER		0			// No manager
#define	KGLOB			1			// glob manager
#define	KUSDC			2			// usdc manager
#define	KUSDCS			3			// Slow usdc manager
#define	KUSDCF			4			// Fast usdc manager

// Peripheral macro
// ----------------

#define	RESERVE_SHARED_SPI4(manager)	while (shared_spi4_reserve(manager) == KSPI4CHBSY) kern_switchFast();
#define	RELEASE_SHARED_SPI4				shared_spi4_release();

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	int32_t	shared_spi4_reserve(uint32_t manager);
extern	int32_t	shared_spi4_release(void);
extern	int32_t	shared_spi4_select(uint32_t manager);
extern	int32_t	shared_spi4_deselect(uint32_t manager);
extern	int32_t	shared_spi4_writeRead(uint8_t *data);

#ifdef	__cplusplus
}
#endif

// Shared spi4 device errors & status
// ----------------------------------

#define	KSPI4NOERR	0				// No error
#define	KSPI4CHBSY	-1				// The spi4 device is busy
#endif
