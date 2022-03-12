/*
; hard.
; =====

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
; Goal:		Some basic hardware initializations.
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
 * \brief hard_onLed
 *
 */
void	hard_onLed(uint32_t ledNb) {

	INTERRUPTION_OFF;
	switch (ledNb) {
		case 0: { GPIOF->ODR |= (1<<BLED0); break; }
		case 1: { GPIOF->ODR |= (1<<BLED1); break; }
		case 2: { GPIOG->ODR |= (1<<BLED2); break; }
		case 3: { GPIOG->ODR |= (1<<BLED3); break; }
		case 4: { GPIOG->ODR |= (1<<BLED4); break; }
		case 5: { GPIOH->ODR |= (1<<BLED5); break; }
		case 6: { GPIOH->ODR |= (1<<BLED6); break; }
		case 7: { GPIOI->ODR |= (1<<BLED7); break; }
		default: {
			break;
		}
	}
	INTERRUPTION_RESTORED;
}

/*
 * \brief hard_offLed
 *
 */
void	hard_offLed(uint32_t ledNb) {

	INTERRUPTION_OFF;
	switch (ledNb) {
		case 0: { GPIOF->ODR &= ~(1<<BLED0); break; }
		case 1: { GPIOF->ODR &= ~(1<<BLED1); break; }
		case 2: { GPIOG->ODR &= ~(1<<BLED2); break; }
		case 3: { GPIOG->ODR &= ~(1<<BLED3); break; }
		case 4: { GPIOG->ODR &= ~(1<<BLED4); break; }
		case 5: { GPIOH->ODR &= ~(1<<BLED5); break; }
		case 6: { GPIOH->ODR &= ~(1<<BLED6); break; }
		case 7: { GPIOI->ODR &= ~(1<<BLED7); break; }
		default: {
			break;
		}
	}
	INTERRUPTION_RESTORED;
}

/*
 * \brief hard_toggleLed
 *
 */
void	hard_toggleLed(uint32_t ledNb) {

	INTERRUPTION_OFF;
	switch (ledNb) {
		case 0: { GPIOF->ODR ^= (1<<BLED0); break; }
		case 1: { GPIOF->ODR ^= (1<<BLED1); break; }
		case 2: { GPIOG->ODR ^= (1<<BLED2); break; }
		case 3: { GPIOG->ODR ^= (1<<BLED3); break; }
		case 4: { GPIOG->ODR ^= (1<<BLED4); break; }
		case 5: { GPIOH->ODR ^= (1<<BLED5); break; }
		case 6: { GPIOH->ODR ^= (1<<BLED6); break; }
		case 7: { GPIOI->ODR ^= (1<<BLED7); break; }
		default: {
			break;
		}
	}
	INTERRUPTION_RESTORED;
}

