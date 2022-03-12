/*
; stm32F429_flash.
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
; Goal:		stm32F429_flash equates.
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

#ifndef __STM32F429_FLASH__
#define __STM32F429_FLASH__

// FLASH address definitions
// -------------------------

typedef struct {
	volatile	uint32_t	ACR;
	volatile	uint32_t	KEYR;
	volatile	uint32_t	OPTKEYR;
	volatile	uint32_t	SR;
	volatile	uint32_t	CR;
	volatile	uint32_t	OPTCR;
	volatile	uint32_t	OPTCR1;
} FLASH_TypeDef;

#define FLASH	((FLASH_TypeDef *)FLASH_R_BASE)

// FLASH_ACR register

#define FLASH_ACR_LATENCY		0x00000007	//
#define FLASH_ACR_LATENCY_0WS	0x00000000	//
#define FLASH_ACR_LATENCY_1WS	0x00000001	//
#define FLASH_ACR_LATENCY_2WS	0x00000002	//
#define FLASH_ACR_LATENCY_3WS	0x00000003	//
#define FLASH_ACR_LATENCY_4WS	0x00000004	//
#define FLASH_ACR_LATENCY_5WS	0x00000005	//
#define FLASH_ACR_LATENCY_6WS	0x00000006	//
#define FLASH_ACR_LATENCY_7WS	0x00000007	//
#define FLASH_ACR_LATENCY_8WS	0x00000008	//
#define FLASH_ACR_LATENCY_9WS	0x00000009	//
#define FLASH_ACR_LATENCY_10WS	0x0000000A	//
#define FLASH_ACR_LATENCY_11WS	0x0000000B	//
#define FLASH_ACR_LATENCY_12WS	0x0000000C	//
#define FLASH_ACR_LATENCY_13WS	0x0000000D	//
#define FLASH_ACR_LATENCY_14WS	0x0000000E	//
#define FLASH_ACR_LATENCY_15WS	0x0000000F	//

#define FLASH_ACR_PRFTEN 		0x00000100	//
#define FLASH_ACR_ICEN   		0x00000200	//
#define FLASH_ACR_DCEN   		0x00000400	//
#define FLASH_ACR_ICRST  		0x00000800	//
#define FLASH_ACR_DCRST  		0x00001000	//
#define FLASH_ACR_BYTE0_ADDRESS	0x40023C00	//
#define FLASH_ACR_BYTE2_ADDRESS	0x40023C03	//

// FLASH_SR register

#define FLASH_SR_EOP     		0x00000001	//
#define FLASH_SR_SOP     		0x00000002	//
#define FLASH_SR_WRPERR  		0x00000010	//
#define FLASH_SR_PGAERR  		0x00000020	//
#define FLASH_SR_PGPERR  		0x00000040	//
#define FLASH_SR_PGSERR  		0x00000080	//
#define FLASH_SR_BSY     		0x00010000	//

// FLASH_CR register

#define FLASH_CR_PG      		0x00000001	//
#define FLASH_CR_SER     		0x00000002	//
#define FLASH_CR_MER     		0x00000004	//
#define FLASH_CR_SNB_0   		0x00000008	//
#define FLASH_CR_SNB_1   		0x00000010	//
#define FLASH_CR_SNB_2   		0x00000020	//
#define FLASH_CR_SNB_3   		0x00000040	//
#define FLASH_CR_SNB_4   		0x00000040	//
#define FLASH_CR_PSIZE	 		0x00000300	//
#define FLASH_CR_PSIZE_0 		0x00000100	//
#define FLASH_CR_PSIZE_1 		0x00000200	//
#define FLASH_CR_MER2    		0x00008000	//
#define FLASH_CR_STRT    		0x00010000	//
#define FLASH_CR_EOPIE   		0x01000000	//
#define FLASH_CR_LOCK    		0x80000000	//

// FLASH_OPTCR register

#define FLASH_OPTCR_OPTLOCK  	0x00000001	//
#define FLASH_OPTCR_OPTSTRT  	0x00000002	//
#define FLASH_OPTCR_BOR_LEV_0	0x00000004	//
#define FLASH_OPTCR_BOR_LEV_1	0x00000008	//
#define FLASH_OPTCR_BOR_LEV  	0x0000000C	//
#define FLASH_OPTCR_BFB2	  	0x00000010	//
#define FLASH_OPTCR_WDG_SW   	0x00000020	//
#define FLASH_OPTCR_nRST_STOP	0x00000040	//
#define FLASH_OPTCR_nRST_STDBY	0x00000080	//
#define FLASH_OPTCR_RDP_0		0x00000100	//
#define FLASH_OPTCR_RDP_1		0x00000200	//
#define FLASH_OPTCR_RDP_2		0x00000400	//
#define FLASH_OPTCR_RDP_3		0x00000800	//
#define FLASH_OPTCR_RDP_4		0x00001000	//
#define FLASH_OPTCR_RDP_5		0x00002000	//
#define FLASH_OPTCR_RDP_6		0x00004000	//
#define FLASH_OPTCR_RDP_7		0x00008000	//
#define FLASH_OPTCR_nWRP_0   	0x00010000	//
#define FLASH_OPTCR_nWRP_1   	0x00020000	//
#define FLASH_OPTCR_nWRP_2   	0x00040000	//
#define FLASH_OPTCR_nWRP_3   	0x00080000	//
#define FLASH_OPTCR_nWRP_4   	0x00100000	//
#define FLASH_OPTCR_nWRP_5   	0x00200000	//
#define FLASH_OPTCR_nWRP_6   	0x00400000	//
#define FLASH_OPTCR_nWRP_7   	0x00800000	//
#define FLASH_OPTCR_nWRP_8   	0x01000000	//
#define FLASH_OPTCR_nWRP_9   	0x02000000	//
#define FLASH_OPTCR_nWRP_10  	0x04000000	//
#define FLASH_OPTCR_nWRP_11  	0x08000000	//

// FLASH_OPTCR1 register

#define FLASH_OPTCR1_nWRP  		0x0FFF0000	//
#define FLASH_OPTCR1_nWRP_0  	0x00010000	//
#define FLASH_OPTCR1_nWRP_1  	0x00020000	//
#define FLASH_OPTCR1_nWRP_2  	0x00040000	//
#define FLASH_OPTCR1_nWRP_3  	0x00080000	//
#define FLASH_OPTCR1_nWRP_4  	0x00100000	//
#define FLASH_OPTCR1_nWRP_5  	0x00200000	//
#define FLASH_OPTCR1_nWRP_6  	0x00400000	//
#define FLASH_OPTCR1_nWRP_7  	0x00800000	//
#define FLASH_OPTCR1_nWRP_8  	0x01000000	//
#define FLASH_OPTCR1_nWRP_9  	0x02000000	//
#define FLASH_OPTCR1_nWRP_10  	0x04000000	//
#define FLASH_OPTCR1_nWRP_11  	0x08000000	//
#endif
