/*
; stm32F777_dcmi.
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
; Goal:		stm32F777_dcmi equates.
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

#ifndef __STM32F777_DCMI__
#define	__STM32F777_DCMI__

// DCMI address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	SR;
	volatile	uint32_t	RISR;
	volatile	uint32_t	IER;
	volatile	uint32_t	MISR;
	volatile	uint32_t	ICR;
	volatile	uint32_t	ESCR;
	volatile	uint32_t	ESUR;
	volatile	uint32_t	CWSTRTR;
	volatile	uint32_t	CWSIZER;
	volatile	uint32_t	DR;
} DCMI_TypeDef;

#define	DCMI	((DCMI_TypeDef *)DCMI_BASE)

// DCMI_CR register

#define	DCMI_CR_CAPTURE			0x00000001	//
#define	DCMI_CR_CM     			0x00000002	//
#define	DCMI_CR_CROP   			0x00000004	//
#define	DCMI_CR_JPEG   			0x00000008	//
#define	DCMI_CR_ESS    			0x00000010	//
#define	DCMI_CR_PCKPOL 			0x00000020	//
#define	DCMI_CR_HSPOL  			0x00000040	//
#define	DCMI_CR_VSPOL  			0x00000080	//
#define	DCMI_CR_FCRC_0 			0x00000100	//
#define	DCMI_CR_FCRC_1 			0x00000200	//
#define	DCMI_CR_EDM_0  			0x00000400	//
#define	DCMI_CR_EDM_1  			0x00000800	//
#define	DCMI_CR_CRE    			0x00001000	//
#define	DCMI_CR_ENABLE 			0x00004000	//
#define	DCMI_CR_BSM				0x00030000	//
#define	DCMI_CR_BSM_0			0x00010000	//
#define	DCMI_CR_BSM_1			0x00020000	//
#define	DCMI_CR_OEBS			0x00040000	//
#define	DCMI_CR_LSM				0x00080000	//
#define	DCMI_CR_OELS			0x00100000	//

// DCMI_SR register

#define	DCMI_SR_HSYNC  			0x00000001	//
#define	DCMI_SR_VSYNC  			0x00000002	//
#define	DCMI_SR_FNE    			0x00000004	//

// DCMI_RISR register

#define	DCMI_RISR_FRAME_RIS		0x00000001	//
#define	DCMI_RISR_OVF_RIS  		0x00000002	//
#define	DCMI_RISR_ERR_RIS  		0x00000004	//
#define	DCMI_RISR_VSYNC_RIS		0x00000008	//
#define	DCMI_RISR_LINE_RIS 		0x00000010	//

// DCMI_IER register

#define	DCMI_IER_FRAME_IE  		0x00000001	//
#define	DCMI_IER_OVF_IE			0x00000002	//
#define	DCMI_IER_ERR_IE			0x00000004	//
#define	DCMI_IER_VSYNC_IE  		0x00000008	//
#define	DCMI_IER_LINE_IE   		0x00000010	//

// DCMI_MISR register

#define	DCMI_MISR_FRAME_MIS		0x00000001	//
#define	DCMI_MISR_OVF_MIS  		0x00000002	//
#define	DCMI_MISR_ERR_MIS  		0x00000004	//
#define	DCMI_MISR_VSYNC_MIS		0x00000008	//
#define	DCMI_MISR_LINE_MIS 		0x00000010	//

// DCMI_ICR register

#define	DCMI_ICR_FRAME_ISC 		0x00000001	//
#define	DCMI_ICR_OVF_ISC   		0x00000002	//
#define	DCMI_ICR_ERR_ISC   		0x00000004	//
#define	DCMI_ICR_VSYNC_ISC 		0x00000008	//
#define	DCMI_ICR_LINE_ISC  		0x00000010	//
#endif
