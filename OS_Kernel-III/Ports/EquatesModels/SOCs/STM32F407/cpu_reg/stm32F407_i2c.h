/*
; stm32F407_i2c.
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
; Goal:		stm32F407_i2c equates.
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

#ifndef __STM32F407_I2C__
#define __STM32F407_I2C__

// I2C address definitions
// -----------------------

typedef struct {
	volatile	uint16_t	CR1;
	volatile	uint16_t	RESERVED0;
	volatile	uint16_t	CR2;
	volatile	uint16_t	RESERVED1;
	volatile	uint16_t	OAR1;
	volatile	uint16_t	RESERVED2;
	volatile	uint16_t	OAR2;
	volatile	uint16_t	RESERVED3;
	volatile	uint16_t	DR;
	volatile	uint16_t	RESERVED4;
	volatile	uint16_t	SR1;
	volatile	uint16_t	RESERVED5;
	volatile	uint16_t	SR2;
	volatile	uint16_t	RESERVED6;
	volatile	uint16_t	CCR;
	volatile	uint16_t	RESERVED7;
	volatile	uint16_t	TRISE;
	volatile	uint16_t	RESERVED8;
} I2C_TypeDef;

#define I2C1	((I2C_TypeDef *)I2C1_BASE)
#define I2C2	((I2C_TypeDef *)I2C2_BASE)
#define I2C3	((I2C_TypeDef *)I2C3_BASE)

// I2C_CR1 register

#define	I2C_CR1_PE				0x0001		//
#define	I2C_CR1_SMBUS			0x0002		//
#define	I2C_CR1_SMBTYPE			0x0008		//
#define	I2C_CR1_ENARP			0x0010		//
#define	I2C_CR1_ENPEC			0x0020		//
#define	I2C_CR1_ENGC 			0x0040		//
#define	I2C_CR1_NOSTRETCH		0x0080		//
#define	I2C_CR1_START			0x0100		//
#define	I2C_CR1_STOP 			0x0200		//
#define	I2C_CR1_ACK  			0x0400		//
#define	I2C_CR1_POS  			0x0800		//
#define	I2C_CR1_PEC  			0x1000		//
#define	I2C_CR1_ALERT			0x2000		//
#define	I2C_CR1_SWRST			0x8000		//

// I2C_CR2 register

#define	I2C_CR2_FREQ 			0x003F		//
#define	I2C_CR2_FREQ_0 			0x0001		//
#define	I2C_CR2_FREQ_1 			0x0002		//
#define	I2C_CR2_FREQ_2 			0x0004		//
#define	I2C_CR2_FREQ_3 			0x0008		//
#define	I2C_CR2_FREQ_4 			0x0010		//
#define	I2C_CR2_FREQ_5 			0x0020		//

#define	I2C_CR2_ITERREN			0x0100		//
#define	I2C_CR2_ITEVTEN			0x0200		//
#define	I2C_CR2_ITBUFEN			0x0400		//
#define	I2C_CR2_DMAEN			0x0800		//
#define	I2C_CR2_LAST 			0x1000		//

// I2C_OAR1 register

#define	I2C_OAR1_ADD1_7			0x00FE		//
#define	I2C_OAR1_ADD8_9			0x0300		//

#define	I2C_OAR1_ADD0			0x0001		//
#define	I2C_OAR1_ADD1			0x0002		//
#define	I2C_OAR1_ADD2			0x0004		//
#define	I2C_OAR1_ADD3			0x0008		//
#define	I2C_OAR1_ADD4			0x0010		//
#define	I2C_OAR1_ADD5			0x0020		//
#define	I2C_OAR1_ADD6			0x0040		//
#define	I2C_OAR1_ADD7			0x0080		//
#define	I2C_OAR1_ADD8			0x0100		//
#define	I2C_OAR1_ADD9			0x0200		//

#define	I2C_OAR1_ADDMODE 		0x8000		//

// I2C_OAR2 register

#define	I2C_OAR2_ENDUAL			0x01		//
#define	I2C_OAR2_ADD2  			0xFE		//

// I2C_DR register

#define	I2C_DR_DR      			0xFF		//

// I2C_SR1 register

#define	I2C_SR1_SB				0x0001		//
#define	I2C_SR1_ADDR 			0x0002		//
#define	I2C_SR1_BTF  			0x0004		//
#define	I2C_SR1_ADD10			0x0008		//
#define	I2C_SR1_STOPF			0x0010		//
#define	I2C_SR1_RXNE 			0x0040		//
#define	I2C_SR1_TXE  			0x0080		//
#define	I2C_SR1_BERR 			0x0100		//
#define	I2C_SR1_ARLO 			0x0200		//
#define	I2C_SR1_AF				0x0400		//
#define	I2C_SR1_OVR  			0x0800		//
#define	I2C_SR1_PECERR 			0x1000		//
#define	I2C_SR1_TIMEOUT			0x4000		//
#define	I2C_SR1_SMBALERT 		0x8000		//

// I2C_SR2 register

#define	I2C_SR2_MSL  			0x0001		//
#define	I2C_SR2_BUSY 			0x0002		//
#define	I2C_SR2_TRA  			0x0004		//
#define	I2C_SR2_GENCALL			0x0010		//
#define	I2C_SR2_SMBDEFAULT		0x0020		//
#define	I2C_SR2_SMBHOST			0x0040		//
#define	I2C_SR2_DUALF			0x0080		//
#define	I2C_SR2_PEC  			0xFF00		//

// I2C_CCR register

#define	I2C_CCR_CCR  			0x0FFF		//
#define	I2C_CCR_DUTY 			0x4000		//
#define	I2C_CCR_FS				0x8000		//

// I2C_TRISE register

#define	I2C_TRISE_TRISE			0x3F		//
#endif
