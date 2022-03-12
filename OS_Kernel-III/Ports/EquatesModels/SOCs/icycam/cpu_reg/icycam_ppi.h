/*
; icycam_ppi.
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
; Goal:		icycam_ppi equates.
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

#ifndef __ICYCAM_PPI__
#define __ICYCAM_PPI__

// PPI address definitions
// -----------------------

#define	PPI_CTRL				(volatile uint8_t  *)(PPI + 0x00)
#define	BPPI_E					0
#define	BPPI_PORT_E				1
#define	BPPI_DIR				2
#define	BPPI_CLK_POL			3
#define	BPPI_VALID_E			4
#define	BPPI_EXT_SYNC			5
#define	BPPI_EXT_CK				6
#define	BPPI_F50				7

#define	PPI_CTRL_FIFO			(volatile uint8_t  *)(PPI + 0x01)
#define	BPPI_RESET_FIFO			0
#define	BPPI_FIFO_OVERWRITE		1
#define	BPPI_TEST_E				6

#define	PPI_IRQ0_E				(volatile uint8_t  *)(PPI + 0x02)
#define	PPI_IRQ1_E				(volatile uint8_t  *)(PPI + 0x03)
#define	BIRQ_FIFO_EMPTY			0
#define	BIRQ_FIFO_FULL			1
#define	BIRQ_FIFO_ALMOST_EMPTY	2
#define	BIRQ_FIFO_ALMOST_FULL	3
#define	BIRQ_FIFO_UNDERERRUN	4
#define	BIRQ_FIFO_OVERRUN		5
#define	BIRQ_FIFO_NOT_EMPTY		6
#define	BIRQ_FIFO_NOT_FULL		7

#define	PPI_STATUS				(volatile uint8_t  *)(PPI + 0x04)
#define	BPPI_FIFO_EMPTY			0
#define	BPPI_FIFO_FULL			1
#define	BPPI_FIFO_ALMOST_EMPTY	2
#define	BPPI_FIFO_ALMOST_FULL	3
#define	BPPI_FIFO_UNDERRUN		4
#define	BPPI_FIFO_OVERRUN		5

#define	PPI_DATA_8B				(volatile uint8_t  *)(PPI + 0x05)
#define	PPI_DATA				(volatile uint16_t *)(PPI + 0x06)
#define	PPI_FIFO_COUNTER		(volatile uint8_t  *)(PPI + 0x08)
#define	PPI_FIFO_TAIL			(volatile uint8_t  *)(PPI + 0x09)
#define	PPI_FIFO_HEAD			(volatile uint8_t  *)(PPI + 0x0A)
#define	PPI_FIFO_REG0			(volatile uint16_t *)(PPI + 0x0C)
#endif
