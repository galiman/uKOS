/*
; stm32F407_sdio.
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
; Goal:		stm32F407_sdio equates.
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

#ifndef __STM32F407_SDIO__
#define __STM32F407_SDIO__

// SDIO address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	POWER;
	volatile	uint32_t	CLKCR;
	volatile	uint32_t	ARG;
	volatile	uint32_t	CMD;
	volatile	uint32_t	RESPCMD;
	volatile	uint32_t	RESP1;
	volatile	uint32_t	RESP2;
	volatile	uint32_t	RESP3;
	volatile	uint32_t	RESP4;
	volatile	uint32_t	DTIMER;
	volatile	uint32_t	DLEN;
	volatile	uint32_t	DCTRL;
	volatile	uint32_t	DCOUNT;
	volatile	uint32_t	STA;
	volatile	uint32_t	ICR;
	volatile	uint32_t	MASK;
	volatile	uint32_t	RESERVED0[2];
	volatile	uint32_t	FIFOCNT;
	volatile	uint32_t	RESERVED1[13];
	volatile	uint32_t	FIFO;
} SDIO_TypeDef;

#define SDIO	((SDIO_TypeDef *)SDIO_BASE)

// SDIO_POWER register

#define	SDIO_POWER_PWRCTRL		0x03		//
#define	SDIO_POWER_PWRCTRL_0	0x01		//
#define	SDIO_POWER_PWRCTRL_1	0x02		//

// SDIO_CLKCR register

#define	SDIO_CLKCR_CLKDIV		0x00FF		//
#define	SDIO_CLKCR_CLKEN 		0x0100		//
#define	SDIO_CLKCR_PWRSAV		0x0200		//
#define	SDIO_CLKCR_BYPASS		0x0400		//

#define	SDIO_CLKCR_WIDBUS		0x1800		//
#define	SDIO_CLKCR_WIDBUS_0		0x0800		//
#define	SDIO_CLKCR_WIDBUS_1		0x1000		//

#define	SDIO_CLKCR_NEGEDGE 		0x2000		//
#define	SDIO_CLKCR_HWFC_EN 		0x4000		//

// SDIO_ARG register

#define	SDIO_ARG_CMDARG			0xFFFFFFFF	//

// SDIO_CMD register

#define	SDIO_CMD_CMDINDEX		0x003F		//

#define	SDIO_CMD_WAITRESP		0x00C0		//
#define	SDIO_CMD_WAITRESP_0		0x0040		//
#define	SDIO_CMD_WAITRESP_1		0x0080		//

#define	SDIO_CMD_WAITINT 		0x0100		//
#define	SDIO_CMD_WAITPEND		0x0200		//
#define	SDIO_CMD_CPSMEN  		0x0400		//
#define	SDIO_CMD_SDIOSUSPEND	0x0800		//
#define	SDIO_CMD_ENCMDCOMPL		0x1000		//
#define	SDIO_CMD_NIEN    		0x2000		//
#define	SDIO_CMD_CEATACMD		0x4000		//

// SDIO_RESPCMD register

#define	SDIO_RESPCMD_RESPCMD	0x3F		//

// SDIO_RESP0..4 registers

#define	SDIO_RESP0_CARDSTATUS0	0xFFFFFFFF	//
#define	SDIO_RESP1_CARDSTATUS1	0xFFFFFFFF	//
#define	SDIO_RESP2_CARDSTATUS2	0xFFFFFFFF	//
#define	SDIO_RESP3_CARDSTATUS3	0xFFFFFFFF	//
#define	SDIO_RESP4_CARDSTATUS4	0xFFFFFFFF	//

// SDIO_DTIMER register

#define	SDIO_DTIMER_DATATIME	0xFFFFFFFF	//

// SDIO_DLEN register

#define	SDIO_DLEN_DATALENGTH	0x01FFFFFF	//

// SDIO_DCTRL register

#define	SDIO_DCTRL_DTEN  		0x0001		//
#define	SDIO_DCTRL_DTDIR 		0x0002		//
#define	SDIO_DCTRL_DTMODE		0x0004		//
#define	SDIO_DCTRL_DMAEN 		0x0008		//

#define	SDIO_DCTRL_DBLOCKSIZE	0x00F0		//
#define	SDIO_DCTRL_DBLOCKSIZE_0	0x0010		//
#define	SDIO_DCTRL_DBLOCKSIZE_1	0x0020		//
#define	SDIO_DCTRL_DBLOCKSIZE_2	0x0040		//
#define	SDIO_DCTRL_DBLOCKSIZE_3	0x0080		//

#define	SDIO_DCTRL_RWSTART 		0x0100		//
#define	SDIO_DCTRL_RWSTOP		0x0200		//
#define	SDIO_DCTRL_RWMOD 		0x0400		//
#define	SDIO_DCTRL_SDIOEN		0x0800		//

// SDIO_DCOUNT register

#define	SDIO_DCOUNT_DATACOUNT	0x01FFFFFF	//

// SDIO_STA register

#define	SDIO_STA_CCRCFAIL		0x00000001	//
#define	SDIO_STA_DCRCFAIL		0x00000002	//
#define	SDIO_STA_CTIMEOUT		0x00000004	//
#define	SDIO_STA_DTIMEOUT		0x00000008	//
#define	SDIO_STA_TXUNDERR		0x00000010	//
#define	SDIO_STA_RXOVERR 		0x00000020	//
#define	SDIO_STA_CMDREND 		0x00000040	//
#define	SDIO_STA_CMDSENT 		0x00000080	//
#define	SDIO_STA_DATAEND 		0x00000100	//
#define	SDIO_STA_STBITERR		0x00000200	//
#define	SDIO_STA_DBCKEND 		0x00000400	//
#define	SDIO_STA_CMDACT			0x00000800	//
#define	SDIO_STA_TXACT 			0x00001000	//
#define	SDIO_STA_RXACT 			0x00002000	//
#define	SDIO_STA_TXFIFOHE		0x00004000	//
#define	SDIO_STA_RXFIFOHF		0x00008000	//
#define	SDIO_STA_TXFIFOF 		0x00010000	//
#define	SDIO_STA_RXFIFOF 		0x00020000	//
#define	SDIO_STA_TXFIFOE 		0x00040000	//
#define	SDIO_STA_RXFIFOE 		0x00080000	//
#define	SDIO_STA_TXDAVL			0x00100000	//
#define	SDIO_STA_RXDAVL			0x00200000	//
#define	SDIO_STA_SDIOIT			0x00400000	//
#define	SDIO_STA_CEATAEND		0x00800000	//

// SDIO_ICR register

#define	SDIO_ICR_CCRCFAILC		0x00000001	//
#define	SDIO_ICR_DCRCFAILC		0x00000002	//
#define	SDIO_ICR_CTIMEOUTC		0x00000004	//
#define	SDIO_ICR_DTIMEOUTC		0x00000008	//
#define	SDIO_ICR_TXUNDERRC		0x00000010	//
#define	SDIO_ICR_RXOVERRC		0x00000020	//
#define	SDIO_ICR_CMDRENDC		0x00000040	//
#define	SDIO_ICR_CMDSENTC		0x00000080	//
#define	SDIO_ICR_DATAENDC		0x00000100	//
#define	SDIO_ICR_STBITERRC		0x00000200	//
#define	SDIO_ICR_DBCKENDC		0x00000400	//
#define	SDIO_ICR_SDIOITC 		0x00400000	//
#define	SDIO_ICR_CEATAENDC		0x00800000	//

// SDIO_MASK register

#define	SDIO_MASK_CCRCFAILIE	0x00000001	//
#define	SDIO_MASK_DCRCFAILIE	0x00000002	//
#define	SDIO_MASK_CTIMEOUTIE	0x00000004	//
#define	SDIO_MASK_DTIMEOUTIE	0x00000008	//
#define	SDIO_MASK_TXUNDERRIE	0x00000010	//
#define	SDIO_MASK_RXOVERRIE 	0x00000020	//
#define	SDIO_MASK_CMDRENDIE 	0x00000040	//
#define	SDIO_MASK_CMDSENTIE 	0x00000080	//
#define	SDIO_MASK_DATAENDIE 	0x00000100	//
#define	SDIO_MASK_STBITERRIE	0x00000200	//
#define	SDIO_MASK_DBCKENDIE 	0x00000400	//
#define	SDIO_MASK_CMDACTIE  	0x00000800	//
#define	SDIO_MASK_TXACTIE		0x00001000	//
#define	SDIO_MASK_RXACTIE		0x00002000	//
#define	SDIO_MASK_TXFIFOHEIE	0x00004000	//
#define	SDIO_MASK_RXFIFOHFIE	0x00008000	//
#define	SDIO_MASK_TXFIFOFIE 	0x00010000	//
#define	SDIO_MASK_RXFIFOFIE 	0x00020000	//
#define	SDIO_MASK_TXFIFOEIE 	0x00040000	//
#define	SDIO_MASK_RXFIFOEIE 	0x00080000	//
#define	SDIO_MASK_TXDAVLIE  	0x00100000	//
#define	SDIO_MASK_RXDAVLIE  	0x00200000	//
#define	SDIO_MASK_SDIOITIE  	0x00400000	//
#define	SDIO_MASK_CEATAENDIE	0x00800000	//

// SDIO_FIFOCNT register

#define	SDIO_FIFOCNT_FIFOCOUNT	0x00FFFFFF	//

// SDIO_FIFO register

#define	SDIO_FIFO_FIFODATA		0xFFFFFFFF	//
#endif
