/*
; stm32F479_spi.
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
; Goal:		stm32F479_spi equates.
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

#ifndef __STM32F479_SPI__
#define __STM32F479_SPI__

// SPI address definitions
// -----------------------

typedef struct {
	volatile	uint16_t	CR1;
	volatile	uint16_t	RESERVED0;
	volatile	uint16_t	CR2;
	volatile	uint16_t	RESERVED1;
	volatile	uint16_t	SR;
	volatile	uint16_t	RESERVED2;
	volatile	uint16_t	DR;
	volatile	uint16_t	RESERVED3;
	volatile	uint16_t	CRCPR;
	volatile	uint16_t	RESERVED4;
	volatile	uint16_t	RXCRCR;
	volatile	uint16_t	RESERVED5;
	volatile	uint16_t	TXCRCR;
	volatile	uint16_t	RESERVED6;
	volatile	uint16_t	I2SCFGR;
	volatile	uint16_t	RESERVED7;
	volatile	uint16_t	I2SPR;
	volatile	uint16_t	RESERVED8;
} SPI_TypeDef;

#define SPI1	((SPI_TypeDef *)SPI1_BASE)
#define SPI2	((SPI_TypeDef *)SPI2_BASE)
#define SPI3	((SPI_TypeDef *)SPI3_BASE)
#define SPI4	((SPI_TypeDef *)SPI4_BASE)
#define SPI5	((SPI_TypeDef *)SPI5_BASE)
#define SPI6	((SPI_TypeDef *)SPI6_BASE)
#define I2S2ext	((SPI_TypeDef *)I2S2ext_BASE)
#define I2S3ext	((SPI_TypeDef *)I2S3ext_BASE)

// SPI_CR1 register

#define	SPI_CR1_CPHA			0x0001		//
#define	SPI_CR1_CPOL			0x0002		//
#define	SPI_CR1_MSTR			0x0004		//

#define	SPI_CR1_BR  			0x0038		//
#define	SPI_CR1_BR_0			0x0008		//
#define	SPI_CR1_BR_1			0x0010		//
#define	SPI_CR1_BR_2			0x0020		//

#define	SPI_CR1_SPE 			0x0040		//
#define	SPI_CR1_LSBFIRST		0x0080		//
#define	SPI_CR1_SSI 			0x0100		//
#define	SPI_CR1_SSM 			0x0200		//
#define	SPI_CR1_RXONLY  		0x0400		//
#define	SPI_CR1_DFF 			0x0800		//
#define	SPI_CR1_CRCNEXT 		0x1000		//
#define	SPI_CR1_CRCEN   		0x2000		//
#define	SPI_CR1_BIDIOE  		0x4000		//
#define	SPI_CR1_BIDIMODE		0x8000		//

// SPI_CR2 register

#define	SPI_CR2_RXDMAEN			0x01		//
#define	SPI_CR2_TXDMAEN			0x02		//
#define	SPI_CR2_SSOE   			0x04		//
#define	SPI_CR2_FRF				0x10		//
#define	SPI_CR2_ERRIE  			0x20		//
#define	SPI_CR2_RXNEIE 			0x40		//
#define	SPI_CR2_TXEIE  			0x80		//

// SPI_SR register

#define	SPI_SR_RXNE    			0x01		//
#define	SPI_SR_TXE     			0x02		//
#define	SPI_SR_CHSIDE  			0x04		//
#define	SPI_SR_UDR     			0x08		//
#define	SPI_SR_CRCERR  			0x10		//
#define	SPI_SR_MODF    			0x20		//
#define	SPI_SR_OVR     			0x40		//
#define	SPI_SR_BSY     			0x80		//

// SPI_DR register

#define	SPI_DR_DR   			0xFFFF		//

// SPI_CRCPR register

#define	SPI_CRCPR_CRCPOLY		0xFFFF		//

// SPI_RXCRCR register

#define	SPI_RXCRCR_RXCRC		0xFFFF		//

// SPI_TXCRCR register

#define	SPI_TXCRCR_TXCRC		0xFFFF		//

// SPI_I2SCFGR register

#define	SPI_I2SCFGR_CHLEN		0x0001		//

#define	SPI_I2SCFGR_DATLEN		0x0006		//
#define	SPI_I2SCFGR_DATLEN_0	0x0002		//
#define	SPI_I2SCFGR_DATLEN_1	0x0004		//

#define	SPI_I2SCFGR_CKPOL		0x0008		//

#define	SPI_I2SCFGR_I2SSTD		0x0030		//
#define	SPI_I2SCFGR_I2SSTD_0	0x0010		//
#define	SPI_I2SCFGR_I2SSTD_1	0x0020		//

#define	SPI_I2SCFGR_PCMSYNC 	0x0080		//

#define	SPI_I2SCFGR_I2SCFG		0x0300		//
#define	SPI_I2SCFGR_I2SCFG_0	0x0100		//
#define	SPI_I2SCFGR_I2SCFG_1	0x0200		//

#define	SPI_I2SCFGR_I2SE		0x0400		//
#define	SPI_I2SCFGR_I2SMOD		0x0800		//
#define	SPI_I2SCFGR_ASTRTEN		0x1000		//

// SPI_I2SPR register

#define	SPI_I2SPR_I2SDIV		0x00FF		//
#define	SPI_I2SPR_ODD   		0x0100		//
#define	SPI_I2SPR_MCKOE 		0x0200		//
#endif
