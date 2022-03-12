/*
; stm32F103_flash.
; ================

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
; Goal:		stm32F103_flash equates.
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

#ifndef __STM32F103_FLASH__
#define __STM32F103_FLASH__

// Flash address definitions
// -------------------------

typedef struct {
	volatile	uint32_t	ACR;
	volatile	uint32_t	KEYR;
	volatile	uint32_t	OPTKEYR;
	volatile	uint32_t	SR;
	volatile	uint32_t	CR;
	volatile	uint32_t	AR;
	volatile	uint32_t	RESERVED;
	volatile	uint32_t	OBR;
	volatile	uint32_t	WRPR;
} FLASH_TypeDef;
  
typedef struct {
	volatile	uint16_t	RDP;
	volatile	uint16_t	USER;
	volatile	uint16_t	Data0;
	volatile	uint16_t	Data1;
	volatile	uint16_t	WRP0;
	volatile	uint16_t	WRP1;
	volatile	uint16_t	WRP2;
	volatile	uint16_t	WRP3;
} OB_TypeDef;

#define FLASH	((FLASH_TypeDef *)FLASH_R_BASE)
#define OB		((OB_TypeDef *)OB_BASE) 

// FLASH_ACR register

#define	FLASH_ACR_LATENCY		0x03			//
#define	FLASH_ACR_LATENCY_0		0x00			//
#define	FLASH_ACR_LATENCY_1		0x01			//
#define	FLASH_ACR_LATENCY_2		0x02			//
#define	FLASH_ACR_HLFCYA		0x08			//
#define	FLASH_ACR_PRFTBE		0x10			//
#define	FLASH_ACR_PRFTBS		0x20			//

// FLASH_KEYR register

#define	FLASH_KEYR_FKEYR		0xFFFFFFFF		//

// FLASH_OPTKEYR register

#define	FLASH_OPTKEYR_OPTKEYR	0xFFFFFFFF		//

// FLASH_SR register

#define	FLASH_SR_BSY			0x01			//
#define	FLASH_SR_PGERR			0x04			//
#define	FLASH_SR_WRPRTERR		0x10			//
#define	FLASH_SR_EOP			0x20			//

// FLASH_CR register

#define	FLASH_CR_PG				0x0001			//
#define	FLASH_CR_PER			0x0002			//
#define	FLASH_CR_MER			0x0004			//
#define	FLASH_CR_OPTPG			0x0010			//
#define	FLASH_CR_OPTER			0x0020			//
#define	FLASH_CR_STRT			0x0040			//
#define	FLASH_CR_LOCK			0x0080			//
#define	FLASH_CR_OPTWRE			0x0200			//
#define	FLASH_CR_ERRIE			0x0400			//
#define	FLASH_CR_EOPIE			0x1000			//

// FLASH_AR register

#define	FLASH_AR_FAR			0xFFFFFFFF		//

// FLASH_OBR register

#define	FLASH_OBR_OPTERR		0x0001			//
#define	FLASH_OBR_RDPRT			0x0002			//
#define	FLASH_OBR_USER			0x03FC			//
#define	FLASH_OBR_WDG_SW		0x0004			//
#define	FLASH_OBR_nRST_STOP		0x0008			//
#define	FLASH_OBR_nRST_STDBY	0x0010			//
#define	FLASH_OBR_Notused		0x03E0			//

// FLASH_WRPR register

#define	FLASH_WRPR_WRP			0xFFFFFFFF		//

// FLASH_RDP register

#define	FLASH_RDP_RDP			0x000000FF		//
#define	FLASH_RDP_nRDP			0x0000FF00		//

// FLASH_USER register

#define	FLASH_USER_USER			0x00FF0000		//
#define	FLASH_USER_nUSER		0xFF000000		//

// FLASH_Data0 register

#define	FLASH_Data0_Data0		0x000000FF		//
#define	FLASH_Data0_nData0		0x0000FF00		//

// FLASH_Data1 register

#define	FLASH_Data1_Data1		0x00FF0000		//
#define	FLASH_Data1_nData1		0xFF000000		//

// FLASH_WRP0 register

#define	FLASH_WRP0_WRP0			0x000000FF		//
#define	FLASH_WRP0_nWRP0		0x0000FF00		//

// FLASH_WRP1 register

#define	FLASH_WRP1_WRP1			0x00FF0000		//
#define	FLASH_WRP1_nWRP1		0xFF000000		//

// FLASH_WRP2 register

#define	FLASH_WRP2_WRP2			0x000000FF		//
#define	FLASH_WRP2_nWRP2		0x0000FF00		//

// FLASH_WRP3 register

#define	FLASH_WRP3_WRP3			0x00FF0000		//
#define	FLASH_WRP3_nWRP3		0xFF000000		//
#endif
