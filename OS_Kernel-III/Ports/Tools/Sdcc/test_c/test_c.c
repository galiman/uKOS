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

#include	"pic16f630.h"
#include	"types.h"

#define KSPI_TRIS_0 			TRISC0									// Direction for nReset
#define KNREST	 				RC0										// nReset
#define KSPI_TRIS_1				TRISC1									// Direction for nCS
#define KNCS					RC1										// nCS
#define KSPI_TRIS_2				TRISC2									// Direction for SCK
#define KSCK					RC2										// SCK
#define KSPI_TRIS_3				TRISC3									// Direction for MOSI
#define KMOSI					RC3										// MOSI

#define	KVALUEMS				100										// For 1-ms (has to be calibrated)

// Prototypes
// ==========

static	void	_writeSPI(uint8_t nbBits, uint32_t *table);
static	void	_delay_ms(uint32_t ms);

__code char_t __at 0x2007 __CONFIG = 0x3F94;

// Table for the ASIC initialization (50-bits)
// -------------------------------------------

static	uint32_t	aTabInit[2] = {
						0b10101010101010101010101010101010,
						0b10101010101010101100000000000000
					};

void	main(void) {

// Initialize the directions of the port bit used

    KSPI_TRIS_0 = 0; KSPI_TRIS_1 = 0; KSPI_TRIS_2 = 0; KSPI_TRIS_3 = 0;
	KNREST = 1; KNCS = 1; KSCK = 0; KMOSI = 1;

// Strobe the nRset 250-ms

	KNREST = 0; _delay_ms(250);		//
	KNREST = 1; _delay_ms(250);		// Pulse the nReset

// Write on the SPI 32 + 18-bits

	KNCS = 0; _delay_ms(250);		// Select the SPI

	_writeSPI(32, &aTabInit[0]);	//
	_writeSPI(18, &aTabInit[1]);	// Write the configuration stream

// Terminated

	KNCS = 1; _delay_ms(250);		// Disable the SPI

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
    	KMOSI = (*table && (shift>>i)) ? 1 : 0;
    	_delay_ms(250);

// Clock the data

		KSCK = 1; _delay_ms(250);	//
		KSCK = 0; _delay_ms(250);	// Clock the data
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

