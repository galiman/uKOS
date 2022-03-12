/*
; stub_usdc_sdcard.
; =================

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the connection of the "usdc" manager to the SCDard, spi4 device.
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

#include 	<uKOS.h>
#include 	<spi4/spi4.h>

// Connect the physical device to the logical manager
// --------------------------------------------------

#define	prmSdcard_t				prmUsdc_t

#define	sdcard_init				stub_usdc_init			
#define	sdcard_initialize		stub_usdc_initialize		
#define	sdcard_read				stub_usdc_read			
#define	sdcard_write			stub_usdc_write			

#define	KSDCARDNOERR			KUSDCNOERR
#define	KSDCARDSYCNA			KUSDCSYCNA
#define	KSDCARDGEERR			KUSDCGEERR
#define	KSDCARDCHBSY			KUSDCCHBSY
#define	KSDCARDNOCRD			KUSDCNOCRD
#define	KSDCARDCANRE			KUSDCCANRE
#define	KSDCARDLNBU0			KUSDCLNBU0
#define	KSDCARDTRANT			KUSDCTRANT

#define KINIT					0
#define KSLOW400KHZ				1
#define KFAST50MHz				2
#define KRELEASE				3
#define KSELECT					4
#define KDESELECT				5

// Model callbacks
// ---------------

/*
 * \brief cb_control
 *
 * - Control of the SPI
 *   - Initialize
 *   - Reserve @ 400-KHz
 *   - Reserve @ 50-MHz
 *   - Release
 *   - Select
 *   - Deselect
 *
 */
static	void	cb_control(uint8_t mode) {

	switch (mode) {
		case KINIT: {
			RESERVE_SHARED_SPI4(KUSDCS);
			shared_spi4_deselect(KUSDC);
			RELEASE_SHARED_SPI4;
			break;
		}
		case KSLOW400KHZ: {
			RESERVE_SHARED_SPI4(KUSDCS);
			break;
		}
		case KFAST50MHz: {
			RESERVE_SHARED_SPI4(KUSDCF);
			break;
		}
		case KRELEASE: {
			RELEASE_SHARED_SPI4;
			break;
		}
		case KSELECT: {
			shared_spi4_select(KUSDC);
			break;
		}
		case KDESELECT: {
			shared_spi4_deselect(KUSDC);
			break;
		}
		default: {
			RELEASE_SHARED_SPI4;
			shared_spi4_deselect(KUSDC);
			break;
		}
	}
}

/*
 * \brief cb_cardInserted
 *
 * - Verify if the SDCard is inserted
 *
 */
static	bool_t	cb_cardInserted(void) {

	if (GPIOA->IDR & (1<<BNOSDCARD)) {
		return (FALSE);
	}
	return (TRUE);
}

/*
 * \brief cb_writeReadSPI
 *
 * - Write/Read on the SPI
 *
 */
static	uint8_t	cb_writeReadSPI(uint8_t data) {
	uint8_t		wRData;

	wRData = data;
	shared_spi4_writeRead(&wRData);
	return (wRData);
}

#include 	<model_sdcard.cm>

