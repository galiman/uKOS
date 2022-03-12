/*
; stm32L476_dma.
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
; Goal:		stm32L476_dma equates.
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

#ifndef __STM32L476_DMA__
#define	__STM32L476_DMA__

// DMA address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CCR;
	volatile	uint32_t	CNDTR;
	volatile	uint32_t	CPAR;
	volatile	uint32_t	CMAR;
} DMA_Channel_TypeDef;

typedef struct {
	volatile	uint32_t	ISR;
	volatile	uint32_t	IFCR;
} DMA_TypeDef;

typedef struct {
	volatile	uint32_t	CSELR;
} DMA_Request_TypeDef;

#define	DMA1				((DMA_TypeDef *)DMA1_BASE)
#define	DMA1_Channel1		((DMA_Channel_TypeDef *)DMA1_Channel1_BASE)
#define	DMA1_Channel2		((DMA_Channel_TypeDef *)DMA1_Channel2_BASE)
#define	DMA1_Channel3		((DMA_Channel_TypeDef *)DMA1_Channel3_BASE)
#define	DMA1_Channel4		((DMA_Channel_TypeDef *)DMA1_Channel4_BASE)
#define	DMA1_Channel5		((DMA_Channel_TypeDef *)DMA1_Channel5_BASE)
#define	DMA1_Channel6		((DMA_Channel_TypeDef *)DMA1_Channel6_BASE)
#define	DMA1_Channel7		((DMA_Channel_TypeDef *)DMA1_Channel7_BASE)
#define	DMA1_CSELR			((DMA_Request_TypeDef *)DMA1_CSELR_BASE)

#define	DMA2				((DMA_TypeDef *)DMA2_BASE)
#define	DMA2_Channel1		((DMA_Channel_TypeDef *)DMA2_Channel1_BASE)
#define	DMA2_Channel2		((DMA_Channel_TypeDef *)DMA2_Channel2_BASE)
#define	DMA2_Channel3		((DMA_Channel_TypeDef *)DMA2_Channel3_BASE)
#define	DMA2_Channel4		((DMA_Channel_TypeDef *)DMA2_Channel4_BASE)
#define	DMA2_Channel5		((DMA_Channel_TypeDef *)DMA2_Channel5_BASE)
#define	DMA2_Channel6		((DMA_Channel_TypeDef *)DMA2_Channel6_BASE)
#define	DMA2_Channel7		((DMA_Channel_TypeDef *)DMA2_Channel7_BASE)
#define	DMA2_CSELR			((DMA_Request_TypeDef *)DMA2_CSELR_BASE)

// DMA_ISR register

#define	DMA_ISR_GIF1			0x00000001	//
#define	DMA_ISR_TCIF1			0x00000002	//
#define	DMA_ISR_HTIF1			0x00000004	//
#define	DMA_ISR_TEIF1			0x00000008	//
#define	DMA_ISR_GIF2			0x00000010	//
#define	DMA_ISR_TCIF2			0x00000020	//
#define	DMA_ISR_HTIF2			0x00000040	//
#define	DMA_ISR_TEIF2			0x00000080	//
#define	DMA_ISR_GIF3			0x00000100	//
#define	DMA_ISR_TCIF3			0x00000200	//
#define	DMA_ISR_HTIF3			0x00000400	//
#define	DMA_ISR_TEIF3			0x00000800	//
#define	DMA_ISR_GIF4			0x00001000	//
#define	DMA_ISR_TCIF4			0x00002000	//
#define	DMA_ISR_HTIF4			0x00004000	//
#define	DMA_ISR_TEIF4			0x00008000	//
#define	DMA_ISR_GIF5			0x00010000	//
#define	DMA_ISR_TCIF5			0x00020000	//
#define	DMA_ISR_HTIF5			0x00040000	//
#define	DMA_ISR_TEIF5			0x00080000	//
#define	DMA_ISR_GIF6			0x00100000	//
#define	DMA_ISR_TCIF6			0x00200000	//
#define	DMA_ISR_HTIF6			0x00400000	//
#define	DMA_ISR_TEIF6			0x00800000	//
#define	DMA_ISR_GIF7			0x01000000	//
#define	DMA_ISR_TCIF7			0x02000000	//
#define	DMA_ISR_HTIF7			0x04000000	//
#define	DMA_ISR_TEIF7			0x08000000	//

// DMA_IFCR register

#define	DMA_IFCR_CGIF1			0x00000001	//
#define	DMA_IFCR_CTCIF1			0x00000002	//
#define	DMA_IFCR_CHTIF1			0x00000004	//
#define	DMA_IFCR_CTEIF1			0x00000008	//
#define	DMA_IFCR_CGIF2			0x00000010	//
#define	DMA_IFCR_CTCIF2			0x00000020	//
#define	DMA_IFCR_CHTIF2			0x00000040	//
#define	DMA_IFCR_CTEIF2			0x00000080	//
#define	DMA_IFCR_CGIF3			0x00000100	//
#define	DMA_IFCR_CTCIF3			0x00000200	//
#define	DMA_IFCR_CHTIF3			0x00000400	//
#define	DMA_IFCR_CTEIF3			0x00000800	//
#define	DMA_IFCR_CGIF4			0x00001000	//
#define	DMA_IFCR_CTCIF4			0x00002000	//
#define	DMA_IFCR_CHTIF4			0x00004000	//
#define	DMA_IFCR_CTEIF4			0x00008000	//
#define	DMA_IFCR_CGIF5			0x00010000	//
#define	DMA_IFCR_CTCIF5			0x00020000	//
#define	DMA_IFCR_CHTIF5			0x00040000	//
#define	DMA_IFCR_CTEIF5			0x00080000	//
#define	DMA_IFCR_CGIF6			0x00100000	//
#define	DMA_IFCR_CTCIF6			0x00200000	//
#define	DMA_IFCR_CHTIF6			0x00400000	//
#define	DMA_IFCR_CTEIF6			0x00800000	//
#define	DMA_IFCR_CGIF7			0x01000000	//
#define	DMA_IFCR_CTCIF7			0x02000000	//
#define	DMA_IFCR_CHTIF7			0x04000000	//
#define	DMA_IFCR_CTEIF7			0x08000000	//

// DMA_CCR register

#define	DMA_CCR_EN				0x00000001	//
#define	DMA_CCR_TCIE			0x00000002	//
#define	DMA_CCR_HTIE			0x00000004	//
#define	DMA_CCR_TEIE			0x00000008	//
#define	DMA_CCR_DIR				0x00000010	//
#define	DMA_CCR_CIRC			0x00000020	//
#define	DMA_CCR_PINC			0x00000040	//
#define	DMA_CCR_MINC			0x00000080	//
#define	DMA_CCR_PSIZE			0x00000300	//
#define	DMA_CCR_PSIZE_0			0x00000100	//
#define	DMA_CCR_PSIZE_1			0x00000200	//
#define	DMA_CCR_MSIZE			0x00000C00	//
#define	DMA_CCR_MSIZE_0			0x00000400	//
#define	DMA_CCR_MSIZE_1			0x00000800	//
#define	DMA_CCR_PL				0x00003000	//
#define	DMA_CCR_PL_0			0x00001000	//
#define	DMA_CCR_PL_1			0x00002000	//
#define	DMA_CCR_MEM2MEM			0x00004000	//

// DMA_CNDTR register

#define	DMA_CNDTR_NDT			0x0000FFFF	//

// DMA_CPAR register

#define	DMA_CPAR_PA				0xFFFFFFFF	//

// DMA_CMAR register

#define	DMA_CMAR_MA				0xFFFFFFFF	//

// DMA_CSELR register

#define	DMA_CSELR_C1S			0x0000000F	//
#define	DMA_CSELR_C2S			0x000000F0	//
#define	DMA_CSELR_C3S			0x00000F00	//
#define	DMA_CSELR_C4S			0x0000F000	//
#define	DMA_CSELR_C5S			0x000F0000	//
#define	DMA_CSELR_C6S			0x00F00000	//
#define	DMA_CSELR_C7S			0x0F000000	//
#endif
