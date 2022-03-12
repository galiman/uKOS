/*
; stm32F777_ltdc.
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
; Goal:		stm32F777_ltdc equates.
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

#ifndef __STM32F777_LTDC__
#define __STM32F777_LTDC__

// LTDC address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	RESERVED0[2];
	volatile	uint32_t	SSCR;
	volatile	uint32_t	BPCR;
	volatile	uint32_t	AWCR;
	volatile	uint32_t	TWCR;
	volatile	uint32_t	GCR;
	volatile	uint32_t	RESERVED1[2];
	volatile	uint32_t	SRCR;
	volatile	uint32_t	RESERVED2[1];
	volatile	uint32_t	BCCR;
	volatile	uint32_t	RESERVED3[1];
	volatile	uint32_t	IER;
	volatile	uint32_t	ISR;
	volatile	uint32_t	ICR;
	volatile	uint32_t	LIPCR;
	volatile	uint32_t	CPSR;
	volatile	uint32_t	CDSR;
} LTDC_TypeDef;
  
typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	WHPCR;
	volatile	uint32_t	WVPCR;
	volatile	uint32_t	CKCR;
	volatile	uint32_t	PFCR;
	volatile	uint32_t	CACR;
	volatile	uint32_t	DCCR;
	volatile	uint32_t	BFCR;
	volatile	uint32_t	RESERVED0[2];
	volatile	uint32_t	CFBAR;
	volatile	uint32_t	CFBLR;
	volatile	uint32_t	CFBLNR;
	volatile	uint32_t	RESERVED1[3];
	volatile	uint32_t	CLUTWR;

} LTDC_Layer_TypeDef;

#define	LTDC			((LTDC_TypeDef		 *)LTDC_BASE)
#define	LTDC_Layer1		((LTDC_Layer_TypeDef *)LTDC_Layer1_BASE)
#define	LTDC_Layer2		((LTDC_Layer_TypeDef *)LTDC_Layer2_BASE)

// LTDC_SSCR register

#define LTDC_SSCR_VSH			0x000007FF	//
#define LTDC_SSCR_HSW			0x0FFF0000	//

// LTDC_BPCR register

#define LTDC_BPCR_AVBP			0x000007FF	//
#define LTDC_BPCR_AHBP			0x0FFF0000	//

// LTDC_AWCR register

#define LTDC_AWCR_AAH			0x000007FF	//
#define LTDC_AWCR_AAW			0x0FFF0000	//

// LTDC_TWCR register 

#define LTDC_TWCR_TOTALH		0x000007FF	//
#define LTDC_TWCR_TOTALW		0x0FFF0000	//

// LTDC_GCR register

#define LTDC_GCR_LTDCEN 		0x00000001	//
#define LTDC_GCR_DBW 			0x00000070	//
#define LTDC_GCR_DGW 			0x00000700	//
#define LTDC_GCR_DRW 			0x00007000	//
#define LTDC_GCR_DTEN			0x00010000	//
#define LTDC_GCR_PCPOL			0x10000000	//
#define LTDC_GCR_DEPOL			0x20000000	//
#define LTDC_GCR_VSPOL			0x40000000	//
#define LTDC_GCR_HSPOL			0x80000000	//

// LTDC_SRCR register 

#define LTDC_SRCR_IMR			0x00000001	//
#define LTDC_SRCR_VBR			0x00000002	//

// LTDC_BCCR register 

#define LTDC_BCCR_BCBLUE		0x000000FF	//
#define LTDC_BCCR_BCGREEN		0x0000FF00	//
#define LTDC_BCCR_BCRED 		0x00FF0000	//

// LTDC_IER register

#define LTDC_IER_LIE 			0x00000001	//
#define LTDC_IER_FUIE			0x00000002	//
#define LTDC_IER_TERRIE 		0x00000004	//
#define LTDC_IER_RRIE			0x00000008	//

// LTDC_ISR register

#define LTDC_ISR_LIF 			0x00000001	//
#define LTDC_ISR_FUIF			0x00000002	//
#define LTDC_ISR_TERRIF 		0x00000004	//
#define LTDC_ISR_RRIF			0x00000008	//

// LTDC_ICR register

#define LTDC_ICR_CLIF			0x00000001	//
#define LTDC_ICR_CFUIF			0x00000002	//
#define LTDC_ICR_CTERRIF		0x00000004	//
#define LTDC_ICR_CRRIF			0x00000008	//

// LTDC_LIPCR register

#define LTDC_LIPCR_LIPOS		0x000007FF	//

// LTDC_CPSR register 

#define LTDC_CPSR_CYPOS 		0x0000FFFF	//
#define LTDC_CPSR_CXPOS 		0xFFFF0000	//

// LTDC_CDSR register 

#define LTDC_CDSR_VDES			0x00000001	//
#define LTDC_CDSR_HDES			0x00000002	//
#define LTDC_CDSR_VSYNCS		0x00000004	//
#define LTDC_CDSR_HSYNCS		0x00000008	//

// LTDC_LxCR register 

#define LTDC_LxCR_LEN			0x00000001	//
#define LTDC_LxCR_COLKEN		0x00000002	//
#define LTDC_LxCR_CLUTEN		0x00000010	//

// LTDC_LxWHPCR register

#define LTDC_LxWHPCR_WHSTPOS	0x00000FFF	//
#define LTDC_LxWHPCR_WHSPPOS	0xFFFF0000	//

// LTDC_LxWVPCR register

#define LTDC_LxWVPCR_WVSTPOS	0x00000FFF	//
#define LTDC_LxWVPCR_WVSPPOS	0xFFFF0000	//

// LTDC_LxCKCR register

#define LTDC_LxCKCR_CKBLUE  	0x000000FF	//
#define LTDC_LxCKCR_CKGREEN 	0x0000FF00	//
#define LTDC_LxCKCR_CKRED		0x00FF0000	//

// LTDC_LxPFCR register

#define LTDC_LxPFCR_PF			0x00000007	//

// LTDC_LxCACR register

#define LTDC_LxCACR_CONSTA  	0x000000FF	//

// LTDC_LxDCCR register

#define LTDC_LxDCCR_DCBLUE  	0x000000FF	//
#define LTDC_LxDCCR_DCGREEN 	0x0000FF00	//
#define LTDC_LxDCCR_DCRED		0x00FF0000	//
#define LTDC_LxDCCR_DCALPHA 	0xFF000000	//

// LTDC_LxBFCR register

#define LTDC_LxBFCR_BF2 		0x00000007	//
#define LTDC_LxBFCR_BF1 		0x00000700	//

// LTDC_LxCFBAR register

#define LTDC_LxCFBAR_CFBADD 	0xFFFFFFFF	//

// LTDC_LxCFBLR register

#define LTDC_LxCFBLR_CFBLL  	0x00001FFF	//
#define LTDC_LxCFBLR_CFBP		0x1FFF0000	//

// LTDC_LxCFBLNR register

#define LTDC_LxCFBLNR_CFBLNBR	0x000007FF	//

// LTDC_LxCLUTWR register

#define LTDC_LxCLUTWR_BLUE  	0x000000FF	//
#define LTDC_LxCLUTWR_GREEN 	0x0000FF00	//
#define LTDC_LxCLUTWR_RED		0x00FF0000	//
#define LTDC_LxCLUTWR_CLUTADD	0xFF000000	//
#endif
