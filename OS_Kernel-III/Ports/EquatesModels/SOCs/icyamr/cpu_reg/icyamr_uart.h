/*
; icyamr_uart.
; ============

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
; Goal:		icyamr_uart equates.
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

#ifndef __ICYAMR_UART__
#define __ICYAMR_UART__

// UART address definitions
// ------------------------

#define	UART_CTRL				(volatile uint8_t  *)(UART + 0x00)
#define	BWORD_LENGTH8			0
#define	BPARITY_E				1
#define	BPARITY_ODD				2
#define	BINV_TX_PAD				3
#define	BINV_RX_PAD				4
#define	BTX_E					5
#define	BRX_E					6
#define	BECHO					7

#define	UART_BAUD				(volatile uint8_t  *)(UART + 0x01)
#define	DIVPR1					(0<<4)
#define	DIVPR2					(1<<4)
#define	DIVPR4					(2<<4)
#define	DIVPR8					(3<<4)
#define	DIVPR16					(4<<4)
#define	DIVPR32					(5<<4)
#define	DIVPR64					(6<<4)
#define	DIVPR128				(7<<4)
#define	DIV14					(1<<7)
#define	DIV16					(0<<7)

#define	UART_TX					(volatile uint8_t  *)(UART + 0x02)
#define	UART_RX					(volatile uint8_t  *)(UART + 0x03)
#define	UART_TXSTA				(volatile uint8_t  *)(UART + 0x04)
#define	BTX_BUSY				0
#define	BTX_FULL				1
#define	BTX_EMPTY				2
#define	BTX_ALMOST_FULL			3
#define	BTX_ALMOST_EMPTY		4
#define	BTX_OVERRUN				5
#define	BTX_UNDERRRUN			6

#define	UART_RXSTA				(volatile uint16_t *)(UART + 0x06)
#define	BRX_BUSY				0
#define	BRX_FULL				1
#define	BRX_EMPTY				2
#define	BRX_ALMOST_FULL			3
#define	BRX_ALMOST_EMPTY		4
#define	BRX_OVERRUN				5
#define	BRX_UNDERRRUN			6
#define	BRX_START_ERROR			7
#define	BRX_PARITY_ERROR		8
#define	BRX_FRAME_ERROR			9

#define	UART_IRQ0_E				(volatile uint32_t *)(UART + 0x08)
#define	UART_IRQ1_E				(volatile uint32_t *)(UART + 0x0C)
#define	BRQX_TX_FULL			0
#define	BRQX_TX_EMPTY			1
#define	BRQX_TX_ALMOST_FULL		2
#define	BRQX_TX_ALMOST_EMPTY	3
#define	BRQX_TX_NOT_FULL		4
#define	BRQX_TX_NOT_EMPTY		5
#define	BRQX_TX_OVERRUN			6
#define	BRQX_TX_UNDERERRUN		7
#define	BRQX_RX_FULL			8
#define	BRQX_RX_EMPTY			9
#define	BRQX_RX_ALMOST_FULL		10
#define	BRQX_RX_ALMOST_EMPTY	11
#define	BRQX_RX_NOT_FULL		12
#define	BRQX_RX_NOT_EMPTY		13
#define	BRQX_RX_OVERRUN			14
#define	BRQX_RX_UNDERERRUN		15
#define	BRQX_RX_START_ERROR		16
#define	BRQX_RX_PARITY_ERROR	17
#define	BRQX_RX_FRAME_ERROR		18

#define	UART_FIFO_TX_CTRL		(volatile uint8_t *)(UART + 0x10)
#define	BRESET_FIFO				0

#define	UART_FIFO_TX_COUNTER	(volatile uint8_t *)(UART + 0x11)
#define	UART_FIFO_TX_TAIL		(volatile uint8_t *)(UART + 0x12)
#define	UART_FIFO_TX_HEAD		(volatile uint8_t *)(UART + 0x13)
#define	UART_FIFO_RX_CTRL		(volatile uint8_t *)(UART + 0x14)
#define	UART_FIFO_RX_COUNTER	(volatile uint8_t *)(UART + 0x15)
#define	UART_FIFO_RX_TAIL		(volatile uint8_t *)(UART + 0x16)
#define	UART_FIFO_RX_HEAD		(volatile uint8_t *)(UART + 0x17)
#define	UART_FIFO_TX_REG0		(volatile uint8_t *)(UART + 0x18)
#define	UART_FIFO_RX_REG0		(volatile uint8_t *)(UART + 0x19)
#endif
