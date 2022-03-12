/*
; stm32L476_spi.
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
; Goal:		stm32L476_spi equates.
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

#ifndef __STM32L476_SPI__
#define __STM32L476_SPI__

// SPI address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR1;
	volatile	uint32_t	CR2;
	volatile	uint32_t	SR;
	volatile	uint32_t	DR;
	volatile	uint32_t	CRCPR;
	volatile	uint32_t	RXCRCR;
	volatile	uint32_t	TXCRCR;
	volatile	uint32_t	RESERVED1;
	volatile	uint32_t	RESERVED2;
} SPI_TypeDef;

#define SPI1	((SPI_TypeDef *)SPI1_BASE)
#define SPI2	((SPI_TypeDef *)SPI2_BASE)
#define SPI3	((SPI_TypeDef *)SPI3_BASE)

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
#define	SPI_CR1_CRCL 			0x0800		//
#define	SPI_CR1_CRCNEXT 		0x1000		//
#define	SPI_CR1_CRCEN   		0x2000		//
#define	SPI_CR1_BIDIOE  		0x4000		//
#define	SPI_CR1_BIDIMODE		0x8000		//

// SPI_CR2 register

#define	SPI_CR2_RXDMAEN			0x0001		//
#define	SPI_CR2_TXDMAEN			0x0002		//
#define	SPI_CR2_SSOE   			0x0004		//
#define	SPI_CR2_NSSP			0x0008		//
#define	SPI_CR2_FRF				0x0010		//
#define	SPI_CR2_ERRIE  			0x0020		//
#define	SPI_CR2_RXNEIE 			0x0040		//
#define	SPI_CR2_TXEIE  			0x0080		//
#define	SPI_CR2_DS				0x0F00		//
#define	SPI_CR2_DS_0			0x0100		//
#define	SPI_CR2_DS_1			0x0200		//
#define	SPI_CR2_DS_2			0x0400		//
#define	SPI_CR2_DS_3			0x0800		//
#define	SPI_CR2_FRXTH			0x1000		//
#define	SPI_CR2_LDMARX			0x2000		//
#define	SPI_CR2_LDMATX			0x4000		//

// SPI_SR register

#define	SPI_SR_RXNE    			0x0001		//
#define	SPI_SR_TXE     			0x0002		//
#define	SPI_SR_CHSIDE  			0x0004		//
#define	SPI_SR_UDR     			0x0008		//
#define	SPI_SR_CRCERR  			0x0010		//
#define	SPI_SR_MODF    			0x0020		//
#define	SPI_SR_OVR     			0x0040		//
#define	SPI_SR_BSY     			0x0080		//
#define	SPI_SR_FRE				0x0100		//
#define	SPI_SR_FRLVL			0x0600		//
#define	SPI_SR_FRLVL_0			0x0200		//
#define	SPI_SR_FRLVL_1			0x0400		//
#define	SPI_SR_FTLVL			0x1800		//
#define	SPI_SR_FTLVL_0			0x0800		//
#define SPI_SR_FTLVL_1			0x1000		//

// SPI_DR register

#define	SPI_DR_DR   			0xFFFF		//

// SPI_CRCPR register

#define	SPI_CRCPR_CRCPOLY		0xFFFF		//

// SPI_RXCRCR register

#define	SPI_RXCRCR_RXCRC		0xFFFF		//

// SPI_TXCRCR register

#define	SPI_TXCRCR_TXCRC		0xFFFF		//
#endif
