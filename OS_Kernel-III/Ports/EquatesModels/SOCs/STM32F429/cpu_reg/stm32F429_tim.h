/*
; stm32F429_tim.
; ==============

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
; Goal:		stm32F429_tim equates.
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

#ifndef __STM32F429_TIM__
#define __STM32F429_TIM__

// TIM address definitions
// -----------------------

typedef struct {
	volatile	uint16_t	CR1;
	volatile	uint16_t	RESERVED0;
	volatile	uint16_t	CR2;
	volatile	uint16_t	RESERVED1;
	volatile	uint16_t	SMCR;
	volatile	uint16_t	RESERVED2;
	volatile	uint16_t	DIER;
	volatile	uint16_t	RESERVED3;
	volatile	uint16_t	SR;
	volatile	uint16_t	RESERVED4;
	volatile	uint16_t	EGR;
	volatile	uint16_t	RESERVED5;
	volatile	uint16_t	CCMR1;
	volatile	uint16_t	RESERVED6;
	volatile	uint16_t	CCMR2;
	volatile	uint16_t	RESERVED7;
	volatile	uint16_t	CCER;
	volatile	uint16_t	RESERVED8;
	volatile	uint32_t	CNT;
	volatile	uint16_t	PSC;
	volatile	uint16_t	RESERVED9;
	volatile	uint32_t	ARR;
	volatile	uint16_t	RCR;
	volatile	uint16_t	RESERVED10;
	volatile	uint32_t	CCR1;
	volatile	uint32_t	CCR2;
	volatile	uint32_t	CCR3;
	volatile	uint32_t	CCR4;
	volatile	uint16_t	BDTR;
	volatile	uint16_t	RESERVED11;
	volatile	uint16_t	DCR;
	volatile	uint16_t	RESERVED12;
	volatile	uint16_t	DMAR;
	volatile	uint16_t	RESERVED13;
	volatile	uint16_t	OR;
	volatile	uint16_t	RESERVED14;
} TIM_TypeDef;

#define	TIM1	((TIM_TypeDef *)TIM1_BASE)
#define	TIM2	((TIM_TypeDef *)TIM2_BASE)
#define	TIM3	((TIM_TypeDef *)TIM3_BASE)
#define	TIM4	((TIM_TypeDef *)TIM4_BASE)
#define	TIM5	((TIM_TypeDef *)TIM5_BASE)
#define	TIM6	((TIM_TypeDef *)TIM6_BASE)
#define	TIM7	((TIM_TypeDef *)TIM7_BASE)
#define	TIM8	((TIM_TypeDef *)TIM8_BASE)
#define	TIM9	((TIM_TypeDef *)TIM9_BASE)
#define	TIM10	((TIM_TypeDef *)TIM10_BASE)
#define	TIM11	((TIM_TypeDef *)TIM11_BASE)
#define	TIM12	((TIM_TypeDef *)TIM12_BASE)
#define	TIM13	((TIM_TypeDef *)TIM13_BASE)
#define	TIM14	((TIM_TypeDef *)TIM14_BASE)

// TIM_CR1 register

#define	TIM_CR1_CEN				0x0001		//
#define	TIM_CR1_UDIS			0x0002		//
#define	TIM_CR1_URS				0x0004		//
#define	TIM_CR1_OPM				0x0008		//
#define	TIM_CR1_DIR				0x0010		//
#define	TIM_CR1_CMS				0x0060		//
#define	TIM_CR1_CMS_0			0x0020		//
#define	TIM_CR1_CMS_1			0x0040		//
#define	TIM_CR1_ARPE			0x0080		//
#define	TIM_CR1_CKD				0x0300		//
#define	TIM_CR1_CKD_0			0x0100		//
#define	TIM_CR1_CKD_1			0x0200		//

// TIM_CR2 register

#define	TIM_CR2_CCPC			0x0001		//
#define	TIM_CR2_CCUS			0x0004		//
#define	TIM_CR2_CCDS			0x0008		//
#define	TIM_CR2_MMS				0x0070		//
#define	TIM_CR2_MMS_0			0x0010		//
#define	TIM_CR2_MMS_1			0x0020		//
#define	TIM_CR2_MMS_2			0x0040		//
#define	TIM_CR2_TI1S			0x0080		//
#define	TIM_CR2_OIS1			0x0100		//
#define	TIM_CR2_OIS1N			0x0200		//
#define	TIM_CR2_OIS2			0x0400		//
#define	TIM_CR2_OIS2N			0x0800		//
#define	TIM_CR2_OIS3			0x1000		//
#define	TIM_CR2_OIS3N			0x2000		//
#define	TIM_CR2_OIS4			0x4000		//

// TIM_SMCR register

#define	TIM_SMCR_SMS			0x0007		//
#define	TIM_SMCR_SMS_0			0x0001		//
#define	TIM_SMCR_SMS_1			0x0002		//
#define	TIM_SMCR_SMS_2			0x0004		//
#define	TIM_SMCR_TS				0x0070		//
#define	TIM_SMCR_TS_0			0x0010		//
#define	TIM_SMCR_TS_1			0x0020		//
#define	TIM_SMCR_TS_2			0x0040		//
#define	TIM_SMCR_MSM			0x0080		//
#define	TIM_SMCR_ETF			0x0F00		//
#define	TIM_SMCR_ETF_0			0x0100		//
#define	TIM_SMCR_ETF_1			0x0200		//
#define	TIM_SMCR_ETF_2			0x0400		//
#define	TIM_SMCR_ETF_3			0x0800		//
#define	TIM_SMCR_ETPS			0x3000		//
#define	TIM_SMCR_ETPS_0			0x1000		//
#define	TIM_SMCR_ETPS_1			0x2000		//
#define	TIM_SMCR_ECE			0x4000		//
#define	TIM_SMCR_ETP			0x8000		//

// TIM_DIER register

#define	TIM_DIER_UIE			0x0001		//
#define	TIM_DIER_CC1IE			0x0002		//
#define	TIM_DIER_CC2IE			0x0004		//
#define	TIM_DIER_CC3IE			0x0008		//
#define	TIM_DIER_CC4IE			0x0010		//
#define	TIM_DIER_COMIE			0x0020		//
#define	TIM_DIER_TIE			0x0040		//
#define	TIM_DIER_BIE			0x0080		//
#define	TIM_DIER_UDE			0x0100		//
#define	TIM_DIER_CC1DE			0x0200		//
#define	TIM_DIER_CC2DE			0x0400		//
#define	TIM_DIER_CC3DE			0x0800		//
#define	TIM_DIER_CC4DE			0x1000		//
#define	TIM_DIER_COMDE			0x2000		//
#define	TIM_DIER_TDE			0x4000		//

// TIM_SR register

#define	TIM_SR_UIF 				0x0001		//
#define	TIM_SR_CC1IF			0x0002		//
#define	TIM_SR_CC2IF			0x0004		//
#define	TIM_SR_CC3IF			0x0008		//
#define	TIM_SR_CC4IF			0x0010		//
#define	TIM_SR_COMIF			0x0020		//
#define	TIM_SR_TIF 				0x0040		//
#define	TIM_SR_BIF 				0x0080		//
#define	TIM_SR_CC1OF			0x0200		//
#define	TIM_SR_CC2OF			0x0400		//
#define	TIM_SR_CC3OF			0x0800		//
#define	TIM_SR_CC4OF			0x1000		//

// TIM_EGR register

#define	TIM_EGR_UG				0x01		//
#define	TIM_EGR_CC1G			0x02		//
#define	TIM_EGR_CC2G			0x04		//
#define	TIM_EGR_CC3G			0x08		//
#define	TIM_EGR_CC4G			0x10		//
#define	TIM_EGR_COMG			0x20		//
#define	TIM_EGR_TG				0x40		//
#define	TIM_EGR_BG				0x80		//

// TIM_CCMR1 register

#define	TIM_CCMR1_CC1S			0x0003		//
#define	TIM_CCMR1_CC1S_0		0x0001		//
#define	TIM_CCMR1_CC1S_1		0x0002		//
#define	TIM_CCMR1_OC1FE			0x0004		//
#define	TIM_CCMR1_OC1PE			0x0008		//
#define	TIM_CCMR1_OC1M			0x0070		//
#define	TIM_CCMR1_OC1M_0		0x0010		//
#define	TIM_CCMR1_OC1M_1		0x0020		//
#define	TIM_CCMR1_OC1M_2		0x0040		//
#define	TIM_CCMR1_OC1CE			0x0080		//
#define	TIM_CCMR1_CC2S			0x0300		//
#define	TIM_CCMR1_CC2S_0		0x0100		//
#define	TIM_CCMR1_CC2S_1		0x0200		//
#define	TIM_CCMR1_OC2FE			0x0400		//
#define	TIM_CCMR1_OC2PE			0x0800		//
#define	TIM_CCMR1_OC2M			0x7000		//
#define	TIM_CCMR1_OC2M_0		0x1000		//
#define	TIM_CCMR1_OC2M_1		0x2000		//
#define	TIM_CCMR1_OC2M_2		0x4000		//
#define	TIM_CCMR1_OC2CE			0x8000		//
#define	TIM_CCMR1_IC1PSC		0x000C		//
#define	TIM_CCMR1_IC1PSC_0		0x0004		//
#define	TIM_CCMR1_IC1PSC_1		0x0008		//
#define	TIM_CCMR1_IC1F			0x00F0		//
#define	TIM_CCMR1_IC1F_0		0x0010		//
#define	TIM_CCMR1_IC1F_1		0x0020		//
#define	TIM_CCMR1_IC1F_2		0x0040		//
#define	TIM_CCMR1_IC1F_3		0x0080		//
#define	TIM_CCMR1_IC2PSC		0x0C00		//
#define	TIM_CCMR1_IC2PSC_0		0x0400		//
#define	TIM_CCMR1_IC2PSC_1		0x0800		//
#define	TIM_CCMR1_IC2F			0xF000		//
#define	TIM_CCMR1_IC2F_0		0x1000		//
#define	TIM_CCMR1_IC2F_1		0x2000		//
#define	TIM_CCMR1_IC2F_2		0x4000		//
#define	TIM_CCMR1_IC2F_3		0x8000		//

// TIM_CCMR2 register

#define	TIM_CCMR2_CC3S			0x0003		//
#define	TIM_CCMR2_CC3S_0		0x0001		//
#define	TIM_CCMR2_CC3S_1		0x0002		//
#define	TIM_CCMR2_OC3FE			0x0004		//
#define	TIM_CCMR2_OC3PE			0x0008		//
#define	TIM_CCMR2_OC3M			0x0070		//
#define	TIM_CCMR2_OC3M_0		0x0010		//
#define	TIM_CCMR2_OC3M_1		0x0020		//
#define	TIM_CCMR2_OC3M_2		0x0040		//
#define	TIM_CCMR2_OC3CE			0x0080		//
#define	TIM_CCMR2_CC4S			0x0300		//
#define	TIM_CCMR2_CC4S_0		0x0100		//
#define	TIM_CCMR2_CC4S_1		0x0200		//
#define	TIM_CCMR2_OC4FE			0x0400		//
#define	TIM_CCMR2_OC4PE			0x0800		//
#define	TIM_CCMR2_OC4M			0x7000		//
#define	TIM_CCMR2_OC4M_0		0x1000		//
#define	TIM_CCMR2_OC4M_1		0x2000		//
#define	TIM_CCMR2_OC4M_2		0x4000		//
#define	TIM_CCMR2_OC4CE			0x8000		//
#define	TIM_CCMR2_IC3PSC		0x000C		//
#define	TIM_CCMR2_IC3PSC_0		0x0004		//
#define	TIM_CCMR2_IC3PSC_1		0x0008		//
#define	TIM_CCMR2_IC3F			0x00F0		//
#define	TIM_CCMR2_IC3F_0		0x0010		//
#define	TIM_CCMR2_IC3F_1		0x0020		//
#define	TIM_CCMR2_IC3F_2		0x0040		//
#define	TIM_CCMR2_IC3F_3		0x0080		//
#define	TIM_CCMR2_IC4PSC		0x0C00		//
#define	TIM_CCMR2_IC4PSC_0		0x0400		//
#define	TIM_CCMR2_IC4PSC_1		0x0800		//
#define	TIM_CCMR2_IC4F			0xF000		//
#define	TIM_CCMR2_IC4F_0		0x1000		//
#define	TIM_CCMR2_IC4F_1		0x2000		//
#define	TIM_CCMR2_IC4F_2		0x4000		//
#define	TIM_CCMR2_IC4F_3		0x8000		//

// TIM_CCER register

#define	TIM_CCER_CC1E			0x0001		//
#define	TIM_CCER_CC1P			0x0002		//
#define	TIM_CCER_CC1NE			0x0004		//
#define	TIM_CCER_CC1NP			0x0008		//
#define	TIM_CCER_CC2E			0x0010		//
#define	TIM_CCER_CC2P			0x0020		//
#define	TIM_CCER_CC2NE			0x0040		//
#define	TIM_CCER_CC2NP			0x0080		//
#define	TIM_CCER_CC3E			0x0100		//
#define	TIM_CCER_CC3P			0x0200		//
#define	TIM_CCER_CC3NE			0x0400		//
#define	TIM_CCER_CC3NP			0x0800		//
#define	TIM_CCER_CC4E			0x1000		//
#define	TIM_CCER_CC4P			0x2000		//
#define	TIM_CCER_CC4NP			0x8000		//

// TIM_CNT register

#define	TIM_CNT_CNT				0xFFFF		//

// TIM_PSC register

#define	TIM_PSC_PSC				0xFFFF		//

// TIM_ARR register

#define	TIM_ARR_ARR				0xFFFF		//

// TIM_RCR register

#define	TIM_RCR_REP 			0xFF		//

// TIM_CCR1 register

#define	TIM_CCR1_CCR1			0xFFFF		//

// TIM_CCR2 register

#define	TIM_CCR2_CCR2			0xFFFF		//

// TIM_CCR3 register

#define	TIM_CCR3_CCR3			0xFFFF		//

// TIM_CCR4 register

#define	TIM_CCR4_CCR4			0xFFFF		//

// TIM_BDTR register

#define	TIM_BDTR_DTG			0x00FF		//
#define	TIM_BDTR_DTG_0			0x0001		//
#define	TIM_BDTR_DTG_1			0x0002		//
#define	TIM_BDTR_DTG_2			0x0004		//
#define	TIM_BDTR_DTG_3			0x0008		//
#define	TIM_BDTR_DTG_4			0x0010		//
#define	TIM_BDTR_DTG_5			0x0020		//
#define	TIM_BDTR_DTG_6			0x0040		//
#define	TIM_BDTR_DTG_7			0x0080		//
#define	TIM_BDTR_LOCK			0x0300		//
#define	TIM_BDTR_LOCK_0			0x0100		//
#define	TIM_BDTR_LOCK_1			0x0200		//
#define	TIM_BDTR_OSSI			0x0400		//
#define	TIM_BDTR_OSSR			0x0800		//
#define	TIM_BDTR_BKE			0x1000		//
#define	TIM_BDTR_BKP			0x2000		//
#define	TIM_BDTR_AOE			0x4000		//
#define	TIM_BDTR_MOE			0x8000		//

// TIM_DCR register

#define	TIM_DCR_DBA				0x001F		//
#define	TIM_DCR_DBA_0			0x0001		//
#define	TIM_DCR_DBA_1			0x0002		//
#define	TIM_DCR_DBA_2			0x0004		//
#define	TIM_DCR_DBA_3			0x0008		//
#define	TIM_DCR_DBA_4			0x0010		//
#define	TIM_DCR_DBL				0x1F00		//
#define	TIM_DCR_DBL_0			0x0100		//
#define	TIM_DCR_DBL_1			0x0200		//
#define	TIM_DCR_DBL_2			0x0400		//
#define	TIM_DCR_DBL_3			0x0800		//
#define	TIM_DCR_DBL_4			0x1000		//

// TIM_DMAR register

#define	TIM_DMAR_DMAB			0xFFFF		//

// TIM_OR register

#define	TIM_OR_TI4_RMP			0x00C0		//
#define	TIM_OR_TI4_RMP_0		0x0040		//
#define	TIM_OR_TI4_RMP_1		0x0080		//
#define	TIM_OR_ITR1_RMP 		0x0C00		//
#define	TIM_OR_ITR1_RMP_0		0x0400		//
#define	TIM_OR_ITR1_RMP_1		0x0800		//
#endif
