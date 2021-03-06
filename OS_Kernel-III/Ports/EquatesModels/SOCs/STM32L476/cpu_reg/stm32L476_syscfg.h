/*
; stm32L476_syscfg.
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
; Goal:		stm32L476_syscfg equates.
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

#ifndef __STM32L476_SYSCFG__
#define __STM32L476_SYSCFG__

// SYSCFG address definitions
// --------------------------

typedef struct {
	volatile	uint32_t	MEMRMP;
	volatile	uint32_t	CFGR1;
	volatile	uint32_t	EXTICR[4];
	volatile	uint32_t	SCSR;
	volatile	uint32_t	CFGR2;
	volatile	uint32_t	SWPR;
	volatile	uint32_t	SKR;
} SYSCFG_TypeDef;

#define	SYSCFG	((SYSCFG_TypeDef *)SYSCFG_BASE)

// SYSCFG_MEMRMP register

#define	SYSCFG_MEMRMP_MEM_MODE		0x00000007	//
#define	SYSCFG_MEMRMP_MEM_MODE_0	0x00000001	//
#define	SYSCFG_MEMRMP_MEM_MODE_1	0x00000002	//
#define	SYSCFG_MEMRMP_MEM_MODE_2	0x00000004	//
#define	SYSCFG_MEMRMP_FB_MODE   	0x00000100	//

// SYSCFG_CFGR1 register

#define	SYSCFG_CFGR1_FWDIS 			0x00000001	//
#define	SYSCFG_CFGR1_BOOSTEN    	0x00000100	//
#define	SYSCFG_CFGR1_I2C_PB6_FMP	0x00010000	//
#define	SYSCFG_CFGR1_I2C_PB7_FMP	0x00020000	//
#define	SYSCFG_CFGR1_I2C_PB8_FMP	0x00040000	//
#define	SYSCFG_CFGR1_I2C_PB9_FMP	0x00080000	//
#define	SYSCFG_CFGR1_I2C1_FMP   	0x00100000	//
#define	SYSCFG_CFGR1_I2C2_FMP   	0x00200000	//
#define	SYSCFG_CFGR1_I2C3_FMP   	0x00400000	//
#define	SYSCFG_CFGR1_FPU_IE_0   	0x04000000	//
#define	SYSCFG_CFGR1_FPU_IE_1   	0x08000000	//
#define	SYSCFG_CFGR1_FPU_IE_2   	0x10000000	//
#define	SYSCFG_CFGR1_FPU_IE_3   	0x20000000	//
#define	SYSCFG_CFGR1_FPU_IE_4   	0x40000000	//
#define	SYSCFG_CFGR1_FPU_IE_5   	0x80000000	//

// SYSCFG_EXTICR1 register

#define	SYSCFG_EXTICR1_EXTI0    	0x00000007	//
#define	SYSCFG_EXTICR1_EXTI1    	0x00000070	//
#define	SYSCFG_EXTICR1_EXTI2    	0x00000700	//
#define	SYSCFG_EXTICR1_EXTI3    	0x00007000	//

#define	SYSCFG_EXTICR1_EXTI0_PA		0x00000000	//
#define	SYSCFG_EXTICR1_EXTI0_PB		0x00000001	//
#define	SYSCFG_EXTICR1_EXTI0_PC		0x00000002	//
#define	SYSCFG_EXTICR1_EXTI0_PD		0x00000003	//
#define	SYSCFG_EXTICR1_EXTI0_PE		0x00000004	//
#define	SYSCFG_EXTICR1_EXTI0_PF		0x00000005	//
#define	SYSCFG_EXTICR1_EXTI0_PG		0x00000006	//
#define	SYSCFG_EXTICR1_EXTI0_PH		0x00000007	//

#define	SYSCFG_EXTICR1_EXTI1_PA		0x00000000	//
#define	SYSCFG_EXTICR1_EXTI1_PB		0x00000010	//
#define	SYSCFG_EXTICR1_EXTI1_PC		0x00000020	//
#define	SYSCFG_EXTICR1_EXTI1_PD		0x00000030	//
#define	SYSCFG_EXTICR1_EXTI1_PE		0x00000040	//
#define	SYSCFG_EXTICR1_EXTI1_PF		0x00000050	//
#define	SYSCFG_EXTICR1_EXTI1_PG		0x00000060	//
#define	SYSCFG_EXTICR1_EXTI1_PH		0x00000070	//

#define	SYSCFG_EXTICR1_EXTI2_PA		0x00000000	//
#define	SYSCFG_EXTICR1_EXTI2_PB		0x00000100	//
#define	SYSCFG_EXTICR1_EXTI2_PC		0x00000200	//
#define	SYSCFG_EXTICR1_EXTI2_PD		0x00000300	//
#define	SYSCFG_EXTICR1_EXTI2_PE		0x00000400	//
#define	SYSCFG_EXTICR1_EXTI2_PF		0x00000500	//
#define	SYSCFG_EXTICR1_EXTI2_PG		0x00000600	//

#define	SYSCFG_EXTICR1_EXTI3_PA		0x00000000	//
#define	SYSCFG_EXTICR1_EXTI3_PB		0x00001000	//
#define	SYSCFG_EXTICR1_EXTI3_PC		0x00002000	//
#define	SYSCFG_EXTICR1_EXTI3_PD		0x00003000	//
#define	SYSCFG_EXTICR1_EXTI3_PE		0x00004000	//
#define	SYSCFG_EXTICR1_EXTI3_PF		0x00005000	//
#define	SYSCFG_EXTICR1_EXTI3_PG		0x00006000	//

// SYSCFG_EXTICR2 register

#define	SYSCFG_EXTICR2_EXTI4    	0x00000007	//
#define	SYSCFG_EXTICR2_EXTI5    	0x00000070	//
#define	SYSCFG_EXTICR2_EXTI6    	0x00000700	//
#define	SYSCFG_EXTICR2_EXTI7    	0x00007000	//

#define	SYSCFG_EXTICR2_EXTI4_PA		0x00000000	//
#define	SYSCFG_EXTICR2_EXTI4_PB		0x00000001	//
#define	SYSCFG_EXTICR2_EXTI4_PC		0x00000002	//
#define	SYSCFG_EXTICR2_EXTI4_PD		0x00000003	//
#define	SYSCFG_EXTICR2_EXTI4_PE		0x00000004	//
#define	SYSCFG_EXTICR2_EXTI4_PF		0x00000005	//
#define	SYSCFG_EXTICR2_EXTI4_PG		0x00000006	//

#define	SYSCFG_EXTICR2_EXTI5_PA		0x00000000	//
#define	SYSCFG_EXTICR2_EXTI5_PB		0x00000010	//
#define	SYSCFG_EXTICR2_EXTI5_PC		0x00000020	//
#define	SYSCFG_EXTICR2_EXTI5_PD		0x00000030	//
#define	SYSCFG_EXTICR2_EXTI5_PE		0x00000040	//
#define	SYSCFG_EXTICR2_EXTI5_PF		0x00000050	//
#define	SYSCFG_EXTICR2_EXTI5_PG		0x00000060	//

#define	SYSCFG_EXTICR2_EXTI6_PA		0x00000000	//
#define	SYSCFG_EXTICR2_EXTI6_PB		0x00000100	//
#define	SYSCFG_EXTICR2_EXTI6_PC		0x00000200	//
#define	SYSCFG_EXTICR2_EXTI6_PD		0x00000300	//
#define	SYSCFG_EXTICR2_EXTI6_PE		0x00000400	//
#define	SYSCFG_EXTICR2_EXTI6_PF		0x00000500	//
#define	SYSCFG_EXTICR2_EXTI6_PG		0x00000600	//

#define	SYSCFG_EXTICR2_EXTI7_PA		0x00000000	//
#define	SYSCFG_EXTICR2_EXTI7_PB		0x00001000	//
#define	SYSCFG_EXTICR2_EXTI7_PC		0x00002000	//
#define	SYSCFG_EXTICR2_EXTI7_PD		0x00003000	//
#define	SYSCFG_EXTICR2_EXTI7_PE		0x00004000	//
#define	SYSCFG_EXTICR2_EXTI7_PF		0x00005000	//
#define	SYSCFG_EXTICR2_EXTI7_PG		0x00006000	//

// SYSCFG_EXTICR3 register

#define	SYSCFG_EXTICR3_EXTI8    	0x00000007	//
#define	SYSCFG_EXTICR3_EXTI9    	0x00000070	//
#define	SYSCFG_EXTICR3_EXTI10   	0x00000700	//
#define	SYSCFG_EXTICR3_EXTI11   	0x00007000	//

#define	SYSCFG_EXTICR3_EXTI8_PA		0x00000000	//
#define	SYSCFG_EXTICR3_EXTI8_PB		0x00000001	//
#define	SYSCFG_EXTICR3_EXTI8_PC		0x00000002	//
#define	SYSCFG_EXTICR3_EXTI8_PD		0x00000003	//
#define	SYSCFG_EXTICR3_EXTI8_PE		0x00000004	//
#define	SYSCFG_EXTICR3_EXTI8_PF		0x00000005	//
#define	SYSCFG_EXTICR3_EXTI8_PG		0x00000006	//

#define	SYSCFG_EXTICR3_EXTI9_PA		0x00000000	//
#define	SYSCFG_EXTICR3_EXTI9_PB		0x00000010	//
#define	SYSCFG_EXTICR3_EXTI9_PC		0x00000020	//
#define	SYSCFG_EXTICR3_EXTI9_PD		0x00000030	//
#define	SYSCFG_EXTICR3_EXTI9_PE		0x00000040	//
#define	SYSCFG_EXTICR3_EXTI9_PF		0x00000050	//
#define	SYSCFG_EXTICR3_EXTI9_PG		0x00000060	//

#define	SYSCFG_EXTICR3_EXTI10_PA	0x00000000	//
#define	SYSCFG_EXTICR3_EXTI10_PB	0x00000100	//
#define	SYSCFG_EXTICR3_EXTI10_PC	0x00000200	//
#define	SYSCFG_EXTICR3_EXTI10_PD	0x00000300	//
#define	SYSCFG_EXTICR3_EXTI10_PE	0x00000400	//
#define	SYSCFG_EXTICR3_EXTI10_PF	0x00000500	//
#define	SYSCFG_EXTICR3_EXTI10_PG	0x00000600	//

#define	SYSCFG_EXTICR3_EXTI11_PA	0x00000000	//
#define	SYSCFG_EXTICR3_EXTI11_PB	0x00001000	//
#define	SYSCFG_EXTICR3_EXTI11_PC	0x00002000	//
#define	SYSCFG_EXTICR3_EXTI11_PD	0x00003000	//
#define	SYSCFG_EXTICR3_EXTI11_PE	0x00004000	//
#define	SYSCFG_EXTICR3_EXTI11_PF	0x00005000	//
#define	SYSCFG_EXTICR3_EXTI11_PG	0x00006000	//

// SYSCFG_EXTICR4 register

#define	SYSCFG_EXTICR4_EXTI12		0x00000007	//
#define	SYSCFG_EXTICR4_EXTI13		0x00000070	//
#define	SYSCFG_EXTICR4_EXTI14		0x00000700	//
#define	SYSCFG_EXTICR4_EXTI15		0x00007000	//

#define	SYSCFG_EXTICR4_EXTI12_PA	0x00000000	//
#define	SYSCFG_EXTICR4_EXTI12_PB	0x00000001	//
#define	SYSCFG_EXTICR4_EXTI12_PC	0x00000002	//
#define	SYSCFG_EXTICR4_EXTI12_PD	0x00000003	//
#define	SYSCFG_EXTICR4_EXTI12_PE	0x00000004	//
#define	SYSCFG_EXTICR4_EXTI12_PF	0x00000005	//
#define	SYSCFG_EXTICR4_EXTI12_PG	0x00000006	//

#define	SYSCFG_EXTICR4_EXTI13_PA	0x00000000	//
#define	SYSCFG_EXTICR4_EXTI13_PB	0x00000010	//
#define	SYSCFG_EXTICR4_EXTI13_PC	0x00000020	//
#define	SYSCFG_EXTICR4_EXTI13_PD	0x00000030	//
#define	SYSCFG_EXTICR4_EXTI13_PE	0x00000040	//
#define	SYSCFG_EXTICR4_EXTI13_PF	0x00000050	//
#define	SYSCFG_EXTICR4_EXTI13_PG	0x00000060	//

#define	SYSCFG_EXTICR4_EXTI14_PA	0x00000000	//
#define	SYSCFG_EXTICR4_EXTI14_PB	0x00000100	//
#define	SYSCFG_EXTICR4_EXTI14_PC	0x00000200	//
#define	SYSCFG_EXTICR4_EXTI14_PD	0x00000300	//
#define	SYSCFG_EXTICR4_EXTI14_PE	0x00000400	//
#define	SYSCFG_EXTICR4_EXTI14_PF	0x00000500	//
#define	SYSCFG_EXTICR4_EXTI14_PG	0x00000600	//

#define	SYSCFG_EXTICR4_EXTI15_PA	0x00000000	//
#define	SYSCFG_EXTICR4_EXTI15_PB	0x00001000	//
#define	SYSCFG_EXTICR4_EXTI15_PC	0x00002000	//
#define	SYSCFG_EXTICR4_EXTI15_PD	0x00003000	//
#define	SYSCFG_EXTICR4_EXTI15_PE	0x00004000	//
#define	SYSCFG_EXTICR4_EXTI15_PF	0x00005000	//
#define	SYSCFG_EXTICR4_EXTI15_PG	0x00006000	//

// SYSCFG_SCSR register

#define	SYSCFG_SCSR_SRAM2ER			0x00000001	//
#define	SYSCFG_SCSR_SRAM2BSY		0x00000002	//

// SYSCFG_CFGR2 register

#define	SYSCFG_CFGR2_CLL			0x00000001	//
#define	SYSCFG_CFGR2_SPL			0x00000002	//
#define	SYSCFG_CFGR2_PVDL			0x00000004	//
#define	SYSCFG_CFGR2_ECCL			0x00000008	//
#define	SYSCFG_CFGR2_SPF			0x00000100	//

// SYSCFG_SWPR register

#define	SYSCFG_SWPR_PAGE0			0x00000001	//
#define	SYSCFG_SWPR_PAGE1			0x00000002	//
#define	SYSCFG_SWPR_PAGE2			0x00000004	//
#define	SYSCFG_SWPR_PAGE3			0x00000008	//
#define	SYSCFG_SWPR_PAGE4			0x00000010	//
#define	SYSCFG_SWPR_PAGE5			0x00000020	//
#define	SYSCFG_SWPR_PAGE6			0x00000040	//
#define	SYSCFG_SWPR_PAGE7			0x00000080	//
#define	SYSCFG_SWPR_PAGE8			0x00000100	//
#define	SYSCFG_SWPR_PAGE9			0x00000200	//
#define	SYSCFG_SWPR_PAGE10			0x00000400	//
#define	SYSCFG_SWPR_PAGE11			0x00000800	//
#define	SYSCFG_SWPR_PAGE12			0x00001000	//
#define	SYSCFG_SWPR_PAGE13			0x00002000	//
#define	SYSCFG_SWPR_PAGE14			0x00004000	//
#define	SYSCFG_SWPR_PAGE15			0x00008000	//
#define	SYSCFG_SWPR_PAGE16			0x00010000	//
#define	SYSCFG_SWPR_PAGE17			0x00020000	//
#define	SYSCFG_SWPR_PAGE18			0x00040000	//
#define	SYSCFG_SWPR_PAGE19			0x00080000	//
#define	SYSCFG_SWPR_PAGE20			0x00100000	//
#define	SYSCFG_SWPR_PAGE21			0x00200000	//
#define	SYSCFG_SWPR_PAGE22			0x00400000	//
#define	SYSCFG_SWPR_PAGE23			0x00800000	//
#define	SYSCFG_SWPR_PAGE24			0x01000000	//
#define	SYSCFG_SWPR_PAGE25			0x02000000	//
#define	SYSCFG_SWPR_PAGE26			0x04000000	//
#define	SYSCFG_SWPR_PAGE27			0x08000000	//
#define	SYSCFG_SWPR_PAGE28			0x10000000	//
#define	SYSCFG_SWPR_PAGE29			0x20000000	//
#define	SYSCFG_SWPR_PAGE30			0x40000000	//
#define	SYSCFG_SWPR_PAGE31			0x80000000	//

// SYSCFG_SKR register

#define	SYSCFG_SKR_KEY				0x000000FF	//
#endif
