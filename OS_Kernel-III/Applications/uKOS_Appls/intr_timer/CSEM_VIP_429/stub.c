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
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
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

#define KTTIM1MS		(0.001)									// For 1-mS (1000-Hz)
#define	KFPRET5			1000000									// 1'000'000-Hz
#define	KFINTT5			(1/KTTIM1MS)							// 1'000-Hz
#define KPSCT5			((KFREQUENCY_APB2/KFPRET5)-1)			// Prescaler for 1'000'000-Hz
#define KARRT5			((KFPRET5/KFINTT5)-1)					// Autoreload		

extern				void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
static				uint32_t 	vEventTimer;
extern	volatile	uint32_t 	vTimer;

// Prototypes
// ==========

static	void	stub_intr_timer_interruption(void);

/*
 * \brief stub_intr_timer_init
 *
 */
void	stub_intr_timer_init(void) {
	volatile	evnt_t	*event;

	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

// Initialise the event timer

	if (kern_createEvent("Timer", &event)     != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_getBitEvent(event, &vEventTimer) != KKERNNOERR) { exit(EXIT_FAILURE); }

// Timer 5 (1-ms)

	vExce_indIntVectors[TIM5_IRQn] = stub_intr_timer_interruption;

	NVIC->IP[TIM5_IRQn] = (KINT_LEVEL_TIMERS<<4);
	NVIC->ISER[1] |= (1<<(TIM5_IRQn & 0x1F));

	TIM5->PSC  = KPSCT5;
	TIM5->ARR  = KARRT5;
	TIM5->DIER = TIM_DIER_UIE;
    TIM5->CR1 |= TIM_CR1_CEN;
}

/*
 * \brief stub_intr_timer_interruption
 *
 */
static	void	stub_intr_timer_interruption(void) {

// INT acknowledge

	if ((TIM5->SR & TIM_SR_UIF) != 0) {
		TIM5->SR &= ~TIM_SR_UIF;
	}
	vTimer++;

// Generate the event "Timer"
// !!! NEVER USE the mode KEVNTSWTC inside an interruption routine !!!

	kern_signalEvent(vEventTimer, KEVNTCONT);
}

