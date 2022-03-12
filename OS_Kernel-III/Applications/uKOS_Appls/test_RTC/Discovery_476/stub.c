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
#include 	"../calendar.h"

extern	void	(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);

// Prototypes
// ==========

static	void	aRTC_IRQHandler(void);

/*
 * \brief stub_setRTC
 *
 */
void	stub_setRTC(void) {

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;

// Make possible the access to the RTC & RTC backup register.
// After the restart, the RTC is in the protected mode
// Select the 32768-Hz clock & enable the RTC

	PWR->CR1  |= PWR_CR1_DBP;

	kern_suspendProcess(10);

	RCC->BDCR = RCC_BDCR_RTCEN
			  | RCC_BDCR_LSEON
			  | RCC_BDCR_RTCSEL_0;
	while (!(RCC->BDCR & RCC_BDCR_LSERDY));

// Remove the write protection
// Enter in the initialization mode

	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

// Waiting for ready to write
// Set the new counter values

	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));

	RTC->PRER = 0x007F00FF;

	RTC->TR = (K24H<<22)
			| ((KHOURS/16)<<20)
			| ((KHOURS&0xF)<<16)
			| ((KMINUTES/16)<<12)
			| ((KMINUTES&0xF)<<8)
			| ((KSECONDS/16)<<4)
			| ((KSECONDS&0xF)<<0);

	RTC->DR = ((KYEARS/16)<<20)
			| ((KYEARS&0xF)<<16)
			| (KASCIIWEEK<<13)
			| ((KMONTHS/16)<<12)
			| ((KMONTHS&0xF)<<8)
			| ((KDAYS/16)<<4)
			| ((KDAYS&0xF)<<0);

// Exit from the initialization mode
// Set the write protection

	RTC->ISR &= ~RTC_ISR_INIT;
	RTC->WPR = 0xFF;
}

/*
 * \brief stub_setPeriodicInt
 *
 */
void	stub_setPeriodicInt(void) {

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;

	vExce_indIntVectors[RTC_WKUP_IRQn] = aRTC_IRQHandler;

	NVIC->IP[RTC_WKUP_IRQn] = (KINT_LEVEL_TIMERS<<4);
	NVIC->ISER[0] |= (1<<(RTC_WKUP_IRQn & 0x1F));

// Make possible the access to the RTC & RTC backup register.
// After the restart, the RTC is in the protected mode
// Select the 32768-Hz clock & enable the RTC

	PWR->CR1  |= PWR_CR1_DBP;

// Remove the write protection
// Enter in the initialization mode

	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

// Waiting for ready to write
// RTC clock / 16 -> 488-us

	RTC->CR  &= ~RTC_CR_WUTE;
	while (!(RTC->ISR & RTC_ISR_WUTWF));

	RTC->WUTR = (10000000/488);
	RTC->ISR &= ~RTC_ISR_WUTF;
	RTC->CR  |=  RTC_CR_WUTE
			  |  RTC_CR_WUTIE;

// RTC interruption is provided via the EXTI20

	EXTI->IMR1  |= (1<<20);
	EXTI->RTSR1 |= (1<<20);

// Exit from the initialization mode
// Set the write protection

	RTC->WPR = 0xFF;
}

/*
 * \brief aRTC_IRQHandler
 *
 * - Blink the LED0 every 10-s
 *
 */
static	void	aRTC_IRQHandler(void) {

	RTC->ISR  &= ~RTC_ISR_WUTF;
	EXTI->PR1 |= (1<<22);
	misc_toggleLed(0);
}

