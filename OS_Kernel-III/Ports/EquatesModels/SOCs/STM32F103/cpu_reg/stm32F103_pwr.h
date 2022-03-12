/*
; stm32F103_pwr.
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
; Goal:		stm32F103_pwr equates.
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

#ifndef __STM32F103_PWR__
#define __STM32F103_PWR__

// PWR address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	CSR;
} PWR_TypeDef;

#define PWR		((PWR_TypeDef *)PWR_BASE)

// PWR_CR register

#define	PWR_CR_LPDS				0x0001		//
#define	PWR_CR_PDDS				0x0002		//
#define	PWR_CR_CWUF				0x0004		//
#define	PWR_CR_CSBF				0x0008		//
#define	PWR_CR_PVDE				0x0010		//
#define	PWR_CR_PLS 				0x00E0		//
#define	PWR_CR_PLS_0			0x0020		//
#define	PWR_CR_PLS_1			0x0040		//
#define	PWR_CR_PLS_2			0x0080		//

// PVD level configuration

#define	PWR_CR_PLS_2V2			0x0000		//
#define	PWR_CR_PLS_2V3			0x0020		//
#define	PWR_CR_PLS_2V4			0x0040		//
#define	PWR_CR_PLS_2V5			0x0060		//
#define	PWR_CR_PLS_2V6			0x0080		//
#define	PWR_CR_PLS_2V7			0x00A0		//
#define	PWR_CR_PLS_2V8			0x00C0		//
#define	PWR_CR_PLS_2V9			0x00E0		//
#define	PWR_CR_DBP 				0x0100		//

// PWR_CSR register

#define	PWR_CSR_WUF				0x0001		//
#define	PWR_CSR_SBF				0x0002		//
#define	PWR_CSR_PVDO			0x0004		//
#define	PWR_CSR_EWUP			0x0100		//
#endif
