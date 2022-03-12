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
; Goal:		stub for the connection of the "usdc" manager to the SCDard, spi3 device.
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
			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;

			GPIOC->ODR   |= (1<<BSELSDCARD);

			SPI3->CR1 = SPI_CR1_MSTR					
			 		  | SPI_CR1_BR_2					
			  		  | SPI_CR1_BR_1;
			  		   
			SPI3->CR2  = SPI_CR2_SSOE;
			SPI3->CR1 |= SPI_CR1_SPE;
			break;
		}
		case KSLOW400KHZ: {
			SPI3->CR1 = SPI_CR1_MSTR					
					  | SPI_CR1_BR_2
					  | SPI_CR1_BR_1;

			SPI3->CR2  = SPI_CR2_SSOE;
			SPI3->CR1 |= SPI_CR1_SPE;
			break;
		}
		case KFAST50MHz: {
			SPI3->CR1 = SPI_CR1_MSTR;					

			SPI3->CR2  = SPI_CR2_SSOE;
			SPI3->CR1 |= SPI_CR1_SPE;
			break;
		}
		case KRELEASE: {
			break;
		}
		case KSELECT: {
			GPIOC->ODR &= ~(1<<BSELSDCARD);
			break;
		}
		case KDESELECT: {
			GPIOC->ODR |= (1<<BSELSDCARD);
			break;
		}
		default: {
			GPIOC->ODR |= (1<<BSELSDCARD);
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

	if (GPIOG->IDR & (1<<BNOSDCARD)) {
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
	volatile	uint16_t	dummy __attribute__ ((unused));

// Send a Byte ... and waiting for the end of the transfer

	dummy = SPI3->DR;
	SPI3->DR = (uint16_t)data;
	while (!(SPI3->SR & SPI_SR_RXNE));
	return ((uint8_t)SPI3->DR);
}

#include 	<model_sdcard.cm>

