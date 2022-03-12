/*
; stm32F777_i2c.
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
; Goal:		stm32F777_i2c equates.
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

#ifndef __STM32F777_I2C__
#define __STM32F777_I2C__

// I2C address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR1;
	volatile	uint32_t	CR2;
	volatile	uint32_t	OAR1;
	volatile	uint32_t	OAR2;
	volatile	uint32_t	TIMINGR;
	volatile	uint32_t	TIMEOUTR;
	volatile	uint32_t	ISR;
	volatile	uint32_t	ICR;
	volatile	uint32_t	PECR;
	volatile	uint32_t	RXDR;
	volatile	uint32_t	TXDR;
} I2C_TypeDef;

#define I2C1	((I2C_TypeDef *)I2C1_BASE)
#define I2C2	((I2C_TypeDef *)I2C2_BASE)
#define I2C3	((I2C_TypeDef *)I2C3_BASE)
#define I2C4	((I2C_TypeDef *)I2C4_BASE)

// I2C_CR1 register

#define	I2C_CR1_PE				0x00000001		//
#define	I2C_CR1_TXIE     		0x00000002		//
#define	I2C_CR1_RXIE     		0x00000004		//
#define	I2C_CR1_ADDRIE   		0x00000008		//
#define	I2C_CR1_NACKIE   		0x00000010		//
#define	I2C_CR1_STOPIE   		0x00000020		//
#define	I2C_CR1_TCIE     		0x00000040		//
#define	I2C_CR1_ERRIE    		0x00000080		//
#define	I2C_CR1_DFN      		0x00000F00		//
#define	I2C_CR1_ANFOFF   		0x00001000		//
#define	I2C_CR1_SWRST    		0x00002000		//
#define	I2C_CR1_TXDMAEN  		0x00004000		//
#define	I2C_CR1_RXDMAEN  		0x00008000		//
#define	I2C_CR1_SBC      		0x00010000		//
#define	I2C_CR1_NOSTRETCH		0x00020000		//
#define	I2C_CR1_WUPEN    		0x00040000		//
#define	I2C_CR1_GCEN     		0x00080000		//
#define	I2C_CR1_SMBHEN   		0x00100000		//
#define	I2C_CR1_SMBDEN   		0x00200000		//
#define	I2C_CR1_ALERTEN  		0x00400000		//
#define	I2C_CR1_PECEN    		0x00800000		//

// I2C_CR2 register

#define	I2C_CR2_SADD    		0x000003FF		//
#define	I2C_CR2_RD_WRN  		0x00000400		//
#define	I2C_CR2_ADD10   		0x00000800		//
#define	I2C_CR2_HEAD10R 		0x00001000		//
#define	I2C_CR2_START   		0x00002000		//
#define	I2C_CR2_STOP    		0x00004000		//
#define	I2C_CR2_NACK    		0x00008000		//
#define	I2C_CR2_NBYTES  		0x00FF0000		//
#define	I2C_CR2_RELOAD  		0x01000000		//
#define	I2C_CR2_AUTOEND 		0x02000000		//
#define	I2C_CR2_PECBYTE 		0x04000000		//

// I2C_OAR1 register

#define	I2C_OAR1_OA1    		0x000003FF		//
#define	I2C_OAR1_OA1MODE		0x00000400		//
#define	I2C_OAR1_OA1EN  		0x00008000		//

// I2C_OAR2 register

#define	I2C_OAR2_OA2      		0x000000FE		//
#define	I2C_OAR2_OA2MSK   		0x00000700		//
#define	I2C_OAR2_OA2NOMASK		0x00000000		//
#define	I2C_OAR2_OA2MASK01		0x00000100		//
#define	I2C_OAR2_OA2MASK02		0x00000200		//
#define	I2C_OAR2_OA2MASK03		0x00000300		//
#define	I2C_OAR2_OA2MASK04		0x00000400		//
#define	I2C_OAR2_OA2MASK05		0x00000500		//
#define	I2C_OAR2_OA2MASK06		0x00000600		//
#define	I2C_OAR2_OA2MASK07		0x00000700		//
#define	I2C_OAR2_OA2EN    		0x00008000		//

// I2C_TIMINGR register

#define	I2C_TIMINGR_SCLL  		0x000000FF		//
#define	I2C_TIMINGR_SCLH  		0x0000FF00		//
#define	I2C_TIMINGR_SDADEL		0x000F0000		//
#define	I2C_TIMINGR_SCLDEL		0x00F00000		//
#define	I2C_TIMINGR_PRESC 		0xF0000000		//

// I2C_TIMEOUTR register

#define	I2C_TIMEOUTR_TIMEOUTA	0x00000FFF		//
#define	I2C_TIMEOUTR_TIDLE   	0x00001000		//
#define	I2C_TIMEOUTR_TIMOUTEN	0x00008000		//
#define	I2C_TIMEOUTR_TIMEOUTB	0x0FFF0000		//
#define	I2C_TIMEOUTR_TEXTEN  	0x80000000		//

// I2C_ISR register

#define	I2C_ISR_TXE    			0x00000001		//
#define	I2C_ISR_TXIS   			0x00000002		//
#define	I2C_ISR_RXNE   			0x00000004		//
#define	I2C_ISR_ADDR   			0x00000008		//
#define	I2C_ISR_NACKF  			0x00000010		//
#define	I2C_ISR_STOPF  			0x00000020		//
#define	I2C_ISR_TC     			0x00000040		//
#define	I2C_ISR_TCR    			0x00000080		//
#define	I2C_ISR_BERR   			0x00000100		//
#define	I2C_ISR_ARLO   			0x00000200		//
#define	I2C_ISR_OVR    			0x00000400		//
#define	I2C_ISR_PECERR 			0x00000800		//
#define	I2C_ISR_TIMEOUT			0x00001000		//
#define	I2C_ISR_ALERT  			0x00002000		//
#define	I2C_ISR_BUSY   			0x00008000		//
#define	I2C_ISR_DIR    			0x00010000		//
#define	I2C_ISR_ADDCODE			0x00FE0000		//

// I2C_ICR register

#define	I2C_ICR_ADDRCF  		0x00000008		//
#define	I2C_ICR_NACKCF  		0x00000010		//
#define	I2C_ICR_STOPCF  		0x00000020		//
#define	I2C_ICR_BERRCF  		0x00000100		//
#define	I2C_ICR_ARLOCF  		0x00000200		//
#define	I2C_ICR_OVRCF   		0x00000400		//
#define	I2C_ICR_PECCF   		0x00000800		//
#define	I2C_ICR_TIMOUTCF		0x00001000		//
#define	I2C_ICR_ALERTCF 		0x00002000		//

// I2C_PECR register

#define	I2C_PECR_PEC			0x000000FF		//

// I2C_RXDR register

#define	I2C_RXDR_RXDATA			0x000000FF		//

// I2C_TXDR register

#define	I2C_TXDR_TXDATA			0x000000FF		//
#endif
