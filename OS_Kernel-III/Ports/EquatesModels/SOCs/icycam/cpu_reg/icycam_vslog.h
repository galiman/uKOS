/*
; icycam_vslog.
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
; Goal:		icycam_vslog equates.
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

#ifndef __ICYCAM_VSLOG__
#define __ICYCAM_VSLOG__

// VSLOG address definitions
// -------------------------

#define	OSTART_SEQUENCE				0x00
#define	VSLOGCTRL_START_SEQUENCE	(volatile uint8_t  *)(VSLOG + OSTART_SEQUENCE)
#define	BSTART_INIT					0
#define	BSTART_EXPO					1
#define	BSTART_RAMP					2
#define	BSTART_READ					3
#define	BSTART_SUMM					4

#define	OIRQ						0x01
#define	VSLOGCTRL_IRQ				(volatile uint8_t  *)(VSLOG + OIRQ)
#define	BEND_OF_INIT				0
#define	BEND_OF_EXPO				1
#define	BEND_OF_RAMP				2
#define	BEND_OF_SUMM_INIT			3

#define	OSTATUS						0x02
#define	VSLOGCTRL_STATUS			(volatile uint8_t  *)(VSLOG + OSTATUS)
#define	MASK_MODE					0x38
#define	SMAODE						(0<<3)
#define	SINIT						(1<<3)
#define	SEXPO						(2<<3)
#define	SRAMP						(3<<3)
#define	SREAD						(4<<3)
#define	SSUMM						(5<<3)

#define	OCTRL_BYPASS				0x03
#define	VSLOGCTRL_CTRL_BYPASS		(volatile uint8_t  *)(VSLOG + OCTRL_BYPASS)
#define	BVREF_PDOWN					0
#define	BRST_PIX					1
#define	BRST_RAM					2
#define	BWR_RAM						3
#define	BD11						4
#define	BBYPASS						5

#define	OVREF_INIT					0x04
#define	VSLOGCTRL_VREF_INIT			(volatile uint8_t  *)(VSLOG + OVREF_INIT)
#define	OVREF_EXPOSURE				0x05
#define	VSLOGCTRL_VREF_EXPOSURE		(volatile uint8_t  *)(VSLOG + OVREF_EXPOSURE)
#define	OLOG_SHIFT					0x06
#define	VSLOGCTRL_LOG_SHIFT			(volatile uint8_t  *)(VSLOG + OLOG_SHIFT)
#define	SHIFT4BITS					0
#define	SHIFT5BITS					1
#define	SHIFT6BITS					2
#define	SHIFT7BITS					3

#define	OLOG_FREEZE					0x07
#define	VSLOGCTRL_LOG_FREEZE		(volatile uint8_t  *)(VSLOG + OLOG_FREEZE)
#define	OLOG_DOWN_NUP				0x08
#define	VSLOGCTRL_LOG_DOWN_NUP		(volatile uint8_t  *)(VSLOG + OLOG_DOWN_NUP)
#define	OVREF_STOP_RAMP				0x09
#define	VSLOGCTRL_VREF_STOP_RAMP	(volatile uint8_t  *)(VSLOG + OVREF_STOP_RAMP)
#define	OVREF_STEP					0x0A
#define	VSLOGCTRL_VREF_STEP			(volatile uint8_t  *)(VSLOG + OVREF_STEP)
#define	STEP2TCYC					0 //   40ns (@50-Mhz) or   60ns (@33-Mhz)
#define	STEP3TCYC					1 //   60ns (@50-Mhz) or   90ns (@33-Mhz)
#define	STEP5TCYC					2 //  100ns (@50-Mhz) or  150ns (@33-Mhz)
#define	STEP9TCYC					3 //  180ns (@50-Mhz) or  270ns (@33-Mhz)
#define	STEP17TCYC					4 //  340ns (@50-Mhz) or  510ns (@33-Mhz)
#define	STEP33TCYC					5 //  660ns (@50-Mhz) or  990ns (@33-Mhz)
#define	STEP65TCYC					6 // 1300ns (@50-Mhz) or 1950ns (@33-Mhz)
#define	STEP97TCYC					7 // 1940ns (@50-Mhz) or 2910ns (@33-Mhz)

#define	OD8_N10						0x0B
#define	VSLOGCTRL_D8_N10			(volatile uint8_t  *)(VSLOG + OD8_N10)
#define	S4WORDS16					(0<<0)
#define	S8WORDS8					(1<<0)

#define	OD_LSB						0x0C
#define	VSLOGCTRL_D_LSB				(volatile uint8_t  *)(VSLOG + OD_LSB)
#define	S8WORDS8MSB					(0<<0)
#define	S8WORDS8LSB					(1<<0)

#define	OMODE_ORICON				0x0D
#define	VSLOGCTRL_MODE_ORICON		(volatile uint8_t  *)(VSLOG + OMODE_ORICON)
#define	SLUMSUM						(0<<0)
#define	SCONTRASTS					(1<<0)
#define	SORIEBT3_CONTRAST5			(2<<0)

#define	OSUM_START_ROW				0x0E
#define	OSUM_STOP_ROW				0x0F
#define	OLOG_START_VALUE			0x18
#define	OLOG_DELAY					0x1A
#define	OEXPOSURE_TIME				0x1C
#define	OEXPOSURE_WAIT				0x1E
#define	OVREF_LIN_NLOG				0x20
#define	ODAC0						0x21
#define	ODAC1						0x22
#define	ODAC2						0x23
#define	ODAC3						0x24
#define	ODAC4						0x25
#define	ODAC5						0x26
#define	ODAC6						0x27
#define	ODAC7						0x28
#define	ODAC8						0x29
#define	ODAC9						0x2A
#define	ODAC10						0x2B
#define	OCONFIG						0x2C
#define	VSLOGCTRL_SUM_START_ROW		(volatile uint8_t  *)(VSLOG + OSUM_START_ROW)
#define	VSLOGCTRL_SUM_STOP_ROW		(volatile uint8_t  *)(VSLOG + OSUM_STOP_ROW)
#define	VSLOGCTRL_LOG_START_VALUE	(volatile uint16_t *)(VSLOG + OLOG_START_VALUE)
#define	VSLOGCTRL_LOG_DELAY			(volatile uint16_t *)(VSLOG + OLOG_DELAY)
#define	VSLOGCTRL_EXPOSURE_TIME		(volatile uint16_t *)(VSLOG + OEXPOSURE_TIME)
#define	VSLOGCTRL_EXPOSURE_WAIT		(volatile uint16_t *)(VSLOG + OEXPOSURE_WAIT)
#define	VSLOGCTRL_VREF_LIN_NLOG		(volatile uint8_t  *)(VSLOG + OVREF_LIN_NLOG)
#define	VSLOGCTRL_DAC0				(volatile uint8_t  *)(VSLOG + ODAC0)
#define	VSLOGCTRL_DAC1				(volatile uint8_t  *)(VSLOG + ODAC1)
#define	VSLOGCTRL_DAC2				(volatile uint8_t  *)(VSLOG + ODAC2)
#define	VSLOGCTRL_DAC3				(volatile uint8_t  *)(VSLOG + ODAC3)
#define	VSLOGCTRL_DAC4				(volatile uint8_t  *)(VSLOG + ODAC4)
#define	VSLOGCTRL_DAC5				(volatile uint8_t  *)(VSLOG + ODAC5)
#define	VSLOGCTRL_DAC6				(volatile uint8_t  *)(VSLOG + ODAC6)
#define	VSLOGCTRL_DAC7				(volatile uint8_t  *)(VSLOG + ODAC7)
#define	VSLOGCTRL_DAC8				(volatile uint8_t  *)(VSLOG + ODAC8)
#define	VSLOGCTRL_DAC9				(volatile uint8_t  *)(VSLOG + ODAC9)
#define	VSLOGCTRL_DAC10				(volatile uint8_t  *)(VSLOG + ODAC10)
#define	VSLOGCTRL_CONFIG			(volatile uint8_t  *)(VSLOG + OCONFIG)
#define	CK_ANA						(0<<0)
#define	CK_18						(1<<0)
#define	CK_14						(2<<0)
#define	FORCE_BOOST					(1<<2)
#define	BOOST_DIS					(1<<3)
#define	INV_POL						(1<<4)

#define	OTEST						0x2D
#define	OROW_TEST					0x2E
#define	OEN							0x2F
#define	VSLOGCTRL_TEST				(volatile uint8_t  *)(VSLOG + OTEST)
#define	VSLOGCTRL_ROW_TEST			(volatile uint8_t  *)(VSLOG + OROW_TEST)
#define	VSLOGCTRL_EN				(volatile uint8_t  *)(VSLOG + OEN)
#define	BENVS						0
#endif
