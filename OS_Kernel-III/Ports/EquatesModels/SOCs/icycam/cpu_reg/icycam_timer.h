/*
; icycam_timer.
; =============

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
; Goal:		icycam_timer equates.
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

#ifndef __ICYCAM_TIMER__
#define __ICYCAM_TIMER__

// TIMER address definitions
// -------------------------

#define	TIMER0_CTRL					(volatile uint8_t  *)(TIMER0 + 0x00)
#define	TIMER1_CTRL					(volatile uint8_t  *)(TIMER1 + 0x00)
#define	TIMER2_CTRL					(volatile uint8_t  *)(TIMER2 + 0x00)
#define	TIMER3_CTRL					(volatile uint8_t  *)(TIMER3 + 0x00)
#define	BTIMER_EN					0
#define	BTIMER_IRQ_EN				1
#define	CPT_INTERNAL				(0<<2)
#define	CPT_EXTERNAL				(1<<2)
#define	MEASURE_EXTERNAL			(2<<2)
#define	GENERATE_PWM				(3<<2)
#define	TRG_EDGE_UP					(0<<4)
#define	TRG_EDGE_DOWN				(1<<4)
#define	TRG_EDGE_UPDOWN				(2<<4)

#define	TIMER0_STATUS				(volatile uint8_t  *)(TIMER0 + 0x01)
#define	TIMER1_STATUS				(volatile uint8_t  *)(TIMER1 + 0x01)
#define	TIMER2_STATUS				(volatile uint8_t  *)(TIMER2 + 0x01)
#define	TIMER3_STATUS				(volatile uint8_t  *)(TIMER3 + 0x01)
#define	TIMER0_IRQEN				(volatile uint8_t  *)(TIMER0 + 0x02)
#define	TIMER1_IRQEN				(volatile uint8_t  *)(TIMER1 + 0x02)
#define	TIMER2_IRQEN				(volatile uint8_t  *)(TIMER2 + 0x02)
#define	TIMER3_IRQEN				(volatile uint8_t  *)(TIMER3 + 0x02)
#define	BBOUND_REACHED				0
#define	BUP_EDGE_DETECTED			1
#define	BDOWN_EDGE_DETECTED			2
#define	BUPDOWNEDGE_DETECTED		3

#define	TIMER0_COUNTER				(volatile uint32_t *)(TIMER0 + 0x04)
#define	TIMER1_COUNTER				(volatile uint32_t *)(TIMER1 + 0x04)
#define	TIMER2_COUNTER				(volatile uint32_t *)(TIMER2 + 0x04)
#define	TIMER3_COUNTER				(volatile uint32_t *)(TIMER3 + 0x04)
#define	TIMER0_PWM_HI				(volatile uint32_t *)(TIMER0 + 0x08)
#define	TIMER1_PWM_HI				(volatile uint32_t *)(TIMER1 + 0x08)
#define	TIMER2_PWM_HI				(volatile uint32_t *)(TIMER2 + 0x08)
#define	TIMER3_PWM_HI				(volatile uint32_t *)(TIMER3 + 0x08)
#define	TIMER0_PWM_LO				(volatile uint32_t *)(TIMER0 + 0x0C)
#define	TIMER1_PWM_LO				(volatile uint32_t *)(TIMER1 + 0x0C)
#define	TIMER2_PWM_LO				(volatile uint32_t *)(TIMER2 + 0x0C)
#define	TIMER3_PWM_LO				(volatile uint32_t *)(TIMER3 + 0x0C)
#define	TIMER0_DURATIONHIGH			(volatile uint32_t *)(TIMER0 + 0x10)
#define	TIMER1_DURATIONHIGH			(volatile uint32_t *)(TIMER1 + 0x10)
#define	TIMER2_DURATIONHIGH			(volatile uint32_t *)(TIMER2 + 0x10)
#define	TIMER3_DURATIONHIGH			(volatile uint32_t *)(TIMER3 + 0x10)
#define	TIMER0_DURATIONHIGHPREV		(volatile uint32_t *)(TIMER0 + 0x14)
#define	TIMER1_DURATIONHIGHPREV		(volatile uint32_t *)(TIMER1 + 0x14)
#define	TIMER2_DURATIONHIGHPREV		(volatile uint32_t *)(TIMER2 + 0x14)
#define	TIMER3_DURATIONHIGHPREV		(volatile uint32_t *)(TIMER3 + 0x14)
#define	TIMER0_DURATIONLOW			(volatile uint32_t *)(TIMER0 + 0x18)
#define	TIMER1_DURATIONLOW			(volatile uint32_t *)(TIMER1 + 0x18)
#define	TIMER2_DURATIONLOW			(volatile uint32_t *)(TIMER2 + 0x18)
#define	TIMER3_DURATIONLOW			(volatile uint32_t *)(TIMER3 + 0x18)
#define	TIMER0_DURATIONLOWPREV		(volatile uint32_t *)(TIMER0 + 0x1C)
#define	TIMER1_DURATIONLOWPREV		(volatile uint32_t *)(TIMER1 + 0x1C)
#define	TIMER2_DURATIONLOWPREV		(volatile uint32_t *)(TIMER2 + 0x1C)
#define	TIMER3_DURATIONLOWPREV		(volatile uint32_t *)(TIMER3 + 0x1C)
#define	TIMER0_COUNTER_EN			(volatile uint8_t  *)(TIMER0 + 0x20)
#define	TIMER1_COUNTER_EN			(volatile uint8_t  *)(TIMER1 + 0x20)
#define	TIMER2_COUNTER_EN			(volatile uint8_t  *)(TIMER2 + 0x20)
#define	TIMER3_COUNTER_EN			(volatile uint8_t  *)(TIMER3 + 0x20)

// WDTIMER address definitions
// ---------------------------

#define	WDTIMER		0x0032C00

#define	WDTIMER_CTRL				(volatile uint8_t  *)(WDTIMER + 0x00)
#define	WDTIMER_STATUS				(volatile uint8_t  *)(WDTIMER + 0x01)
#define	WDTIMER_COUNTER				(volatile uint32_t *)(WDTIMER + 0x04)
#define	WDTIMER_PERIOD				(volatile uint32_t *)(WDTIMER + 0x08)
#define	WDTIMER_UNLOCK				(volatile uint32_t *)(WDTIMER + 0x0C)
#define	UNLOCK_WORD					0xABACAB81

#define	WDTIMER_SOFTRESET			(volatile uint8_t  *)(WDTIMER + 0x10)
#endif
