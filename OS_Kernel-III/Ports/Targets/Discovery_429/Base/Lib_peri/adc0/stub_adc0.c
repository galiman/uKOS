/*
; stub_adc0.
; ==========

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
; Goal:		stub for the "adc0" manager module.
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

#define	KNBCHANNEL	3
#define	KVALCCR_BAT	(                  ADC_CCR_VBATE | ADC_CCR_ADCPRE_1 | ADC_CCR_ADCPRE_0)
#define	KVALCCR_ALL	(ADC_CCR_TSVREFE |                 ADC_CCR_ADCPRE_1 | ADC_CCR_ADCPRE_0)
#define	KVALCR1		0
#define	KVALCR2		ADC_CR2_ADON

#define KTSAMPLEADC	(0.001)														// For 1-mS
#define	KFPRET4		1000000														// 1'000'000-Hz
#define	KFINTT4		(1/KTSAMPLEADC)												// 1000-Hz
#define KPSCT4		((KFREQUENCY_AHB/KFPRET4)-1)								// Prescaler for 1'000'000-Hz
#define KARRT4		((KFPRET4/KFINTT4)-1)										// Autoreload

#define	KVREF		(3.0)														// Reference of 3.0-V
#define	KDYNAMICS	(1<<12)														// Dynamics of 12-bits

extern				void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);	//
extern	volatile	uint16_t	vTimeException;									//
static	volatile	float64_t	vADCResult[KNBCHANNEL];							// ADC results
static	const		uint32_t	aChannel[KNBCHANNEL] = { 18, 17, 18 };			// The analog input channels to convert

// Prototypes
// ==========

static	void	aTIM4_IRQHandler(void);

/*
 * \brief stub_adc0_init
 *
 * - Initialize the manager
 *
 */
void	stub_adc0_init(void) {

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

// Initialize the ADC1

	ADC->CCR	= KVALCCR_ALL;
	ADC1->CR1   = KVALCR1;
	ADC1->CR2   = KVALCR2;
	ADC1->SMPR1 = ((4<<24) | (4<<21) | (4<<18));
	ADC1->SQR3  = aChannel[0];
	ADC1->SR    = 0;
	ADC1->CR2  |= ADC_CR2_SWSTART;

// Initialize the TIM4 for the 1000-Hz sampling rate

	vExce_indIntVectors[TIM4_IRQn] = aTIM4_IRQHandler;

	NVIC->IP[TIM4_IRQn] = (KINT_LEVEL_TIMERS<<4);
	NVIC->ISER[0] |= (1<<(TIM4_IRQn & 0x1F));

	TIM4->PSC  = KPSCT4;
	TIM4->ARR  = KARRT4;
	TIM4->DIER = TIM_DIER_UIE;
    TIM4->CR1 |= TIM_CR1_CEN;
}

/*
 * \brief stub_adc0_getAnalog
 *
 * - Read the ADC channel
 *
 */
int32_t	stub_adc0_getAnalog(uint8_t channel, float64_t *reference, float64_t *data) {

	if (channel >= KNBCHANNEL) {
		return (KADC0NODEV);
	}

	*reference = KVREF;
	*data      = vADCResult[channel];
	return (KADC0NOERR);
}

// Local routines
// ==============

/*
 * \brief aTIM4_IRQHandler
 *
 * - ADC sampling interruption
 * - Save the previous conversion
 * - Start the next one
 *
 */
static	void	aTIM4_IRQHandler(void) {
			uint16_t	result;
	static	uint8_t		channel = 0;

 // INT acknowledge

    if (TIM4->SR &   TIM_SR_UIF) {
		TIM4->SR &= ~TIM_SR_UIF;
	}

// Save the previous conversion

    result = ADC1->DR;

	vADCResult[channel++] = ((float64_t)result * KVREF) / (float64_t)KDYNAMICS;
	if (channel >= KNBCHANNEL) {
		channel = 0;
	}

// Start the next conversion

	ADC->CCR = (channel == 2) ? (KVALCCR_BAT) : (KVALCCR_ALL);

	ADC1->SQR3 = aChannel[channel];
	ADC1->SR   = 0;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

