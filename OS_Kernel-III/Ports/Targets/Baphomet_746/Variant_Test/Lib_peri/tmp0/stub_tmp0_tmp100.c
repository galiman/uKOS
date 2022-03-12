/*
; stub_tmp0_tmp100.
; =================

;------------------------------------------------------------------------
; Author:	Lorenzin Florence.	The 2017-04-13
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the connection of the "tmp0" manager to the tmp100 device.
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
#include 	<TMP100/TMP100.h>

// Connect the physical device to the logical manager
// --------------------------------------------------

#define	tmp100_init				stub_tmp0_init			
#define	tmp100_getTemperature	stub_tmp0_getTemperature		
#define	tmp100_setTemperature	stub_tmp0_setTemperature		

// Model callbacks
// ---------------

/*
 * \brief cb_configure
 *
 * - Configure
 *   - Initialize the i2c0
 *   - Configure the tmp100 for 12-bits
 *
 */
static	void	cb_configure(void) {
	cnfI2cx_t	configureI2C0;
	uint8_t		buffer[5];

// Initialise the i2c0

	configureI2C0.oSpeed    = K100KHZ;
	configureI2C0.oKernSync = 0;
	i2c0_configure(&configureI2C0);

// Initialise the tmp100_getTemperature

	buffer[0] = REGCNF;
	buffer[1] = (1<<BALERT) | (1<<BR1) | (1<<BR0);
	
	RESERVE(I2C0, KDEVALL);
	i2c0_write(KADDTMP100, buffer, 2);
	RELEASE(I2C0, KDEVALL);
}

/*
 * \brief cb_readI2C0
 *
 * - Read the temperature register
 *
 */
static	void	cb_readI2C0(int32_t *value) {
	uint8_t		buffer[5];

	buffer[0] = REGTMP;

	RESERVE(I2C0, KDEVALL);
	i2c0_read(KADDTMP100, buffer, 2);
	RELEASE(I2C0, KDEVALL);

	*value = (int32_t)(((buffer[0] << 8) | buffer[1]) >> 4);
}

/*
 * \brief cb_writeI2C0
 *
 * - Write the temperature register
 *
 */
static	void	cb_writeI2C0(int32_t value) {
	uint8_t		buffer[5];

	buffer[0] = REGHI;
	buffer[1] = (uint8_t)(value>>8);
	buffer[2] = (uint8_t)(value);

	RESERVE(I2C0, KDEVALL);
	i2c0_write(KADDTMP100, buffer, 2);
	RELEASE(I2C0, KDEVALL);
}

#include 	<model_tmp100.cm>

