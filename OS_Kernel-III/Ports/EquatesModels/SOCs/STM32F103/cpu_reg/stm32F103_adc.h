/*
; stm32F103_adc.
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
; Goal:		stm32F103_adc equates.
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

#ifndef __STM32F103_ADC__
#define __STM32F103_ADC__

// ADC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	SR;
	volatile	uint32_t	CR1;
	volatile	uint32_t	CR2;
	volatile	uint32_t	SMPR1;
	volatile	uint32_t	SMPR2;
	volatile	uint32_t	JOFR1;
	volatile	uint32_t	JOFR2;
	volatile	uint32_t	JOFR3;
	volatile	uint32_t	JOFR4;
	volatile	uint32_t	HTR;
	volatile	uint32_t	LTR;
	volatile	uint32_t	SQR1;
	volatile	uint32_t	SQR2;
	volatile	uint32_t	SQR3;
	volatile	uint32_t	JSQR;
	volatile	uint32_t	JDR1;
	volatile	uint32_t	JDR2;
	volatile	uint32_t	JDR3;
	volatile	uint32_t	JDR4;
	volatile	uint32_t	DR;
} ADC_TypeDef;

#define ADC1	((ADC_TypeDef *)ADC1_BASE)
#define ADC2	((ADC_TypeDef *)ADC2_BASE)
#define ADC3	((ADC_TypeDef *)ADC3_BASE)

// ADC_SR register

#define	ADC_SR_AWD				0x01		//
#define	ADC_SR_EOC				0x02		//
#define	ADC_SR_JEOC				0x04		//
#define	ADC_SR_JSTRT			0x08		//
#define	ADC_SR_STRT				0x10		//

// ADC_CR1 register

#define	ADC_CR1_AWDCH			0x0000001F	//
#define	ADC_CR1_AWDCH_0			0x00000001	//
#define	ADC_CR1_AWDCH_1			0x00000002	//
#define	ADC_CR1_AWDCH_2			0x00000004	//
#define	ADC_CR1_AWDCH_3			0x00000008	//
#define	ADC_CR1_AWDCH_4			0x00000010	//

#define	ADC_CR1_EOCIE			0x00000020	//
#define	ADC_CR1_AWDIE			0x00000040	//
#define	ADC_CR1_JEOCIE 			0x00000080	//
#define	ADC_CR1_SCAN 			0x00000100	//
#define	ADC_CR1_AWDSGL 			0x00000200	//
#define	ADC_CR1_JAUTO			0x00000400	//
#define	ADC_CR1_DISCEN 			0x00000800	//
#define	ADC_CR1_JDISCEN			0x00001000	//

#define	ADC_CR1_DISCNUM			0x0000E000	//
#define	ADC_CR1_DISCNUM_0		0x00002000	//
#define	ADC_CR1_DISCNUM_1		0x00004000	//
#define	ADC_CR1_DISCNUM_2		0x00008000	//

#define	ADC_CR1_DUALMOD			0x000F0000	//
#define	ADC_CR1_DUALMOD_0		0x00010000	//
#define	ADC_CR1_DUALMOD_1		0x00020000	//
#define	ADC_CR1_DUALMOD_2		0x00040000	//
#define	ADC_CR1_DUALMOD_3		0x00080000	//

#define	ADC_CR1_JAWDEN 			0x00400000	//
#define	ADC_CR1_AWDEN			0x00800000	//

// ADC_CR2 register

#define	ADC_CR2_ADON 			0x00000001	//
#define	ADC_CR2_CONT 			0x00000002	//
#define	ADC_CR2_CAL  			0x00000004	//
#define	ADC_CR2_RSTCAL 			0x00000008	//
#define	ADC_CR2_DMA  			0x00000100	//
#define	ADC_CR2_ALIGN			0x00000800	//

#define	ADC_CR2_JEXTSEL			0x00007000	//
#define	ADC_CR2_JEXTSEL_0		0x00001000	//
#define	ADC_CR2_JEXTSEL_1		0x00002000	//
#define	ADC_CR2_JEXTSEL_2		0x00004000	//
#define	ADC_CR2_JEXTTRIG 		0x00008000	//

#define	ADC_CR2_EXTSEL 			0x000E0000	//
#define	ADC_CR2_EXTSEL_0 		0x00020000	//
#define	ADC_CR2_EXTSEL_1 		0x00040000	//
#define	ADC_CR2_EXTSEL_2 		0x00080000	//

#define	ADC_CR2_EXTTRIG			0x00100000	//
#define	ADC_CR2_JSWSTART 		0x00200000	//
#define	ADC_CR2_SWSTART			0x00400000	//
#define	ADC_CR2_TSVREFE			0x00800000	//

// ADC_SMPR1 register

#define	ADC_SMPR1_SMP10			0x00000007	//
#define	ADC_SMPR1_SMP10_0		0x00000001	//
#define	ADC_SMPR1_SMP10_1		0x00000002	//
#define	ADC_SMPR1_SMP10_2		0x00000004	//

#define	ADC_SMPR1_SMP11			0x00000038	//
#define	ADC_SMPR1_SMP11_0		0x00000008	//
#define	ADC_SMPR1_SMP11_1		0x00000010	//
#define	ADC_SMPR1_SMP11_2		0x00000020	//

#define	ADC_SMPR1_SMP12			0x000001C0	//
#define	ADC_SMPR1_SMP12_0		0x00000040	//
#define	ADC_SMPR1_SMP12_1		0x00000080	//
#define	ADC_SMPR1_SMP12_2		0x00000100	//

#define	ADC_SMPR1_SMP13			0x00000E00	//
#define	ADC_SMPR1_SMP13_0		0x00000200	//
#define	ADC_SMPR1_SMP13_1		0x00000400	//
#define	ADC_SMPR1_SMP13_2		0x00000800	//

#define	ADC_SMPR1_SMP14			0x00007000	//
#define	ADC_SMPR1_SMP14_0		0x00001000	//
#define	ADC_SMPR1_SMP14_1		0x00002000	//
#define	ADC_SMPR1_SMP14_2		0x00004000	//

#define	ADC_SMPR1_SMP15			0x00038000	//
#define	ADC_SMPR1_SMP15_0		0x00008000	//
#define	ADC_SMPR1_SMP15_1		0x00010000	//
#define	ADC_SMPR1_SMP15_2		0x00020000	//

#define	ADC_SMPR1_SMP16			0x001C0000	//
#define	ADC_SMPR1_SMP16_0		0x00040000	//
#define	ADC_SMPR1_SMP16_1		0x00080000	//
#define	ADC_SMPR1_SMP16_2		0x00100000	//

#define	ADC_SMPR1_SMP17			0x00E00000	//
#define	ADC_SMPR1_SMP17_0		0x00200000	//
#define	ADC_SMPR1_SMP17_1		0x00400000	//
#define	ADC_SMPR1_SMP17_2		0x00800000	//

// ADC_SMPR2 register

#define	ADC_SMPR2_SMP0 			0x00000007	//
#define	ADC_SMPR2_SMP0_0 		0x00000001	//
#define	ADC_SMPR2_SMP0_1 		0x00000002	//
#define	ADC_SMPR2_SMP0_2 		0x00000004	//

#define	ADC_SMPR2_SMP1 			0x00000038	//
#define	ADC_SMPR2_SMP1_0 		0x00000008	//
#define	ADC_SMPR2_SMP1_1 		0x00000010	//
#define	ADC_SMPR2_SMP1_2 		0x00000020	//

#define	ADC_SMPR2_SMP2 			0x000001C0	//
#define	ADC_SMPR2_SMP2_0 		0x00000040	//
#define	ADC_SMPR2_SMP2_1 		0x00000080	//
#define	ADC_SMPR2_SMP2_2 		0x00000100	//

#define	ADC_SMPR2_SMP3 			0x00000E00	//
#define	ADC_SMPR2_SMP3_0 		0x00000200	//
#define	ADC_SMPR2_SMP3_1 		0x00000400	//
#define	ADC_SMPR2_SMP3_2 		0x00000800	//

#define	ADC_SMPR2_SMP4 			0x00007000	//
#define	ADC_SMPR2_SMP4_0 		0x00001000	//
#define	ADC_SMPR2_SMP4_1 		0x00002000	//
#define	ADC_SMPR2_SMP4_2 		0x00004000	//

#define	ADC_SMPR2_SMP5 			0x00038000	//
#define	ADC_SMPR2_SMP5_0 		0x00008000	//
#define	ADC_SMPR2_SMP5_1 		0x00010000	//
#define	ADC_SMPR2_SMP5_2 		0x00020000	//

#define	ADC_SMPR2_SMP6 			0x001C0000	//
#define	ADC_SMPR2_SMP6_0 		0x00040000	//
#define	ADC_SMPR2_SMP6_1 		0x00080000	//
#define	ADC_SMPR2_SMP6_2 		0x00100000	//

#define	ADC_SMPR2_SMP7 			0x00E00000	//
#define	ADC_SMPR2_SMP7_0 		0x00200000	//
#define	ADC_SMPR2_SMP7_1 		0x00400000	//
#define	ADC_SMPR2_SMP7_2 		0x00800000	//

#define	ADC_SMPR2_SMP8 			0x07000000	//
#define	ADC_SMPR2_SMP8_0 		0x01000000	//
#define	ADC_SMPR2_SMP8_1 		0x02000000	//
#define	ADC_SMPR2_SMP8_2 		0x04000000	//

#define	ADC_SMPR2_SMP9 			0x38000000	//
#define	ADC_SMPR2_SMP9_0 		0x08000000	//
#define	ADC_SMPR2_SMP9_1 		0x10000000	//
#define	ADC_SMPR2_SMP9_2 		0x20000000	//

// ADC_JOFR1,2,3,4 register

#define	ADC_JOFR1_JOFFSET1		0x0FFF		//
#define	ADC_JOFR2_JOFFSET2		0x0FFF		//
#define	ADC_JOFR3_JOFFSET3		0x0FFF		//
#define	ADC_JOFR4_JOFFSET4		0x0FFF		//

// ADC_HTR register

#define	ADC_HTR_HT        		0x0FFF		//

// ADC_LTR register

#define	ADC_LTR_LT        		0x0FFF		//

// ADC_SQR1 register

#define	ADC_SQR1_SQ13			0x0000001F	//
#define	ADC_SQR1_SQ13_0			0x00000001	//
#define	ADC_SQR1_SQ13_1			0x00000002	//
#define	ADC_SQR1_SQ13_2			0x00000004	//
#define	ADC_SQR1_SQ13_3			0x00000008	//
#define	ADC_SQR1_SQ13_4			0x00000010	//

#define	ADC_SQR1_SQ14			0x000003E0	//
#define	ADC_SQR1_SQ14_0			0x00000020	//
#define	ADC_SQR1_SQ14_1			0x00000040	//
#define	ADC_SQR1_SQ14_2			0x00000080	//
#define	ADC_SQR1_SQ14_3			0x00000100	//
#define	ADC_SQR1_SQ14_4			0x00000200	//

#define	ADC_SQR1_SQ15			0x00007C00	//
#define	ADC_SQR1_SQ15_0			0x00000400	//
#define	ADC_SQR1_SQ15_1			0x00000800	//
#define	ADC_SQR1_SQ15_2			0x00001000	//
#define	ADC_SQR1_SQ15_3			0x00002000	//
#define	ADC_SQR1_SQ15_4			0x00004000	//

#define	ADC_SQR1_SQ16			0x000F8000	//
#define	ADC_SQR1_SQ16_0			0x00008000	//
#define	ADC_SQR1_SQ16_1			0x00010000	//
#define	ADC_SQR1_SQ16_2			0x00020000	//
#define	ADC_SQR1_SQ16_3			0x00040000	//
#define	ADC_SQR1_SQ16_4			0x00080000	//

#define	ADC_SQR1_L   			0x00F00000	//
#define	ADC_SQR1_L_0 			0x00100000	//
#define	ADC_SQR1_L_1 			0x00200000	//
#define	ADC_SQR1_L_2 			0x00400000	//
#define	ADC_SQR1_L_3 			0x00800000	//

// ADC_SQR2 register

#define	ADC_SQR2_SQ7 			0x0000001F	//
#define	ADC_SQR2_SQ7_0 			0x00000001	//
#define	ADC_SQR2_SQ7_1 			0x00000002	//
#define	ADC_SQR2_SQ7_2 			0x00000004	//
#define	ADC_SQR2_SQ7_3 			0x00000008	//
#define	ADC_SQR2_SQ7_4 			0x00000010	//

#define	ADC_SQR2_SQ8 			0x000003E0	//
#define	ADC_SQR2_SQ8_0 			0x00000020	//
#define	ADC_SQR2_SQ8_1 			0x00000040	//
#define	ADC_SQR2_SQ8_2 			0x00000080	//
#define	ADC_SQR2_SQ8_3 			0x00000100	//
#define	ADC_SQR2_SQ8_4 			0x00000200	//

#define	ADC_SQR2_SQ9 			0x00007C00	//
#define	ADC_SQR2_SQ9_0 			0x00000400	//
#define	ADC_SQR2_SQ9_1 			0x00000800	//
#define	ADC_SQR2_SQ9_2 			0x00001000	//
#define	ADC_SQR2_SQ9_3 			0x00002000	//
#define	ADC_SQR2_SQ9_4 			0x00004000	//

#define	ADC_SQR2_SQ10			0x000F8000	//
#define	ADC_SQR2_SQ10_0			0x00008000	//
#define	ADC_SQR2_SQ10_1			0x00010000	//
#define	ADC_SQR2_SQ10_2			0x00020000	//
#define	ADC_SQR2_SQ10_3			0x00040000	//
#define	ADC_SQR2_SQ10_4			0x00080000	//

#define	ADC_SQR2_SQ11			0x01F00000	//
#define	ADC_SQR2_SQ11_0			0x00100000	//
#define	ADC_SQR2_SQ11_1			0x00200000	//
#define	ADC_SQR2_SQ11_2			0x00400000	//
#define	ADC_SQR2_SQ11_3			0x00800000	//
#define	ADC_SQR2_SQ11_4			0x01000000	//

#define	ADC_SQR2_SQ12			0x3E000000	//
#define	ADC_SQR2_SQ12_0			0x02000000	//
#define	ADC_SQR2_SQ12_1			0x04000000	//
#define	ADC_SQR2_SQ12_2			0x08000000	//
#define	ADC_SQR2_SQ12_3			0x10000000	//
#define	ADC_SQR2_SQ12_4			0x20000000	//

// ADC_SQR3 register

#define	ADC_SQR3_SQ1 			0x0000001F	//
#define	ADC_SQR3_SQ1_0 			0x00000001	//
#define	ADC_SQR3_SQ1_1 			0x00000002	//
#define	ADC_SQR3_SQ1_2 			0x00000004	//
#define	ADC_SQR3_SQ1_3 			0x00000008	//
#define	ADC_SQR3_SQ1_4 			0x00000010	//

#define	ADC_SQR3_SQ2 			0x000003E0	//
#define	ADC_SQR3_SQ2_0 			0x00000020	//
#define	ADC_SQR3_SQ2_1 			0x00000040	//
#define	ADC_SQR3_SQ2_2 			0x00000080	//
#define	ADC_SQR3_SQ2_3 			0x00000100	//
#define	ADC_SQR3_SQ2_4 			0x00000200	//

#define	ADC_SQR3_SQ3 			0x00007C00	//
#define	ADC_SQR3_SQ3_0 			0x00000400	//
#define	ADC_SQR3_SQ3_1 			0x00000800	//
#define	ADC_SQR3_SQ3_2 			0x00001000	//
#define	ADC_SQR3_SQ3_3 			0x00002000	//
#define	ADC_SQR3_SQ3_4 			0x00004000	//

#define	ADC_SQR3_SQ4 			0x000F8000	//
#define	ADC_SQR3_SQ4_0 			0x00008000	//
#define	ADC_SQR3_SQ4_1 			0x00010000	//
#define	ADC_SQR3_SQ4_2 			0x00020000	//
#define	ADC_SQR3_SQ4_3 			0x00040000	//
#define	ADC_SQR3_SQ4_4 			0x00080000	//

#define	ADC_SQR3_SQ5 			0x01F00000	//
#define	ADC_SQR3_SQ5_0 			0x00100000	//
#define	ADC_SQR3_SQ5_1 			0x00200000	//
#define	ADC_SQR3_SQ5_2 			0x00400000	//
#define	ADC_SQR3_SQ5_3 			0x00800000	//
#define	ADC_SQR3_SQ5_4 			0x01000000	//

#define	ADC_SQR3_SQ6 			0x3E000000	//
#define	ADC_SQR3_SQ6_0 			0x02000000	//
#define	ADC_SQR3_SQ6_1 			0x04000000	//
#define	ADC_SQR3_SQ6_2 			0x08000000	//
#define	ADC_SQR3_SQ6_3 			0x10000000	//
#define	ADC_SQR3_SQ6_4 			0x20000000	//

// ADC_JSQR register

#define	ADC_JSQR_JSQ1			0x0000001F	//
#define	ADC_JSQR_JSQ1_0			0x00000001	//
#define	ADC_JSQR_JSQ1_1			0x00000002	//
#define	ADC_JSQR_JSQ1_2			0x00000004	//
#define	ADC_JSQR_JSQ1_3			0x00000008	//
#define	ADC_JSQR_JSQ1_4			0x00000010	//

#define	ADC_JSQR_JSQ2			0x000003E0	//
#define	ADC_JSQR_JSQ2_0			0x00000020	//
#define	ADC_JSQR_JSQ2_1			0x00000040	//
#define	ADC_JSQR_JSQ2_2			0x00000080	//
#define	ADC_JSQR_JSQ2_3			0x00000100	//
#define	ADC_JSQR_JSQ2_4			0x00000200	//

#define	ADC_JSQR_JSQ3			0x00007C00	//
#define	ADC_JSQR_JSQ3_0			0x00000400	//
#define	ADC_JSQR_JSQ3_1			0x00000800	//
#define	ADC_JSQR_JSQ3_2			0x00001000	//
#define	ADC_JSQR_JSQ3_3			0x00002000	//
#define	ADC_JSQR_JSQ3_4			0x00004000	//

#define	ADC_JSQR_JSQ4			0x000F8000	//
#define	ADC_JSQR_JSQ4_0			0x00008000	//
#define	ADC_JSQR_JSQ4_1			0x00010000	//
#define	ADC_JSQR_JSQ4_2			0x00020000	//
#define	ADC_JSQR_JSQ4_3			0x00040000	//
#define	ADC_JSQR_JSQ4_4			0x00080000	//

#define	ADC_JSQR_JL  			0x00300000	//
#define	ADC_JSQR_JL_0			0x00100000	//
#define	ADC_JSQR_JL_1			0x00200000	//

// ADC_JDR1,2,3,4 register

#define	ADC_JDR1_JDATA    		0xFFFF		//
#define	ADC_JDR2_JDATA    		0xFFFF		//
#define	ADC_JDR3_JDATA    		0xFFFF		//
#define	ADC_JDR4_JDATA    		0xFFFF		//

// ADC_DR register

#define	ADC_DR_DATA  			0x0000FFFF	//
#define	ADC_DR_ADC2DATA			0xFFFF0000	//
#endif
