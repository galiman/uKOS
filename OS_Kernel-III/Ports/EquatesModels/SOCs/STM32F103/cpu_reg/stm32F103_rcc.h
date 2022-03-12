/*
; stm32F103_rcc.
; ==============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 113                  $:  Revision of last commit
; $Date:: 2017-04-16 16:55:48#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stm32F103_rcc equates.
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

#ifndef __STM32F103_RCC__
#define __STM32F103_RCC__

// RCC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	CFGR;
	volatile	uint32_t	CIR;
	volatile	uint32_t	APB2RSTR;
	volatile	uint32_t	APB1RSTR;
	volatile	uint32_t	AHBENR;
	volatile	uint32_t	APB2ENR;
	volatile	uint32_t	APB1ENR;
	volatile	uint32_t	BDCR;
	volatile	uint32_t	CSR;
} RCC_TypeDef;

#define RCC		((RCC_TypeDef *)RCC_BASE)

// RCC_CR register

#define	RCC_CR_HSION				0x00000001		//
#define	RCC_CR_HSIRDY				0x00000002		//
#define	RCC_CR_HSITRIM				0x000000F8		//
#define	RCC_CR_HSICAL				0x0000FF00		//
#define	RCC_CR_HSEON				0x00010000		//
#define	RCC_CR_HSERDY				0x00020000		//
#define	RCC_CR_HSEBYP				0x00040000		//
#define	RCC_CR_CSSON				0x00080000		//
#define	RCC_CR_PLLON				0x01000000		//
#define	RCC_CR_PLLRDY				0x02000000		//

// RCC_CFGR register

#define	RCC_CFGR_SW					0x00000003		//
#define	RCC_CFGR_SW_0				0x00000001		//
#define	RCC_CFGR_SW_1				0x00000002		//
#define	RCC_CFGR_SW_HSI				0x00000000		//
#define	RCC_CFGR_SW_HSE				0x00000001		//
#define	RCC_CFGR_SW_PLL				0x00000002		//
#define	RCC_CFGR_SWS				0x0000000C		//
#define	RCC_CFGR_SWS_0				0x00000004		//
#define	RCC_CFGR_SWS_1				0x00000008		//
#define	RCC_CFGR_SWS_HSI			0x00000000		//
#define	RCC_CFGR_SWS_HSE			0x00000004		//
#define	RCC_CFGR_SWS_PLL			0x00000008		//
#define	RCC_CFGR_HPRE_DIV1			0x00000000		//
#define	RCC_CFGR_PPRE1_DIV1			0x00000000		//
#define	RCC_CFGR_PPRE1_DIV2			0x00000400		//
#define	RCC_CFGR_ADCPRE_DIV4		0x00004000		//
#define	RCC_CFGR_ADCPRE_DIV6		0x00008000		//
#define	RCC_CFGR_ADCPRE_DIV8		0x0000A000		//
#define	RCC_CFGR_PLLSRC				0x00010000		//
#define	RCC_CFGR_PLLMULL9			0x001C0000		//
#define	RCC_CFGR_SW_PLL				0x00000002		//

// RCC_AHBENR register

#define	RCC_AHBENR_DMA1EN			0x00000001		//
#define	RCC_AHBENR_DMA2EN			0x00000002		//
#define	RCC_AHBENR_SRAMEN			0x00000004		//
#define	RCC_AHBENR_FLITFEN			0x00000010		//
#define	RCC_AHBENR_CRCEN			0x00000040		//
#define	RCC_AHBENR_FSMCEN			0x00000100		//
#define	RCC_AHBENR_SDIOEN			0x00000400		//

// RCC_APB2ENR register

#define	RCC_APB2ENR_AFIOEN			0x0001			//
#define	RCC_APB2ENR_IOPAEN			0x0004			//
#define	RCC_APB2ENR_IOPBEN			0x0008			//
#define	RCC_APB2ENR_IOPCEN			0x0010			//
#define	RCC_APB2ENR_IOPDEN			0x0020			//
#define	RCC_APB2ENR_IOPEEN			0x0040			//
#define	RCC_APB2ENR_IOPFEN			0x0080			//
#define	RCC_APB2ENR_IOPGEN			0x0100			//
#define	RCC_APB2ENR_ADC1EN			0x0200			//
#define	RCC_APB2ENR_ADC2EN			0x0400			//
#define	RCC_APB2ENR_TIM1EN			0x0800			//
#define	RCC_APB2ENR_SPI1EN			0x1000			//
#define	RCC_APB2ENR_USART1EN		0x4000			//
#define	RCC_APB2ENR_TIM8EN			0x2000			//
#define	RCC_APB2ENR_ADC3EN			0x8000			//

// RCC_APB1ENR register

#define	RCC_APB1ENR_TIM2EN			0x00000001		//
#define	RCC_APB1ENR_TIM3EN			0x00000002		//
#define	RCC_APB1ENR_WWDGEN			0x00000800		//
#define	RCC_APB1ENR_USART2EN		0x00020000		//
#define	RCC_APB1ENR_I2C1EN			0x00200000		//
#define	RCC_APB1ENR_CAN1EN			0x02000000		//
#define	RCC_APB1ENR_BKPEN			0x08000000		//
#define	RCC_APB1ENR_PWREN			0x10000000		//
#define	RCC_APB1ENR_TIM4EN			0x00000004		//
#define	RCC_APB1ENR_SPI2EN			0x00004000		//
#define	RCC_APB1ENR_USART3EN		0x00040000		//
#define	RCC_APB1ENR_I2C2EN			0x00400000		//
#define	RCC_APB1ENR_USBEN			0x00800000		//
#define	RCC_APB1ENR_TIM5EN			0x00000008		//
#define	RCC_APB1ENR_TIM6EN			0x00000010		//
#define	RCC_APB1ENR_TIM7EN			0x00000020		//
#define	RCC_APB1ENR_SPI3EN			0x00008000		//
#define	RCC_APB1ENR_UART4EN			0x00080000		//
#define	RCC_APB1ENR_UART5EN			0x00100000		//
#define	RCC_APB1ENR_DACEN			0x20000000		//

// RCC_BDCR register

#define  RCC_BDCR_LSEON				0x00000001		//
#define  RCC_BDCR_LSERDY			0x00000002		//
#define  RCC_BDCR_LSEBYP			0x00000004		//
#define  RCC_BDCR_RTCSEL			0x00000300		//
#define  RCC_BDCR_RTCSEL_0			0x00000100		//
#define  RCC_BDCR_RTCSEL_1			0x00000200		//

#define  RCC_BDCR_RTCSEL_NOCLOCK	0x00000000		//
#define  RCC_BDCR_RTCSEL_LSE    	0x00000100		//
#define  RCC_BDCR_RTCSEL_LSI    	0x00000200		//
#define  RCC_BDCR_RTCSEL_HSE    	0x00000300		//

#define  RCC_BDCR_RTCEN				0x00008000		//
#define  RCC_BDCR_BDRST				0x00010000		//

// RCC_CSR register

#define  RCC_CSR_LSION 				0x00000001		//
#define  RCC_CSR_LSIRDY				0x00000002		//
#define  RCC_CSR_RMVF  				0x01000000		//
#define  RCC_CSR_PINRSTF			0x04000000		//
#define  RCC_CSR_PORRSTF			0x08000000		//
#define  RCC_CSR_SFTRSTF			0x10000000		//
#define  RCC_CSR_IWDGRSTF 			0x20000000		//
#define  RCC_CSR_WWDGRSTF 			0x40000000		//
#define  RCC_CSR_LPWRRSTF 			0x80000000		//
#endif
