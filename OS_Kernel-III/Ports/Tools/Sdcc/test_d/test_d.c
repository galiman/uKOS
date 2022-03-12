/*
; test_c.
; =======

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
; Goal:		Simple test for emulating a master SPI.
;			The target can be requested at:
;
;			http://www.didel.com
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

#include	"pic12f629.h"
#include	"types.h"

#define	KVALUEMS	130													// For 1-ms (has to be calibrated)

#define BNCS		0													// nCS
#define BMOSI		1													// MOSI
#define BSCK		2													// SCK

static	uint8_t		vImage;												// Image of the GPIO

#define	SETBIT(bit)	vImage |=  (1<<bit); \
					GPIO = vImage;

#define	RESBIT(bit)	vImage &= ~(1<<bit); \
					GPIO = vImage;

// Prototypes
// ==========

static	void	_writeSPI(uint8_t nbBits, uint32_t *table);
static	void	_delay_ms(uint32_t ms);

__code char_t __at 0x2007 __CONFIG = 0x3195;

// Table for the ASIC initialization (50-bits)
// -------------------------------------------

static	uint32_t	aTabInit[2] = {
						0b10101010101010101010101010101010,
						0b00110011001100110100000000000000
					};

void	main(void) {

// Initialize the directions of the port bit used

	vImage = 0x00;						//
	TRISIO = 0x00;						//
	SETBIT(BNCS); _delay_ms(500);		// Quite state

// Write on the SPI 32 + 18-bits

	RESBIT(BNCS); _delay_ms(500);		// Select the SPI

	_writeSPI(32, &aTabInit[0]);		//
	_writeSPI(18, &aTabInit[1]);		// Write the configuration stream

// Terminated

	SETBIT(BNCS); _delay_ms(250);		// Disable the SPI

    while (TRUE) { }
}

// Local routines
// ==============

/*
 * \brief _writeSPI
 *
 * - Write on the pseudo SPI
 *
 * \param[in]	nbBits
 * \param[in]	*table
 * \param[out]	-
 *
 */
static	void	_writeSPI(uint8_t nbBits, uint32_t *table) { 
    uint8_t		i;
    uint32_t	shift = 0x80000000;

    for (i = 0; i < nbBits; i++) {
  		if (*table & (shift>>i)) { SETBIT(BMOSI); }
  		else					 { RESBIT(BMOSI); }
   		_delay_ms(500);

// Clock the data

		SETBIT(BSCK); _delay_ms(500);
		RESBIT(BSCK); _delay_ms(500);
    }
}

/*
 * \brief _delay_ms
 *
 * - Calibrated routine for waiting periods of nxms
 *
 * \param[in]	ms	value in ms
 * \param[out]	-
 *
 */
static	void	_delay_ms(uint32_t ms) { 
    uint16_t	i;

    while (ms--) {
        for (i = 0; i < KVALUEMS; i++) { }
    }
}

