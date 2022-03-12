/*
; stm32F103_gpio.
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
; Goal:		stm32F103_gpio equates.
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

#ifndef __STM32F103_GPIO__
#define __STM32F103_GPIO__

// GPIO address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	CRL;
	volatile	uint32_t	CRH;
	volatile	uint32_t	IDR;
	volatile	uint32_t	ODR;
	volatile	uint32_t	BSRR;
	volatile	uint32_t	BRR;
	volatile	uint32_t	LCKR;
} GPIO_TypeDef;

#define GPIOA	((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB	((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC	((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD	((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE	((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF	((GPIO_TypeDef *)GPIOF_BASE)
#define GPIOG	((GPIO_TypeDef *)GPIOG_BASE)

// GPIO specifications

#define	KPPOUT10M	0x1							//
#define	KPPOUT02M	0x2							//
#define	KPPOUT50M	0x3							//

#define	KODOUT10M	0x5							//
#define	KODOUT02M	0x6							//
#define	KODOUT50M	0x7							//

#define	KPPALT10M	0x9							//
#define	KPPALT02M	0xA							//
#define	KPPALT50M	0xB							//

#define	KODALT10M	0xD							//
#define	KODALT02M	0xE							//
#define	KODALT50M	0xF							//

#define	KANALOINP	0x0							//
#define	KFLOAOINP	0x4							//
#define	KPULDWINP	0x8							//
#define	KPULUPINP	0x8							//

// AFIO address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	EVCR;
	volatile	uint32_t	MAPR;
	volatile	uint32_t	EXTICR[4];
} AFIO_TypeDef;

#define AFIO	((AFIO_TypeDef *)AFIO_BASE)

// EXTI0 register

#define	AFIO_EXTICR1_EXTI0_PA		0x0000		//
#define	AFIO_EXTICR1_EXTI0_PB		0x0001		//
#define	AFIO_EXTICR1_EXTI0_PC		0x0002		//
#define	AFIO_EXTICR1_EXTI0_PD		0x0003		//
#define	AFIO_EXTICR1_EXTI0_PE		0x0004		//
#define	AFIO_EXTICR1_EXTI0_PF		0x0005		//
#define	AFIO_EXTICR1_EXTI0_PG		0x0006		//
#endif
