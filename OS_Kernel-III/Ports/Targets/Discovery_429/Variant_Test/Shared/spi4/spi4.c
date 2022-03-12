/*
; spi4.
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

#include 	<uKOS.h>
#include 	"spi4.h"

static	bool_t		vReserved   = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"spi4         Sharing of the spi4 device.               (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "spi4 shared device\n"
									"==================\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_spi4_glob_init(void);
static	void	_spi4_usdc_initFast(void);
static	void	_spi4_usdc_initSlow(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Spi4, KIDSHAR, KSPI4NUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief Reserve the shared spi4 device
 *
 * \param[in]	manager		The manager id
 * \param[out]	KSPI4NOERR	The spi4 device is reserved
 * \param[out]  KSPI4CHBSY 	The spi4 device is busy
 *
 */
int32_t	shared_spi4_reserve(uint32_t manager) {
	static	uint32_t	vPreManager = KNOMANAGER;

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KSPI4CHBSY); }
	else {
		vReserved = TRUE;
		if (manager != vPreManager) {
			vPreManager = manager;

			switch (manager) {
				case KGLOB:  { _spi4_glob_init();     break; }
				case KUSDCS: { _spi4_usdc_initSlow(); break; }
				case KUSDCF: { _spi4_usdc_initFast(); break; }
				default: {
					break;
				}
			}		
		}
	}
	RETURN_INT_RESTORED(KSPI4NOERR);
}

/*
 * \brief Release the shared spi4 device
 *
 * \param[in]	-
 * \param[out]	KSPI4NOERR	OK
 *
 */
int32_t	shared_spi4_release(void) {

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KSPI4NOERR);
}

/*
 * \brief Select the shared spi4 manager
 *
 * \param[in]	manager		The manager id
 * \param[out]	KSPI4NOERR	OK
 *
 */
int32_t	shared_spi4_select(uint32_t manager) {

	INTERRUPTION_OFF;
	switch (manager) {
		case KGLOB:  { GPIOE->ODR &= ~(1<<BSELEEPROM); break; }
		case KUSDC:
		case KUSDCS:
		case KUSDCF: { GPIOC->ODR &= ~(1<<BSELSDCARD); break; }
		default: {
			break;
		}
	}	
	RETURN_INT_RESTORED(KSPI4NOERR);
}

/*
 * \brief Deselect the shared spi4 manager
 *
 * \param[in]	manager		The manager id
 * \param[out]	KSPI4NOERR	OK
 *
 */
int32_t	shared_spi4_deselect(uint32_t manager) {

	INTERRUPTION_OFF;
	switch (manager) {
		case KGLOB:  { GPIOE->ODR |= (1<<BSELEEPROM); break; }
		case KUSDC:
		case KUSDCS:
		case KUSDCF: { GPIOC->ODR |= (1<<BSELSDCARD); break; }
		default: {
			break;
		}
	}	
	RETURN_INT_RESTORED(KSPI4NOERR);
}

/*
 * \brief Write/Read to/on the shared spi4 manager
 *
 * \param[in]	*data		Ptr on the data
 * \param[out]	KSPI4NOERR	OK
 *
 */
int32_t	shared_spi4_writeRead(uint8_t *data) {
	volatile	uint16_t	dummy __attribute__ ((unused));

// Send a Byte ... and waiting for the end of the transfer

	dummy = SPI4->DR;
	SPI4->DR = (uint16_t)*data;
	while (!(SPI4->SR & SPI_SR_RXNE));
	*data = (uint8_t)SPI4->DR;
	return (KSPI4NOERR);
}

// Local routines
// ==============

/*
 * \brief _spi4_glob_init
 *
 * - Initialize the SPI4 interface for working with the glob manager
 *	 - High speed accesses 11.2-MBit/s
 *
 */
static	void	_spi4_glob_init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

// Initialize the SPI4 Master, POL0 = CPHA0 = 0
// Baudrate f90 / 8 (11.2-MBit/s)

	SPI4->CR1 = SPI_CR1_MSTR
			  | SPI_CR1_BR_1;

	SPI4->CR2  = SPI_CR2_SSOE;
	SPI4->CR1 |= SPI_CR1_SPE;
}

/*
 * \brief _spi4_usdc_initSlow
 *
 * - Initialize the SPI4 interface for working with the usdc manager
 *	 - Slow speed accesses 351-KBit/s
 *
 */
static	void	_spi4_usdc_initSlow(void) {

	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

// Initialize the SPI4 Master, POL0 = CPHA0 = 0
// Baudrate f90 / 256 (351-KBit/s)

	SPI4->CR1 = SPI_CR1_MSTR
			  | SPI_CR1_BR_2
			  | SPI_CR1_BR_1
			  | SPI_CR1_BR_0;

	SPI4->CR2  = SPI_CR2_SSOE;
	SPI4->CR1 |= SPI_CR1_SPE;
}

/*
 * \brief shared_spi4_usdc_initFast
 *
 * - Initialize the SPI4 interface for working with the usdc manager
 *	 - High speed accesses 45-MBit/s
 *
 */
static	void	_spi4_usdc_initFast(void) {

	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;

// Initialize the SPI4 Master, POL0 = CPHA0 = 0
// Baudrate f90 / 2 (45-MBit/s)

	SPI4->CR1  = SPI_CR1_MSTR;
	SPI4->CR2  = SPI_CR2_SSOE;
	SPI4->CR1 |= SPI_CR1_SPE;
}

