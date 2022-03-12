/*
; vectors.
; ========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Indirect Exception Vectors for M4.
;			The "reset" exception is not considered. The indirect table
;			is reserved in the first.s file.
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

#ifndef	__VECTORS__
#define	__VECTORS__

typedef enum IRQn {

// Cortex-M4 Processor Exceptions Numbers

	NonMaskableInt_IRQn		= -14,	// 2 Non Maskable Interrupt
	HardFault_IRQn			= -13,	// 3 Cortex-M4 Hard Fault Interrupt
	MemoryManagement_IRQn	= -12,	// 4 Cortex-M3 Memory Management Interrupt
	BusFault_IRQn			= -11,	// 5 Cortex-M3 Bus Fault Interrupt
	UsageFault_IRQn			= -10,	// 6 Cortex-M3 Usage Fault Interrupt
	SVCall_IRQn				= -5, 	// 11 Cortex-M3 SV Call Interrupt
	DebugMonitor_IRQn		= -4, 	// 12 Cortex-M3 Debug Monitor Interrupt
	PendSV_IRQn				= -2, 	// 14 Cortex-M3 Pend SV Interrupt
	SysTick_IRQn			= -1, 	// 15 Cortex-M3 System Tick Interrupt

// STM32 specific Interrupt Numbers

	WWDG_IRQn				= 0,	// Window WatchDog Interrupt
	PVD_PVM_IRQn			= 1,	// PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection Interrupts
	TAMP_STAMP_IRQn			= 2,	// Tamper and TimeStamp interrupts through the EXTI line
	RTC_WKUP_IRQn  			= 3,	// RTC Wakeup interrupt through the EXTI line
	FLASH_IRQn  			= 4,	// FLASH global Interrupt
	RCC_IRQn 				= 5,	// RCC global Interrupt
	EXTI0_IRQn  			= 6,	// EXTI Line0 Interrupt
	EXTI1_IRQn  			= 7,	// EXTI Line1 Interrupt
	EXTI2_IRQn  			= 8,	// EXTI Line2 Interrupt
	EXTI3_IRQn  			= 9,	// EXTI Line3 Interrupt
	EXTI4_IRQn  			= 10,	// EXTI Line4 Interrupt
	DMA1_Channel1_IRQn		= 11,	// DMA1 Channel 1 global Interrupt
	DMA1_Channel2_IRQn		= 12,	// DMA1 Channel 2 global Interrupt
	DMA1_Channel3_IRQn		= 13,	// DMA1 Channel 3 global Interrupt
	DMA1_Channel4_IRQn		= 14,	// DMA1 Channel 4 global Interrupt
	DMA1_Channel5_IRQn		= 15,	// DMA1 Channel 5 global Interrupt
	DMA1_Channel6_IRQn		= 16,	// DMA1 Channel 6 global Interrupt
	DMA1_Channel7_IRQn		= 17,	// DMA1 Channel 7 global Interrupt
	ADC1_2_IRQn 			= 18,	// ADC1, ADC2 SAR global Interrupts
	CAN1_TX_IRQn			= 19,	// CAN1 TX Interrupt
	CAN1_RX0_IRQn  			= 20,	// CAN1 RX0 Interrupt
	CAN1_RX1_IRQn  			= 21,	// CAN1 RX1 Interrupt
	CAN1_SCE_IRQn  			= 22,	// CAN1 SCE Interrupt
	EXTI9_5_IRQn			= 23,	// External Line[9:5] Interrupts
	TIM1_BRK_TIM15_IRQn		= 24,	// TIM1 Break interrupt and TIM15 global interrupt
	TIM1_UP_TIM16_IRQn		= 25,	// TIM1 Update Interrupt and TIM16 global interrupt
	TIM1_TRG_COM_TIM17_IRQn	= 26,	// TIM1 Trigger and Commutation Interrupt and TIM17 global interrupt 
	TIM1_CC_IRQn			= 27,	// TIM1 Capture Compare Interrupt
	TIM2_IRQn				= 28,	// TIM2 global Interrupt
	TIM3_IRQn				= 29,	// TIM3 global Interrupt
	TIM4_IRQn				= 30,	// TIM4 global Interrupt
	I2C1_EV_IRQn			= 31,	// I2C1 Event Interrupt
	I2C1_ER_IRQn			= 32,	// I2C1 Error Interrupt
	I2C2_EV_IRQn			= 33,	// I2C2 Event Interrupt
	I2C2_ER_IRQn			= 34,	// I2C2 Error Interrupt
	SPI1_IRQn				= 35,	// SPI1 global Interrupt
	SPI2_IRQn				= 36,	// SPI2 global Interrupt
	USART1_IRQn 			= 37,	// USART1 global Interrupt
	USART2_IRQn 			= 38,	// USART2 global Interrupt
	USART3_IRQn 			= 39,	// USART3 global Interrupt
	EXTI15_10_IRQn 			= 40,	// External Line[15:10] Interrupts
	RTC_Alarm_IRQn 			= 41,	// RTC Alarm (A and B) through EXTI Line Interrupt
	DFSDM1_FLT3_IRQn  		= 42,	// DFSDM1 Filter 3 global Interrupt
	TIM8_BRK_IRQn  			= 43,	// TIM8 Break Interrupt
	TIM8_UP_IRQn			= 44,	// TIM8 Update Interrupt
	TIM8_TRG_COM_IRQn 		= 45,	// TIM8 Trigger and Commutation Interrupt
	TIM8_CC_IRQn			= 46,	// TIM8 Capture Compare Interrupt
	ADC3_IRQn				= 47,	// ADC3 global  Interrupt
	FMC_IRQn 				= 48,	// FMC global Interrupt
	SDMMC1_IRQn 			= 49,	// SDMMC1 global Interrupt
	TIM5_IRQn				= 50,	// TIM5 global Interrupt
	SPI3_IRQn				= 51,	// SPI3 global Interrupt
	UART4_IRQn  			= 52,	// UART4 global Interrupt
	UART5_IRQn  			= 53,	// UART5 global Interrupt
	TIM6_DAC_IRQn			= 54,	// TIM6 global and DAC1&2 underrun error  interrupts
	TIM7_IRQn				= 55,	// TIM7 global interrupt
	DMA2_Channel1_IRQn		= 56,	// DMA2 Channel 1 global Interrupt
	DMA2_Channel2_IRQn		= 57,	// DMA2 Channel 2 global Interrupt
	DMA2_Channel3_IRQn		= 58,	// DMA2 Channel 3 global Interrupt
	DMA2_Channel4_IRQn		= 59,	// DMA2 Channel 4 global Interrupt
	DMA2_Channel5_IRQn		= 60,	// DMA2 Channel 5 global Interrupt
	DFSDM1_FLT0_IRQn  		= 61,	// DFSDM1 Filter 0 global Interrupt
	DFSDM1_FLT1_IRQn  		= 62,	// DFSDM1 Filter 1 global Interrupt
	DFSDM1_FLT2_IRQn  		= 63,	// DFSDM1 Filter 2 global Interrupt
	COMP_IRQn				= 64,	// COMP1 and COMP2 Interrupts
	LPTIM1_IRQn 			= 65,	// LP TIM1 interrupt
	LPTIM2_IRQn 			= 66,	// LP TIM2 interrupt
	OTG_FS_IRQn 			= 67,	// USB OTG FS global Interrupt
	DMA2_Channel6_IRQn		= 68,	// DMA2 Channel 6 global interrupt
	DMA2_Channel7_IRQn		= 69,	// DMA2 Channel 7 global interrupt
	LPUART1_IRQn			= 70,	// LP UART1 interrupt
	QUADSPI_IRQn			= 71,	// Quad SPI global interrupt
	I2C3_EV_IRQn			= 72,	// I2C3 event interrupt
	I2C3_ER_IRQn			= 73,	// I2C3 error interrupt
	SAI1_IRQn				= 74,	// Serial Audio Interface 1 global interrupt
	SAI2_IRQn				= 75,	// Serial Audio Interface 2 global interrupt
	SWPMI1_IRQn 			= 76,	// Serial Wire Interface 1 global interrupt
	TSC_IRQn 				= 77,	// Touch Sense Controller global interrupt
	LCD_IRQn				= 78,	// LCD global interrupt
	RNG_IRQn 				= 80,	// RNG global interrupt
	FPU_IRQn 				= 81	// FPU global interrupt
} IRQn_Type;

#define	KNBEXCEPTIONS		14
#define	KNBINTERRUPTIONS	82

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

// Exception

void	NMIException(void);
void	HardFaultException(void);
void	MemManageException(void);
void	BusFaultException(void);
void	UsageFaultException(void);
void	DebugMonitor(void);
void	SVCHandler(void);
void	PendSVCHandler(void);
void	SysTickHandler(void);

// IRQHandler

void	WWDG_IRQHandler(void);
void	PVD_PVM_IRQHandler(void);
void	TAMP_STAMP_IRQHandler(void);
void	RTC_WKUP_IRQHandler(void);
void	FLASH_IRQHandler(void);
void	RCC_IRQHandler(void);
void	EXTI0_IRQHandler(void);
void	EXTI1_IRQHandler(void);
void	EXTI2_IRQHandler(void);
void	EXTI3_IRQHandler(void);
void	EXTI4_IRQHandler(void);
void	DMA1_Channel1_IRQHandler(void);
void	DMA1_Channel2_IRQHandler(void);
void	DMA1_Channel3_IRQHandler(void);
void	DMA1_Channel4_IRQHandler(void);
void	DMA1_Channel5_IRQHandler(void);
void	DMA1_Channel6_IRQHandler(void);
void	DMA1_Channel7_IRQHandler(void);
void	ADC1_2_IRQHandler(void);
void	CAN1_TX_IRQHandler(void);
void	CAN1_RX0_IRQHandler(void);
void	CAN1_RX1_IRQHandler(void);
void	CAN1_SCE_IRQHandler(void);
void	EXTI9_5_IRQHandler(void);
void	TIM1_BRK_TIM15_IRQHandler(void);
void	TIM1_UP_TIM16_IRQHandler(void);
void	TIM1_TRG_COM_TIM17_IRQHandler(void);
void	TIM1_CC_IRQHandler(void);
void	TIM2_IRQHandler(void);
void	TIM3_IRQHandler(void);
void	TIM4_IRQHandler(void);
void	I2C1_EV_IRQHandler(void);
void	I2C1_ER_IRQHandler(void);
void	I2C2_EV_IRQHandler(void);
void	I2C2_ER_IRQHandler(void);
void	SPI1_IRQHandler(void);
void	SPI2_IRQHandler(void);
void	USART1_IRQHandler(void);
void	USART2_IRQHandler(void);
void	USART3_IRQHandler(void);
void	EXTI15_10_IRQHandler(void);
void	RTC_Alarm_IRQHandler(void);
void	DFSDM1_FLT3_IRQHandler(void);
void	TIM8_BRK_IRQHandler(void);
void	TIM8_UP_IRQHandler(void);
void	TIM8_TRG_COM_IRQHandler(void);
void	TIM8_CC_IRQHandler(void);
void	ADC3_IRQHandler(void);
void	FMC_IRQHandler(void);
void	SDMMC1_IRQHandler(void);
void	TIM5_IRQHandler(void);
void	SPI3_IRQHandler(void);
void	UART4_IRQHandler(void);
void	UART5_IRQHandler(void);
void	TIM6_DAC_IRQHandler(void);
void	TIM7_IRQHandler(void);
void	DMA2_Channel1_IRQHandler(void);
void	DMA2_Channel2_IRQHandler(void);
void	DMA2_Channel3_IRQHandler(void);
void	DMA2_Channel4_IRQHandler(void);
void	DMA2_Channel5_IRQHandler(void);
void	DFSDM1_FLT0_IRQHandler(void);
void	DFSDM1_FLT1_IRQHandler(void);
void	DFSDM1_FLT2_IRQHandler(void);
void	COMP_IRQHandler(void);
void	LPTIM1_IRQHandler(void);
void	LPTIM2_IRQHandler(void);
void	OTG_FS_IRQHandler(void);
void	DMA2_Channel6_IRQHandler(void);
void	DMA2_Channel7_IRQHandler(void);
void	LPUART1_IRQHandler(void);
void	QUADSPI_IRQHandler(void);
void	I2C3_EV_IRQHandler(void);
void	I2C3_ER_IRQHandler(void);
void	SAI1_IRQHandler(void);
void	SAI2_IRQHandler(void);
void	SWPMI1_IRQHandler(void);
void	TSC_IRQHandler(void);
void	LCD_IRQHandler(void);
void	RNG_IRQHandler(void);
void	FPU_IRQHandler(void);

#ifdef	__cplusplus
}
#endif

#endif
