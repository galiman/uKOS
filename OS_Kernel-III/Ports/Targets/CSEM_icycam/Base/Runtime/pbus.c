/*
; pbus.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2007-05-27
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Pseudo bus access
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

#include	<uKOS.h>

/*
 * \brief pbus_read
 *
 * \param[in]	address	Relative address to read
 * \param[in]	*data	Ptr on the data (result)
 * \param[out]	-
 *
 */
void	pbus_read(uint16_t address, uint8_t *data) {
	uint16_t	bus;

	INTERRUPTION_OFF;
	*GPIO_DATAOUT_EN = KDIRIN;						// Direction IN
	bus = address | (1<<BRW);						// Address + R
	*GPIO_DATAOUT = bus;							// Address + R
	*GPIO_DATAOUT = bus | (1<<BCS);					// Address + R + CS
	NOP;
	NOP;
	*data = (uint8_t)(*GPIO_DATAIN>>8);				// Read the data
	NOP;
	NOP;
	*GPIO_DATAOUT = bus;							// Disable the bus
	INTERRUPTION_RESTORED;
}

/*
 * \brief pbus_write
 *
 * \param[in]	address	Relative address to read
 * \param[in]	data	Data
 * \param[out]	-
 *
 */
void	pbus_write(uint16_t address, uint8_t data) {
	uint16_t	bus;

	INTERRUPTION_OFF;
	*GPIO_DATAOUT_EN = KDIROUT;						// Direction OUT
	bus = (data<<8) | address;						// Data + Address + W
	*GPIO_DATAOUT = bus;							// Data + Address + W
	NOP;
	NOP;
	*GPIO_DATAOUT = bus | (1<<BCS);					// Data + Address + W + CS
	NOP;
	NOP;
	*GPIO_DATAOUT = bus;							// Disable the bus
	INTERRUPTION_RESTORED;
}

