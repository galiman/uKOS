/*
; stm32L476_lcd.
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
; Goal:		stm32L476_lcd equates.
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

#ifndef __STM32L476_LCD__
#define __STM32L476_LCD__

// LCD address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR;
	volatile	uint32_t	FCR;
	volatile	uint32_t	SR;
	volatile	uint32_t	CLR;
	volatile	uint32_t	RESERVED;
	volatile	uint32_t	RAM[16];
} LCD_TypeDef;

#define	LCD		((LCD_TypeDef *)LCD_BASE)

// LCD_CR register

#define	LCD_CR_LCDEN				0x00000001	//
#define	LCD_CR_VSEL					0x00000002	//
#define	LCD_CR_DUTY					0x0000001C	//
#define	LCD_CR_DUTY_				0x00000004	//
#define	LCD_CR_DUTY_1				0x00000008	//
#define	LCD_CR_DUTY_2				0x00000010	//
#define	LCD_CR_BIAS					0x00000060	//
#define	LCD_CR_BIAS_				0x00000020	//
#define	LCD_CR_BIAS_1				0x00000040	//
#define	LCD_CR_MUX_SEG				0x00000080	//
#define	LCD_CR_BUFEN				0x00000100	//

// LCD_FCR register

#define	LCD_FCR_HD					0x00000001	//
#define	LCD_FCR_SOFIE				0x00000002	//
#define	LCD_FCR_UDDIE				0x00000008	//
#define	LCD_FCR_PON					0x00000070	//
#define	LCD_FCR_PON_				0x00000010	//
#define	LCD_FCR_PON_1				0x00000020	//
#define	LCD_FCR_PON_2				0x00000040	//
#define	LCD_FCR_DEAD				0x00000380	//
#define	LCD_FCR_DEAD_0				0x00000080	//
#define	LCD_FCR_DEAD_1				0x00000100	//
#define	LCD_FCR_DEAD_2				0x00000200	//
#define	LCD_FCR_CC					0x00001C00	//
#define	LCD_FCR_CC_					0x00000400	//
#define	LCD_FCR_CC_1				0x00000800	//
#define	LCD_FCR_CC_2				0x00001000	//
#define	LCD_FCR_BLINKF				0x0000E000	//
#define	LCD_FCR_BLINKF_0			0x00002000	//
#define	LCD_FCR_BLINKF_1			0x00004000	//
#define	LCD_FCR_BLINKF_2			0x00008000	//
#define	LCD_FCR_BLINK				0x00030000	//
#define	LCD_FCR_BLINK_0				0x00010000	//
#define	LCD_FCR_BLINK_1				0x00020000	//
#define	LCD_FCR_DIV					0x003C0000	//
#define	LCD_FCR_PS					0x03C00000	//

// LCD_SR register

#define	LCD_SR_ENS					0x00000001	//
#define	LCD_SR_SOF					0x00000002	//
#define	LCD_SR_UDR					0x00000004	//
#define	LCD_SR_UDD					0x00000008	//
#define	LCD_SR_RDY					0x00000010	//
#define	LCD_SR_FCRSR				0x00000020	//

// LCD_CLR register

#define	LCD_CLR_SOFC				0x00000002	//
#define	LCD_CLR_UDDC				0x00000008	//

// LCD_RAM register

#define	LCD_RAM_SEGMENT_DATA		0xFFFFFFFF	//
#endif
