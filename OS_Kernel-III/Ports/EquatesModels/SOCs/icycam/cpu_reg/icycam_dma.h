/*
; icycam_dma.
; ===========

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
; Goal:		icycam_dma equates.
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

#ifndef __ICYCAM_DMA__
#define __ICYCAM_DMA__

// DMA address definitions
// -----------------------

#define	DMA0_PRIORITY			(volatile uint8_t  *)(DMA0 + 0x00)
#define	DMA1_PRIORITY			(volatile uint8_t  *)(DMA1 + 0x00)
#define	DMA2_PRIORITY			(volatile uint8_t  *)(DMA2 + 0x00)
#define	DMA3_PRIORITY			(volatile uint8_t  *)(DMA3 + 0x00)
#define	DMA0_IRQCONFIG			(volatile uint8_t  *)(DMA0 + 0x01)
#define	DMA1_IRQCONFIG			(volatile uint8_t  *)(DMA1 + 0x01)
#define	DMA2_IRQCONFIG			(volatile uint8_t  *)(DMA2 + 0x01)
#define	DMA3_IRQCONFIG			(volatile uint8_t  *)(DMA3 + 0x01)
#define	BSRCIRQBUFEN			2
#define	BSRCIRQLINEN			3
#define	BDSTIRQBUFEN			4
#define	BDSTIRQLINEN			5

#define	DMA0_STATUS				(volatile uint8_t  *)(DMA0 + 0x02)
#define	DMA1_STATUS				(volatile uint8_t  *)(DMA1 + 0x02)
#define	DMA2_STATUS				(volatile uint8_t  *)(DMA2 + 0x02)
#define	DMA3_STATUS				(volatile uint8_t  *)(DMA3 + 0x02)
#define	BEN						0
#define	BPAUSE					1
#define	BSRCIRQBUFF				2
#define	BSRCIRQLINE				3
#define	BDSTIRQBUFF				4
#define	BDSTIRQLINE				5
#define	BDMAERR					6

#define	DMA0_DESCR				(volatile uint16_t *)(DMA0 + 0x04)
#define	DMA1_DESCR				(volatile uint16_t *)(DMA1 + 0x04)
#define	DMA2_DESCR				(volatile uint16_t *)(DMA2 + 0x04)
#define	DMA3_DESCR				(volatile uint16_t *)(DMA3 + 0x04)
#define	DMA0_CTRL				(volatile uint16_t *)(DMA0 + 0x06)
#define	DMA1_CTRL				(volatile uint16_t *)(DMA1 + 0x06)
#define	DMA2_CTRL				(volatile uint16_t *)(DMA2 + 0x06)
#define	DMA3_CTRL				(volatile uint16_t *)(DMA3 + 0x06)
#define	BEN						0
#define	BPAUSE					1
#define	BDSTRESTART				2
#define	BSRCRESTART				3
#define	BMODEDST2D				4
#define	BMODESRC2D				5
#define	BMEMDST					6
#define	BMEMSRC					7
#define	DSTSIZE_8				(0<<8)
#define	DSTSIZE_16				(1<<8)
#define	DSTSIZE_32				(2<<8)
#define	DSTSIZE_64				(3<<8)
#define	SRCSIZE_8				(0<<10)
#define	SRCSIZE_16				(1<<10)
#define	SRCSIZE_32				(2<<10)
#define	SRCSIZE_64				(3<<10)
#define	BBYTESWAP				12
#define	BFLUSH					13
#define	BDESCR					14
#define	BALLOWRW				15

#define	DMA0_SRC_X_COUNT		(volatile uint16_t *)(DMA0 + 0x08)
#define	DMA1_SRC_X_COUNT		(volatile uint16_t *)(DMA1 + 0x08)
#define	DMA2_SRC_X_COUNT		(volatile uint16_t *)(DMA2 + 0x08)
#define	DMA3_SRC_X_COUNT		(volatile uint16_t *)(DMA3 + 0x08)
#define	DMA0_SRC_Y_COUNT		(volatile uint16_t *)(DMA0 + 0x0A)
#define	DMA1_SRC_Y_COUNT		(volatile uint16_t *)(DMA1 + 0x0A)
#define	DMA2_SRC_Y_COUNT		(volatile uint16_t *)(DMA2 + 0x0A)
#define	DMA3_SRC_Y_COUNT		(volatile uint16_t *)(DMA3 + 0x0A)
#define	DMA0_SRC_X_CUR_COUNT	(volatile uint16_t *)(DMA0 + 0x0C)
#define	DMA1_SRC_X_CUR_COUNT	(volatile uint16_t *)(DMA1 + 0x0C)
#define	DMA2_SRC_X_CUR_COUNT	(volatile uint16_t *)(DMA2 + 0x0C)
#define	DMA3_SRC_X_CUR_COUNT	(volatile uint16_t *)(DMA3 + 0x0C)
#define	DMA0_SRC_Y_CUR_COUNT	(volatile uint16_t *)(DMA0 + 0x0E)
#define	DMA1_SRC_Y_CUR_COUNT	(volatile uint16_t *)(DMA1 + 0x0E)
#define	DMA2_SRC_Y_CUR_COUNT	(volatile uint16_t *)(DMA2 + 0x0E)
#define	DMA3_SRC_Y_CUR_COUNT	(volatile uint16_t *)(DMA3 + 0x0E)
#define	DMA0_DST_X_COUNT		(volatile uint16_t *)(DMA0 + 0x12)
#define	DMA1_DST_X_COUNT		(volatile uint16_t *)(DMA1 + 0x12)
#define	DMA2_DST_X_COUNT		(volatile uint16_t *)(DMA2 + 0x12)
#define	DMA3_DST_X_COUNT		(volatile uint16_t *)(DMA3 + 0x12)
#define	DMA0_DST_Y_COUNT		(volatile uint16_t *)(DMA0 + 0x14)
#define	DMA1_DST_Y_COUNT		(volatile uint16_t *)(DMA1 + 0x14)
#define	DMA2_DST_Y_COUNT		(volatile uint16_t *)(DMA2 + 0x14)
#define	DMA3_DST_Y_COUNT		(volatile uint16_t *)(DMA3 + 0x14)
#define	DMA0_DST_X_CUR_COUNT	(volatile uint16_t *)(DMA0 + 0x16)
#define	DMA1_DST_X_CUR_COUNT	(volatile uint16_t *)(DMA1 + 0x16)
#define	DMA2_DST_X_CUR_COUNT	(volatile uint16_t *)(DMA2 + 0x16)
#define	DMA3_DST_X_CUR_COUNT	(volatile uint16_t *)(DMA3 + 0x16)
#define	DMA0_DST_Y_CUR_COUNT	(volatile uint16_t *)(DMA0 + 0x18)
#define	DMA1_DST_Y_CUR_COUNT	(volatile uint16_t *)(DMA1 + 0x18)
#define	DMA2_DST_Y_CUR_COUNT	(volatile uint16_t *)(DMA2 + 0x18)
#define	DMA3_DST_Y_CUR_COUNT	(volatile uint16_t *)(DMA3 + 0x18)
#define	DMA0_SRC_X_STRIDE		(volatile uint32_t *)(DMA0 + 0x1C)
#define	DMA1_SRC_X_STRIDE		(volatile uint32_t *)(DMA1 + 0x1C)
#define	DMA2_SRC_X_STRIDE		(volatile uint32_t *)(DMA2 + 0x1C)
#define	DMA3_SRC_X_STRIDE		(volatile uint32_t *)(DMA3 + 0x1C)
#define	DMA0_SRC_Y_STRIDE		(volatile uint32_t *)(DMA0 + 0x20)
#define	DMA1_SRC_Y_STRIDE		(volatile uint32_t *)(DMA1 + 0x20)
#define	DMA2_SRC_Y_STRIDE		(volatile uint32_t *)(DMA2 + 0x20)
#define	DMA3_SRC_Y_STRIDE		(volatile uint32_t *)(DMA3 + 0x20)
#define	DMA0_DST_X_STRIDE		(volatile uint32_t *)(DMA0 + 0x24)
#define	DMA1_DST_X_STRIDE		(volatile uint32_t *)(DMA1 + 0x24)
#define	DMA2_DST_X_STRIDE		(volatile uint32_t *)(DMA2 + 0x24)
#define	DMA3_DST_X_STRIDE		(volatile uint32_t *)(DMA3 + 0x24)
#define	DMA0_DST_Y_STRIDE		(volatile uint32_t *)(DMA0 + 0x28)
#define	DMA1_DST_Y_STRIDE		(volatile uint32_t *)(DMA1 + 0x28)
#define	DMA2_DST_Y_STRIDE		(volatile uint32_t *)(DMA2 + 0x28)
#define	DMA3_DST_Y_STRIDE		(volatile uint32_t *)(DMA3 + 0x28)
#define	DMA0_SRC_START_ADDR		(volatile uint32_t *)(DMA0 + 0x2C)
#define	DMA1_SRC_START_ADDR		(volatile uint32_t *)(DMA1 + 0x2C)
#define	DMA2_SRC_START_ADDR		(volatile uint32_t *)(DMA2 + 0x2C)
#define	DMA3_SRC_START_ADDR		(volatile uint32_t *)(DMA3 + 0x2C)
#define	DMA0_SRC_CUR_ADDR		(volatile uint32_t *)(DMA0 + 0x30)
#define	DMA1_SRC_CUR_ADDR		(volatile uint32_t *)(DMA1 + 0x30)
#define	DMA2_SRC_CUR_ADDR		(volatile uint32_t *)(DMA2 + 0x30)
#define	DMA3_SRC_CUR_ADDR		(volatile uint32_t *)(DMA3 + 0x30)
#define	DMA0_DST_START_ADDR		(volatile uint32_t *)(DMA0 + 0x34)
#define	DMA1_DST_START_ADDR		(volatile uint32_t *)(DMA1 + 0x34)
#define	DMA2_DST_START_ADDR		(volatile uint32_t *)(DMA2 + 0x34)
#define	DMA3_DST_START_ADDR		(volatile uint32_t *)(DMA3 + 0x34)
#define	DMA0_DST_CUR_ADDR		(volatile uint32_t *)(DMA0 + 0x38)
#define	DMA1_DST_CUR_ADDR		(volatile uint32_t *)(DMA1 + 0x38)
#define	DMA2_DST_CUR_ADDR		(volatile uint32_t *)(DMA2 + 0x38)
#define	DMA3_DST_CUR_ADDR		(volatile uint32_t *)(DMA3 + 0x38)
#endif
