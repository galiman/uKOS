/*
; stub.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2009-01-08
; Modifs:
; Version:	1.2
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Hardware specific stub.
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

extern				void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
extern	volatile	uint32_t 	vCounter;

// Prototypes
// ==========

static	void	stub_intr_io_interruption(void);

/*
 * \brief stub_intr_io_init
 *
 */
void	stub_intr_io_init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI5_PA;

	NVIC->IP[EXTI9_5_IRQn] = (KINT_LEVEL_PERIPH<<4);
	NVIC->ISER[0] |= (1<<(EXTI9_5_IRQn & 0x1F));
	vExce_indIntVectors[EXTI9_5_IRQn] = stub_intr_io_interruption;

	EXTI->FTSR |= (1<<BSWITCH);
	EXTI->IMR  |= (1<<BSWITCH);
}

/*
 * \brief stub_intr_io_interruption
 *
 */
static	void	stub_intr_io_interruption(void) {

	EXTI->PR |= (1<<BSWITCH);
	vCounter++;
}

