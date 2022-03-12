/*
; stm32F479_rcc.
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
; Goal:		stm32F479_rcc equates.
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

#ifndef __STM32F479_RCC__
#define __STM32F479_RCC__

// RCC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	PLLCFGR;
	volatile	uint32_t	CFGR;
	volatile	uint32_t	CIR;
	volatile	uint32_t	AHB1RSTR;
	volatile	uint32_t	AHB2RSTR;
	volatile	uint32_t	AHB3RSTR;
	volatile	uint32_t	RESERVED0;
	volatile	uint32_t	APB1RSTR;
	volatile	uint32_t	APB2RSTR;
	volatile	uint32_t	RESERVED1[2];
	volatile	uint32_t	AHB1ENR;
	volatile	uint32_t	AHB2ENR;
	volatile	uint32_t	AHB3ENR;
	volatile	uint32_t	RESERVED2;
	volatile	uint32_t	APB1ENR;
	volatile	uint32_t	APB2ENR;
	volatile	uint32_t	RESERVED3[2];
	volatile	uint32_t	AHB1LPENR;
	volatile	uint32_t	AHB2LPENR;
	volatile	uint32_t	AHB3LPENR;
	volatile	uint32_t	RESERVED4;
	volatile	uint32_t	APB1LPENR;
	volatile	uint32_t	APB2LPENR;
	volatile	uint32_t	RESERVED5[2];
	volatile	uint32_t	BDCR;
	volatile	uint32_t	CSR;
	volatile	uint32_t	RESERVED6[2];
	volatile	uint32_t	SSCGR;
	volatile	uint32_t	PLLI2SCFGR;
	volatile	uint32_t	PLLSAICFGR;
	volatile	uint32_t	DCKCFGR;
} RCC_TypeDef;

#define RCC		((RCC_TypeDef *)RCC_BASE)

#define RCC_CR_HSEON_bb		bitband_t	BB_PERIPH(&RCC->CR, RCC_CR_HSEON_bit)
#define RCC_CR_HSERDY_bb	bitband_t	BB_PERIPH(&RCC->CR, RCC_CR_HSERDY_bit)
#define RCC_CR_PLLON_bb		bitband_t	BB_PERIPH(&RCC->CR, RCC_CR_PLLON_bit)
#define RCC_CR_PLLRDY_bb	bitband_t	BB_PERIPH(&RCC->CR, RCC_CR_PLLRDY_bit)

// RCC_CR register

#define	RCC_CR_HSION				0x00000001	//
#define	RCC_CR_HSIRDY				0x00000002	//
#define	RCC_CR_HSITRIM				0x000000F8	//
#define	RCC_CR_HSITRIM_0			0x00000008	//
#define	RCC_CR_HSITRIM_1			0x00000010	//
#define	RCC_CR_HSITRIM_2			0x00000020	//
#define	RCC_CR_HSITRIM_3			0x00000040	//
#define	RCC_CR_HSITRIM_4			0x00000080	//
#define	RCC_CR_HSICAL				0x0000FF00	//
#define	RCC_CR_HSICAL_0 			0x00000100	//
#define	RCC_CR_HSICAL_1 			0x00000200	//
#define	RCC_CR_HSICAL_2 			0x00000400	//
#define	RCC_CR_HSICAL_3 			0x00000800	//
#define	RCC_CR_HSICAL_4 			0x00001000	//
#define	RCC_CR_HSICAL_5 			0x00002000	//
#define	RCC_CR_HSICAL_6 			0x00004000	//
#define	RCC_CR_HSICAL_7 			0x00008000	//
#define	RCC_CR_HSEON				0x00010000	//
#define	RCC_CR_HSERDY				0x00020000	//
#define	RCC_CR_HSEBYP				0x00040000	//
#define	RCC_CR_CSSON				0x00080000	//
#define	RCC_CR_PLLON				0x01000000	//
#define	RCC_CR_PLLRDY				0x02000000	//
#define	RCC_CR_PLLI2SON 			0x04000000	//
#define	RCC_CR_PLLI2SRDY			0x08000000	//
#define	RCC_CR_PLLSAION				0x10000000	//
#define	RCC_CR_PLLSAIRDY			0x20000000	//

#define RCC_CR_HSEON_bit			16			//
#define RCC_CR_HSERDY_bit			17			//
#define RCC_CR_PLLON_bit			24			//
#define RCC_CR_PLLRDY_bit			25			//

// RCC_PLLCFGR register

#define	RCC_PLLCFGR_PLLM			0x0000003F	//
#define	RCC_PLLCFGR_PLLM_0			0x00000001	//
#define	RCC_PLLCFGR_PLLM_1			0x00000002	//
#define	RCC_PLLCFGR_PLLM_2			0x00000004	//
#define	RCC_PLLCFGR_PLLM_3			0x00000008	//
#define	RCC_PLLCFGR_PLLM_4			0x00000010	//
#define	RCC_PLLCFGR_PLLM_5			0x00000020	//
#define	RCC_PLLCFGR_PLLN 			0x00007FC0	//
#define	RCC_PLLCFGR_PLLN_0 			0x00000040	//
#define	RCC_PLLCFGR_PLLN_1 			0x00000080	//
#define	RCC_PLLCFGR_PLLN_2 			0x00000100	//
#define	RCC_PLLCFGR_PLLN_3 			0x00000200	//
#define	RCC_PLLCFGR_PLLN_4 			0x00000400	//
#define	RCC_PLLCFGR_PLLN_5 			0x00000800	//
#define	RCC_PLLCFGR_PLLN_6 			0x00001000	//
#define	RCC_PLLCFGR_PLLN_7 			0x00002000	//
#define	RCC_PLLCFGR_PLLN_8 			0x00004000	//
#define	RCC_PLLCFGR_PLLP			0x00030000	//
#define	RCC_PLLCFGR_PLLP_0			0x00010000	//
#define	RCC_PLLCFGR_PLLP_1			0x00020000	//
#define	RCC_PLLCFGR_PLLSRC			0x00400000	//
#define	RCC_PLLCFGR_PLLSRC_HSE		0x00400000	//
#define	RCC_PLLCFGR_PLLSRC_HSI		0x00000000	//
#define	RCC_PLLCFGR_PLLQ			0x0F000000	//
#define	RCC_PLLCFGR_PLLQ_0			0x01000000	//
#define	RCC_PLLCFGR_PLLQ_1			0x02000000	//
#define	RCC_PLLCFGR_PLLQ_2			0x04000000	//
#define	RCC_PLLCFGR_PLLQ_3			0x08000000	//
#define	RCC_PLLCFGR_PLLR			0x70000000	//
#define	RCC_PLLCFGR_PLLR_0			0x10000000	//
#define	RCC_PLLCFGR_PLLR_1			0x20000000	//
#define	RCC_PLLCFGR_PLLR_2			0x40000000	//

#define RCC_PLLCFGR_PLLM_bit		0			//
#define RCC_PLLCFGR_PLLN_bit		6			//
#define RCC_PLLCFGR_PLLP_bit		16			//
#define RCC_PLLCFGR_PLLQ_bit		24			//
#define RCC_PLLCFGR_PLLQ_DIV9		(9<<24)		//

// RCC_CFGR register

#define	RCC_CFGR_SW 				0x00000003	//
#define	RCC_CFGR_SW_0				0x00000001	//
#define	RCC_CFGR_SW_1				0x00000002	//
#define	RCC_CFGR_SW_HSI 			0x00000000	//
#define	RCC_CFGR_SW_HSE 			0x00000001	//
#define	RCC_CFGR_SW_PLL 			0x00000002	//
#define	RCC_CFGR_SWS				0x0000000C	//
#define	RCC_CFGR_SWS_0				0x00000004	//
#define	RCC_CFGR_SWS_1				0x00000008	//
#define	RCC_CFGR_SWS_HSI			0x00000000	//
#define	RCC_CFGR_SWS_HSE			0x00000004	//
#define	RCC_CFGR_SWS_PLL			0x00000008	//
#define	RCC_CFGR_HPRE				0x000000F0	//
#define	RCC_CFGR_HPRE_0 			0x00000010	//
#define	RCC_CFGR_HPRE_1 			0x00000020	//
#define	RCC_CFGR_HPRE_2 			0x00000040	//
#define	RCC_CFGR_HPRE_3 			0x00000080	//
#define	RCC_CFGR_HPRE_DIV1			0x00000000	//
#define	RCC_CFGR_HPRE_DIV2			0x00000080	//
#define	RCC_CFGR_HPRE_DIV4			0x00000090	//
#define	RCC_CFGR_HPRE_DIV8			0x000000A0	//
#define	RCC_CFGR_HPRE_DIV16   		0x000000B0	//
#define	RCC_CFGR_HPRE_DIV64   		0x000000C0	//
#define	RCC_CFGR_HPRE_DIV128  		0x000000D0	//
#define	RCC_CFGR_HPRE_DIV256  		0x000000E0	//
#define	RCC_CFGR_HPRE_DIV512  		0x000000F0	//
#define	RCC_CFGR_PPRE1				0x00001C00	//
#define	RCC_CFGR_PPRE1_0			0x00000400	//
#define	RCC_CFGR_PPRE1_1			0x00000800	//
#define	RCC_CFGR_PPRE1_2			0x00001000	//
#define	RCC_CFGR_PPRE1_DIV1   		0x00000000	//
#define	RCC_CFGR_PPRE1_DIV2   		0x00001000	//
#define	RCC_CFGR_PPRE1_DIV4   		0x00001400	//
#define	RCC_CFGR_PPRE1_DIV8   		0x00001800	//
#define	RCC_CFGR_PPRE1_DIV16  		0x00001C00	//
#define	RCC_CFGR_PPRE2				0x0000E000	//
#define	RCC_CFGR_PPRE2_0			0x00002000	//
#define	RCC_CFGR_PPRE2_1			0x00004000	//
#define	RCC_CFGR_PPRE2_2			0x00008000	//
#define	RCC_CFGR_PPRE2_DIV1   		0x00000000	//
#define	RCC_CFGR_PPRE2_DIV2   		0x00008000	//
#define	RCC_CFGR_PPRE2_DIV4   		0x0000A000	//
#define	RCC_CFGR_PPRE2_DIV8   		0x0000C000	//
#define	RCC_CFGR_PPRE2_DIV16  		0x0000E000	//
#define	RCC_CFGR_RTCPRE 			0x001F0000	//
#define	RCC_CFGR_RTCPRE_0 			0x00010000	//
#define	RCC_CFGR_RTCPRE_1 			0x00020000	//
#define	RCC_CFGR_RTCPRE_2 			0x00040000	//
#define	RCC_CFGR_RTCPRE_3 			0x00080000	//
#define	RCC_CFGR_RTCPRE_4 			0x00100000	//
#define	RCC_CFGR_MCO1				0x00600000	//
#define	RCC_CFGR_MCO1_0 			0x00200000	//
#define	RCC_CFGR_MCO1_1 			0x00400000	//
#define	RCC_CFGR_I2SSRC 			0x00800000	//
#define	RCC_CFGR_MCO1PRE			0x07000000	//
#define	RCC_CFGR_MCO1PRE_0			0x01000000	//
#define	RCC_CFGR_MCO1PRE_1			0x02000000	//
#define	RCC_CFGR_MCO1PRE_2			0x04000000	//
#define	RCC_CFGR_MCO2PRE			0x38000000	//
#define	RCC_CFGR_MCO2PRE_0			0x08000000	//
#define	RCC_CFGR_MCO2PRE_1			0x10000000	//
#define	RCC_CFGR_MCO2PRE_2			0x20000000	//
#define	RCC_CFGR_MCO2				0xC0000000	//
#define	RCC_CFGR_MCO2_0 			0x40000000	//
#define	RCC_CFGR_MCO2_1 			0x80000000	//

// RCC_CIR register

#define	RCC_CIR_LSIRDYF 			0x00000001	//
#define	RCC_CIR_LSERDYF 			0x00000002	//
#define	RCC_CIR_HSIRDYF 			0x00000004	//
#define	RCC_CIR_HSERDYF 			0x00000008	//
#define	RCC_CIR_PLLRDYF 			0x00000010	//
#define	RCC_CIR_PLLI2SRDYF			0x00000020	//
#define	RCC_CIR_PLLSAIRDYF			0x00000040	//
#define	RCC_CIR_CSSF				0x00000080	//
#define	RCC_CIR_LSIRDYIE			0x00000100	//
#define	RCC_CIR_LSERDYIE			0x00000200	//
#define	RCC_CIR_HSIRDYIE			0x00000400	//
#define	RCC_CIR_HSERDYIE			0x00000800	//
#define	RCC_CIR_PLLRDYIE			0x00001000	//
#define	RCC_CIR_PLLI2SRDYIE   		0x00002000	//
#define	RCC_CIR_PLLSAIRDYIE   		0x00004000	//
#define	RCC_CIR_LSIRDYC 			0x00010000	//
#define	RCC_CIR_LSERDYC 			0x00020000	//
#define	RCC_CIR_HSIRDYC 			0x00040000	//
#define	RCC_CIR_HSERDYC 			0x00080000	//
#define	RCC_CIR_PLLRDYC 			0x00100000	//
#define	RCC_CIR_PLLI2SRDYC			0x00200000	//
#define	RCC_CIR_PLLSAIRDYC			0x00400000	//
#define	RCC_CIR_CSSC				0x00800000	//

// RCC_AHB1RSTR register

#define	RCC_AHB1RSTR_GPIOARST 		0x00000001	//
#define	RCC_AHB1RSTR_GPIOBRST 		0x00000002	//
#define	RCC_AHB1RSTR_GPIOCRST 		0x00000004	//
#define	RCC_AHB1RSTR_GPIODRST 		0x00000008	//
#define	RCC_AHB1RSTR_GPIOERST 		0x00000010	//
#define	RCC_AHB1RSTR_GPIOFRST 		0x00000020	//
#define	RCC_AHB1RSTR_GPIOGRST 		0x00000040	//
#define	RCC_AHB1RSTR_GPIOHRST 		0x00000080	//
#define	RCC_AHB1RSTR_GPIOIRST 		0x00000100	//
#define	RCC_AHB1RSTR_GPIOJRST 		0x00000200	//
#define	RCC_AHB1RSTR_GPIOKRST 		0x00000400	//
#define	RCC_AHB1RSTR_CRCRST   		0x00001000	//
#define	RCC_AHB1RSTR_DMA1RST  		0x00200000	//
#define	RCC_AHB1RSTR_DMA2RST  		0x00400000	//
#define	RCC_AHB1RSTR_DMA2RST  		0x00400000	//
#define	RCC_AHB1RSTR_DMA2DRST		0x00800000	//
#define	RCC_AHB1RSTR_ETHMACRST		0x02000000	//
#define	RCC_AHB1RSTR_OTGHRST  		0x10000000	//

// RCC_AHB2RSTR register

#define	RCC_AHB2RSTR_DCMIRST  		0x00000001	//
#define	RCC_AHB2RSTR_CRYPRST  		0x00000010	//
#define	RCC_AHB2RSTR_HSAHRST  		0x00000020	//
#define	RCC_AHB2RSTR_RNGRST   		0x00000040	//
#define	RCC_AHB2RSTR_OTGFSRST 		0x00000080	//

// RCC_AHB3RSTR register

#define	RCC_AHB3RSTR_FSMCRST  		0x00000001	//
#define	RCC_AHB3RSTR_QSPIRST		0x00000002	//

// RCC_APB1RSTR register

#define	RCC_APB1RSTR_TIM2RST  		0x00000001	//
#define	RCC_APB1RSTR_TIM3RST  		0x00000002	//
#define	RCC_APB1RSTR_TIM4RST  		0x00000004	//
#define	RCC_APB1RSTR_TIM5RST  		0x00000008	//
#define	RCC_APB1RSTR_TIM6RST  		0x00000010	//
#define	RCC_APB1RSTR_TIM7RST  		0x00000020	//
#define	RCC_APB1RSTR_TIM12RST 		0x00000040	//
#define	RCC_APB1RSTR_TIM13RST 		0x00000080	//
#define	RCC_APB1RSTR_TIM14RST 		0x00000100	//
#define	RCC_APB1RSTR_WWDGEN   		0x00000800	//
#define	RCC_APB1RSTR_SPI2RST  		0x00008000	//
#define	RCC_APB1RSTR_SPI3RST  		0x00010000	//
#define	RCC_APB1RSTR_USART2RST		0x00020000	//
#define	RCC_APB1RSTR_USART3RST		0x00040000	//
#define	RCC_APB1RSTR_UART4RST 		0x00080000	//
#define	RCC_APB1RSTR_UART5RST 		0x00100000	//
#define	RCC_APB1RSTR_I2C1RST  		0x00200000	//
#define	RCC_APB1RSTR_I2C2RST  		0x00400000	//
#define	RCC_APB1RSTR_I2C3RST  		0x00800000	//
#define	RCC_APB1RSTR_CAN1RST  		0x02000000	//
#define	RCC_APB1RSTR_CAN2RST  		0x04000000	//
#define	RCC_APB1RSTR_PWRRST   		0x10000000	//
#define	RCC_APB1RSTR_DACRST   		0x20000000	//
#define	RCC_APB1RSTR_UART7RST 		0x40000000	//
#define	RCC_APB1RSTR_UART8RST 		0x80000000	//

// RCC_APB2RSTR register

#define	RCC_APB2RSTR_TIM1RST  		0x00000001	//
#define	RCC_APB2RSTR_TIM8RST  		0x00000002	//
#define	RCC_APB2RSTR_USART1RST		0x00000010	//
#define	RCC_APB2RSTR_USART6RST		0x00000020	//
#define	RCC_APB2RSTR_ADCRST   		0x00000100	//
#define	RCC_APB2RSTR_SDIORST  		0x00000800	//
#define	RCC_APB2RSTR_SPI1RST  		0x00001000	//
#define	RCC_APB2RSTR_SPI4RST		0x00002000	//
#define	RCC_APB2RSTR_SYSCFGRST		0x00004000	//
#define	RCC_APB2RSTR_TIM9RST  		0x00010000	//
#define	RCC_APB2RSTR_TIM10RST 		0x00020000	//
#define	RCC_APB2RSTR_TIM11RST 		0x00040000	//
#define	RCC_APB2RSTR_SPI5RST		0x00100000	//
#define	RCC_APB2RSTR_SPI6RST  		0x00200000	//
#define	RCC_APB2RSTR_SAI1RST 		0x00400000	//
#define	RCC_APB2RSTR_LTDCRST 		0x04000000	//
#define	RCC_APB2RSTR_DSIRST			0x08000000	//

// RCC_AHB1ENR register

#define	RCC_AHB1ENR_GPIOAEN   		0x00000001	//
#define	RCC_AHB1ENR_GPIOBEN   		0x00000002	//
#define	RCC_AHB1ENR_GPIOCEN   		0x00000004	//
#define	RCC_AHB1ENR_GPIODEN   		0x00000008	//
#define	RCC_AHB1ENR_GPIOEEN   		0x00000010	//
#define	RCC_AHB1ENR_GPIOFEN   		0x00000020	//
#define	RCC_AHB1ENR_GPIOGEN   		0x00000040	//
#define	RCC_AHB1ENR_GPIOHEN   		0x00000080	//
#define	RCC_AHB1ENR_GPIOIEN   		0x00000100	//
#define	RCC_AHB1ENR_GPIOJEN   		0x00000200	//
#define	RCC_AHB1ENR_GPIOKEN   		0x00000400	//
#define	RCC_AHB1ENR_CRCEN 			0x00001000	//
#define	RCC_AHB1ENR_BKPSRAMEN 		0x00040000	//
#define	RCC_AHB1ENR_CCMDATARAMEN	0x00100000	//
#define	RCC_AHB1ENR_DMA1EN			0x00200000	//
#define	RCC_AHB1ENR_DMA2EN			0x00400000	//
#define	RCC_AHB1ENR_DMA2DEN			0x00800000	//
#define	RCC_AHB1ENR_ETHMACEN  		0x02000000	//
#define	RCC_AHB1ENR_ETHMACTXEN		0x04000000	//
#define	RCC_AHB1ENR_ETHMACRXEN		0x08000000	//
#define	RCC_AHB1ENR_ETHMACPTPEN		0x10000000	//
#define	RCC_AHB1ENR_OTGHSEN   		0x20000000	//
#define	RCC_AHB1ENR_OTGHSULPIEN		0x40000000	//

// RCC_AHB2ENR register

#define	RCC_AHB2ENR_DCMIEN			0x00000001	//
#define	RCC_AHB2ENR_CRYPEN			0x00000010	//
#define	RCC_AHB2ENR_HASHEN			0x00000020	//
#define	RCC_AHB2ENR_RNGEN 			0x00000040	//
#define	RCC_AHB2ENR_OTGFSEN   		0x00000080	//

// RCC_AHB3ENR register

#define	RCC_AHB3ENR_FSMCEN			0x00000001	//
#define	RCC_AHB3ENR_QSPIEN			0x00000002	//

// RCC_APB1ENR register

#define	RCC_APB1ENR_TIM2EN			0x00000001	//
#define	RCC_APB1ENR_TIM3EN			0x00000002	//
#define	RCC_APB1ENR_TIM4EN			0x00000004	//
#define	RCC_APB1ENR_TIM5EN			0x00000008	//
#define	RCC_APB1ENR_TIM6EN			0x00000010	//
#define	RCC_APB1ENR_TIM7EN			0x00000020	//
#define	RCC_APB1ENR_TIM12EN   		0x00000040	//
#define	RCC_APB1ENR_TIM13EN   		0x00000080	//
#define	RCC_APB1ENR_TIM14EN   		0x00000100	//
#define	RCC_APB1ENR_WWDGEN			0x00000800	//
#define	RCC_APB1ENR_SPI2EN			0x00004000	//
#define	RCC_APB1ENR_SPI3EN			0x00008000	//
#define	RCC_APB1ENR_USART2EN  		0x00020000	//
#define	RCC_APB1ENR_USART3EN  		0x00040000	//
#define	RCC_APB1ENR_UART4EN   		0x00080000	//
#define	RCC_APB1ENR_UART5EN   		0x00100000	//
#define	RCC_APB1ENR_I2C1EN			0x00200000	//
#define	RCC_APB1ENR_I2C2EN			0x00400000	//
#define	RCC_APB1ENR_I2C3EN			0x00800000	//
#define	RCC_APB1ENR_CAN1EN			0x02000000	//
#define	RCC_APB1ENR_CAN2EN			0x04000000	//
#define	RCC_APB1ENR_PWREN 			0x10000000	//
#define	RCC_APB1ENR_DACEN 			0x20000000	//
#define	RCC_APB1ENR_UART7EN 		0x40000000	//
#define	RCC_APB1ENR_UART8EN 		0x80000000	//

// RCC_APB2ENR register

#define	RCC_APB2ENR_TIM1EN			0x00000001	//
#define	RCC_APB2ENR_TIM8EN			0x00000002	//
#define	RCC_APB2ENR_USART1EN  		0x00000010	//
#define	RCC_APB2ENR_USART6EN  		0x00000020	//
#define	RCC_APB2ENR_ADC1EN			0x00000100	//
#define	RCC_APB2ENR_ADC2EN			0x00000200	//
#define	RCC_APB2ENR_ADC3EN			0x00000400	//
#define	RCC_APB2ENR_SDIOEN			0x00000800	//
#define	RCC_APB2ENR_SPI1EN			0x00001000	//
#define	RCC_APB2ENR_SPI4EN			0x00002000	//
#define	RCC_APB2ENR_SYSCFGEN  		0x00004000	//
#define	RCC_APB2ENR_TIM9EN			0x00010000	//
#define	RCC_APB2ENR_TIM10EN   		0x00020000	//
#define	RCC_APB2ENR_TIM11EN   		0x00040000	//
#define	RCC_APB2ENR_SPI5EN   		0x00100000	//
#define	RCC_APB2ENR_SPI6EN   		0x00200000	//
#define	RCC_APB2ENR_SAI1EN   		0x00400000	//
#define	RCC_APB2ENR_LTDCEN   		0x04000000	//
#define	RCC_APB2ENR_DSIEN			0x08000000	//

// RCC_AHB1LPENR register

#define	RCC_AHB1LPENR_GPIOALPEN		0x00000001	//
#define	RCC_AHB1LPENR_GPIOBLPEN		0x00000002	//
#define	RCC_AHB1LPENR_GPIOCLPEN		0x00000004	//
#define	RCC_AHB1LPENR_GPIODLPEN		0x00000008	//
#define	RCC_AHB1LPENR_GPIOELPEN		0x00000010	//
#define	RCC_AHB1LPENR_GPIOFLPEN		0x00000020	//
#define	RCC_AHB1LPENR_GPIOGLPEN		0x00000040	//
#define	RCC_AHB1LPENR_GPIOHLPEN		0x00000080	//
#define	RCC_AHB1LPENR_GPIOILPEN		0x00000100	//
#define	RCC_AHB1LPENR_GPIOJLPEN		0x00000200	//
#define	RCC_AHB1LPENR_GPIOKLPEN		0x00000400	//
#define	RCC_AHB1LPENR_CRCLPEN 		0x00001000	//
#define	RCC_AHB1LPENR_FLITFLPEN		0x00008000	//
#define	RCC_AHB1LPENR_SRAM1LPEN		0x00010000	//
#define	RCC_AHB1LPENR_SRAM2LPEN		0x00020000	//
#define	RCC_AHB1LPENR_BKPSRAMLPEN	0x00040000	//
#define	RCC_AHB1LPENR_DMA1LPEN		0x00200000	//
#define	RCC_AHB1LPENR_DMA2LPEN		0x00400000	//
#define	RCC_AHB1LPENR_ETHMACLPEN	0x02000000	//
#define	RCC_AHB1LPENR_ETHMACTXLPEN	0x04000000	//
#define	RCC_AHB1LPENR_ETHMACRXLPEN	0x08000000	//
#define	RCC_AHB1LPENR_ETHMACPTPLPEN	0x10000000	//
#define	RCC_AHB1LPENR_OTGHSLPEN		0x20000000	//
#define	RCC_AHB1LPENR_OTGHSULPILPEN	0x40000000	//

// RCC_AHB2LPENR register

#define	RCC_AHB2LPENR_DCMILPEN		0x00000001	//
#define	RCC_AHB2LPENR_CRYPLPEN		0x00000010	//
#define	RCC_AHB2LPENR_HASHLPEN		0x00000020	//
#define	RCC_AHB2LPENR_RNGLPEN 		0x00000040	//
#define	RCC_AHB2LPENR_OTGFSLPEN		0x00000080	//

// RCC_AHB3LPENR register

#define	RCC_AHB3LPENR_FSMCLPEN		0x00000001	//
#define	RCC_AHB3LPENR_QSPILPEN		0x00000002	//

// RCC_APB1LPENR register

#define	RCC_APB1LPENR_TIM2LPEN		0x00000001	//
#define	RCC_APB1LPENR_TIM3LPEN		0x00000002	//
#define	RCC_APB1LPENR_TIM4LPEN		0x00000004	//
#define	RCC_APB1LPENR_TIM5LPEN		0x00000008	//
#define	RCC_APB1LPENR_TIM6LPEN		0x00000010	//
#define	RCC_APB1LPENR_TIM7LPEN		0x00000020	//
#define	RCC_APB1LPENR_TIM12LPEN		0x00000040	//
#define	RCC_APB1LPENR_TIM13LPEN		0x00000080	//
#define	RCC_APB1LPENR_TIM14LPEN		0x00000100	//
#define	RCC_APB1LPENR_WWDGLPEN		0x00000800	//
#define	RCC_APB1LPENR_SPI2LPEN		0x00004000	//
#define	RCC_APB1LPENR_SPI3LPEN		0x00008000	//
#define	RCC_APB1LPENR_USART2LPEN	0x00020000	//
#define	RCC_APB1LPENR_USART3LPEN	0x00040000	//
#define	RCC_APB1LPENR_UART4LPEN		0x00080000	//
#define	RCC_APB1LPENR_UART5LPEN		0x00100000	//
#define	RCC_APB1LPENR_I2C1LPEN		0x00200000	//
#define	RCC_APB1LPENR_I2C2LPEN		0x00400000	//
#define	RCC_APB1LPENR_I2C3LPEN		0x00800000	//
#define	RCC_APB1LPENR_CAN1LPEN		0x02000000	//
#define	RCC_APB1LPENR_CAN2LPEN		0x04000000	//
#define	RCC_APB1LPENR_PWRLPEN 		0x10000000	//
#define	RCC_APB1LPENR_DACLPEN 		0x20000000	//
#define	RCC_APB1LPENR_UART7LPEN		0x40000000	//
#define	RCC_APB1LPENR_UART8LPEN		0x80000000	//

// RCC_APB2LPENR register

#define	RCC_APB2LPENR_TIM1LPEN		0x00000001	//
#define	RCC_APB2LPENR_TIM8LPEN		0x00000002	//
#define	RCC_APB2LPENR_USART1LPEN	0x00000010	//
#define	RCC_APB2LPENR_USART6LPEN	0x00000020	//
#define	RCC_APB2LPENR_ADC1LPEN		0x00000100	//
#define	RCC_APB2LPENR_ADC2PEN 		0x00000200	//
#define	RCC_APB2LPENR_ADC3LPEN		0x00000400	//
#define	RCC_APB2LPENR_SDIOLPEN		0x00000800	//
#define	RCC_APB2LPENR_SPI1LPEN		0x00001000	//
#define	RCC_APB2LPENR_SYSCFGLPEN	0x00004000	//
#define	RCC_APB2LPENR_TIM9LPEN		0x00010000	//
#define	RCC_APB2LPENR_TIM10LPEN		0x00020000	//
#define	RCC_APB2LPENR_TIM11LPEN		0x00040000	//
#define	RCC_APB2LPENR_SPI5LPEN		0x00100000	//
#define	RCC_APB2LPENR_SPI6LPEN		0x00200000	//
#define	RCC_APB2LPENR_SAI1LPEN		0x00400000	//
#define	RCC_APB2LPENR_LTDCLPEN		0x04000000	//
#define	RCC_APB2LPENR_DSILPEN		0x08000000	//

// RCC_BDCR register

#define	RCC_BDCR_LSEON				0x00000001	//
#define	RCC_BDCR_LSERDY 			0x00000002	//
#define	RCC_BDCR_LSEBYP 			0x00000004	//
#define	RCC_BDCR_RTCSEL				0x00000300	//
#define	RCC_BDCR_RTCSEL_0			0x00000100	//
#define	RCC_BDCR_RTCSEL_1			0x00000200	//
#define	RCC_BDCR_RTCEN				0x00008000	//
#define	RCC_BDCR_BDRST				0x00010000	//

// RCC_CSR register

#define	RCC_CSR_LSION				0x00000001	//
#define	RCC_CSR_LSIRDY				0x00000002	//
#define	RCC_CSR_RMVF				0x01000000	//
#define	RCC_CSR_BORRSTF 			0x02000000	//
#define	RCC_CSR_PADRSTF 			0x04000000	//
#define	RCC_CSR_PORRSTF 			0x08000000	//
#define	RCC_CSR_SFTRSTF 			0x10000000	//
#define	RCC_CSR_WDGRSTF 			0x20000000	//
#define	RCC_CSR_WWDGRSTF			0x40000000	//
#define	RCC_CSR_LPWRRSTF			0x80000000	//

// RCC_SSCGR register

#define	RCC_SSCGR_MODPER			0x00001FFF	//
#define	RCC_SSCGR_INCSTEP 			0x0FFFE000	//
#define	RCC_SSCGR_SPREADSEL   		0x40000000	//
#define	RCC_SSCGR_SSCGEN			0x80000000	//

// RCC_PLLI2SCFGR register

#define	RCC_PLLI2SCFGR_PLLI2SN		0x00007FC0	//
#define	RCC_PLLI2SCFGR_PLLI2SQ		0x0F000000	//
#define	RCC_PLLI2SCFGR_PLLI2SR		0x70000000	//

// RCC_PLLSAICFGR register

#define	RCC_PLLSAICFGR_PLLSAIN		0x00007FC0	//
#define	RCC_PLLSAICFGR_PLLSAIP		0x00030000	//
#define	RCC_PLLSAICFGR_PLLSAIQ		0x0F000000	//
#define	RCC_PLLSAICFGR_PLLSAIR		0x70000000	//

// RCC_DCKCFGR register

#define	RCC_DCKCFGR_PLLI2SDIVQ		0x0000001F	//
#define	RCC_DCKCFGR_PLLSAIDIVQ		0x00001F00	//
#define	RCC_DCKCFGR_PLLSAIDIVR		0x00030000	//
#define	RCC_DCKCFGR_SAI1ASRC		0x00300000	//
#define	RCC_DCKCFGR_SAI1ASRC_0		0x00100000	//
#define	RCC_DCKCFGR_SAI1ASRC_1		0x00200000	//
#define	RCC_DCKCFGR_SAI1BSRC		0x00C00000	//
#define	RCC_DCKCFGR_SAI1BSRC_0		0x00400000	//
#define	RCC_DCKCFGR_SAI1BSRC_1		0x00800000	//
#define	RCC_DCKCFGR_TIMPRE			0x01000000	//
#define	RCC_DCKCFGR_CK48MSEL		0x08000000	//
#define	RCC_DCKCFGR_SDIOSEL			0x10000000	//
#define	RCC_DCKCFGR_DSISEL			0x20000000	//
#endif
