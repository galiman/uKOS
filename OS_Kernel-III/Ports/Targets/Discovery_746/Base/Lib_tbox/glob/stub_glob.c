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
#include 	<spi4/spi4.h>

#define	KFAMILY		Discovery_746

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

	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;

	shared_spi4_deselect(KGLOB);

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

	RESERVE_SHARED_SPI4(KGLOB);
	for (index = 0; index < size; index++) {

// A read cycle 0x03 0xAA 0xAA 0xAA 0x00
// 0x03 --> Read order
// 0xAA --> 24-bit address
// 0x00 --> dummy

		kern_waitAtLeast(1);
		shared_spi4_select(KGLOB);
		_writeReadSPI(KCMDREAD);
		_writeReadSPI((uint8_t)(addressEEPROM>>16));
		_writeReadSPI((uint8_t)(addressEEPROM>>8));
		_writeReadSPI((uint8_t)(addressEEPROM));
		data = _writeReadSPI(0x00);
		shared_spi4_deselect(KGLOB);

		if (data != 0xFF) {
			RELEASE_SHARED_SPI4;
			return (KGLOBFLNOE);
		}
		addressEEPROM++;
	}
	RELEASE_SHARED_SPI4;
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

	addressEEPROM = address & 0x00FF0000;

	RESERVE_SHARED_SPI4(KGLOB);
	for (sector = 0; sector < nbSectors; sector++) {

// First, place the EEPROM in the WRITE ENABLE mode

		kern_waitAtLeast(1);
		shared_spi4_select(KGLOB);
		_writeReadSPI(KCMDWRENABLE);
		shared_spi4_deselect(KGLOB);

// Erase a sector 64-KBytes
// 0x20 --> Sector erase order
// 0xAA --> 24-bit address (boundary of 64-KBytes)

		kern_waitAtLeast(1);
		shared_spi4_select(KGLOB);
		_writeReadSPI(KCMDERASESECTOR);
		_writeReadSPI((uint8_t)(addressEEPROM>>16));
		_writeReadSPI((uint8_t)(addressEEPROM>>8));
		_writeReadSPI((uint8_t)(addressEEPROM));
		shared_spi4_deselect(KGLOB);
		kern_suspendProcess(3000);

		addressEEPROM += KSZSECTOR;
	}
	RELEASE_SHARED_SPI4;
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

	RESERVE_SHARED_SPI4(KGLOB);
	for (pages = 0; pages < nbPages; pages++) {

// First, place the EEPROM in the WRITE ENABLE mode

		kern_waitAtLeast(1);
		shared_spi4_select(KGLOB);
		_writeReadSPI(KCMDWRENABLE);
		shared_spi4_deselect(KGLOB);

// Write a page 0x02 0xAA 0xAA 0xAA (256 x 0xDD) (waiting for 5-ms)
// 0x02 --> Write order
// 0xAA --> 24-bit address (boundary of 256-Bytes)
// 0xDD --> 256 x Data

		kern_waitAtLeast(1);
		shared_spi4_select(KGLOB);
		_writeReadSPI(KCMDWRITE);
		_writeReadSPI((uint8_t)(addressEEPROM>>16));
		_writeReadSPI((uint8_t)(addressEEPROM>>8));
		_writeReadSPI((uint8_t)(addressEEPROM));

		for (index = 0; index < KSZPAGE; index++) {
			_writeReadSPI(*buffer++);
		}

		shared_spi4_deselect(KGLOB);
		kern_suspendProcess(5);

		addressEEPROM += KSZPAGE;
	}
	RELEASE_SHARED_SPI4;
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

	RESERVE_SHARED_SPI4(KGLOB);
	kern_waitAtLeast(1);
	shared_spi4_select(KGLOB);
	_writeReadSPI(KCMDREAD);
	_writeReadSPI((uint8_t)(addressEEPROM>>16));
	_writeReadSPI((uint8_t)(addressEEPROM>>8));
	_writeReadSPI((uint8_t)(addressEEPROM));

	for (index = 0; index < size; index++) {
		*buffer++ = _writeReadSPI(0x00);
	}

	shared_spi4_deselect(KGLOB);
	RELEASE_SHARED_SPI4;
	return (KGLOBNOERR);
}

// Local routines
// ==============

/*
 * \brief _writeReadSPI
 *
 */
static	uint8_t	_writeReadSPI(uint8_t data) {
	uint8_t		wRData;

	wRData = data;
	shared_spi4_writeRead(&wRData);
	return (wRData);
}

