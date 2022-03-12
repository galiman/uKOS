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

#define	BCS				0		// Pseudo CS
#define	BADD0			8		// Pseudo address 0
#define	BADD1			9		// Pseudo address 1
#define	BADD2			10		// Pseudo address 2
#define	BRW				11		// Pseudo R/W

#define KCRLC_R			((KFLOAOINP<<28) | (KFLOAOINP<<24) | (KFLOAOINP<<20) | (KFLOAOINP<<16) | (KFLOAOINP<<12) | (KFLOAOINP<<8) | (KFLOAOINP<<4) | (KFLOAOINP<<0))
#define KCRLC_W			((KPPOUT50M<<28) | (KPPOUT50M<<24) | (KPPOUT50M<<20) | (KPPOUT50M<<16) | (KPPOUT50M<<12) | (KPPOUT50M<<8) | (KPPOUT50M<<4) | (KPPOUT50M<<0))

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

// Data bus (PBUS) in input mode

	INTERRUPTION_OFF;
	GPIOC->CRL = KCRLC_R;

// Output the addresses and the RW

	bus = GPIOF->ODR;
	bus = (bus & ~((1<<BADD2) | (1<<BADD1) | (1<<BADD0)));
	bus = bus | ((address & 0x07)<<8) ;
	bus = bus | (1<<BRW);
	GPIOF->ODR = bus;

	GPIOB->BRR = (1<<BCS);
	NOP;
	NOP;
	NOP;
	NOP;
	*data = (uint8_t)GPIOC->IDR;
	NOP;
	NOP;
	NOP;
	NOP;
	GPIOB->BSRR = (1<<BCS);
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
	uint16_t	bus, gpiodata;

// Data bus (PBUS) in output mode

	INTERRUPTION_OFF;
	GPIOC->ODR = 0x0000;
	GPIOC->CRL = KCRLC_W;

// Prepare the data, the addresses and the RW

	gpiodata = GPIOC->ODR;
	gpiodata = (gpiodata & 0xFF00) | data;
	GPIOC->ODR = gpiodata;

	bus = GPIOF->ODR;
	bus = (bus & ~((1<<BADD2) | (1<<BADD1) | (1<<BADD0)));
	bus = bus | ((address & 0x07)<<8) ;
	bus = bus & ~(1<<BRW);
	GPIOF->ODR = bus;

// Activate-desactivate the chip select

	GPIOB->BRR = (1<<BCS);
	NOP;
	NOP;
	NOP;
	NOP;
	GPIOB->BSRR = (1<<BCS);
	INTERRUPTION_RESTORED;
}

