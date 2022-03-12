/*
; stm32F103.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2008-09-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 113                  $:  Revision of last commit
; $Date:: 2017-04-16 16:55:48#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stm32F103 equates.
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

#ifndef __CPU_REG__
#define __CPU_REG__

// SoC bloc mapping
// ----------------

#define PERIPH_BB_BASE		0x42000000						// Peripheral base address in the alias region
#define SRAM_BB_BASE		0x22000000						// SRAM base address in the alias region
#define SRAM_BASE			0x20000000						// Peripheral base address in the bit-band region
#define PERIPH_BASE			0x40000000						// SRAM base address in the bit-band region
#define FSMC_R_BASE			0xA0000000						// FSMC registers base address
#define OB_BASE				0x1FFFF800						// Flash Option Bytes base address
#define DBGMCU_BASE			0xE0042000						// Debug MCU registers base address

#define APB1PERIPH_BASE		PERIPH_BASE						//
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x10000)			//
#define AHBPERIPH_BASE		(PERIPH_BASE + 0x20000)			// Periph bases

#define TIM1_BASE			(APB2PERIPH_BASE + 0x2C00)		//
#define TIM2_BASE			(APB1PERIPH_BASE + 0x0000)		//
#define TIM3_BASE			(APB1PERIPH_BASE + 0x0400)		//
#define TIM4_BASE			(APB1PERIPH_BASE + 0x0800)		//
#define TIM5_BASE			(APB1PERIPH_BASE + 0x0C00)		//
#define TIM6_BASE			(APB1PERIPH_BASE + 0x1000)		//
#define TIM7_BASE			(APB1PERIPH_BASE + 0x1400)		//
#define TIM8_BASE			(APB2PERIPH_BASE + 0x3400)		//
#define RTC_BASE			(APB1PERIPH_BASE + 0x2800)		//
#define WWDG_BASE			(APB1PERIPH_BASE + 0x2C00)		//
#define IWDG_BASE			(APB1PERIPH_BASE + 0x3000)		// Timers + RTC + watchdog
#define SPI1_BASE			(APB2PERIPH_BASE + 0x3000)		//
#define SPI2_BASE			(APB1PERIPH_BASE + 0x3800)		//
#define SPI3_BASE			(APB1PERIPH_BASE + 0x3C00)		// SPIs
#define USART1_BASE			(APB2PERIPH_BASE + 0x3800)		//
#define USART2_BASE			(APB1PERIPH_BASE + 0x4400)		//
#define USART3_BASE			(APB1PERIPH_BASE + 0x4800)		//
#define USART4_BASE			(APB1PERIPH_BASE + 0x4C00)		//
#define USART5_BASE			(APB1PERIPH_BASE + 0x5000)		// USARTs
#define I2C1_BASE			(APB1PERIPH_BASE + 0x5400)		//
#define I2C2_BASE			(APB1PERIPH_BASE + 0x5800)		// I2Cs
#define CAN1_BASE			(APB1PERIPH_BASE + 0x6400)		//
#define CAN2_BASE			(APB1PERIPH_BASE + 0x6800)		// CANs
#define BKP_BASE			(APB1PERIPH_BASE + 0x6C00)		//
#define PWR_BASE			(APB1PERIPH_BASE + 0x7000)		// Power + Backup
#define DAC_BASE			(APB1PERIPH_BASE + 0x7400)		// DAC
#define EXTI_BASE			(APB2PERIPH_BASE + 0x0400)		// EXTI
#define GPIOA_BASE			(APB2PERIPH_BASE + 0x0800)		//
#define GPIOB_BASE			(APB2PERIPH_BASE + 0x0C00)		//
#define GPIOC_BASE			(APB2PERIPH_BASE + 0x1000)		//
#define GPIOD_BASE			(APB2PERIPH_BASE + 0x1400)		//
#define GPIOE_BASE			(APB2PERIPH_BASE + 0x1800)		//
#define GPIOF_BASE			(APB2PERIPH_BASE + 0x1C00)		//
#define GPIOG_BASE			(APB2PERIPH_BASE + 0x2000)		// GPIOs
#define AFIO_BASE			(APB2PERIPH_BASE + 0x0000)		// AFIO
#define ADC1_BASE			(APB2PERIPH_BASE + 0x2400)		//
#define ADC2_BASE			(APB2PERIPH_BASE + 0x2800)		//
#define ADC3_BASE			(APB2PERIPH_BASE + 0x3C00)		// ADCs
#define DMA1_BASE			(AHBPERIPH_BASE  + 0x0000)		//
#define DMA1_Channel1_BASE	(AHBPERIPH_BASE  + 0x0008)		//
#define DMA1_Channel2_BASE	(AHBPERIPH_BASE  + 0x001C)		//
#define DMA1_Channel3_BASE	(AHBPERIPH_BASE  + 0x0030)		//
#define DMA1_Channel4_BASE	(AHBPERIPH_BASE  + 0x0044)		//
#define DMA1_Channel5_BASE	(AHBPERIPH_BASE  + 0x0058)		//
#define DMA1_Channel6_BASE	(AHBPERIPH_BASE  + 0x006C)		//
#define DMA1_Channel7_BASE	(AHBPERIPH_BASE  + 0x0080)		//
#define DMA2_BASE			(AHBPERIPH_BASE  + 0x0400)		//
#define DMA2_Channel1_BASE	(AHBPERIPH_BASE  + 0x0408)		//
#define DMA2_Channel2_BASE	(AHBPERIPH_BASE  + 0x041C)		//
#define DMA2_Channel3_BASE	(AHBPERIPH_BASE  + 0x0430)		//
#define DMA2_Channel4_BASE	(AHBPERIPH_BASE  + 0x0444)		//
#define DMA2_Channel5_BASE	(AHBPERIPH_BASE  + 0x0458)		// DMAs
#define RCC_BASE			(AHBPERIPH_BASE  + 0x1000)		// RCC
#define CRC_BASE			(AHBPERIPH_BASE  + 0x3000)		// CRC
#define FSMC_Bank1_R_BASE	(FSMC_R_BASE     + 0x0000)		//
#define FSMC_Bank2_R_BASE	(FSMC_R_BASE     + 0x0060)		//
#define FSMC_Bank3_R_BASE	(FSMC_R_BASE     + 0x0080)		//
#define FSMC_Bank4_R_BASE	(FSMC_R_BASE     + 0x00A0)		//
#define FSMC_Bank1E_R_BASE	(FSMC_R_BASE     + 0x0104)		// FSMC Bank registers base address
#define FLASH_R_BASE		(AHBPERIPH_BASE  + 0x2000)		// Flash registers base address
#define SDIO_BASE			(PERIPH_BASE     + 0x18000)		// SDIO

#include 	"vectors.h"
#include 	"cortex_m3_core.h"
#include 	"stm32F103_rcc.h"
#include 	"stm32F103_flash.h"
#include 	"stm32F103_gpio.h"
#include 	"stm32F103_exti.h"
#include 	"stm32F103_usart.h"
#include 	"stm32F103_tim.h"
#include 	"stm32F103_adc.h"
#include 	"stm32F103_fsmc.h"
#include 	"stm32F103_rtc.h"
#include 	"stm32F103_pwr.h"
#endif
