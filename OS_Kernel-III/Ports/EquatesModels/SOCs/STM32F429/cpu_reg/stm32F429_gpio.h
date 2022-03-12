/*
; stm32F429_gpio.
; ===============

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
; Goal:		stm32F429_gpio equates.
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

#ifndef __STM32F429_GPIO__
#define __STM32F429_GPIO__

// GPIO address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	MODER;
	volatile	uint32_t	OTYPER;
	volatile	uint32_t	OSPEEDR;
	volatile	uint32_t	PUPDR;
	volatile	uint32_t	IDR;
	volatile	uint32_t	ODR;
	volatile	uint16_t	BSRRL;
	volatile	uint16_t	BSRRH;
	volatile	uint32_t	LCKR;
	volatile	uint32_t	AFR[2];
} GPIO_TypeDef;

#define GPIOA	((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB	((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC	((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD	((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE	((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF	((GPIO_TypeDef *)GPIOF_BASE)
#define GPIOG	((GPIO_TypeDef *)GPIOG_BASE)
#define GPIOH	((GPIO_TypeDef *)GPIOH_BASE)
#define GPIOI	((GPIO_TypeDef *)GPIOI_BASE)
#define GPIOJ	((GPIO_TypeDef *)GPIOJ_BASE)
#define GPIOK	((GPIO_TypeDef *)GPIOK_BASE)

// GPIO for MODE configuration

#define	KIN		0x0								// Input (reset state)
#define	KOU		0x1								// General purpose output
#define	KAL		0x2								// Alternate function
#define	KAN		0x3								// Analog function

// GPIO for OTYPER configuration

#define	KPP		0x0								// Output Push-pull
#define	KOD		0x1								// Output Open Drain

// GPIO for OSPEEDR configuration

#define	K02		0x0								// Low speed 2-MHz
#define	K25		0x1								// Medium speed 25-MHz
#define	K50		0x2								// Fast speed 50-MHz
#define	K99		0x3								// High speed 100-MHz

// GPIO for PUPDR configuration

#define	KNO		0x0								// No pull-up, pull-down
#define	KPU		0x1								// Pull-up
#define	KPD		0x2								// Pull-down
#define	KRE		0x3								// Reserved

// GPIO for AFR[x] configuration

#define	A00		0								//
#define	A01		1								//
#define	A02		2								//
#define	A03		3								//
#define	A04		4								//
#define	A05		5								//
#define	A06		6								//
#define	A07		7								//
#define	A08		8								//
#define	A09		9								//
#define	A10		10								//
#define	A11		11								//
#define	A12		12								//
#define	A13		13								//
#define	A14		14								//
#define	A15		15								//

// AFIO address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	EVCR;
	volatile	uint32_t	MAPR;
	volatile	uint32_t	EXTICR[4];
	volatile	uint32_t	RESERVED0;
	volatile	uint32_t	MAPR2;
} AFIO_TypeDef;

#define AFIO	((AFIO_TypeDef *)AFIO_BASE)

// EXTI0 register

#define	AFIO_EXTICR1_EXTI0_PA		0x0000		// PA[0] pin
#define	AFIO_EXTICR1_EXTI0_PB		0x0001		// PB[0] pin
#define	AFIO_EXTICR1_EXTI0_PC		0x0002		// PC[0] pin
#define	AFIO_EXTICR1_EXTI0_PD		0x0003		// PD[0] pin
#define	AFIO_EXTICR1_EXTI0_PE		0x0004		// PE[0] pin
#define	AFIO_EXTICR1_EXTI0_PF		0x0005		// PF[0] pin
#define	AFIO_EXTICR1_EXTI0_PG		0x0006		// PG[0] pin
#endif
