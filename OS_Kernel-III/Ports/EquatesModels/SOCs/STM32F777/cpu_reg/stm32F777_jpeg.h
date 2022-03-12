/*
; stm32F777_jpeg.
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
; Goal:		stm32F777_jpeg equates.
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

#ifndef __STM32F777_JPEG__
#define __STM32F777_JPEG__

// JPEG address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	CONFR0;
	volatile	uint32_t	CONFR1;
	volatile	uint32_t	CONFR2;
	volatile	uint32_t	CONFR3;
	volatile	uint32_t	CONFR4;
	volatile	uint32_t	CONFR5;
	volatile	uint32_t	CONFR6;
	volatile	uint32_t	CONFR7;
	volatile	uint32_t	Reserved20[4];
	volatile	uint32_t	CR;
	volatile	uint32_t	SR;
	volatile	uint32_t	CFR;
	volatile	uint32_t	Reserved3c;
	volatile	uint32_t	DIR;
	volatile	uint32_t	DOR;
	volatile	uint32_t	Reserved48[2];
	volatile	uint32_t	QMEM0[16];
	volatile	uint32_t	QMEM1[16];
	volatile	uint32_t	QMEM2[16];
	volatile	uint32_t	QMEM3[16];
	volatile	uint32_t	HUFFMIN[16];
	volatile	uint32_t	HUFFBASE[32];
	volatile	uint32_t	HUFFSYMB[84];
	volatile	uint32_t	DHTMEM[103];
	volatile	uint32_t	Reserved4FC;
	volatile	uint32_t	HUFFENC_AC0[88];
	volatile	uint32_t	HUFFENC_AC1[88];
	volatile	uint32_t	HUFFENC_DC0[8];
	volatile	uint32_t	HUFFENC_DC1[8];
} JPEG_TypeDef;

#define JPEG		((JPEG_TypeDef *)JPEG_BASE)

// CONFR0 register

#define	JPEG_CONFR0_START			0x00000001	//

// CONFR1 register

#define	JPEG_CONFR1_NF				0x00000003	//
#define	JPEG_CONFR1_NF_0			0x00000001	//
#define	JPEG_CONFR1_NF_1			0x00000002	//
#define	JPEG_CONFR1_RE				0x00000004	//
#define	JPEG_CONFR1_DE				0x00000008	//
#define	JPEG_CONFR1_COLORSPACE		0x00000030	//
#define	JPEG_CONFR1_COLORSPACE_0	0x00000010	//
#define	JPEG_CONFR1_COLORSPACE_1	0x00000020	//
#define	JPEG_CONFR1_NS				0x000000C0	//
#define	JPEG_CONFR1_NS_0			0x00000040	//
#define	JPEG_CONFR1_NS_1			0x00000080	//
#define	JPEG_CONFR1_HDR				0x00000100	//
#define	JPEG_CONFR1_YSIZE			0xFFFF0000	//

// CONFR2 register

#define	JPEG_CONFR2_NMCU			0x03FFFFFF	//

// CONFR3 register

#define	JPEG_CONFR3_NRST			0x0000FFFF	//
#define	JPEG_CONFR3_XSIZE			0xFFFF0000	//

// CONFR4 register

#define	JPEG_CONFR4_HD				0x00000001	//
#define	JPEG_CONFR4_HA				0x00000002	//
#define	JPEG_CONFR4_QT				0x0000000C	//
#define	JPEG_CONFR4_QT_0			0x00000004	//
#define	JPEG_CONFR4_QT_1			0x00000008	//
#define	JPEG_CONFR4_NB				0x000000F0	//
#define	JPEG_CONFR4_NB_0			0x00000010	//
#define	JPEG_CONFR4_NB_1			0x00000020	//
#define	JPEG_CONFR4_NB_2			0x00000040	//
#define	JPEG_CONFR4_NB_3			0x00000080	//
#define	JPEG_CONFR4_VSF				0x00000F00	//
#define	JPEG_CONFR4_VSF_0			0x00000100	//
#define	JPEG_CONFR4_VSF_1			0x00000200	//
#define	JPEG_CONFR4_VSF_2			0x00000400	//
#define	JPEG_CONFR4_VSF_3			0x00000800	//
#define	JPEG_CONFR4_HSF				0x0000F000	//
#define	JPEG_CONFR4_HSF_0			0x00001000	//
#define	JPEG_CONFR4_HSF_1			0x00002000	//
#define	JPEG_CONFR4_HSF_2			0x00004000	//
#define	JPEG_CONFR4_HSF_3			0x00008000	//

// CONFR5 register

#define	JPEG_CONFR5_HD				0x00000001	//
#define	JPEG_CONFR5_HA				0x00000002	//
#define	JPEG_CONFR5_QT				0x0000000C	//
#define	JPEG_CONFR5_QT_0			0x00000004	//
#define	JPEG_CONFR5_QT_1			0x00000008	//
#define	JPEG_CONFR5_NB				0x000000F0	//
#define	JPEG_CONFR5_NB_0			0x00000010	//
#define	JPEG_CONFR5_NB_1			0x00000020	//
#define	JPEG_CONFR5_NB_2			0x00000040	//
#define	JPEG_CONFR5_NB_3			0x00000080	//
#define	JPEG_CONFR5_VSF				0x00000F00	//
#define	JPEG_CONFR5_VSF_0			0x00000100	//
#define	JPEG_CONFR5_VSF_1			0x00000200	//
#define	JPEG_CONFR5_VSF_2			0x00000400	//
#define	JPEG_CONFR5_VSF_3			0x00000800	//
#define	JPEG_CONFR5_HSF				0x0000F000	//
#define	JPEG_CONFR5_HSF_0			0x00001000	//
#define	JPEG_CONFR5_HSF_1			0x00002000	//
#define	JPEG_CONFR5_HSF_2			0x00004000	//
#define	JPEG_CONFR5_HSF_3			0x00008000	//

// CONFR6 register

#define	JPEG_CONFR6_HD				0x00000001	//
#define	JPEG_CONFR6_HA				0x00000002	//
#define	JPEG_CONFR6_QT				0x0000000C	//
#define	JPEG_CONFR6_QT_0			0x00000004	//
#define	JPEG_CONFR6_QT_1			0x00000008	//
#define	JPEG_CONFR6_NB				0x000000F0	//
#define	JPEG_CONFR6_NB_0			0x00000010	//
#define	JPEG_CONFR6_NB_1			0x00000020	//
#define	JPEG_CONFR6_NB_2			0x00000040	//
#define	JPEG_CONFR6_NB_3			0x00000080	//
#define	JPEG_CONFR6_VSF				0x00000F00	//
#define	JPEG_CONFR6_VSF_0			0x00000100	//
#define	JPEG_CONFR6_VSF_1			0x00000200	//
#define	JPEG_CONFR6_VSF_2			0x00000400	//
#define	JPEG_CONFR6_VSF_3			0x00000800	//
#define	JPEG_CONFR6_HSF				0x0000F000	//
#define	JPEG_CONFR6_HSF_0			0x00001000	//
#define	JPEG_CONFR6_HSF_1			0x00002000	//
#define	JPEG_CONFR6_HSF_2			0x00004000	//
#define	JPEG_CONFR6_HSF_3			0x00008000	//

// CONFR7 register

#define	JPEG_CONFR7_HD				0x00000001	//
#define	JPEG_CONFR7_HA				0x00000002	//
#define	JPEG_CONFR7_QT				0x0000000C	//
#define	JPEG_CONFR7_QT_0			0x00000004	//
#define	JPEG_CONFR7_QT_1			0x00000008	//
#define	JPEG_CONFR7_NB				0x000000F0	//
#define	JPEG_CONFR7_NB_0			0x00000010	//
#define	JPEG_CONFR7_NB_1			0x00000020	//
#define	JPEG_CONFR7_NB_2			0x00000040	//
#define	JPEG_CONFR7_NB_3			0x00000080	//
#define	JPEG_CONFR7_VSF				0x00000F00	//
#define	JPEG_CONFR7_VSF_0			0x00000100	//
#define	JPEG_CONFR7_VSF_1			0x00000200	//
#define	JPEG_CONFR7_VSF_2			0x00000400	//
#define	JPEG_CONFR7_VSF_3			0x00000800	//
#define	JPEG_CONFR7_HSF				0x0000F000	//
#define	JPEG_CONFR7_HSF_0			0x00001000	//
#define	JPEG_CONFR7_HSF_1			0x00002000	//
#define	JPEG_CONFR7_HSF_2			0x00004000	//
#define	JPEG_CONFR7_HSF_3			0x00008000	//

// CR register

#define	JPEG_CR_JCEN				0x00000001	//
#define	JPEG_CR_IFTIE				0x00000002	//
#define	JPEG_CR_IFNFIE				0x00000004	//
#define	JPEG_CR_OFTIE				0x00000008	//
#define	JPEG_CR_OFNEIE				0x00000010	//
#define	JPEG_CR_EOCIE				0x00000020	//
#define	JPEG_CR_HPDIE				0x00000040	//
#define	JPEG_CR_IDMAEN				0x00000800	//
#define	JPEG_CR_ODMAEN				0x00001000	//
#define	JPEG_CR_IFF					0x00002000	//
#define	JPEG_CR_OFF					0x00004000	//

// SR register

#define	JPEG_SR_IFTF				0x00000002	//
#define	JPEG_SR_IFNFF				0x00000004	//
#define	JPEG_SR_OFTF				0x00000008	//
#define	JPEG_SR_OFNEF				0x00000010	//
#define	JPEG_SR_EOCF				0x00000020	//
#define	JPEG_SR_HPDF				0x00000040	//
#define	JPEG_SR_COF					0x00000080	//

// CFR register

#define	JPEG_CFR_CEOCF				0x00000020	//
#define	JPEG_CFR_CHPDF				0x00000040	//

// DIR register

#define	JPEG_DIR_DATAIN				0xFFFFFFFF	//

// DOR register

#define	JPEG_DOR_DATAOUT			0xFFFFFFFF	//
#endif
