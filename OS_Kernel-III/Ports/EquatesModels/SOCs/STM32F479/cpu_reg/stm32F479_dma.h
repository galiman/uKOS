/*
; stm32F479_dma.
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
; Goal:		stm32F479_dma equates.
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

#ifndef __STM32F479_DMA__
#define	__STM32F479_DMA__

// DMA address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	NDTR;
	volatile	uint32_t	PAR;
	volatile	uint32_t	M0AR;
	volatile	uint32_t	M1AR;
	volatile	uint32_t	FCR;
} DMA_Stream_TypeDef;

typedef struct {
	volatile	uint32_t	LISR;
	volatile	uint32_t	HISR;
	volatile	uint32_t	LIFCR;
	volatile	uint32_t	HIFCR;
} DMA_TypeDef;

#define	DMA1			((DMA_TypeDef *)DMA1_BASE)
#define	DMA1_Stream0	((DMA_Stream_TypeDef *)DMA1_Stream0_BASE)
#define	DMA1_Stream1	((DMA_Stream_TypeDef *)DMA1_Stream1_BASE)
#define	DMA1_Stream2	((DMA_Stream_TypeDef *)DMA1_Stream2_BASE)
#define	DMA1_Stream3	((DMA_Stream_TypeDef *)DMA1_Stream3_BASE)
#define	DMA1_Stream4	((DMA_Stream_TypeDef *)DMA1_Stream4_BASE)
#define	DMA1_Stream5	((DMA_Stream_TypeDef *)DMA1_Stream5_BASE)
#define	DMA1_Stream6	((DMA_Stream_TypeDef *)DMA1_Stream6_BASE)
#define	DMA1_Stream7	((DMA_Stream_TypeDef *)DMA1_Stream7_BASE)
#define	DMA2			((DMA_TypeDef *)DMA2_BASE)
#define	DMA2_Stream0	((DMA_Stream_TypeDef *)DMA2_Stream0_BASE)
#define	DMA2_Stream1	((DMA_Stream_TypeDef *)DMA2_Stream1_BASE)
#define	DMA2_Stream2	((DMA_Stream_TypeDef *)DMA2_Stream2_BASE)
#define	DMA2_Stream3	((DMA_Stream_TypeDef *)DMA2_Stream3_BASE)
#define	DMA2_Stream4	((DMA_Stream_TypeDef *)DMA2_Stream4_BASE)
#define	DMA2_Stream5	((DMA_Stream_TypeDef *)DMA2_Stream5_BASE)
#define	DMA2_Stream6	((DMA_Stream_TypeDef *)DMA2_Stream6_BASE)
#define	DMA2_Stream7	((DMA_Stream_TypeDef *)DMA2_Stream7_BASE)

// DMA_CR register

#define	DMA_SxCR_CHSEL			0x0E000000	//
#define	DMA_SxCR_CHSEL_0		0x02000000	//
#define	DMA_SxCR_CHSEL_1		0x04000000	//
#define	DMA_SxCR_CHSEL_2		0x08000000	//
#define	DMA_SxCR_MBURST 		0x01800000	//
#define	DMA_SxCR_MBURST_0		0x00800000	//
#define	DMA_SxCR_MBURST_1		0x01000000	//
#define	DMA_SxCR_PBURST 		0x00600000	//
#define	DMA_SxCR_PBURST_0		0x00200000	//
#define	DMA_SxCR_PBURST_1		0x00400000	//
#define	DMA_SxCR_ACK  			0x00100000	//
#define	DMA_SxCR_CT   			0x00080000	//
#define	DMA_SxCR_DBM  			0x00040000	//
#define	DMA_SxCR_PL   			0x00030000	//
#define	DMA_SxCR_PL_0 			0x00010000	//
#define	DMA_SxCR_PL_1 			0x00020000	//
#define	DMA_SxCR_PINCOS 		0x00008000	//
#define	DMA_SxCR_MSIZE			0x00006000	//
#define	DMA_SxCR_MSIZE_0		0x00002000	//
#define	DMA_SxCR_MSIZE_1		0x00004000	//
#define	DMA_SxCR_PSIZE			0x00001800	//
#define	DMA_SxCR_PSIZE_0		0x00000800	//
#define	DMA_SxCR_PSIZE_1		0x00001000	//
#define	DMA_SxCR_MINC 			0x00000400	//
#define	DMA_SxCR_PINC 			0x00000200	//
#define	DMA_SxCR_CIRC 			0x00000100	//
#define	DMA_SxCR_DIR  			0x000000C0	//
#define	DMA_SxCR_DIR_0			0x00000040	//
#define	DMA_SxCR_DIR_1			0x00000080	//
#define	DMA_SxCR_PFCTRL 		0x00000020	//
#define	DMA_SxCR_TCIE 			0x00000010	//
#define	DMA_SxCR_HTIE 			0x00000008	//
#define	DMA_SxCR_TEIE 			0x00000004	//
#define	DMA_SxCR_DMEIE			0x00000002	//
#define	DMA_SxCR_EN   			0x00000001	//

// DMA_NDTR register

#define	DMA_SxNDT     			0x0000FFFF	//
#define	DMA_SxNDT_0   			0x00000001	//
#define	DMA_SxNDT_1   			0x00000002	//
#define	DMA_SxNDT_2   			0x00000004	//
#define	DMA_SxNDT_3   			0x00000008	//
#define	DMA_SxNDT_4   			0x00000010	//
#define	DMA_SxNDT_5   			0x00000020	//
#define	DMA_SxNDT_6   			0x00000040	//
#define	DMA_SxNDT_7   			0x00000080	//
#define	DMA_SxNDT_8   			0x00000100	//
#define	DMA_SxNDT_9   			0x00000200	//
#define	DMA_SxNDT_10  			0x00000400	//
#define	DMA_SxNDT_11  			0x00000800	//
#define	DMA_SxNDT_12  			0x00001000	//
#define	DMA_SxNDT_13  			0x00002000	//
#define	DMA_SxNDT_14  			0x00004000	//
#define	DMA_SxNDT_15  			0x00008000	//

// DMA_FCR register

#define	DMA_SxFCR_FEIE			0x00000080	//
#define	DMA_SxFCR_FS			0x00000038	//
#define	DMA_SxFCR_FS_0			0x00000008	//
#define	DMA_SxFCR_FS_1			0x00000010	//
#define	DMA_SxFCR_FS_2			0x00000020	//
#define	DMA_SxFCR_DMDIS			0x00000004	//
#define	DMA_SxFCR_FTH			0x00000003	//
#define	DMA_SxFCR_FTH_0			0x00000001	//
#define	DMA_SxFCR_FTH_1			0x00000002	//

// DMA_LISR register

#define	DMA_LISR_TCIF3			0x08000000	//
#define	DMA_LISR_HTIF3			0x04000000	//
#define	DMA_LISR_TEIF3			0x02000000	//
#define	DMA_LISR_DMEIF3 		0x01000000	//
#define	DMA_LISR_FEIF3			0x00400000	//
#define	DMA_LISR_TCIF2			0x00200000	//
#define	DMA_LISR_HTIF2			0x00100000	//
#define	DMA_LISR_TEIF2			0x00080000	//
#define	DMA_LISR_DMEIF2 		0x00040000	//
#define	DMA_LISR_FEIF2			0x00010000	//
#define	DMA_LISR_TCIF1			0x00000800	//
#define	DMA_LISR_HTIF1			0x00000400	//
#define	DMA_LISR_TEIF1			0x00000200	//
#define	DMA_LISR_DMEIF1 		0x00000100	//
#define	DMA_LISR_FEIF1			0x00000040	//
#define	DMA_LISR_TCIF0			0x00000020	//
#define	DMA_LISR_HTIF0			0x00000010	//
#define	DMA_LISR_TEIF0			0x00000008	//
#define	DMA_LISR_DMEIF0 		0x00000004	//
#define	DMA_LISR_FEIF0			0x00000001	//

// DMA_HISR register

#define	DMA_HISR_TCIF7			0x08000000	//
#define	DMA_HISR_HTIF7			0x04000000	//
#define	DMA_HISR_TEIF7			0x02000000	//
#define	DMA_HISR_DMEIF7 		0x01000000	//
#define	DMA_HISR_FEIF7			0x00400000	//
#define	DMA_HISR_TCIF6			0x00200000	//
#define	DMA_HISR_HTIF6			0x00100000	//
#define	DMA_HISR_TEIF6			0x00080000	//
#define	DMA_HISR_DMEIF6 		0x00040000	//
#define	DMA_HISR_FEIF6			0x00010000	//
#define	DMA_HISR_TCIF5			0x00000800	//
#define	DMA_HISR_HTIF5			0x00000400	//
#define	DMA_HISR_TEIF5			0x00000200	//
#define	DMA_HISR_DMEIF5 		0x00000100	//
#define	DMA_HISR_FEIF5			0x00000040	//
#define	DMA_HISR_TCIF4			0x00000020	//
#define	DMA_HISR_HTIF4			0x00000010	//
#define	DMA_HISR_TEIF4			0x00000008	//
#define	DMA_HISR_DMEIF4 		0x00000004	//
#define	DMA_HISR_FEIF4			0x00000001	//

// DMA_LIFCR register

#define	DMA_LIFCR_CTCIF3		0x08000000	//
#define	DMA_LIFCR_CHTIF3		0x04000000	//
#define	DMA_LIFCR_CTEIF3		0x02000000	//
#define	DMA_LIFCR_CDMEIF3		0x01000000	//
#define	DMA_LIFCR_CFEIF3		0x00400000	//
#define	DMA_LIFCR_CTCIF2		0x00200000	//
#define	DMA_LIFCR_CHTIF2		0x00100000	//
#define	DMA_LIFCR_CTEIF2		0x00080000	//
#define	DMA_LIFCR_CDMEIF2		0x00040000	//
#define	DMA_LIFCR_CFEIF2		0x00010000	//
#define	DMA_LIFCR_CTCIF1		0x00000800	//
#define	DMA_LIFCR_CHTIF1		0x00000400	//
#define	DMA_LIFCR_CTEIF1		0x00000200	//
#define	DMA_LIFCR_CDMEIF1		0x00000100	//
#define	DMA_LIFCR_CFEIF1		0x00000040	//
#define	DMA_LIFCR_CTCIF0		0x00000020	//
#define	DMA_LIFCR_CHTIF0		0x00000010	//
#define	DMA_LIFCR_CTEIF0		0x00000008	//
#define	DMA_LIFCR_CDMEIF0		0x00000004	//
#define	DMA_LIFCR_CFEIF0		0x00000001	//

// DMA_HIFCR register

#define	DMA_HIFCR_CTCIF7		0x08000000	//
#define	DMA_HIFCR_CHTIF7		0x04000000	//
#define	DMA_HIFCR_CTEIF7		0x02000000	//
#define	DMA_HIFCR_CDMEIF7		0x01000000	//
#define	DMA_HIFCR_CFEIF7		0x00400000	//
#define	DMA_HIFCR_CTCIF6		0x00200000	//
#define	DMA_HIFCR_CHTIF6		0x00100000	//
#define	DMA_HIFCR_CTEIF6		0x00080000	//
#define	DMA_HIFCR_CDMEIF6		0x00040000	//
#define	DMA_HIFCR_CFEIF6		0x00010000	//
#define	DMA_HIFCR_CTCIF5		0x00000800	//
#define	DMA_HIFCR_CHTIF5		0x00000400	//
#define	DMA_HIFCR_CTEIF5		0x00000200	//
#define	DMA_HIFCR_CDMEIF5		0x00000100	//
#define	DMA_HIFCR_CFEIF5		0x00000040	//
#define	DMA_HIFCR_CTCIF4		0x00000020	//
#define	DMA_HIFCR_CHTIF4		0x00000010	//
#define	DMA_HIFCR_CTEIF4		0x00000008	//
#define	DMA_HIFCR_CDMEIF4		0x00000004	//
#define	DMA_HIFCR_CFEIF4		0x00000001	//
#endif
