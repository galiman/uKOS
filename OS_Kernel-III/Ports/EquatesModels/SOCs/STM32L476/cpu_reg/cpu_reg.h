/*
; stm32L476.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2008-09-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 114                  $:  Revision of last commit
; $Date:: 2017-04-17 11:45:18#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stm32L471 equates.
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

#define FLASH_BASE					0x08000000						// FLASH(up to 1-MB) base address in the alias region
#define SRAM1_BASE					0x20000000						// SRAM1(96-KB) base address in the alias region
#define SRAM2_BASE					0x10000000						// SRAM2(32-KB) base address in the alias region
#define	PERIPH_BASE					0x40000000						// Peripheral base address in the alias region
#define	FMC_BASE					0x60000000						// FMC base address
#define	QSPI_BASE					0x90000000						// QUADSPI memories accessible over AHB base address
#define FMC_R_BASE					0xA0000000						// FMC registers base address
#define QSPI_R_BASE					0xA0001000						// QUADSPI control registers base address
#define SRAM1_BB_BASE				0x22000000						// SRAM1(96-KB) base address in the bit-band region
#define SRAM2_BB_BASE				0x12000000						// SRAM2(32-KB) base address in the bit-band region
#define PERIPH_BB_BASE				0x42000000						// Peripheral base address in the alias region

#define DBGMCU_BASE					0xE0042000						//
#define PACKAGE_BASE				0x1FFF7500						//
#define UID_BASE					0x1FFF7590						//
#define FLASHSIZE_BASE				0x1FFF75E0						//

#define SRAM_BASE					SRAM1_BASE
#define SRAM_BB_BASE				SRAM1_BB_BASE

#define	bitband_t					*(volatile unsigned long *)
#define	BB_PERIPH(add, bit)			(PERIPH_BB_BASE + (((unsigned long)add) - PERIPH_BASE) * 32 + (bit) * 4)

#define APB1PERIPH_BASE				PERIPH_BASE						//
#define APB2PERIPH_BASE				(PERIPH_BASE + 0x00010000)		//
#define AHB1PERIPH_BASE				(PERIPH_BASE + 0x00020000)		//
#define AHB2PERIPH_BASE				(PERIPH_BASE + 0x08000000)		// Periph bases

#define	FMC_BANK1					FMC_BASE
#define	FMC_BANK1_1					FMC_BANK1
#define	FMC_BANK1_2					(FMC_BANK1 + 0x04000000)		//
#define	FMC_BANK1_3					(FMC_BANK1 + 0x08000000)		//
#define	FMC_BANK1_4					(FMC_BANK1 + 0x0C000000)		//
#define	FMC_BANK3					(FMC_BASE  + 0x20000000)		//

#define TIM1_BASE					(APB2PERIPH_BASE + 0x00002C00)	//
#define TIM2_BASE					(APB1PERIPH_BASE + 0x00000000)	//
#define TIM3_BASE					(APB1PERIPH_BASE + 0x00000400)	//
#define TIM4_BASE					(APB1PERIPH_BASE + 0x00000800)	//
#define TIM5_BASE					(APB1PERIPH_BASE + 0x00000C00)	//
#define TIM6_BASE					(APB1PERIPH_BASE + 0x00001000)	//
#define TIM7_BASE					(APB1PERIPH_BASE + 0x00001400)	//
#define	LCD_BASE					(APB1PERIPH_BASE + 0x00002400)	//
#define TIM8_BASE					(APB2PERIPH_BASE + 0x00003400)	//
#define TIM15_BASE					(APB2PERIPH_BASE + 0x00004000)	//
#define TIM16_BASE					(APB2PERIPH_BASE + 0x00004400)	//
#define TIM17_BASE					(APB2PERIPH_BASE + 0x00004800)	//
#define LPTIM1_BASE					(APB1PERIPH_BASE + 0x00007C00)	//
#define LPTIM2_BASE					(APB1PERIPH_BASE + 0x00009400)	//
#define RTC_BASE					(APB1PERIPH_BASE + 0x00002800)	//
#define WWDG_BASE					(APB1PERIPH_BASE + 0x00002C00)	//
#define IWDG_BASE					(APB1PERIPH_BASE + 0x00003000)	// Timers + RTC + watchdog
#define SPI2_BASE					(APB1PERIPH_BASE + 0x00003800)	//
#define SPI3_BASE					(APB1PERIPH_BASE + 0x00003C00)	// SPIs
#define USART1_BASE					(APB2PERIPH_BASE + 0x00003800)	//
#define USART2_BASE					(APB1PERIPH_BASE + 0x00004400)	//
#define USART3_BASE					(APB1PERIPH_BASE + 0x00004800)	//
#define UART4_BASE					(APB1PERIPH_BASE + 0x00004C00)	//
#define UART5_BASE					(APB1PERIPH_BASE + 0x00005000)	//
#define LPUART1_BASE				(APB1PERIPH_BASE + 0x00008000)	// USARTs
#define I2C1_BASE					(APB1PERIPH_BASE + 0x00005400)	//
#define I2C2_BASE					(APB1PERIPH_BASE + 0x00005800)	//
#define I2C3_BASE					(APB1PERIPH_BASE + 0x00005C00)	// I2Cs
#define CAN1_BASE					(APB1PERIPH_BASE + 0x00006400)	// CANs
#define PWR_BASE					(APB1PERIPH_BASE + 0x00007000)	// Power
#define DAC_BASE					(APB1PERIPH_BASE + 0x00007400)	//
#define DAC1_BASE					(APB1PERIPH_BASE + 0x00007400)	// DAC
#define OPAMP_BASE					(APB1PERIPH_BASE + 0x00007800)	//
#define OPAMP1_BASE					(APB1PERIPH_BASE + 0x00007800)	//
#define OPAMP2_BASE					(APB1PERIPH_BASE + 0x00007810)	// OPAmp
#define SWPMI1_BASE					(APB1PERIPH_BASE + 0x00008800)	//
#define SYSCFG_BASE					(APB2PERIPH_BASE + 0x00000000)	// SYSCFG
#define VREFBUF_BASE				(APB2PERIPH_BASE + 0x00000030)	//
#define COMP1_BASE					(APB2PERIPH_BASE + 0x00000200)	//
#define COMP2_BASE					(APB2PERIPH_BASE + 0x00000204)	//
#define EXTI_BASE					(APB2PERIPH_BASE + 0x00000400)	// EXTI
#define FIREWALL_BASE				(APB2PERIPH_BASE + 0x00001C00)	//
#define SDMMC1_BASE					(APB2PERIPH_BASE + 0x00002800)	//
#define SPI1_BASE					(APB2PERIPH_BASE + 0x00003000)	//
#define SAI1_BASE					(APB2PERIPH_BASE + 0x00005400)	//
#define SAI1_Block_A_BASE			(SAI1_BASE 		 + 0x00000004)	//
#define SAI1_Block_B_BASE			(SAI1_BASE 		 + 0x00000024)	//
#define SAI2_BASE					(APB2PERIPH_BASE + 0x00005800)	//
#define SAI2_Block_A_BASE			(SAI2_BASE 		 + 0x00000004)	//
#define SAI2_Block_B_BASE			(SAI2_BASE 		 + 0x00000024)	//
#define DFSDM1_BASE					(APB2PERIPH_BASE + 0x00006000)	//
#define DFSDM1_Channel0_BASE		(DFSDM1_BASE 	 + 0x00000000)	//
#define DFSDM1_Channel1_BASE		(DFSDM1_BASE 	 + 0x00000020)	//
#define DFSDM1_Channel2_BASE		(DFSDM1_BASE 	 + 0x00000040)	//
#define DFSDM1_Channel3_BASE		(DFSDM1_BASE 	 + 0x00000060)	//
#define DFSDM1_Channel4_BASE		(DFSDM1_BASE 	 + 0x00000080)	//
#define DFSDM1_Channel5_BASE		(DFSDM1_BASE 	 + 0x000000A0)	//
#define DFSDM1_Channel6_BASE		(DFSDM1_BASE 	 + 0x000000C0)	//
#define DFSDM1_Channel7_BASE		(DFSDM1_BASE 	 + 0x000000E0)	//
#define DFSDM1_Filter0_BASE			(DFSDM1_BASE 	 + 0x00000100)	//
#define DFSDM1_Filter1_BASE			(DFSDM1_BASE 	 + 0x00000180)	//
#define DFSDM1_Filter2_BASE			(DFSDM1_BASE 	 + 0x00000200)	//
#define DFSDM1_Filter3_BASE			(DFSDM1_BASE 	 + 0x00000280)	//
#define CRC_BASE					(AHB1PERIPH_BASE + 0x00003000)	//
#define RCC_BASE					(AHB1PERIPH_BASE + 0x00001000)	//
#define FLASH_R_BASE				(AHB1PERIPH_BASE + 0x00002000)	// Flash registers base address
#define TSC_BASE					(AHB1PERIPH_BASE + 0x00004000)	//
#define DMA1_BASE					(AHB1PERIPH_BASE + 0x00000000)	//
#define DMA1_Channel1_BASE			(DMA1_BASE 		 + 0x00000008)	//
#define DMA1_Channel2_BASE			(DMA1_BASE 		 + 0x0000001C)	//
#define DMA1_Channel3_BASE			(DMA1_BASE 		 + 0x00000030)	//
#define DMA1_Channel4_BASE			(DMA1_BASE 		 + 0x00000044)	//
#define DMA1_Channel5_BASE			(DMA1_BASE 		 + 0x00000058)	//
#define DMA1_Channel6_BASE			(DMA1_BASE 		 + 0x0000006C)	//
#define DMA1_Channel7_BASE			(DMA1_BASE 		 + 0x00000080)	//
#define DMA1_CSELR_BASE				(DMA1_BASE 		 + 0x000000A8)	//
#define DMA2_BASE					(AHB1PERIPH_BASE + 0x00000400)	//
#define DMA2_Channel1_BASE			(DMA2_BASE 		 + 0x00000008)	//
#define DMA2_Channel2_BASE			(DMA2_BASE 		 + 0x0000001C)	//
#define DMA2_Channel3_BASE			(DMA2_BASE 		 + 0x00000030)	//
#define DMA2_Channel4_BASE			(DMA2_BASE 		 + 0x00000044)	//
#define DMA2_Channel5_BASE			(DMA2_BASE 		 + 0x00000058)	//
#define DMA2_Channel6_BASE			(DMA2_BASE 		 + 0x0000006C)	//
#define DMA2_Channel7_BASE			(DMA2_BASE 		 + 0x00000080)	//
#define DMA2_CSELR_BASE				(DMA2_BASE 		 + 0x000000A8)	//
#define GPIOA_BASE					(AHB2PERIPH_BASE + 0x00000000)	//
#define GPIOB_BASE					(AHB2PERIPH_BASE + 0x00000400)	//
#define GPIOC_BASE					(AHB2PERIPH_BASE + 0x00000800)	//
#define GPIOD_BASE					(AHB2PERIPH_BASE + 0x00000C00)	//
#define GPIOE_BASE					(AHB2PERIPH_BASE + 0x00001000)	//
#define GPIOF_BASE					(AHB2PERIPH_BASE + 0x00001400)	//
#define GPIOG_BASE					(AHB2PERIPH_BASE + 0x00001800)	//
#define GPIOH_BASE					(AHB2PERIPH_BASE + 0x00001C00)	//
#define ADC1_BASE					(AHB2PERIPH_BASE + 0x08040000)	//
#define ADC2_BASE					(AHB2PERIPH_BASE + 0x08040100)	//
#define ADC3_BASE					(AHB2PERIPH_BASE + 0x08040200)	//
#define ADC123_COMMON_BASE			(AHB2PERIPH_BASE + 0x08040300)	//
#define RNG_BASE					(AHB2PERIPH_BASE + 0x08060800)	//
#define FMC_Bank1_R_BASE			(FMC_R_BASE 	 + 0x00000000)	//
#define FMC_Bank1E_R_BASE			(FMC_R_BASE 	 + 0x00000104)	//
#define FMC_Bank3_R_BASE			(FMC_R_BASE 	 + 0x00000080)	//

#include 	"vectors.h"
#include 	"cortex_m4_core.h"
#include 	"stm32L476_rcc.h"
#include 	"stm32L476_flash.h"
#include 	"stm32L476_gpio.h"
#include 	"stm32L476_exti.h"
#include 	"stm32L476_usart.h"
#include 	"stm32L476_tim.h"
#include 	"stm32L476_fmc.h"
#include 	"stm32L476_pwr.h"
#include 	"stm32L476_spi.h"
#include 	"stm32L476_i2c.h"
#include 	"stm32L476_dma.h"
#include 	"stm32L476_rng.h"
#include 	"stm32L476_syscfg.h"
#include 	"stm32L476_lcd.h"
#include 	"stm32L476_rtc.h"
#endif
