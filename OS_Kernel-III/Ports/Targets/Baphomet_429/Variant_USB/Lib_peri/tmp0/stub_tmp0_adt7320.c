/*
; stub_tmp0_adt7320.
; ==================

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
; Goal:		stub for the connection of the "tmp0" manager to the adt7320, spi5 device.
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

#define	adt7320_init			stub_tmp0_init			
#define	adt7320_getTemperature	stub_tmp0_getTemperature		
#define	adt7320_setTemperature	stub_tmp0_setTemperature			

#define	KADT7320NOERR			KTMP0NOERR
#define	KADT7320SYCNA			KTMP0SYCNA
#define	KADT7320GEERR			KTMP0GEERR
#define	KADT7320CHBSY			KTMP0CHBSY

#define KINIT					0
#define KRESERVE				1
#define KRELEASE				2
#define KSELECT					3
#define KDESELECT				4

// Model callbacks
// ---------------

/*
 * \brief cb_control
 *
 * - Control of the SPI
 *   - Initialize
 *   - Reserve
 *   - Release
 *   - Select
 *   - Deselect
 *
 */
static	void	cb_control(uint8_t mode) {

	switch (mode) {
		case KINIT: {
			RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;

			GPIOF->ODR   |= (1<<BSELADT7320);
			kern_suspendProcess(1);

			SPI5->CR1 = SPI_CR1_MSTR					
					  | SPI_CR1_CPHA					
					  | SPI_CR1_CPOL					
					  | SPI_CR1_BR_2;

			SPI5->CR2  = SPI_CR2_SSOE;
			SPI5->CR1 |= SPI_CR1_SPE;
			break;
		}
		case KRESERVE: {
			break;
		}
		case KRELEASE: {
			break;
		}
		case KSELECT: {
			GPIOF->ODR &= ~(1<<BSELADT7320);
			break;
		}
		case KDESELECT: {
			GPIOF->ODR |= (1<<BSELADT7320);
			break;
		}
		default: {
			GPIOF->ODR |= (1<<BSELADT7320);
			break;
		}
	}
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

	dummy = SPI5->DR;
	SPI5->DR = (uint16_t)data;
	while (!(SPI5->SR & SPI_SR_RXNE));
	return ((uint8_t)SPI5->DR);
}

#include 	<model_adt7320.cm>

