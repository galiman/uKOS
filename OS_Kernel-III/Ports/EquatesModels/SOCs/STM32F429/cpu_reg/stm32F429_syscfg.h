/*
; stm32F429_syscfg.
; =================

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
; Goal:		stm32F429_syscfg equates.
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

#ifndef __STM32F429_SYSCFG__
#define __STM32F429_SYSCFG__

// SYSCFG address definitions
// --------------------------

typedef struct {
	volatile	uint32_t	MEMRMP;
	volatile	uint32_t	PMC;
	volatile	uint32_t	EXTICR[4];
	volatile	uint32_t	RESERVED[2];
	volatile	uint32_t	CMPCR;
} SYSCFG_TypeDef;

#define	SYSCFG	((SYSCFG_TypeDef *)SYSCFG_BASE)

// SYSCFG_MEMRMP register

#define	SYSCFG_MEMRMP_MEM_MODE		0x00000003	//
#define	SYSCFG_MEMRMP_MEM_MODE_0	0x00000001	//
#define	SYSCFG_MEMRMP_MEM_MODE_1	0x00000002	//
#define	SYSCFG_MEMRMP_MEM_MODE_2	0x00000004	//
#define	SYSCFG_MEMRMP_FB_MODE		0x00000100	//
#define	SYSCFG_MEMRMP_SWP_FMC		0x00000C00	//
#define	SYSCFG_MEMRMP_SWP_FMC_0		0x00000400	//
#define	SYSCFG_MEMRMP_SWP_FMC_1		0x00000800	//

// SYSCFG_PMC register

#define	SYSCFG_PMC_ADCxDC2			0x00070000	//
#define	SYSCFG_PMC_ADC1DC2			0x00010000	//
#define	SYSCFG_PMC_ADC2DC2			0x00020000	//
#define	SYSCFG_PMC_ADC3DC2			0x00040000	//

// SYSCFG_EXTICR1 register

#define	SYSCFG_EXTICR1_EXTI0		0x000F		//
#define	SYSCFG_EXTICR1_EXTI1		0x00F0		//
#define	SYSCFG_EXTICR1_EXTI2		0x0F00		//
#define	SYSCFG_EXTICR1_EXTI3		0xF000		//

#define	SYSCFG_EXTICR1_EXTI0_PA		0x0000		//
#define	SYSCFG_EXTICR1_EXTI0_PB		0x0001		//
#define	SYSCFG_EXTICR1_EXTI0_PC		0x0002		//
#define	SYSCFG_EXTICR1_EXTI0_PD		0x0003		//
#define	SYSCFG_EXTICR1_EXTI0_PE		0x0004		//
#define	SYSCFG_EXTICR1_EXTI0_PF		0x0005		//
#define	SYSCFG_EXTICR1_EXTI0_PG		0x0006		//
#define	SYSCFG_EXTICR1_EXTI0_PH		0x0007		//
#define	SYSCFG_EXTICR1_EXTI0_PI		0x0008		//
#define	SYSCFG_EXTICR1_EXTI0_PJ		0x0009		//
#define	SYSCFG_EXTICR1_EXTI0_PK		0x000A		//

#define	SYSCFG_EXTICR1_EXTI1_PA		0x0000		//
#define	SYSCFG_EXTICR1_EXTI1_PB		0x0010		//
#define	SYSCFG_EXTICR1_EXTI1_PC		0x0020		//
#define	SYSCFG_EXTICR1_EXTI1_PD		0x0030		//
#define	SYSCFG_EXTICR1_EXTI1_PE		0x0040		//
#define	SYSCFG_EXTICR1_EXTI1_PF		0x0050		//
#define	SYSCFG_EXTICR1_EXTI1_PG		0x0060		//
#define	SYSCFG_EXTICR1_EXTI1_PH		0x0070		//
#define	SYSCFG_EXTICR1_EXTI1_PI		0x0080		//
#define	SYSCFG_EXTICR1_EXTI1_PJ		0x0090		//
#define	SYSCFG_EXTICR1_EXTI1_PK		0x00A0		//

#define	SYSCFG_EXTICR1_EXTI2_PA		0x0000		//
#define	SYSCFG_EXTICR1_EXTI2_PB		0x0100		//
#define	SYSCFG_EXTICR1_EXTI2_PC		0x0200		//
#define	SYSCFG_EXTICR1_EXTI2_PD		0x0300		//
#define	SYSCFG_EXTICR1_EXTI2_PE		0x0400		//
#define	SYSCFG_EXTICR1_EXTI2_PF		0x0500		//
#define	SYSCFG_EXTICR1_EXTI2_PG		0x0600		//
#define	SYSCFG_EXTICR1_EXTI2_PH		0x0700		//
#define	SYSCFG_EXTICR1_EXTI2_PI		0x0800		//
#define	SYSCFG_EXTICR1_EXTI2_PJ		0x0900		//
#define	SYSCFG_EXTICR1_EXTI2_PK		0x0A00		//

#define	SYSCFG_EXTICR1_EXTI3_PA		0x0000		//
#define	SYSCFG_EXTICR1_EXTI3_PB		0x1000		//
#define	SYSCFG_EXTICR1_EXTI3_PC		0x2000		//
#define	SYSCFG_EXTICR1_EXTI3_PD		0x3000		//
#define	SYSCFG_EXTICR1_EXTI3_PE		0x4000		//
#define	SYSCFG_EXTICR1_EXTI3_PF		0x5000		//
#define	SYSCFG_EXTICR1_EXTI3_PG		0x6000		//
#define	SYSCFG_EXTICR1_EXTI3_PH		0x7000		//
#define	SYSCFG_EXTICR1_EXTI3_PI		0x8000		//
#define	SYSCFG_EXTICR1_EXTI3_PJ		0x9000		//
#define	SYSCFG_EXTICR1_EXTI3_PK		0xA000		//

// SYSCFG_EXTICR2 register

#define	SYSCFG_EXTICR2_EXTI4		0x000F		//
#define	SYSCFG_EXTICR2_EXTI5		0x00F0		//
#define	SYSCFG_EXTICR2_EXTI6		0x0F00		//
#define	SYSCFG_EXTICR2_EXTI7		0xF000		//

#define	SYSCFG_EXTICR2_EXTI4_PA		0x0000		//
#define	SYSCFG_EXTICR2_EXTI4_PB		0x0001		//
#define	SYSCFG_EXTICR2_EXTI4_PC		0x0002		//
#define	SYSCFG_EXTICR2_EXTI4_PD		0x0003		//
#define	SYSCFG_EXTICR2_EXTI4_PE		0x0004		//
#define	SYSCFG_EXTICR2_EXTI4_PF		0x0005		//
#define	SYSCFG_EXTICR2_EXTI4_PG		0x0006		//
#define	SYSCFG_EXTICR2_EXTI4_PH		0x0007		//
#define	SYSCFG_EXTICR2_EXTI4_PI		0x0008		//
#define	SYSCFG_EXTICR2_EXTI4_PJ		0x0009		//
#define	SYSCFG_EXTICR2_EXTI4_PK		0x000A		//

#define	SYSCFG_EXTICR2_EXTI5_PA		0x0000		//
#define	SYSCFG_EXTICR2_EXTI5_PB		0x0010		//
#define	SYSCFG_EXTICR2_EXTI5_PC		0x0020		//
#define	SYSCFG_EXTICR2_EXTI5_PD		0x0030		//
#define	SYSCFG_EXTICR2_EXTI5_PE		0x0040		//
#define	SYSCFG_EXTICR2_EXTI5_PF		0x0050		//
#define	SYSCFG_EXTICR2_EXTI5_PG		0x0060		//
#define	SYSCFG_EXTICR2_EXTI5_PH		0x0070		//
#define	SYSCFG_EXTICR2_EXTI5_PI		0x0080		//
#define	SYSCFG_EXTICR2_EXTI5_PJ		0x0090		//
#define	SYSCFG_EXTICR2_EXTI5_PK		0x00A0		//

#define	SYSCFG_EXTICR2_EXTI6_PA		0x0000		//
#define	SYSCFG_EXTICR2_EXTI6_PB		0x0100		//
#define	SYSCFG_EXTICR2_EXTI6_PC		0x0200		//
#define	SYSCFG_EXTICR2_EXTI6_PD		0x0300		//
#define	SYSCFG_EXTICR2_EXTI6_PE		0x0400		//
#define	SYSCFG_EXTICR2_EXTI6_PF		0x0500		//
#define	SYSCFG_EXTICR2_EXTI6_PG		0x0600		//
#define	SYSCFG_EXTICR2_EXTI6_PH		0x0700		//
#define	SYSCFG_EXTICR2_EXTI6_PI		0x0800		//
#define	SYSCFG_EXTICR2_EXTI6_PJ		0x0900		//
#define	SYSCFG_EXTICR2_EXTI6_PK		0x0A00		//

#define	SYSCFG_EXTICR2_EXTI7_PA		0x0000		//
#define	SYSCFG_EXTICR2_EXTI7_PB		0x1000		//
#define	SYSCFG_EXTICR2_EXTI7_PC		0x2000		//
#define	SYSCFG_EXTICR2_EXTI7_PD		0x3000		//
#define	SYSCFG_EXTICR2_EXTI7_PE		0x4000		//
#define	SYSCFG_EXTICR2_EXTI7_PF		0x5000		//
#define	SYSCFG_EXTICR2_EXTI7_PG		0x6000		//
#define	SYSCFG_EXTICR2_EXTI7_PH		0x7000		//
#define	SYSCFG_EXTICR2_EXTI7_PI		0x8000		//
#define	SYSCFG_EXTICR2_EXTI7_PJ		0x9000		//
#define	SYSCFG_EXTICR2_EXTI7_PK		0xA000		//

// SYSCFG_EXTICR3 register

#define	SYSCFG_EXTICR3_EXTI8		0x000F		//
#define	SYSCFG_EXTICR3_EXTI9		0x00F0		//
#define	SYSCFG_EXTICR3_EXTI10		0x0F00		//
#define	SYSCFG_EXTICR3_EXTI11		0xF000		//

#define	SYSCFG_EXTICR3_EXTI8_PA		0x0000		//
#define	SYSCFG_EXTICR3_EXTI8_PB		0x0001		//
#define	SYSCFG_EXTICR3_EXTI8_PC		0x0002		//
#define	SYSCFG_EXTICR3_EXTI8_PD		0x0003		//
#define	SYSCFG_EXTICR3_EXTI8_PE		0x0004		//
#define	SYSCFG_EXTICR3_EXTI8_PF		0x0005		//
#define	SYSCFG_EXTICR3_EXTI8_PG		0x0006		//
#define	SYSCFG_EXTICR3_EXTI8_PH		0x0007		//
#define	SYSCFG_EXTICR3_EXTI8_PI		0x0008		//
#define	SYSCFG_EXTICR3_EXTI8_PJ		0x0009		//

#define	SYSCFG_EXTICR3_EXTI9_PA		0x0000		//
#define	SYSCFG_EXTICR3_EXTI9_PB		0x0010		//
#define	SYSCFG_EXTICR3_EXTI9_PC		0x0020		//
#define	SYSCFG_EXTICR3_EXTI9_PD		0x0030		//
#define	SYSCFG_EXTICR3_EXTI9_PE		0x0040		//
#define	SYSCFG_EXTICR3_EXTI9_PF		0x0050		//
#define	SYSCFG_EXTICR3_EXTI9_PG		0x0060		//
#define	SYSCFG_EXTICR3_EXTI9_PH		0x0070		//
#define	SYSCFG_EXTICR3_EXTI9_PI		0x0080		//
#define	SYSCFG_EXTICR3_EXTI9_PJ		0x0090		//

#define	SYSCFG_EXTICR3_EXTI10_PA	0x0000		//
#define	SYSCFG_EXTICR3_EXTI10_PB	0x0100		//
#define	SYSCFG_EXTICR3_EXTI10_PC	0x0200		//
#define	SYSCFG_EXTICR3_EXTI10_PD	0x0300		//
#define	SYSCFG_EXTICR3_EXTI10_PE	0x0400		//
#define	SYSCFG_EXTICR3_EXTI10_PF	0x0500		//
#define	SYSCFG_EXTICR3_EXTI10_PG	0x0600		//
#define	SYSCFG_EXTICR3_EXTI10_PH	0x0700		//
#define	SYSCFG_EXTICR3_EXTI10_PI	0x0800		//
#define	SYSCFG_EXTICR3_EXTI10_PJ	0x0900		//

#define	SYSCFG_EXTICR3_EXTI11_PA	0x0000		//
#define	SYSCFG_EXTICR3_EXTI11_PB	0x1000		//
#define	SYSCFG_EXTICR3_EXTI11_PC	0x2000		//
#define	SYSCFG_EXTICR3_EXTI11_PD	0x3000		//
#define	SYSCFG_EXTICR3_EXTI11_PE	0x4000		//
#define	SYSCFG_EXTICR3_EXTI11_PF	0x5000		//
#define	SYSCFG_EXTICR3_EXTI11_PG	0x6000		//
#define	SYSCFG_EXTICR3_EXTI11_PH	0x7000		//
#define	SYSCFG_EXTICR3_EXTI11_PI	0x8000		//
#define	SYSCFG_EXTICR3_EXTI11_PJ	0x9000		//

// SYSCFG_EXTICR4 register

#define	SYSCFG_EXTICR4_EXTI12		0x000F		//
#define	SYSCFG_EXTICR4_EXTI13		0x00F0		//
#define	SYSCFG_EXTICR4_EXTI14		0x0F00		//
#define	SYSCFG_EXTICR4_EXTI15		0xF000		//

#define	SYSCFG_EXTICR4_EXTI12_PA	0x0000		//
#define	SYSCFG_EXTICR4_EXTI12_PB	0x0001		//
#define	SYSCFG_EXTICR4_EXTI12_PC	0x0002		//
#define	SYSCFG_EXTICR4_EXTI12_PD	0x0003		//
#define	SYSCFG_EXTICR4_EXTI12_PE	0x0004		//
#define	SYSCFG_EXTICR4_EXTI12_PF	0x0005		//
#define	SYSCFG_EXTICR4_EXTI12_PG	0x0006		//
#define	SYSCFG_EXTICR4_EXTI12_PH	0x0007		//
#define	SYSCFG_EXTICR4_EXTI12_PI	0x0008		//
#define	SYSCFG_EXTICR4_EXTI12_PJ	0x0009		//

#define	SYSCFG_EXTICR4_EXTI13_PA	0x0000		//
#define	SYSCFG_EXTICR4_EXTI13_PB	0x0010		//
#define	SYSCFG_EXTICR4_EXTI13_PC	0x0020		//
#define	SYSCFG_EXTICR4_EXTI13_PD	0x0030		//
#define	SYSCFG_EXTICR4_EXTI13_PE	0x0040		//
#define	SYSCFG_EXTICR4_EXTI13_PF	0x0050		//
#define	SYSCFG_EXTICR4_EXTI13_PG	0x0060		//
#define	SYSCFG_EXTICR4_EXTI13_PH	0x0070		//
#define	SYSCFG_EXTICR4_EXTI13_PI	0x0080		//
#define	SYSCFG_EXTICR4_EXTI13_PJ	0x0090		//

#define	SYSCFG_EXTICR4_EXTI14_PA	0x0000		//
#define	SYSCFG_EXTICR4_EXTI14_PB	0x0100		//
#define	SYSCFG_EXTICR4_EXTI14_PC	0x0200		//
#define	SYSCFG_EXTICR4_EXTI14_PD	0x0300		//
#define	SYSCFG_EXTICR4_EXTI14_PE	0x0400		//
#define	SYSCFG_EXTICR4_EXTI14_PF	0x0500		//
#define	SYSCFG_EXTICR4_EXTI14_PG	0x0600		//
#define	SYSCFG_EXTICR4_EXTI14_PH	0x0700		//
#define	SYSCFG_EXTICR4_EXTI14_PI	0x0800		//
#define	SYSCFG_EXTICR4_EXTI14_PJ	0x0900		//

#define	SYSCFG_EXTICR4_EXTI15_PA	0x0000		//
#define	SYSCFG_EXTICR4_EXTI15_PB	0x1000		//
#define	SYSCFG_EXTICR4_EXTI15_PC	0x2000		//
#define	SYSCFG_EXTICR4_EXTI15_PD	0x3000		//
#define	SYSCFG_EXTICR4_EXTI15_PE	0x4000		//
#define	SYSCFG_EXTICR4_EXTI15_PF	0x5000		//
#define	SYSCFG_EXTICR4_EXTI15_PG	0x6000		//
#define	SYSCFG_EXTICR4_EXTI15_PH	0x7000		//
#define	SYSCFG_EXTICR4_EXTI15_PI	0x8000		//
#define	SYSCFG_EXTICR4_EXTI15_PJ	0x9000		//

// SYSCFG_CMPCR register

#define	SYSCFG_CMPCR_CMP_PD			0x00000001	//
#define	SYSCFG_CMPCR_READY			0x00000100	//
#endif
