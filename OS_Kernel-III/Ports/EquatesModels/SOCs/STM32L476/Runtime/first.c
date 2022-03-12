/*
; first.
; ======

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2007-05-27
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Vectors for the uKOS system (first).
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

#include	<uKOS.h>

// Vector table: ....
// However rather than start at zero the vector table starts at address 0x00000004,
// the first four bytes are used to store the starting address of the stack pointer.

extern				void		(*vExce_indExcVectors[KNBEXCEPTIONS])(void);
extern				void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
extern	volatile	uint16_t	vTimeException;
extern				uint32_t	_stStMSP(void);
extern				int32_t		_start(void);

__attribute__ ((section(".isr_vector")))
void	(* const g_pfnVectors[])(void) = {

	(void (*)(void))_stStMSP,		// MSP Stack !! We simulate the stack position as a "function"
									// This is just to avoid a compiler warning (ISO)

// Cortex-M4 Processor Exceptions Numbers

	(void (*)(void))_start,			// Address: 0x0000_0004
	NMIException,					// Address: 0x0000_0008
	HardFaultException,				// Address: 0x0000_000C
	MemManageException,				// Address: 0x0000_0010
	BusFaultException,				// Address: 0x0000_0014
	UsageFaultException,			// Address: 0x0000_0018
	0,								// Address: 0x0000_001C
	0,								// Address: 0x0000_0020
	0,								// Address: 0x0000_0024
	0,								// Address: 0x0000_0028
	SVCHandler,						// Address: 0x0000_002C
	DebugMonitor,					// Address: 0x0000_0030
	0,								// Address: 0x0000_0034
	PendSVCHandler,					// Address: 0x0000_0038
	SysTickHandler,					// Address: 0x0000_003C

// STM32 specific Interrupt Numbers

	WWDG_IRQHandler,				// Address: 0x0000_0040
	PVD_PVM_IRQHandler,				// Address: 0x0000_0044
	TAMP_STAMP_IRQHandler,			// Address: 0x0000_0048
	RTC_WKUP_IRQHandler,			// Address: 0x0000_004C
	FLASH_IRQHandler,				// Address: 0x0000_0050
	RCC_IRQHandler,					// Address: 0x0000_0054
	EXTI0_IRQHandler,				// Address: 0x0000_0058
	EXTI1_IRQHandler,				// Address: 0x0000_005C
	EXTI2_IRQHandler,				// Address: 0x0000_0060
	EXTI3_IRQHandler,				// Address: 0x0000_0064
	EXTI4_IRQHandler,				// Address: 0x0000_0068
	DMA1_Channel1_IRQHandler,		// Address: 0x0000_006C
	DMA1_Channel2_IRQHandler,		// Address: 0x0000_0070
	DMA1_Channel3_IRQHandler,		// Address: 0x0000_0074
	DMA1_Channel4_IRQHandler,		// Address: 0x0000_0078
	DMA1_Channel5_IRQHandler,		// Address: 0x0000_007C
	DMA1_Channel6_IRQHandler,		// Address: 0x0000_0080
	DMA1_Channel7_IRQHandler,		// Address: 0x0000_0084
	ADC1_2_IRQHandler,				// Address: 0x0000_0088 
	CAN1_TX_IRQHandler,				// Address: 0x0000_008C
	CAN1_RX0_IRQHandler,			// Address: 0x0000_0090
	CAN1_RX1_IRQHandler,			// Address: 0x0000_0094
	CAN1_SCE_IRQHandler,			// Address: 0x0000_0098
	EXTI9_5_IRQHandler,				// Address: 0x0000_009C
	TIM1_BRK_TIM15_IRQHandler,		// Address: 0x0000_00A0
	TIM1_UP_TIM16_IRQHandler,		// Address: 0x0000_00A4
	TIM1_TRG_COM_TIM17_IRQHandler,	// Address: 0x0000_00A8
	TIM1_CC_IRQHandler,				// Address: 0x0000_00AC
	TIM2_IRQHandler,				// Address: 0x0000_00B0
	TIM3_IRQHandler,				// Address: 0x0000_00B4
	TIM4_IRQHandler,				// Address: 0x0000_00B8
	I2C1_EV_IRQHandler,				// Address: 0x0000_00BC
	I2C1_ER_IRQHandler,				// Address: 0x0000_00C0
	I2C2_EV_IRQHandler,				// Address: 0x0000_00C4
	I2C2_ER_IRQHandler,				// Address: 0x0000_00C8
	SPI1_IRQHandler,				// Address: 0x0000_00CC
	SPI2_IRQHandler,				// Address: 0x0000_00D0
	USART1_IRQHandler,				// Address: 0x0000_00D4
	USART2_IRQHandler,				// Address: 0x0000_00D8
	USART3_IRQHandler,				// Address: 0x0000_00DC
	EXTI15_10_IRQHandler,			// Address: 0x0000_00E0
	RTC_Alarm_IRQHandler,			// Address: 0x0000_00E4
	DFSDM1_FLT3_IRQHandler,			// Address: 0x0000_00E8
	TIM8_BRK_IRQHandler,			// Address: 0x0000_00EC
	TIM8_UP_IRQHandler,				// Address: 0x0000_00F0
	TIM8_TRG_COM_IRQHandler,		// Address: 0x0000_00F4
	TIM8_CC_IRQHandler,				// Address: 0x0000_00F8
	ADC3_IRQHandler,				// Address: 0x0000_00FC
	FMC_IRQHandler,					// Address: 0x0000_0100
	SDMMC1_IRQHandler,				// Address: 0x0000_0104
	TIM5_IRQHandler,				// Address: 0x0000_0108
	SPI3_IRQHandler,				// Address: 0x0000_010C
	UART4_IRQHandler,				// Address: 0x0000_0110
	UART5_IRQHandler,				// Address: 0x0000_0114
	TIM6_DAC_IRQHandler,			// Address: 0x0000_0118
	TIM7_IRQHandler,				// Address: 0x0000_011C
	DMA2_Channel1_IRQHandler,		// Address: 0x0000_0120
	DMA2_Channel2_IRQHandler,		// Address: 0x0000_0124
	DMA2_Channel3_IRQHandler,		// Address: 0x0000_0128
	DMA2_Channel4_IRQHandler,		// Address: 0x0000_012C
	DMA2_Channel5_IRQHandler,		// Address: 0x0000_0130
	DFSDM1_FLT0_IRQHandler,			// Address: 0x0000_0134
	DFSDM1_FLT1_IRQHandler,			// Address: 0x0000_0138
	DFSDM1_FLT2_IRQHandler,			// Address: 0x0000_013C
	COMP_IRQHandler,				// Address: 0x0000_0140
	LPTIM1_IRQHandler,				// Address: 0x0000_0144
	LPTIM2_IRQHandler,				// Address: 0x0000_0148
	OTG_FS_IRQHandler,				// Address: 0x0000_014C
	DMA2_Channel6_IRQHandler,		// Address: 0x0000_0150
	DMA2_Channel7_IRQHandler,		// Address: 0x0000_0154
	LPUART1_IRQHandler,				// Address: 0x0000_0158
	QUADSPI_IRQHandler,				// Address: 0x0000_015C
	I2C3_EV_IRQHandler,				// Address: 0x0000_0160
	I2C3_ER_IRQHandler,				// Address: 0x0000_0164
	SAI1_IRQHandler,				// Address: 0x0000_0168
	SAI2_IRQHandler,				// Address: 0x0000_016C
	SWPMI1_IRQHandler,				// Address: 0x0000_0170
	TSC_IRQHandler,					// Address: 0x0000_0174
	LCD_IRQHandler,					// Address: 0x0000_0178
	0,								// Reserved
	RNG_IRQHandler,					// Address: 0x0000_0180
	FPU_IRQHandler					// Address: 0x0000_0184
};

/*
 * \brief Exception handels
 *
 * \param[in]	-
 * \param[out]	-
 *
 */

// Cortex-M4 Processor Exceptions Numbers

void	NMIException(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[0];
	(*go)(0);
}

void	HardFaultException(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[1];
	(*go)(1);
}

void	MemManageException(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[2];
	(*go)(2);
}

void	BusFaultException(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[3];
	(*go)(3);
}

void	UsageFaultException(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[4];
	(*go)(4);
}

void	SVCHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[9];
	(*go)(9);
}

void	DebugMonitor(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[10];
	(*go)(10);
}

void	PendSVCHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[12];
	(*go)(12);
}

void	SysTickHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	go = (void (*)(uint8_t exceptionNb))vExce_indExcVectors[13];
	(*go)(13);
}

// STM32 specific Interrupt Numbers

void	WWDG_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[WWDG_IRQn];
	(*go)(WWDG_IRQn);
	TAC_EXCEPTION_TIME;
}

void	PVD_PVM_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[PVD_PVM_IRQn];
	(*go)(PVD_PVM_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TAMP_STAMP_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TAMP_STAMP_IRQn];
	(*go)(TAMP_STAMP_IRQn);
	TAC_EXCEPTION_TIME;
}

void	RTC_WKUP_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[RTC_WKUP_IRQn];
	(*go)(RTC_WKUP_IRQn);
	TAC_EXCEPTION_TIME;
}

void	FLASH_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[FLASH_IRQn];
	(*go)(FLASH_IRQn);
	TAC_EXCEPTION_TIME;
}

void	RCC_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[RCC_IRQn];
	(*go)(RCC_IRQn);
	TAC_EXCEPTION_TIME;
}

void	EXTI0_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI0_IRQn];
	(*go)(EXTI0_IRQn);
	TAC_EXCEPTION_TIME;
}

void	EXTI1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI1_IRQn];
	(*go)(EXTI1_IRQn);
	TAC_EXCEPTION_TIME;
}

// Kernel entry vector:
// mandatory to maintain a clean stack-frame

#if	(defined(__NO_KERNEL__))
void	EXTI2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI2_IRQn];
	(*go)(EXTI2_IRQn);
	TAC_EXCEPTION_TIME;
}
#endif

// Kernel entry vector:
// mandatory to maintain a clean stack-frame

#if	(defined(__NO_KERNEL__))
void	EXTI3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI3_IRQn];
	(*go)(EXTI3_IRQn);
	TAC_EXCEPTION_TIME;
}
#endif

void	EXTI4_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI4_IRQn];
	(*go)(EXTI4_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel1_IRQn];
	(*go)(DMA1_Channel1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel2_IRQn];
	(*go)(DMA1_Channel2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel3_IRQn];
	(*go)(DMA1_Channel3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel4_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel4_IRQn];
	(*go)(DMA1_Channel4_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel5_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel5_IRQn];
	(*go)(DMA1_Channel5_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel6_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel6_IRQn];
	(*go)(DMA1_Channel6_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA1_Channel7_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA1_Channel7_IRQn];
	(*go)(DMA1_Channel7_IRQn);
	TAC_EXCEPTION_TIME;
}

void	ADC1_2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[ADC1_2_IRQn];
	(*go)(ADC1_2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	CAN1_TX_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[CAN1_TX_IRQn];
	(*go)(CAN1_TX_IRQn);
	TAC_EXCEPTION_TIME;
}

void	CAN1_RX0_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[CAN1_RX0_IRQn];
	(*go)(CAN1_RX0_IRQn);
	TAC_EXCEPTION_TIME;
}

void	CAN1_RX1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[CAN1_RX1_IRQn];
	(*go)(CAN1_RX1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	CAN1_SCE_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[CAN1_SCE_IRQn];
	(*go)(CAN1_SCE_IRQn);
	TAC_EXCEPTION_TIME;
}

void	EXTI9_5_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI9_5_IRQn];
	(*go)(EXTI9_5_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM1_BRK_TIM15_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM1_BRK_TIM15_IRQn];
	(*go)(TIM1_BRK_TIM15_IRQn);
	TAC_EXCEPTION_TIME;
}

// Kernel entry vector:
// mandatory to maintain a clean stack-frame

#if	(defined(__NO_KERNEL__))
void	TIM1_UP_TIM16_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM1_UP_TIM16_IRQn];
	(*go)(TIM1_UP_TIM16_IRQn);
	TAC_EXCEPTION_TIME;
}
#endif

void	TIM1_TRG_COM_TIM17_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM1_TRG_COM_TIM17_IRQn];
	(*go)(TIM1_TRG_COM_TIM17_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM1_CC_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM1_CC_IRQn];
	(*go)(TIM1_CC_IRQn);
	TAC_EXCEPTION_TIME;
}

// Kernel entry vector:
// mandatory to maintain a clean stack-frame

#if	(defined(__NO_KERNEL__))
void	TIM2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM2_IRQn];
	(*go)(TIM2_IRQn);
	TAC_EXCEPTION_TIME;
}
#endif

void	TIM3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM3_IRQn];
	(*go)(TIM3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM4_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM4_IRQn];
	(*go)(TIM4_IRQn);
	TAC_EXCEPTION_TIME;
}

void	I2C1_EV_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[I2C1_EV_IRQn];
	(*go)(I2C1_EV_IRQn);
	TAC_EXCEPTION_TIME;
}

void	I2C1_ER_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[I2C1_ER_IRQn];
	(*go)(I2C1_ER_IRQn);
	TAC_EXCEPTION_TIME;
}

void	I2C2_EV_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[I2C2_EV_IRQn];
	(*go)(I2C2_EV_IRQn);
	TAC_EXCEPTION_TIME;
}

void	I2C2_ER_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[I2C2_ER_IRQn];
	(*go)(I2C2_ER_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SPI1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SPI1_IRQn];
	(*go)(SPI1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SPI2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SPI2_IRQn];
	(*go)(SPI2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	USART1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[USART1_IRQn];
	(*go)(USART1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	USART2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[USART2_IRQn];
	(*go)(USART2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	USART3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[USART3_IRQn];
	(*go)(USART3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	EXTI15_10_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[EXTI15_10_IRQn];
	(*go)(EXTI15_10_IRQn);
	TAC_EXCEPTION_TIME;
}

void	RTC_Alarm_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[RTC_Alarm_IRQn];
	(*go)(RTC_Alarm_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DFSDM1_FLT3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DFSDM1_FLT3_IRQn];
	(*go)(DFSDM1_FLT3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM8_BRK_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM8_BRK_IRQn];
	(*go)(TIM8_BRK_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM8_UP_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM8_UP_IRQn];
	(*go)(TIM8_UP_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM8_TRG_COM_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM8_TRG_COM_IRQn];
	(*go)(TIM8_TRG_COM_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM8_CC_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM8_CC_IRQn];
	(*go)(TIM8_CC_IRQn);
	TAC_EXCEPTION_TIME;
}

void	ADC3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[ADC3_IRQn];
	(*go)(ADC3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	FMC_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[FMC_IRQn];
	(*go)(FMC_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SDMMC1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SDMMC1_IRQn];
	(*go)(SDMMC1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM5_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM5_IRQn];
	(*go)(TIM5_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SPI3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SPI3_IRQn];
	(*go)(SPI3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	UART4_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[UART4_IRQn];
	(*go)(UART4_IRQn);
	TAC_EXCEPTION_TIME;
}

void	UART5_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[UART5_IRQn];
	(*go)(UART5_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM6_DAC_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM6_DAC_IRQn];
	(*go)(TIM6_DAC_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TIM7_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TIM7_IRQn];
	(*go)(TIM7_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel1_IRQn];
	(*go)(DMA2_Channel1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel2_IRQn];
	(*go)(DMA2_Channel2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel3_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel3_IRQn];
	(*go)(DMA2_Channel3_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel4_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel4_IRQn];
	(*go)(DMA2_Channel4_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel5_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel5_IRQn];
	(*go)(DMA2_Channel5_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DFSDM1_FLT0_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DFSDM1_FLT0_IRQn];
	(*go)(DFSDM1_FLT0_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DFSDM1_FLT1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DFSDM1_FLT1_IRQn];
	(*go)(DFSDM1_FLT1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DFSDM1_FLT2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DFSDM1_FLT2_IRQn];
	(*go)(DFSDM1_FLT2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	COMP_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[COMP_IRQn];
	(*go)(COMP_IRQn);
	TAC_EXCEPTION_TIME;
}

void	LPTIM1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[LPTIM1_IRQn];
	(*go)(LPTIM1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	LPTIM2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[LPTIM2_IRQn];
	(*go)(LPTIM2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	OTG_FS_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[OTG_FS_IRQn];
	(*go)(OTG_FS_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel6_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel6_IRQn];
	(*go)(DMA2_Channel6_IRQn);
	TAC_EXCEPTION_TIME;
}

void	DMA2_Channel7_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[DMA2_Channel7_IRQn];
	(*go)(DMA2_Channel7_IRQn);
	TAC_EXCEPTION_TIME;
}

void	LPUART1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[LPUART1_IRQn];
	(*go)(LPUART1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	QUADSPI_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[QUADSPI_IRQn];
	(*go)(QUADSPI_IRQn);
	TAC_EXCEPTION_TIME;
}

void	I2C3_EV_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[I2C3_EV_IRQn];
	(*go)(I2C3_EV_IRQn);
	TAC_EXCEPTION_TIME;
}

void	I2C3_ER_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[I2C3_ER_IRQn];
	(*go)(I2C3_ER_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SAI1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SAI1_IRQn];
	(*go)(SAI1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SAI2_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SAI2_IRQn];
	(*go)(SAI2_IRQn);
	TAC_EXCEPTION_TIME;
}

void	SWPMI1_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[SWPMI1_IRQn];
	(*go)(SWPMI1_IRQn);
	TAC_EXCEPTION_TIME;
}

void	TSC_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[TSC_IRQn];
	(*go)(TSC_IRQn);
	TAC_EXCEPTION_TIME;
}

void	LCD_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[LCD_IRQn];
	(*go)(LCD_IRQn);
	TAC_EXCEPTION_TIME;
}

void	RNG_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[RNG_IRQn];
	(*go)(RNG_IRQn);
	TAC_EXCEPTION_TIME;
}

void	FPU_IRQHandler(void) {
	void	(*go)(uint8_t exceptionNb);

	TIC_EXCEPTION_TIME;
	go = (void (*)(uint8_t exceptionNb))vExce_indIntVectors[FPU_IRQn];
	(*go)(FPU_IRQn);
	TAC_EXCEPTION_TIME;
}

