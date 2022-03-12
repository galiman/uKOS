/*
; stub_glob.
; ==========

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
; Goal:		stub for the "glob" manager module.
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

#define	KFAMILY		CSEM_VIP_777

// Prototypes
// ==========

static	uint8_t	_writeReadSPI(uint8_t data);

/*
 * \brief stub_glob_init
 *
 * - Initialize some specific CPU parts
 *
 */
void	stub_glob_init(void) {

	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;

	GPIOB->ODR |= (1<<BSELEEPROM);

// Initialize the SPI2 Master, POL0 = CPHA0 = 0, f54 / 4 (13.5-MBit/s)

	SPI2->CR1 = SPI_CR1_MSTR					
			  | SPI_CR1_BR_0;

	SPI2->CR2  = SPI_CR2_SSOE | ((4-1)<<8);
	SPI2->CR1 |= SPI_CR1_SPE;

// Turn on the RNG
// Read the first random number: it has to be considered as "wrong" 

	RNG->CR = RNG_CR_RNGEN;
	kern_waitAtLeast(10);
	RNG->DR;
}

/*
 * \brief stub_glob_getFamily
 *
 * - Return the family Id
 *
 */
int32_t	stub_glob_getFamily(uint32_t *family) {

	*family = KFAMILY;
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_getRND
 *
 * - Return the random number
 *
 */
int32_t	stub_glob_getRND(uint32_t *number) {

	while (!(RNG->SR & (RNG_SR_DRDY))) {
		kern_suspendProcess(1);
	}

	*number = RNG->DR;
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_restart
 *
 * - Disable all the system interruption
 * - Restart
 *
 */
int32_t	stub_glob_restart(void) {

// Stop all the interruptions and restart

	INTERRUPTION_OFF;
	*((volatile uint32_t *)0xE000ED0C) = 0x05FA0004;
	while (TRUE);

	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_blankMemory
 *
 * - Blank checking
 *
 */
int32_t	stub_glob_blankMemory(uint32_t address, uint32_t size) {
	uint8_t		data;
	uint32_t	addressEEPROM, index;

	if (size == 0) {
		return (KGLOBNOERR);
	}

	addressEEPROM = address;
	for (index = 0; index < size; index++) {

// A read cycle 0x03 0xAA 0xAA 0xAA 0x00
// 0x03 --> Read order
// 0xAA --> 24-bit address
// 0x00 --> dummy

		kern_waitAtLeast(1);
		GPIOB->ODR &= ~(1<<BSELEEPROM);
		_writeReadSPI(KCMDREAD);
		_writeReadSPI((uint8_t)(addressEEPROM>>16));
		_writeReadSPI((uint8_t)(addressEEPROM>>8));
		_writeReadSPI((uint8_t)(addressEEPROM));
		data = _writeReadSPI(0x00);
		GPIOB->ODR |= (1<<BSELEEPROM);

		if (data != 0xFF) {
			return (KGLOBFLNOE);
		}

		addressEEPROM++;
	}
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_eraseMemory
 *
 * - Erase sectors
 *
 */
int32_t	stub_glob_eraseMemory(uint32_t address, uint32_t nbSectors) {
	uint32_t	sector, addressEEPROM;

	if (nbSectors == 0) {
		return (KGLOBNOERR);
	}

	addressEEPROM = address & 0x00FF8000;

	for (sector = 0; sector < nbSectors; sector++) {

// First, place the EEPROM in the WRITE ENABLE mode

		kern_waitAtLeast(1);
		GPIOB->ODR &= ~(1<<BSELEEPROM);
		_writeReadSPI(KCMDWRENABLE);
		GPIOB->ODR |= (1<<BSELEEPROM);

// Erase a sector 32-KBytes
// 0x20 --> Sector erase order
// 0xAA --> 24-bit address (boundary of 32-KBytes)

		kern_waitAtLeast(1);
		GPIOB->ODR &= ~(1<<BSELEEPROM);
		_writeReadSPI(KCMDERASESECTOR);
		_writeReadSPI((uint8_t)(addressEEPROM>>16));
		_writeReadSPI((uint8_t)(addressEEPROM>>8));
		_writeReadSPI((uint8_t)(addressEEPROM));
		GPIOB->ODR |= (1<<BSELEEPROM);
		kern_suspendProcess(3000);

		addressEEPROM += KSZSECTOR;
	}
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_writeMemory
 *
 * - Write pages
 * - This function writes pages. If a page is not complete
 *   this system call complete it with an unpredictable content
 *
 */
int32_t	stub_glob_writeMemory(uint32_t address, const uint8_t *buffer, uint32_t size) {
	uint32_t	addressEEPROM, index, pages, nbPages;

	if (size == 0) {
		return (KGLOBNOERR);
	}

	nbPages = (size>>8);
	if ((size - (nbPages<<8)) > 0) {
		nbPages++;
	}

	addressEEPROM = address & 0x00FFFF00;

// Write complete pages of 256-Bytes
// In case of the page is not complete,
// the page is completed with dummy data

	for (pages = 0; pages < nbPages; pages++) {

// First, place the EEPROM in the WRITE ENABLE mode

		kern_waitAtLeast(1);
		GPIOB->ODR &= ~(1<<BSELEEPROM);
		_writeReadSPI(KCMDWRENABLE);
		GPIOB->ODR |= (1<<BSELEEPROM);

// Write a page 0x02 0xAA 0xAA 0xAA (256 x 0xDD) (waiting for 5-ms)
// 0x02 --> Write order
// 0xAA --> 24-bit address (boundary of 256-Bytes)
// 0xDD --> 256 x Data

		kern_waitAtLeast(1);
		GPIOB->ODR &= ~(1<<BSELEEPROM);
		_writeReadSPI(KCMDWRITE);
		_writeReadSPI((uint8_t)(addressEEPROM>>16));
		_writeReadSPI((uint8_t)(addressEEPROM>>8));
		_writeReadSPI((uint8_t)(addressEEPROM));

		for (index = 0; index < KSZPAGE; index++) {
			_writeReadSPI(*buffer++);
		}

		GPIOB->ODR |= (1<<BSELEEPROM);
		kern_suspendProcess(5);

		addressEEPROM += KSZPAGE;
	}
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_readMemory
 *
 * - Read bytes
 *
 */
int32_t	stub_glob_readMemory(uint32_t address, uint8_t *buffer, uint32_t size) {
	uint32_t	addressEEPROM, index;

	if (size == 0) {
		return (KGLOBNOERR);
	}

	addressEEPROM = address;

// A read cycle 0x03 0xAA 0xAA 0xAA 0x00
// 0x03 --> Read order
// 0xAA --> 24-bit address
// 0x00 --> dummy

	kern_waitAtLeast(1);
	GPIOB->ODR &= ~(1<<BSELEEPROM);
	_writeReadSPI(KCMDREAD);
	_writeReadSPI((uint8_t)(addressEEPROM>>16));
	_writeReadSPI((uint8_t)(addressEEPROM>>8));
	_writeReadSPI((uint8_t)(addressEEPROM));

	for (index = 0; index < size; index++) {
		*buffer++ = _writeReadSPI(0x00);
	}

	GPIOB->ODR |= (1<<BSELEEPROM);
	return (KGLOBNOERR);
}

// Local routines
// ==============

/*
 * \brief _writeReadSPI
 *
 */
static	uint8_t	_writeReadSPI(uint8_t data) {
	volatile	uint16_t	dummy __attribute__ ((unused));
 	volatile	uint16_t	dr;
 	volatile	uint8_t		msb, lsb;

// Send a Byte ... and waiting for the end of the transfer

	dummy = SPI2->DR;

// Write only 8-bits (packed in 2, 4 bit transfer)

	dr  = (data<<8) | (data>>4);
	dr &= 0x0F0F;
	SPI2->DR = dr;

// Waiting for the end of the transfer and the data available
// Read 16-bits (1 byte packed in 2, (2x4 bit transfer))

	while (!(SPI2->SR & SPI_SR_TXE) || (SPI2->SR & SPI_SR_FTLVL))
	while (!(SPI2->SR & SPI_SR_FRLVL_1));

	dr = SPI2->DR;
	msb = (dr<<4) & 0xF0;
	lsb = (dr>>8) & 0x0F;
	return (msb | lsb);
}

