/*
; stm32F479_usart.
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
; Goal:		stm32F479_usart equates.
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

#ifndef __STM32F479_USART__
#define __STM32F479_USART__

// USART address definitions
// -------------------------

typedef struct {
	volatile	uint16_t	SR;
	volatile	uint16_t	RESERVED0;
	volatile	uint16_t	DR;
	volatile	uint16_t	RESERVED1;
	volatile	uint16_t	BRR;
	volatile	uint16_t	RESERVED2;
	volatile	uint16_t	CR1;
	volatile	uint16_t	RESERVED3;
	volatile	uint16_t	CR2;
	volatile	uint16_t	RESERVED4;
	volatile	uint16_t	CR3;
	volatile	uint16_t	RESERVED5;
	volatile	uint16_t	GTPR;
	volatile	uint16_t	RESERVED6;
} USART_TypeDef;

#define USART1	((USART_TypeDef *)USART1_BASE)
#define USART2	((USART_TypeDef *)USART2_BASE)
#define USART3	((USART_TypeDef *)USART3_BASE)
#define UART4	((USART_TypeDef *)UART4_BASE)
#define UART5	((USART_TypeDef *)UART5_BASE)
#define USART6	((USART_TypeDef *)USART6_BASE)
#define UART7	((USART_TypeDef *)UART7_BASE)
#define UART8	((USART_TypeDef *)UART8_BASE)

// USART_SR register

#define	USART_SR_PE				0x0001		//
#define	USART_SR_FE				0x0002		//
#define	USART_SR_NE				0x0004		//
#define	USART_SR_ORE			0x0008		//
#define	USART_SR_IDLE			0x0010		//
#define	USART_SR_RXNE			0x0020		//
#define	USART_SR_TC				0x0040		//
#define	USART_SR_TXE			0x0080		//
#define	USART_SR_LBD			0x0100		//
#define	USART_SR_CTS			0x0200		//

// USART_DR register

#define	USART_DR_DR				0x01FF		//

// USART_BRR register

#define	USART_BRR_DIV_Fraction  0x000F		//
#define	USART_BRR_DIV_Mantissa  0xFFF0		//

// USART_CR1 register

#define	USART_CR1_SBK			0x0001		//
#define	USART_CR1_RWU			0x0002		//
#define	USART_CR1_RE			0x0004		//
#define	USART_CR1_TE			0x0008		//
#define	USART_CR1_IDLEIE		0x0010		//
#define	USART_CR1_RXNEIE		0x0020		//
#define	USART_CR1_TCIE  		0x0040		//
#define	USART_CR1_TXEIE 		0x0080		//
#define	USART_CR1_PEIE  		0x0100		//
#define	USART_CR1_PS			0x0200		//
#define	USART_CR1_PCE			0x0400		//
#define	USART_CR1_WAKE  		0x0800		//
#define	USART_CR1_M				0x1000		//
#define	USART_CR1_UE			0x2000		//
#define	USART_CR1_OVER8			0x8000		//

// USART_CR2 register

#define	USART_CR2_ADD			0x000F		//
#define	USART_CR2_LBDL  		0x0020		//
#define	USART_CR2_LBDIE 		0x0040		//
#define	USART_CR2_LBCL  		0x0100		//
#define	USART_CR2_CPHA  		0x0200		//
#define	USART_CR2_CPOL  		0x0400		//
#define	USART_CR2_CLKEN 		0x0800		//
#define	USART_CR2_STOP  		0x3000		//
#define	USART_CR2_STOP_0		0x1000		//
#define	USART_CR2_STOP_1		0x2000		//
#define	USART_CR2_LINEN 		0x4000		//

// USART_CR3 register

#define	USART_CR3_EIE			0x0001		//
#define	USART_CR3_IREN  		0x0002		//
#define	USART_CR3_IRLP  		0x0004		//
#define	USART_CR3_HDSEL 		0x0008		//
#define	USART_CR3_NACK  		0x0010		//
#define	USART_CR3_SCEN  		0x0020		//
#define	USART_CR3_DMAR  		0x0040		//
#define	USART_CR3_DMAT  		0x0080		//
#define	USART_CR3_RTSE  		0x0100		//
#define	USART_CR3_CTSE  		0x0200		//
#define	USART_CR3_CTSIE 		0x0400		//
#define	USART_CR3_ONEBIT 		0x0800		//

// USART_GTPR register

#define	USART_GTPR_PSC  		0x00FF		//
#define	USART_GTPR_PSC_0		0x0001		//
#define	USART_GTPR_PSC_1		0x0002		//
#define	USART_GTPR_PSC_2		0x0004		//
#define	USART_GTPR_PSC_3		0x0008		//
#define	USART_GTPR_PSC_4		0x0010		//
#define	USART_GTPR_PSC_5		0x0020		//
#define	USART_GTPR_PSC_6		0x0040		//
#define	USART_GTPR_PSC_7		0x0080		//
#define	USART_GTPR_GT			0xFF00		//
#endif
