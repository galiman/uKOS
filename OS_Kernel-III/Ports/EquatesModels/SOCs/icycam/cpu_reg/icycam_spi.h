/*
; icycam_spi.
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
; Goal:		icycam_spi equates.
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

#ifndef __ICYCAM_SPI__
#define __ICYCAM_SPI__

// SPI address definitions
// -----------------------

#define	SPI1_CTRL			(volatile uint16_t *)(SPI1 + 0x00)
#define	SPI2_CTRL			(volatile uint16_t *)(SPI2 + 0x00)
#define	BRXEN				0
#define	BTXEN				1
#define	BNSSALWAYSLOW		2
#define	BNSSEN				3
#define	BINIT				4
#define	BMSTR				5
#define	BCPOL				6
#define	BCPHA				7
#define	BLSB1ST				8
#define	BTX0				9
#define	BRX0				10
#define	BWS					11
#define	AETHRESH			(0<<12)
#define	AFTHRESH			(0<<14)

#define	SPI1_STATUS			(volatile uint16_t *)(SPI1 + 0x02)
#define	SPI2_STATUS			(volatile uint16_t *)(SPI2 + 0x02)
#define	SPI1_IRQ0_EN		(volatile uint16_t *)(SPI1 + 0x0C)
#define	SPI2_IRQ0_EN		(volatile uint16_t *)(SPI2 + 0x0C)
#define	SPI1_IRQ1_EN		(volatile uint16_t *)(SPI1 + 0x0E)
#define	SPI2_IRQ1_EN		(volatile uint16_t *)(SPI2 + 0x0E)
#define	BRXNE				0
#define	BRXUR				1
#define	BRXFU				2
#define	BRXOR				3
#define	BTXE				4
#define	BTXUR				5
#define	BTXNF				6
#define	BTXOR				7
#define	BRXAE				8
#define	BRXAF				9
#define	BTXAE				10
#define	BTXAF				11
#define	BRX					12
#define	BTX					13
#define	BNSS				14

#define	SPI1_BAUD_RATE		(volatile uint16_t *)(SPI1 + 0x04)
#define	SPI2_BAUD_RATE		(volatile uint16_t *)(SPI2 + 0x04)
#define	SPI1_NSS_CONFIG		(volatile uint16_t *)(SPI1 + 0x06)
#define	SPI2_NSS_CONFIG		(volatile uint16_t *)(SPI2 + 0x06)
#define	BNSS0				0

#define	SPI1_TX_DATA		(volatile uint8_t  *)(SPI1 + 0x08)
#define	SPI2_TX_DATA		(volatile uint8_t  *)(SPI2 + 0x08)
#define	SPI1_RX_DATA		(volatile uint8_t  *)(SPI1 + 0x0A)
#define	SPI2_RX_DATA		(volatile uint8_t  *)(SPI2 + 0x0A)
#endif
