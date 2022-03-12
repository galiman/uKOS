/*
; stm32F407.
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
; Goal:		stm32F407 equates.
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

#define FLASH_BASE			0x08000000						// FLASH(up to 1-MB) base address in the alias region
#define CCMDATARAM_BASE		0x10000000						// CCM(core coupled memory) data RAM(64 KB) base address in the alias region
#define SRAM1_BASE			0x20000000						// SRAM1(112-KB) base address in the alias region
#define SRAM2_BASE			0x2001C000						// SRAM2(16-KB) base address in the alias region
#define PERIPH_BASE			0x40000000						// Peripheral base address in the alias region
#define BKPSRAM_BASE		0x40024000						// Backup SRAM(4-KB) base address in the alias region
#define FSMC_R_BASE			0xA0000000						// FSMC registers base address

#define CCMDATARAM_BB_BASE	0x12000000						// CCM(core coupled memory) data RAM(64-KB) base address in the bit-band region
#define SRAM1_BB_BASE		0x22000000						// SRAM1(112-KB) base address in the bit-band region
#define SRAM2_BB_BASE		0x2201C000						// SRAM2(16-KB) base address in the bit-band region
#define PERIPH_BB_BASE		0x42000000						// Peripheral base address in the alias region
#define BKPSRAM_BB_BASE		0x42024000						// Backup SRAM(4-KB) base address in the bit-band region

#define SRAM_BASE			SRAM1_BASE
#define SRAM_BB_BASE		SRAM1_BB_BASE

#define	bitband_t			*(volatile unsigned long *)
#define	BB_PERIPH(add, bit)	(PERIPH_BB_BASE + (((unsigned long)add) - PERIPH_BASE) * 32 + (bit) * 4)

#define APB1PERIPH_BASE		PERIPH_BASE						//
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x10000)			//
#define AHB1PERIPH_BASE		(PERIPH_BASE + 0x20000)			//
#define AHB2PERIPH_BASE		(PERIPH_BASE + 0x10000000)		// Periph bases

#define TIM1_BASE			(APB2PERIPH_BASE + 0x0000)		//
#define TIM2_BASE			(APB1PERIPH_BASE + 0x0000)		//
#define TIM3_BASE			(APB1PERIPH_BASE + 0x0400)		//
#define TIM4_BASE			(APB1PERIPH_BASE + 0x0800)		//
#define TIM5_BASE			(APB1PERIPH_BASE + 0x0C00)		//
#define TIM6_BASE			(APB1PERIPH_BASE + 0x1000)		//
#define TIM7_BASE			(APB1PERIPH_BASE + 0x1400)		//
#define TIM8_BASE			(APB2PERIPH_BASE + 0x0400)		//
#define TIM9_BASE			(APB2PERIPH_BASE + 0x4000)		//
#define TIM10_BASE			(APB2PERIPH_BASE + 0x4400)		//
#define TIM11_BASE			(APB2PERIPH_BASE + 0x4800)		//
#define TIM12_BASE			(APB1PERIPH_BASE + 0x1800)		//
#define TIM13_BASE			(APB1PERIPH_BASE + 0x1C00)		//
#define TIM14_BASE			(APB1PERIPH_BASE + 0x2000)		//
#define RTC_BASE			(APB1PERIPH_BASE + 0x2800)		//
#define WWDG_BASE			(APB1PERIPH_BASE + 0x2C00)		//
#define IWDG_BASE			(APB1PERIPH_BASE + 0x3000)		// Timers + RTC + watchdog
#define I2S2ext_BASE		(APB1PERIPH_BASE + 0x3400)		//
#define SPI2_BASE			(APB1PERIPH_BASE + 0x3800)		//
#define SPI3_BASE			(APB1PERIPH_BASE + 0x3C00)		// SPIs
#define I2S3ext_BASE		(APB1PERIPH_BASE + 0x4000)		//
#define USART1_BASE			(APB2PERIPH_BASE + 0x1000)		//
#define USART2_BASE			(APB1PERIPH_BASE + 0x4400)		//
#define USART3_BASE			(APB1PERIPH_BASE + 0x4800)		//
#define UART4_BASE			(APB1PERIPH_BASE + 0x4C00)		//
#define UART5_BASE			(APB1PERIPH_BASE + 0x5000)		//
#define USART6_BASE			(APB2PERIPH_BASE + 0x1400)		// USARTs
#define I2C1_BASE			(APB1PERIPH_BASE + 0x5400)		//
#define I2C2_BASE			(APB1PERIPH_BASE + 0x5800)		//
#define I2C3_BASE			(APB1PERIPH_BASE + 0x5C00)		// I2Cs
#define CAN1_BASE			(APB1PERIPH_BASE + 0x6400)		//
#define CAN2_BASE			(APB1PERIPH_BASE + 0x6800)		// CANs
#define PWR_BASE			(APB1PERIPH_BASE + 0x7000)		// Power
#define DAC_BASE			(APB1PERIPH_BASE + 0x7400)		// DAC
#define ADC1_BASE			(APB2PERIPH_BASE + 0x2000)		//
#define ADC2_BASE			(APB2PERIPH_BASE + 0x2100)		//
#define ADC3_BASE			(APB2PERIPH_BASE + 0x2200)		//
#define ADC_BASE			(APB2PERIPH_BASE + 0x2300)		// ADCs
#define SDIO_BASE			(APB2PERIPH_BASE + 0x2C00)		// SDIO
#define SPI1_BASE			(APB2PERIPH_BASE + 0x3000)		//
#define SYSCFG_BASE			(APB2PERIPH_BASE + 0x3800)		// SYSCFG
#define EXTI_BASE			(APB2PERIPH_BASE + 0x3C00)		// EXTI
#define GPIOA_BASE			(AHB1PERIPH_BASE + 0x0000)		//
#define GPIOB_BASE			(AHB1PERIPH_BASE + 0x0400)		//
#define GPIOC_BASE			(AHB1PERIPH_BASE + 0x0800)		//
#define GPIOD_BASE			(AHB1PERIPH_BASE + 0x0C00)		//
#define GPIOE_BASE			(AHB1PERIPH_BASE + 0x1000)		//
#define GPIOF_BASE			(AHB1PERIPH_BASE + 0x1400)		//
#define GPIOG_BASE			(AHB1PERIPH_BASE + 0x1800)		//
#define GPIOH_BASE			(AHB1PERIPH_BASE + 0x1C00)		//
#define GPIOI_BASE			(AHB1PERIPH_BASE + 0x2000)		// GPIOs
#define CRC_BASE			(AHB1PERIPH_BASE + 0x3000)		// CRC
#define RCC_BASE			(AHB1PERIPH_BASE + 0x3800)		// RCC
#define FLASH_R_BASE		(AHB1PERIPH_BASE + 0x3C00)		// Flash registers base address
#define DMA1_BASE			(AHB1PERIPH_BASE + 0x6000)		//
#define DMA1_Stream0_BASE	(DMA1_BASE       + 0x0010)		//
#define DMA1_Stream1_BASE	(DMA1_BASE       + 0x0028)		//
#define DMA1_Stream2_BASE	(DMA1_BASE       + 0x0040)		//
#define DMA1_Stream3_BASE	(DMA1_BASE       + 0x0058)		//
#define DMA1_Stream4_BASE	(DMA1_BASE       + 0x0070)		//
#define DMA1_Stream5_BASE	(DMA1_BASE       + 0x0088)		//
#define DMA1_Stream6_BASE	(DMA1_BASE       + 0x00A0)		//
#define DMA1_Stream7_BASE	(DMA1_BASE       + 0x00B8)		//
#define DMA2_BASE			(AHB1PERIPH_BASE + 0x6400)		//
#define DMA2_Stream0_BASE	(DMA2_BASE       + 0x0010)		//
#define DMA2_Stream1_BASE	(DMA2_BASE       + 0x0028)		//
#define DMA2_Stream2_BASE	(DMA2_BASE       + 0x0040)		//
#define DMA2_Stream3_BASE	(DMA2_BASE       + 0x0058)		//
#define DMA2_Stream4_BASE	(DMA2_BASE       + 0x0070)		//
#define DMA2_Stream5_BASE	(DMA2_BASE       + 0x0088)		//
#define DMA2_Stream6_BASE	(DMA2_BASE       + 0x00A0)		//
#define DMA2_Stream7_BASE	(DMA2_BASE       + 0x00B8)		// DMAs
#define ETH_BASE			(AHB1PERIPH_BASE + 0x8000)		//
#define ETH_MAC_BASE		(ETH_BASE        + 0x0000)		//
#define ETH_MMC_BASE		(ETH_BASE        + 0x0100)		//
#define ETH_PTP_BASE		(ETH_BASE        + 0x0700)		//
#define ETH_DMA_BASE		(ETH_BASE        + 0x1000)		//
#define DCMI_BASE			(AHB2PERIPH_BASE + 0x50000)		//
#define CRYP_BASE			(AHB2PERIPH_BASE + 0x60000)		//
#define HASH_BASE			(AHB2PERIPH_BASE + 0x60400)		//
#define RNG_BASE			(AHB2PERIPH_BASE + 0x60800)		//
#define FSMC_Bank1_R_BASE	(FSMC_R_BASE     + 0x0000)		//
#define FSMC_Bank2_R_BASE	(FSMC_R_BASE     + 0x0060)		//
#define FSMC_Bank3_R_BASE	(FSMC_R_BASE     + 0x0080)		//
#define FSMC_Bank4_R_BASE	(FSMC_R_BASE     + 0x00A0)		//
#define FSMC_Bank1E_R_BASE	(FSMC_R_BASE     + 0x0104)		// FSMC Bank registers base address

#include 	"vectors.h"
#include 	"cortex_m4_core.h"
#include 	"stm32F407_rcc.h"
#include 	"stm32F407_flash.h"
#include 	"stm32F407_gpio.h"
#include 	"stm32F407_exti.h"
#include 	"stm32F407_usart.h"
#include 	"stm32F407_tim.h"
#include 	"stm32F407_adc.h"
#include 	"stm32F407_fsmc.h"
#include 	"stm32F407_rtc.h"
#include 	"stm32F407_pwr.h"
#include 	"stm32F407_spi.h"
#include 	"stm32F407_i2c.h"
#include 	"stm32F407_dma.h"
#include 	"stm32F407_dcmi.h"
#include 	"stm32F407_sdio.h"
#include 	"stm32F407_rng.h"
#include 	"stm32F407_syscfg.h"
#endif
