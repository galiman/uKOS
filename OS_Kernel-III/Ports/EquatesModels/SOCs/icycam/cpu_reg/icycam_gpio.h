/*
; icycam_gpio.
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
; Goal:		icycam_gpio equates.
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

#ifndef __ICYCAM_GPIO__
#define __ICYCAM_GPIO__

// GPIO address	definitions
// ------------------------

#define	GPIO_PULL_UP			(volatile uint16_t *)(GPIO + 0x00)
#define	GPIO_PULL_DOWN			(volatile uint16_t *)(GPIO + 0x02)
#define	GPIO_DEBOUNCE			(volatile uint16_t *)(GPIO + 0x04)
#define	GPIO_PRESCALER			(volatile uint16_t *)(GPIO + 0x06)
#define	GPIO_DATAIN				(volatile uint16_t *)(GPIO + 0x08)
#define	GPIO_DATAOUT			(volatile uint16_t *)(GPIO + 0x0A)
#define	GPIO_DATAOUT_EN			(volatile uint16_t *)(GPIO + 0x0C)
#define	GPIO_DATAOUT_W1S		(volatile uint16_t *)(GPIO + 0x0E)
#define	GPIO_DATAOUT_W1C		(volatile uint16_t *)(GPIO + 0x10)
#define	GPIO_IRQ_CONFIG_1		(volatile uint16_t *)(GPIO + 0x12)
#define	GPIO_IRQ_CONFIG_2		(volatile uint16_t *)(GPIO + 0x14)
#define	GPIO_IRQ_STATUS			(volatile uint16_t *)(GPIO + 0x16)
#define	GPIO_IRQ0_EN			(volatile uint16_t *)(GPIO + 0x18)
#define	GPIO_IRQ1_EN			(volatile uint16_t *)(GPIO + 0x1A)
#define	GPIO_IRQ2_EN			(volatile uint16_t *)(GPIO + 0x1C)
#define	GPIO_IRQ3_EN			(volatile uint16_t *)(GPIO + 0x1E)

// GPIOSWITCH address	definitions
// --------------------------------

#define	GPIOSWITCH_CTRL			(volatile uint32_t *)(GPIOSWITCH + 0x00)
#define	GPIOSWITCH_TIMER_SEL	(volatile uint32_t *)(GPIOSWITCH + 0x04)
#endif
