/*
; test_b.
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
; Goal:		Simple test for displaying a pattern on the board LEDs.
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

#define LEDA_TRIS_0 			TRISC0									// Direction for LED 0
#define LEDA_PIN_0 				RC0										// LED 0
#define LEDA_TRIS_1				TRISC1									// Direction for LED 1
#define LEDA_PIN_1				RC1										// LED 1
#define LEDA_TRIS_2				TRISC2									// Direction for LED 2
#define LEDA_PIN_2				RC2										// LED 2
#define LEDA_TRIS_3				TRISC3									// Direction for LED 3
#define LEDA_PIN_3				RC3										// LED 3

#define	KVALUEMS				100										// For 1-ms (has to be calibrated)

__code char_t __at 0x2007 __CONFIG = 0x3F94;

/*
 * \brief _delay_ms
 *
 * - Calibrated routine for waiting periods of nxms
 *
 * \param[in]	value in ms
 * \param[out]	-
 *
 */
void	_delay_ms(uint32_t ms) {
    uint16_t	i;

    while (ms--) {
        for (i = 0; i < KVALUEMS; i++) { }
    }
}

void	main(void) {

// Initialize the directions of the port bit used

    LEDA_TRIS_0 = 0; LEDA_TRIS_1 = 0; LEDA_TRIS_2 = 0; LEDA_TRIS_3 = 0;

// Forever, change the pattern
//
//      LED 0  LED 1  LED 2  LED 3
// 1:    1      0      0      0
// 2:    0      1      0      0
// 3:    0      0      1      0
// 4:    0      0      0      1
// 1:    1      0      0      0
// ...

    while (TRUE) {
        LEDA_PIN_0 = 1; LEDA_PIN_1 = 0; LEDA_PIN_2 = 0; LEDA_PIN_3 = 0;
        _delay_ms(250);
        LEDA_PIN_0 = 0; LEDA_PIN_1 = 1; LEDA_PIN_2 = 0; LEDA_PIN_3 = 0;
        _delay_ms(250);
        LEDA_PIN_0 = 0; LEDA_PIN_1 = 0; LEDA_PIN_2 = 1; LEDA_PIN_3 = 0;
        _delay_ms(250);
        LEDA_PIN_0 = 0; LEDA_PIN_1 = 0; LEDA_PIN_2 = 0; LEDA_PIN_3 = 1;
        _delay_ms(250);
    }
}

