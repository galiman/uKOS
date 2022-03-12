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
; Goal:		Indirect Exception Vectors for M3.
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

// Cortex-M3 Processor Exceptions Numbers

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

	WWDG_IRQn				= 0,  	// Window WatchDog Interrupt
	PVD_IRQn				= 1,  	// PVD through EXTI Line detection Interrupt
	TAMPER_IRQn				= 2,  	// Tamper Interrupt
	RTC_IRQn				= 3,  	// RTC global Interrupt
	FLASH_IRQn				= 4,  	// FLASH global Interrupt
	RCC_IRQn				= 5,  	// RCC global Interrupt
	EXTI0_IRQn				= 6,  	// EXTI Line0 Interrupt
	EXTI1_IRQn				= 7,  	// EXTI Line1 Interrupt
	EXTI2_IRQn				= 8,  	// EXTI Line2 Interrupt
	EXTI3_IRQn				= 9,  	// EXTI Line3 Interrupt
	EXTI4_IRQn				= 10, 	// EXTI Line4 Interrupt
	DMA1_Stream1_IRQn		= 11, 	// DMA1 Channel 1 global Interrupt
	DMA1_Stream2_IRQn		= 12, 	// DMA1 Channel 2 global Interrupt
	DMA1_Stream3_IRQn		= 13, 	// DMA1 Channel 3 global Interrupt
	DMA1_Stream4_IRQn		= 14, 	// DMA1 Channel 4 global Interrupt
	DMA1_Stream5_IRQn		= 15, 	// DMA1 Channel 5 global Interrupt
	DMA1_Stream6_IRQn		= 16, 	// DMA1 Channel 6 global Interrupt
	DMA1_Channel7_IRQn		= 17, 	// DMA1 Channel 7 global Interrupt
	ADC1_2_IRQn				= 18, 	// ADC1 and ADC2 global Interrupt
	USB_HP_CAN1_TX_IRQn		= 19, 	// USB Device High Priority or CAN1 TX Interrupts
	USB_LP_CAN1_RX0_IRQn 	= 20, 	// USB Device Low Priority or CAN1 RX0 Interrupts
	CAN1_RX1_IRQn			= 21, 	// CAN1 RX1 Interrupt
	CAN1_SCE_IRQn			= 22, 	// CAN1 SCE Interrupt
	EXTI9_5_IRQn 			= 23, 	// External Line[9:5] Interrupts
	TIM1_BRK_IRQn			= 24, 	// TIM1 Break Interrupt
	TIM1_UP_IRQn			= 25, 	// TIM1 Update Interrupt
	TIM1_TRG_COM_IRQn		= 26, 	// TIM1 Trigger and Commutation Interrupt
	TIM1_CC_IRQn			= 27, 	// TIM1 Capture Compare Interrupt
	TIM2_IRQn				= 28, 	// TIM2 global Interrupt
	TIM3_IRQn				= 29, 	// TIM3 global Interrupt
	TIM4_IRQn				= 30, 	// TIM4 global Interrupt
	I2C1_EV_IRQn			= 31, 	// I2C1 Event Interrupt
	I2C1_ER_IRQn			= 32, 	// I2C1 Error Interrupt
	I2C2_EV_IRQn			= 33, 	// I2C2 Event Interrupt
	I2C2_ER_IRQn			= 34, 	// I2C2 Error Interrupt
	SPI1_IRQn				= 35, 	// SPI1 global Interrupt
	SPI2_IRQn				= 36, 	// SPI2 global Interrupt
	USART1_IRQn				= 37, 	// USART1 global Interrupt
	USART2_IRQn				= 38, 	// USART2 global Interrupt
	USART3_IRQn				= 39, 	// USART3 global Interrupt
	EXTI15_10_IRQn			= 40, 	// External Line[15:10] Interrupts
	RTCAlarm_IRQn			= 41, 	// RTC Alarm through EXTI Line Interrupt
	USBWakeUp_IRQn			= 42, 	// USB Device WakeUp from suspend through EXTI Line Interrupt */
	TIM8_BRK_IRQn			= 43, 	// TIM8 Break Interrupt
	TIM8_UP_IRQn			= 44, 	// TIM8 Update Interrupt
	TIM8_TRG_COM_IRQn		= 45, 	// TIM8 Trigger and Commutation Interrupt
	TIM8_CC_IRQn			= 46, 	// TIM8 Capture Compare Interrupt
	ADC3_IRQn				= 47, 	// ADC3 global Interrupt
	FSMC_IRQn				= 48, 	// FSMC global Interrupt
	SDIO_IRQn				= 49, 	// SDIO global Interrupt
	TIM5_IRQn				= 50, 	// TIM5 global Interrupt
	SPI3_IRQn				= 51, 	// SPI3 global Interrupt
	UART4_IRQn				= 52, 	// UART4 global Interrupt
	UART5_IRQn				= 53, 	// UART5 global Interrupt
	TIM6_IRQn				= 54, 	// TIM6 global Interrupt
	TIM7_IRQn				= 55, 	// TIM7 global Interrupt
	DMA2_Channel1_IRQn		= 56, 	// DMA2 Channel 1 global Interrupt
	DMA2_Channel2_IRQn		= 57, 	// DMA2 Channel 2 global Interrupt
	DMA2_Channel3_IRQn		= 58, 	// DMA2 Channel 3 global Interrupt
	DMA2_Channel4_5_IRQn 	= 59  	// DMA2 Channel 4 and Channel 5 global Interrupt
} IRQn_Type;

#define	KNBEXCEPTIONS		14
#define	KNBINTERRUPTIONS	60

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
void	PVD_IRQHandler(void);
void	TAMPER_IRQHandler(void);
void	RTC_IRQHandler(void);
void	FLASH_IRQHandler(void);
void	RCC_IRQHandler(void);
void	EXTI0_IRQHandler(void);
void	EXTI1_IRQHandler(void);
void	EXTI2_IRQHandler(void);
void	EXTI3_IRQHandler(void);
void	EXTI4_IRQHandler(void);
void	DMAChannel1_IRQHandler(void);
void	DMAChannel2_IRQHandler(void);
void	DMAChannel3_IRQHandler(void);
void	DMAChannel4_IRQHandler(void);
void	DMAChannel5_IRQHandler(void);
void	DMAChannel6_IRQHandler(void);
void	DMAChannel7_IRQHandler(void);
void	ADC1_2_IRQHandler(void);
void	USB_HP_CAN_TX_IRQHandler(void);
void	USB_LP_CAN_RX0_IRQHandler(void);
void	CAN_RX1_IRQHandler(void);
void	CAN_SCE_IRQHandler(void);
void	EXTI9_5_IRQHandler(void);
void	TIM1_BRK_IRQHandler(void);
void	TIM1_UP_IRQHandler(void);
void	TIM1_TRG_COM_IRQHandler(void);
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
void	RTCAlarm_IRQHandler(void);
void	USBWakeUp_IRQHandler(void);
void	TIM8_BRK_IRQHandler(void);
void	TIM8_UP_IRQHandler(void);
void	TIM8_TRG_COM_IRQHandler(void);
void	TIM8_CC_IRQHandler(void);
void	ADC3_IRQHandler(void);
void	FSMC_IRQHandler(void);
void	SDIO_IRQHandler(void);
void	TIM5_IRQHandler(void);
void	SPI3_IRQHandler(void);
void	UART4_IRQHandler(void);
void	UART5_IRQHandler(void);
void	TIM6_IRQHandler(void);
void	TIM7_IRQHandler(void);
void	DMA2Channel1_IRQHandler(void);
void	DMA2Channel2_IRQHandler(void);
void	DMA2Channel3_IRQHandler(void);
void	DMA2Channel4_5_IRQHandler(void);

#ifdef	__cplusplus
}
#endif

#endif
