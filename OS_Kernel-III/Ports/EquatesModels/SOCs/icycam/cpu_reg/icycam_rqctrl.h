/*
; icycam_rqctrl.
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
; Goal:		icycam_rqctrl equates.
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

#ifndef __ICYCAM_RQCTRL__
#define __ICYCAM_RQCTRL__

// RQCTRL address definitions
// --------------------------

#define	RQCTRL_IRQ_LEVEL_EN		(volatile uint8_t  *)(RQCTRL + 0x90)
#define	LEVEL0_0				0x01
#define	LEVEL0_1				0x03
#define	LEVEL0_2				0x07
#define	LEVEL0_3				0x0F
#define	LEVEL0_4				0x1F
#define	LEVEL0_5				0x3F
#define	LEVEL0_6				0x7F
#define	LEVEL0_7				0xFF

#define	RQCTRL_DMA_EN			(volatile uint8_t  *)(RQCTRL + 0x50)
#define	BRQRDCHANNEL0			0
#define	BRQRDCHANNEL1			1
#define	BRQRDCHANNEL2			2
#define	BRQRDCHANNEL3			3
#define	BRQWRCHANNEL0			4
#define	BRQWRCHANNEL1			5
#define	BRQRWRHANNEL2			6
#define	BRQWRCHANNEL3			7

#define	RQCTRL_SOURCE_EN		(volatile uint32_t *)(RQCTRL + 0x80)
#define	RQCTRL_SOURCE_EN_W1S	(volatile uint32_t *)(RQCTRL + 0x20)
#define	RQCTRL_SOURCE_EN_W1C	(volatile uint32_t *)(RQCTRL + 0x40)
#define	RQCTRL_SOURCE_SEL		(volatile uint32_t *)(RQCTRL + 0x30)
#define	BDMA					0
#define	BGPIO0					1
#define	BGPIO1					2
#define	BGPIO2					3
#define	BGPIO3					4
#define	BTIMER0					5
#define	BTIMER1					6
#define	BTIMER2					7
#define	BTIMER3					8
#define	BWATCHDOG				9
#define	BSPI0_0					10
#define	BSPI0_1					11
#define	BSPI1_0					12
#define	BSPI1_1					13
#define	BVSLOG_END_INIT			14
#define	BVSLOG_END_EXPOSURE		15
#define	BVSLOG_END_RAMP			16
#define	BVSLOG_END_SUMM			17
#define	BPPI0_0					18
#define	BPPI0_1					19
#define	BSDRAM_ERR				20
#define	BSDRAM_DATA				21
#define	BUART_0					22
#define	BUART_1					23
#define	BGPU0					24
#define	BGPU1					25
#define	BGPU2					26

#define	RQCTRL_DMA_CHANIX_0		(volatile uint32_t *)(RQCTRL + 0x18)
#define	DMACONFIG0				0
#define	DMACONFIG1				5
#define	DMACONFIG2				10
#define	DMACONFIG3				15
#define	DMACONFIG4				20
#define	DMACONFIG5				25

#define	RQCTRL_DMA_CHANIX_1		(volatile uint32_t *)(RQCTRL + 0x58)
#define	DMACONFIG6				0
#define	DMACONFIG7				5

#define	RQCTRL_IRQ_CONFIG_0		(volatile uint32_t *)(RQCTRL + 0x08)
#define	BPRIREQ00				0
#define	BPRIREQ01				3
#define	BPRIREQ02				6
#define	BPRIREQ03				9
#define	BPRIREQ04				12
#define	BPRIREQ05				15
#define	BPRIREQ06				18
#define	BPRIREQ07				21
#define	BPRIREQ08				24
#define	BPRIREQ09				27

#define	RQCTRL_IRQ_CONFIG_1		(volatile uint32_t *)(RQCTRL + 0x48)
#define	BPRIREQ10				0
#define	BPRIREQ11				3
#define	BPRIREQ12				6
#define	BPRIREQ13				9
#define	BPRIREQ14				12
#define	BPRIREQ15				15
#define	BPRIREQ16				18
#define	BPRIREQ17				21
#define	BPRIREQ18				24
#define	BPRIREQ19				27

#define	RQCTRL_IRQ_CONFIG_2		(volatile uint32_t *)(RQCTRL + 0x88)
#define	BPRIREQ20				0
#define	BPRIREQ21				3
#define	BPRIREQ22				6
#define	BPRIREQ23				9
#define	BPRIREQ24				12
#define	BPRIREQ25				15
#define	BPRIREQ26				18

#define	RQCTRL_IRQ_VEC_0		(volatile uint32_t *)(RQCTRL + 0x04)
#define	RQCTRL_IRQ_VEC_1		(volatile uint32_t *)(RQCTRL + 0x0C)
#define	RQCTRL_IRQ_VEC_2		(volatile uint32_t *)(RQCTRL + 0x14)
#define	RQCTRL_IRQ_VEC_3		(volatile uint32_t *)(RQCTRL + 0x1C)
#define	RQCTRL_IRQ_VEC_4		(volatile uint32_t *)(RQCTRL + 0x24)
#define	RQCTRL_IRQ_VEC_5		(volatile uint32_t *)(RQCTRL + 0x2C)
#define	RQCTRL_IRQ_VEC_6		(volatile uint32_t *)(RQCTRL + 0x34)
#define	RQCTRL_IRQ_VEC_7		(volatile uint32_t *)(RQCTRL + 0x3C)
#define	RQCTRL_IRQ_VEC_8		(volatile uint32_t *)(RQCTRL + 0x44)
#define	RQCTRL_IRQ_VEC_9		(volatile uint32_t *)(RQCTRL + 0x4C)
#define	RQCTRL_IRQ_VEC_10		(volatile uint32_t *)(RQCTRL + 0x54)
#define	RQCTRL_IRQ_VEC_11		(volatile uint32_t *)(RQCTRL + 0x5C)
#define	RQCTRL_IRQ_VEC_12		(volatile uint32_t *)(RQCTRL + 0x64)
#define	RQCTRL_IRQ_VEC_13		(volatile uint32_t *)(RQCTRL + 0x6C)
#define	RQCTRL_IRQ_VEC_14		(volatile uint32_t *)(RQCTRL + 0x74)
#define	RQCTRL_IRQ_VEC_15		(volatile uint32_t *)(RQCTRL + 0x7C)
#define	RQCTRL_IRQ_VEC_16		(volatile uint32_t *)(RQCTRL + 0x84)
#define	RQCTRL_IRQ_VEC_17		(volatile uint32_t *)(RQCTRL + 0x8C)
#define	RQCTRL_IRQ_VEC_18		(volatile uint32_t *)(RQCTRL + 0x94)
#define	RQCTRL_IRQ_VEC_19		(volatile uint32_t *)(RQCTRL + 0x9C)
#define	RQCTRL_IRQ_VEC_20		(volatile uint32_t *)(RQCTRL + 0xA4)
#define	RQCTRL_IRQ_VEC_21		(volatile uint32_t *)(RQCTRL + 0xAC)
#define	RQCTRL_IRQ_VEC_22		(volatile uint32_t *)(RQCTRL + 0xB4)
#define	RQCTRL_IRQ_VEC_23		(volatile uint32_t *)(RQCTRL + 0xBC)
#define	RQCTRL_IRQ_VEC_24		(volatile uint32_t *)(RQCTRL + 0xC4)
#define	RQCTRL_IRQ_VEC_25		(volatile uint32_t *)(RQCTRL + 0xCC)
#define	RQCTRL_IRQ_VEC_26		(volatile uint32_t *)(RQCTRL + 0xD4)
#endif
