/*
; spi5.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
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

#include 	<uKOS.h>
#include 	"spi5.h"

static	bool_t		vReserved = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"spi5         Sharing of the spi5 device.               (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "spi5 shared device\n"
									"==================\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_spi5_imu0_init(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Spi5, KIDSHAR, KSPI5NUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief Reserve the shared spi5 device
 *
 * \param[in]	manager		The manager id
 * \param[out]	KSPI5NOERR	The spi5 device is reserved
 * \param[out]  KSPI5CHBSY 	The spi5 device is busy
 *
 */
int32_t	shared_spi5_reserve(uint32_t manager) {
	static	uint32_t	vPreManager = KNOMANAGER;

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KSPI5CHBSY); }
	else {
		vReserved = TRUE;
		if (manager != vPreManager) {
			vPreManager = manager;

			switch (manager) {
				case KIMU0:
				case KIMUA:
				case KIMUM:
				case KTMP0: { _spi5_imu0_init(); break; }
				default: {
					break;
				}
			}		
		}
	}
	RETURN_INT_RESTORED(KSPI5NOERR);
}

/*
 * \brief Release the shared spi5 device
 *
 * \param[in]	-
 * \param[out]	KSPI5NOERR	OK
 *
 */
int32_t	shared_spi5_release(void) {

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KSPI5NOERR);
}

/*
 * \brief Select the shared spi5 manager
 *
 * \param[in]	manager		The manager id
 * \param[out]	KSPI5NOERR	OK
 *
 */
int32_t	shared_spi5_select(uint32_t manager) {

	INTERRUPTION_OFF;
	switch (manager) {
		case KTMP0:
		case KIMUA: { GPIOG->ODR &= ~(1<<BSELACCELERO); break; }
		case KIMUM: { GPIOF->ODR &= ~(1<<BSELMAGNETO);  break; }
		default: {
			break;
		}
	}	
	RETURN_INT_RESTORED(KSPI5NOERR);
}

/*
 * \brief Deselect the shared spi5 manager
 *
 * \param[in]	manager		The manager id
 * \param[out]	KSPI5NOERR	OK
 *
 */
int32_t	shared_spi5_deselect(uint32_t manager) {

	INTERRUPTION_OFF;
	switch (manager) {
		case KTMP0:
		case KIMUA: { GPIOG->ODR |= (1<<BSELACCELERO);                                 break; }
		case KIMUM: { GPIOF->ODR |= (1<<BSELMAGNETO);                                  break; }
		case KIMU0: { GPIOG->ODR |= (1<<BSELACCELERO); GPIOF->ODR |= (1<<BSELMAGNETO); break; }
		default: {
			break;
		}
	}	
	RETURN_INT_RESTORED(KSPI5NOERR);
}

/*
 * \brief Write/Read to/on the shared spi5 manager
 *
 * \param[in]	*data		Ptr on the data
 * \param[out]	KSPI5NOERR	OK
 *
 */
int32_t	shared_spi5_writeRead(uint8_t *data) {
 	volatile	uint16_t	dr;

// Send a Byte ... and waiting for the end of the transfer
// Write only 8-bits (packed in 2, 4 bit transfer)

	SPI5->DR;
	SPI5->DR = ((*data<<8) | (*data>>4)) & 0x0F0F;

// Waiting for the end of the transfer and the data available
// Read 16-bits (1 byte packed in 2, (2x4 bit transfer))

	while ((SPI5->SR & SPI_SR_BSY));

	dr = SPI5->DR;
	*data = ((uint8_t)(((dr<<4) & 0xF0) | ((dr>>8) & 0x0F)));
	return (KSPI5NOERR);
}

// Local routines
// ==============

/*
 * \brief _spi5_imu0_init
 *
 * - Initialize the SPI5 interface for working with the imu0 manager
 *	 - High speed accesses 5.62-MBit/s
 *
 */
static	void	_spi5_imu0_init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;

// Initialize the SPI5 Master, POL1 = CPHA0 = 1
// Baudrate f108 / 16 (6.75-MBit/s)

	SPI5->CR1 = SPI_CR1_MSTR
			  | SPI_CR1_CPHA
			  | SPI_CR1_BR_0
			  | SPI_CR1_BR_1
			  | SPI_CR1_CPOL;

	SPI5->CR2  = SPI_CR2_SSOE | ((4-1)<<8);
	SPI5->CR1 |= SPI_CR1_SPE;
}

