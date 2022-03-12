/*
; stm32F777.
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
; Goal:		stm32F777 equates.
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

#define RAMITCM_BASE		0x00000000						// 16-KB RAM reserved for CPU execution/instruction accessible over ITCM
#define FLASHITCM_BASE		0x00200000						// (up to 2-MB) embedded FLASH memory accessible over ITCM                       
#define FLASHAXI_BASE		0x08000000						// (up to 2-MB) embedded FLASH memory accessible over AXI                       
#define RAMDTCM_BASE		0x20000000						// 64KB system data RAM accessible over DTCM
#define SRAM1_BASE			0x20020000						// SRAM1(368-KB) base address in the alias region
#define SRAM2_BASE			0x2007C000						// SRAM2(16-KB) base address in the alias region
#define PERIPH_BASE			0x40000000						// Peripheral base address in the alias region
#define BKPSRAM_BASE		0x40024000						// Backup SRAM(4-KB) base address in the alias region
#define QSPI_BASE			0x90000000						// QSPI memories accessible over AXI
#define FMC_R_BASE			0xA0000000						// FMC registers base address
#define QSPI_R_BASE			0xA0001000						// QSPI Control registers
#define FLASH_END			0x081FFFFF						// FLASH end address

#define FLASH_BASE			FLASHAXI_BASE					// FLASH(up to 2-MB) base address in the alias region
#define SRAM_BASE			SRAM1_BASE

#define	bitband_t			*(volatile unsigned long *)
#define	BB_PERIPH(add, bit)	(PERIPH_BB_BASE + (((unsigned long)add) - PERIPH_BASE) * 32 + (bit) * 4)

#define APB1PERIPH_BASE		PERIPH_BASE						//
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x00010000)		//
#define AHB1PERIPH_BASE		(PERIPH_BASE + 0x00020000)		//
#define AHB2PERIPH_BASE		(PERIPH_BASE + 0x10000000)		// Periph bases

#define TIM2_BASE			(APB1PERIPH_BASE + 0x00000)		//
#define TIM3_BASE			(APB1PERIPH_BASE + 0x00400)		//
#define TIM4_BASE			(APB1PERIPH_BASE + 0x00800)		//
#define TIM5_BASE			(APB1PERIPH_BASE + 0x00C00)		//
#define TIM6_BASE			(APB1PERIPH_BASE + 0x01000)		//
#define TIM7_BASE			(APB1PERIPH_BASE + 0x01400)		//
#define TIM12_BASE			(APB1PERIPH_BASE + 0x01800)		//
#define TIM13_BASE			(APB1PERIPH_BASE + 0x01C00)		//
#define TIM14_BASE			(APB1PERIPH_BASE + 0x02000)		//
#define LPTIM1_BASE			(APB1PERIPH_BASE + 0x02400)		//
#define RTC_BASE			(APB1PERIPH_BASE + 0x02800)		//
#define WWDG_BASE			(APB1PERIPH_BASE + 0x02C00)		//
#define IWDG_BASE			(APB1PERIPH_BASE + 0x03000)		// Timers + RTC + watchdog
#define CAN3_BASE			(APB1PERIPH_BASE + 0x03400)		//
#define SPI2_BASE			(APB1PERIPH_BASE + 0x03800)		//
#define SPI3_BASE			(APB1PERIPH_BASE + 0x03C00)		//
#define SPDIFRX_BASE		(APB1PERIPH_BASE + 0x04000)		//
#define USART2_BASE			(APB1PERIPH_BASE + 0x04400)		//
#define USART3_BASE			(APB1PERIPH_BASE + 0x04800)		//
#define UART4_BASE			(APB1PERIPH_BASE + 0x04C00)		//
#define UART5_BASE			(APB1PERIPH_BASE + 0x05000)		//
#define I2C1_BASE			(APB1PERIPH_BASE + 0x05400)		//
#define I2C2_BASE			(APB1PERIPH_BASE + 0x05800)		//
#define I2C3_BASE			(APB1PERIPH_BASE + 0x05C00)		//
#define I2C4_BASE			(APB1PERIPH_BASE + 0x06000)		// I2Cs
#define CAN1_BASE			(APB1PERIPH_BASE + 0x06400)		//
#define CAN2_BASE			(APB1PERIPH_BASE + 0x06800)		// CANs
#define CEC_BASE			(APB1PERIPH_BASE + 0x06C00)		//
#define PWR_BASE			(APB1PERIPH_BASE + 0x07000)		// Power
#define DAC_BASE			(APB1PERIPH_BASE + 0x07400)		// DAC
#define UART7_BASE			(APB1PERIPH_BASE + 0x07800)		//
#define UART8_BASE			(APB1PERIPH_BASE + 0x07C00)		//

#define TIM1_BASE			(APB2PERIPH_BASE + 0x00000)		//
#define TIM8_BASE			(APB2PERIPH_BASE + 0x00400)		//
#define USART1_BASE			(APB2PERIPH_BASE + 0x01000)		//
#define USART6_BASE			(APB2PERIPH_BASE + 0x01400)		//
#define SDMMC2_BASE			(APB2PERIPH_BASE + 0x01C00)		//
#define ADC1_BASE			(APB2PERIPH_BASE + 0x02000)		//
#define ADC2_BASE			(APB2PERIPH_BASE + 0x02100)		//
#define ADC3_BASE			(APB2PERIPH_BASE + 0x02200)		//
#define ADC_BASE			(APB2PERIPH_BASE + 0x02300)		// ADCs
#define SDMMC1_BASE			(APB2PERIPH_BASE + 0x02C00)		// SDIO
#define SPI1_BASE			(APB2PERIPH_BASE + 0x03000)		//
#define SPI4_BASE			(APB2PERIPH_BASE + 0x03400)		//
#define SYSCFG_BASE			(APB2PERIPH_BASE + 0x03800)		// SYSCFG
#define EXTI_BASE			(APB2PERIPH_BASE + 0x03C00)		// EXTI
#define TIM9_BASE			(APB2PERIPH_BASE + 0x04000)		//
#define TIM10_BASE			(APB2PERIPH_BASE + 0x04400)		//
#define TIM11_BASE			(APB2PERIPH_BASE + 0x04800)		//
#define SPI5_BASE			(APB2PERIPH_BASE + 0x05000)		//
#define SPI6_BASE			(APB2PERIPH_BASE + 0x05400)		//
#define SAI1_BASE			(APB2PERIPH_BASE + 0x05800)		//
#define SAI2_BASE			(APB2PERIPH_BASE + 0x05C00)		//

#define GPIOA_BASE			(AHB1PERIPH_BASE + 0x00000)		//
#define GPIOB_BASE			(AHB1PERIPH_BASE + 0x00400)		//
#define GPIOC_BASE			(AHB1PERIPH_BASE + 0x00800)		//
#define GPIOD_BASE			(AHB1PERIPH_BASE + 0x00C00)		//
#define GPIOE_BASE			(AHB1PERIPH_BASE + 0x01000)		//
#define GPIOF_BASE			(AHB1PERIPH_BASE + 0x01400)		//
#define GPIOG_BASE			(AHB1PERIPH_BASE + 0x01800)		//
#define GPIOH_BASE			(AHB1PERIPH_BASE + 0x01C00)		//
#define GPIOI_BASE			(AHB1PERIPH_BASE + 0x02000)		//
#define GPIOJ_BASE			(AHB1PERIPH_BASE + 0x02400)		//
#define GPIOK_BASE			(AHB1PERIPH_BASE + 0x02800)		// GPIOs
#define CRC_BASE			(AHB1PERIPH_BASE + 0x03000)		// CRC
#define RCC_BASE			(AHB1PERIPH_BASE + 0x03800)		// RCC
#define FLASH_R_BASE		(AHB1PERIPH_BASE + 0x03C00)		// Flash registers base address
#define DMA1_BASE			(AHB1PERIPH_BASE + 0x06000)		//
#define DMA2_BASE			(AHB1PERIPH_BASE + 0x06400)		//
#define ETH_BASE			(AHB1PERIPH_BASE + 0x08000)		//
#define DMA2D_BASE			(AHB1PERIPH_BASE + 0x0B000)		//

#define DCMI_BASE			(AHB2PERIPH_BASE + 0x50000)		//
#define JPEG_BASE			(AHB2PERIPH_BASE + 0x51000)		//
#define CRYP_BASE			(AHB2PERIPH_BASE + 0x60000)		//
#define HASH_BASE			(AHB2PERIPH_BASE + 0x60400)		//
#define HASH_DIGEST_BASE	(AHB2PERIPH_BASE + 0x60710)		//
#define RNG_BASE			(AHB2PERIPH_BASE + 0x60800)		//

#define DMA1_Stream0_BASE	(DMA1_BASE       + 0x00010)		//
#define DMA1_Stream1_BASE	(DMA1_BASE       + 0x00028)		//
#define DMA1_Stream2_BASE	(DMA1_BASE       + 0x00040)		//
#define DMA1_Stream3_BASE	(DMA1_BASE       + 0x00058)		//
#define DMA1_Stream4_BASE	(DMA1_BASE       + 0x00070)		//
#define DMA1_Stream5_BASE	(DMA1_BASE       + 0x00088)		//
#define DMA1_Stream6_BASE	(DMA1_BASE       + 0x000A0)		//
#define DMA1_Stream7_BASE	(DMA1_BASE       + 0x000B8)		//
#define DMA2_Stream0_BASE	(DMA2_BASE       + 0x00010)		//
#define DMA2_Stream1_BASE	(DMA2_BASE       + 0x00028)		//
#define DMA2_Stream2_BASE	(DMA2_BASE       + 0x00040)		//
#define DMA2_Stream3_BASE	(DMA2_BASE       + 0x00058)		//
#define DMA2_Stream4_BASE	(DMA2_BASE       + 0x00070)		//
#define DMA2_Stream5_BASE	(DMA2_BASE       + 0x00088)		//
#define DMA2_Stream6_BASE	(DMA2_BASE       + 0x000A0)		//
#define DMA2_Stream7_BASE	(DMA2_BASE       + 0x000B8)		// DMAs

#define ETH_MAC_BASE		(ETH_BASE        + 0x00000)		//
#define ETH_MMC_BASE		(ETH_BASE        + 0x00100)		//
#define ETH_PTP_BASE		(ETH_BASE        + 0x00700)		//
#define ETH_DMA_BASE		(ETH_BASE        + 0x01000)		//

#define FMC_Bank1_R_BASE	(FMC_R_BASE      + 0x00000)		//
#define FMC_Bank3_R_BASE	(FMC_R_BASE      + 0x00080)		//
#define FMC_Bank1E_R_BASE	(FMC_R_BASE      + 0x00104)		//
#define FMC_Bank5_6_R_BASE	(FMC_R_BASE      + 0x00140)		// FMC Bank registers base address

#include 	"vectors.h"
#include 	"cortex_m7_core.h"
#include 	"stm32F777_rcc.h"
#include 	"stm32F777_flash.h"
#include 	"stm32F777_gpio.h"
#include 	"stm32F777_exti.h"
#include 	"stm32F777_usart.h"
#include 	"stm32F777_tim.h"
#include 	"stm32F777_adc.h"
#include 	"stm32F777_fmc.h"
#include 	"stm32F777_rtc.h"
#include 	"stm32F777_pwr.h"
#include 	"stm32F777_spi.h"
#include 	"stm32F777_i2c.h"
#include 	"stm32F777_jpeg.h"
#include 	"stm32F777_dma.h"
#include 	"stm32F777_dcmi.h"
#include 	"stm32F777_sdio.h"
#include 	"stm32F777_rng.h"
#include 	"stm32F777_ltdc.h"
#include 	"stm32F777_syscfg.h"
#endif
