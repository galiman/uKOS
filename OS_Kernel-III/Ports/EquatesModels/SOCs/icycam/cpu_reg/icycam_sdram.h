/*
; icycam_sdram.
; =============

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
; Goal:		icycam_sdram equates.
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

#ifndef __ICYCAM_SDRAM__
#define __ICYCAM_SDRAM__

// SDRAM address definitions
// -------------------------

#define	SDRAMCTRL_MR			(volatile uint16_t *)(SDRAM + 0x00)
#define	KBURST_LEN_1			0
#define	KBURST_LEN_2			1
#define	KBURST_LEN_4			2
#define	KBURST_LEN_8			3
#define	BURST_INTERLEAVED		3
#define	CAS_LETENCY_2			(2<<4)
#define	CAS_LETENCY_3			(3<<4)
#define	BWRITE_SINGLE_LOCATION	9

#define	SDRAMCTRL_EMR			(volatile uint16_t *)(SDRAM + 0x02)
#define	PARTIAL_ARRAY_FULL		0
#define	PARTIAL_ARRAY_HALF		1
#define	PARTIAL_ARRAY_QUAT		2
#define	PARTIAL_ARRAY_1O8		5
#define	PARTIAL_ARRAY_1O16		6
#define	DRIVER_STRENGT_FULL		(0<<5)
#define	DRIVER_STRENGT_HALF		(1<<5)
#define	DRIVER_STRENGT_1O4		(2<<5)
#define	DRIVER_STRENGT_1O8		(3<<5)
#define	MODE_EXTENDED			(2<<13)

#define	SDRAMCTRL_TREF			(volatile uint16_t *)(SDRAM + 0x04)
#define	SDRAMCTRL_TPUP			(volatile uint16_t *)(SDRAM + 0x06)
#define	SDRAMCTRL_CTRL			(volatile uint8_t  *)(SDRAM + 0x08)
#define	BSYNC_RESET				0
#define	BSDRAM_ENABLE			1
#define	BSDRAM_INIT				2
#define	SIZE_9					(0<3)
#define	SIZE_10					(1<3)
#define	SIZE_11					(2<3)
#define	SIZE_12					(3<3)


#define	SDRAMCTRL_TRFC			(volatile uint8_t  *)(SDRAM + 0x0A)
#define	SDRAMCTRL_TRP			(volatile uint8_t  *)(SDRAM + 0x0B)
#define	SDRAMCTRL_TMRD			(volatile uint8_t  *)(SDRAM + 0x0C)
#define	SDRAMCTRL_TCUSTOM		(volatile uint8_t  *)(SDRAM + 0x0D)
#define	SDRAMCTRL_TRAS			(volatile uint8_t  *)(SDRAM + 0x0E)
#define	SDRAMCTRL_TRCD			(volatile uint8_t  *)(SDRAM + 0x0F)
#define	SDRAMCTRL_TWR			(volatile uint8_t  *)(SDRAM + 0x10)
#define	SDRAMCTRL_TCCD			(volatile uint8_t  *)(SDRAM + 0x11)
#define	SDRAMCTRL_TIDLE			(volatile uint8_t  *)(SDRAM + 0x12)
#define	SDRAMCTRL_IRQ0_ENABLE	(volatile uint8_t  *)(SDRAM + 0x14)
#define	SDRAMCTRL_IRQ1_ENABLE	(volatile uint8_t  *)(SDRAM + 0x18)
#define	SDRAMCTRL_IRQ_STATUS	(volatile uint8_t  *)(SDRAM + 0x1C)
#define	BIRQ_INIT_DONE			0

#define	SDRAMCTRL_SD_CTRL		(volatile uint8_t  *)(SDRAM + 0x20)
#endif
