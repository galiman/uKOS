/*
; stub_tmp0_lsm9ds1.
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
; Goal:		stub for the "tmp0" manager module.
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
#include 	<LSM9DS1/LSM9DS1.h>
#include 	<spi5/spi5.h>

// Prototypes
// ==========

static	uint8_t	_writeReadSPI(uint8_t data);
static	void	_writeAccGyro(uint8_t address, uint8_t *data, uint8_t number);
static	void	_readAccGyro(uint8_t address, uint8_t *data, uint8_t number);

/*
 * \brief stub_tmp0_init
 *
 * - Initialize some specific CPU parts
 *
 */
void	stub_tmp0_init(void) {
	uint8_t		data;

	RESERVE_SHARED_SPI5(KTMP0);
	shared_spi5_deselect(KTMP0);
	
	data = 0x78; _writeAccGyro(CTRL_REG1_G,  &data, 1);
	RELEASE_SHARED_SPI5;
}

/*
 * \brief stub_tmp0_getTemperature
 *
 * - Get the temperature
 *
 */
int32_t	stub_tmp0_getTemperature(float64_t *temperature) {
	uint8_t		xsb[2];
	int16_t		value;

	RESERVE_SHARED_SPI5(KTMP0);
	_readAccGyro(OUT_TEMP_L, xsb, 2);
	value = (int16_t)(xsb[1]<<8 | xsb[0]);
	*temperature = ((float64_t)(value)) / 16.0 + 25.0;
	RELEASE_SHARED_SPI5;
	return (KTMP0NOERR);
}

/*
 * \brief stub_tmp0_setTemperature
 *
 * - Set the temperature
 *
 */
int32_t	stub_tmp0_setTemperature(float64_t temperature) {

	return (KTMP0SYCNA);
}

// Local routines
// ==============

/*
 * \brief _writeAccGyro
 *
 */
static	void	_writeAccGyro(uint8_t address, uint8_t *data, uint8_t number) {
	uint8_t		i;

// Write the register address
// Then, write 1..n data

	shared_spi5_select(KTMP0);
	_writeReadSPI(address);
	for (i = 0; i < number; i++) {
		_writeReadSPI(*data++);
	}

	shared_spi5_deselect(KTMP0);
}

/*
 * \brief _readAccGyro
 *
 */
static	void	_readAccGyro(uint8_t address, uint8_t *data, uint8_t number) {
	uint8_t		i;

// Write the register address
// Then, Read 1..n data

	shared_spi5_select(KTMP0);
	_writeReadSPI(0x80 | address);
	for (i = 0; i < number; i++) {
		*data++ = _writeReadSPI(0x00);
	}

	shared_spi5_deselect(KTMP0);
}

/*
 * \brief _writeReadSPI
 *
 */
static	uint8_t	_writeReadSPI(uint8_t data) {
	uint8_t		wRData;

	wRData = data;
	shared_spi5_writeRead(&wRData);
	return (wRData);
}

