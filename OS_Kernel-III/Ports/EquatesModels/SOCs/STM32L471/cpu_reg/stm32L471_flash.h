/*
; stm32L471_flash.
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
; Goal:		stm32L471_flash equates.
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

#ifndef __STM32L471_FLASH__
#define __STM32L471_FLASH__

// FLASH address definitions
// -------------------------

typedef struct {
	volatile	uint32_t	ACR;
	volatile	uint32_t	PDKEYR;
	volatile	uint32_t	KEYR;
	volatile	uint32_t	OPTKEYR;
	volatile	uint32_t	SR;
	volatile	uint32_t	CR;
	volatile	uint32_t	ECCR;
	volatile	uint32_t	RESERVED1;
	volatile	uint32_t	OPTR;
	volatile	uint32_t	PCROP1SR;
	volatile	uint32_t	PCROP1ER;
	volatile	uint32_t	WRP1AR;
	volatile	uint32_t	WRP1BR;
	volatile	uint32_t	RESERVED2[4];
	volatile	uint32_t	PCROP2SR;
	volatile	uint32_t	PCROP2ER;
	volatile	uint32_t	WRP2AR;
	volatile	uint32_t	WRP2BR;
} FLASH_TypeDef;

#define FLASH	((FLASH_TypeDef *)FLASH_R_BASE)

// FLASH_ACR register

#define	FLASH_ACR_LATENCY			0x00000007	//
#define	FLASH_ACR_LATENCY_0WS		0x00000000	//
#define	FLASH_ACR_LATENCY_1WS		0x00000001	//
#define	FLASH_ACR_LATENCY_2WS		0x00000002	//
#define	FLASH_ACR_LATENCY_3WS		0x00000003	//
#define	FLASH_ACR_LATENCY_4WS		0x00000004	//
#define	FLASH_ACR_PRFTEN 			0x00000100	//
#define	FLASH_ACR_ICEN   			0x00000200	//
#define	FLASH_ACR_DCEN   			0x00000400	//
#define	FLASH_ACR_ICRST  			0x00000800	//
#define	FLASH_ACR_DCRST  			0x00001000	//
#define	FLASH_ACR_RUN_PD			0x00002000	//
#define	FLASH_ACR_SLEEP_PD			0x00004000	//

// FLASH_SR register

#define	FLASH_SR_EOP     			0x00000001	//
#define	FLASH_SR_OPERR     			0x00000002	//
#define	FLASH_SR_PROGERR     		0x00000008	//
#define	FLASH_SR_WRPERR  			0x00000010	//
#define	FLASH_SR_PGAERR  			0x00000020	//
#define	FLASH_SR_SIZERR  			0x00000040	//
#define	FLASH_SR_PGSERR  			0x00000080	//
#define	FLASH_SR_MISERR  			0x00000100	//
#define	FLASH_SR_FASTERR  			0x00000200	//
#define	FLASH_SR_RDERR  			0x00004000	//
#define	FLASH_SR_OPTVERR  			0x00008000	//
#define	FLASH_SR_BSY     			0x00010000	//

// FLASH_CR register

#define	FLASH_CR_PG      			0x00000001	//
#define	FLASH_CR_PER     			0x00000002	//
#define	FLASH_CR_MER1     			0x00000004	//
#define	FLASH_CR_PNB	   			0x000007F8	//
#define	FLASH_CR_BKER   			0x00000800	//
#define	FLASH_CR_MER2   			0x00008000	//
#define	FLASH_CR_STRT   			0x00010000	//
#define	FLASH_CR_OPTSTRT   			0x00020000	//
#define	FLASH_CR_FSTPG	 			0x00040000	//
#define	FLASH_CR_EOPIE	 			0x01000000	//
#define	FLASH_CR_ERRIE	 			0x02000000	//
#define	FLASH_CR_RDERRIE			0x04000000	//
#define	FLASH_CR_OBL_LAUNCH			0x08000000	//
#define	FLASH_CR_OPTLOCK   			0x40000000	//
#define	FLASH_CR_LOCK    			0x80000000	//

// FLASH_ECCR register

#define	FLASH_ECCR_ADDR_ECC			0x0007FFFF	//
#define	FLASH_ECCR_BK_ECC			0x00080000	//
#define	FLASH_ECCR_SYSF_ECC			0x00100000	//
#define	FLASH_ECCR_ECCIE			0x01000000	//
#define	FLASH_ECCR_ECCC				0x40000000	//
#define	FLASH_ECCR_ECCD				0x80000000	//

// FLASH_OPTR register

#define	FLASH_OPTR_RDP				0x000000FF	//
#define	FLASH_OPTR_BOR_LEV			0x00000700	//
#define	FLASH_OPTR_BOR_LEV_0		0x00000000	//
#define	FLASH_OPTR_BOR_LEV_1		0x00000100	//
#define	FLASH_OPTR_BOR_LEV_2		0x00000200	//
#define	FLASH_OPTR_BOR_LEV_3		0x00000300	//
#define	FLASH_OPTR_BOR_LEV_4		0x00000400	//
#define	FLASH_OPTR_nRST_STOP		0x00001000	//
#define	FLASH_OPTR_nRST_STDBY		0x00002000	//
#define	FLASH_OPTR_nRST_SHDW		0x00004000	//
#define	FLASH_OPTR_IWDG_SW			0x00010000	//
#define	FLASH_OPTR_IWDG_STOP		0x00020000	//
#define	FLASH_OPTR_IWDG_STDBY		0x00040000	//
#define	FLASH_OPTR_WWDG_SW			0x00080000	//
#define	FLASH_OPTR_BFB2				0x00100000	//
#define	FLASH_OPTR_DUALBANK			0x00200000	//
#define	FLASH_OPTR_nBOOT1			0x00800000	//
#define	FLASH_OPTR_SRAM2_PE			0x01000000	//
#define	FLASH_OPTR_SRAM2_RST		0x02000000	//

// FLASH_PCROP1SR register

#define	FLASH_PCROP1SR_PCROP1_STRT	0x0000FFFF	//

// FLASH_PCROP1ER register

#define	FLASH_PCROP1ER_PCROP1_END	0x0000FFFF	//
#define	FLASH_PCROP1ER_PCROP_RDP	0x80000000	//

// FLASH_WRP1AR register

#define	FLASH_WRP1AR_WRP1A_STRT		0x000000FF	//
#define	FLASH_WRP1AR_WRP1A_END		0x00FF0000	//

// FLASH_WRPB1R register

#define	FLASH_WRP1BR_WRP1B_STRT		0x000000FF	//
#define	FLASH_WRP1BR_WRP1B_END		0x00FF0000	//

// FLASH_PCROP2SR register

#define	FLASH_PCROP2SR_PCROP2_STRT	0x0000FFFF	//

// FLASH_PCROP2ER register

#define	FLASH_PCROP2ER_PCROP2_END	0x0000FFFF	//

// FLASH_WRP2AR register

#define	FLASH_WRP2AR_WRP2A_STRT		0x000000FF	//
#define	FLASH_WRP2AR_WRP2A_END		0x00FF0000	//

// FLASH_WRP2BR register

#define	FLASH_WRP2BR_WRP2B_STRT		0x000000FF	//
#define	FLASH_WRP2BR_WRP2B_END		0x00FF0000	//
#endif
