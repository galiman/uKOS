/*
; stm32F777_pwr.
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
; Goal:		stm32F777_pwr equates.
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

#ifndef __STM32F777_PWR__
#define __STM32F777_PWR__

// PWR address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR1;
	volatile	uint32_t	CSR1;
	volatile	uint32_t	CR2;
	volatile	uint32_t	CSR2;
} PWR_TypeDef;

#define PWR		((PWR_TypeDef *)PWR_BASE)

// PWR_CR1 register

#define	PWR_CR_LPDS				0x0001			//
#define	PWR_CR_PDDS				0x0002			//
#define	PWR_CR_CWUF				0x0004			//
#define	PWR_CR_CSBF				0x0008			//
#define	PWR_CR_PVDE				0x0010			//
#define	PWR_CR_PLS 				0x00E0			//
#define	PWR_CR_PLS_0			0x0020			//
#define	PWR_CR_PLS_1			0x0040			//
#define	PWR_CR_PLS_2			0x0080			//

// PVD level configuration

#define	PWR_CR_PLS_LEV0			0x00000000		//
#define	PWR_CR_PLS_LEV1			0x00000020		//
#define	PWR_CR_PLS_LEV2			0x00000040		//
#define	PWR_CR_PLS_LEV3			0x00000060		//
#define	PWR_CR_PLS_LEV4			0x00000080		//
#define	PWR_CR_PLS_LEV5			0x000000A0		//
#define	PWR_CR_PLS_LEV6			0x000000C0		//
#define	PWR_CR_PLS_LEV7			0x000000E0		//
#define	PWR_CR_DBP 				0x00000100		//
#define	PWR_CR_FPDS 			0x00000200		//
#define	PWR_CR_LPUDS			0x00000400		//
#define	PWR_CR_MRUDS			0x00000800		//
#define	PWR_CR_ADCDC1			0x00002000		//
#define	PWR_CR_VOS  			0x0000C000		//
#define	PWR_CR_VOS_0			0x00004000		//
#define	PWR_CR_VOS_1			0x00008000		//
#define	PWR_CR_ODEN 			0x00010000		//
#define	PWR_CR_ODSWEN			0x00020000		//
#define	PWR_CR_UDEN 			0x000C0000		//
#define	PWR_CR_UDEN_0			0x00040000		//
#define	PWR_CR_UDEN_1			0x00080000		//

// PWR_CSR1 register

#define	PWR_CSR_WUF				0x00000001		//
#define	PWR_CSR_SBF				0x00000002		//
#define	PWR_CSR_PVDO			0x00000004		//
#define	PWR_CSR_BRR				0x00000008		//
#define	PWR_CSR_BRE				0x00000200		//
#define	PWR_CSR_VOSRDY			0x00004000		//
#define	PWR_CSR_ODRDY			0x00010000		//
#define	PWR_CSR_ODSWRDY			0x00020000		//
#define	PWR_CSR_UDSWRDY			0x000C0000		//

// PWR_CR2 register

#define	PWR_CR2_CWUPF1			0x00000001		//
#define	PWR_CR2_CWUPF2			0x00000002		//
#define	PWR_CR2_CWUPF3			0x00000004		//
#define	PWR_CR2_CWUPF4			0x00000008		//
#define	PWR_CR2_CWUPF5			0x00000010		//
#define	PWR_CR2_CWUPF6			0x00000020		//
#define	PWR_CR2_WUPP1 			0x00000100		//
#define	PWR_CR2_WUPP2 			0x00000200		//
#define	PWR_CR2_WUPP3 			0x00000400		//
#define	PWR_CR2_WUPP4 			0x00000800		//
#define	PWR_CR2_WUPP5 			0x00001000		//
#define	PWR_CR2_WUPP6 			0x00002000		//

// PWR_CSR2 register

#define	PWR_CSR2_WUPF1			0x00000001		//
#define	PWR_CSR2_WUPF2			0x00000002		//
#define	PWR_CSR2_WUPF3			0x00000004		//
#define	PWR_CSR2_WUPF4			0x00000008		//
#define	PWR_CSR2_WUPF5			0x00000010		//
#define	PWR_CSR2_WUPF6			0x00000020		//
#define	PWR_CSR2_EWUP1			0x00000100		//
#define	PWR_CSR2_EWUP2			0x00000200		//
#define	PWR_CSR2_EWUP3			0x00000400		//
#define	PWR_CSR2_EWUP4			0x00000800		//
#define	PWR_CSR2_EWUP5			0x00001000		//
#define	PWR_CSR2_EWUP6			0x00002000		//
#endif
