/*
; stm32F103_rtc.
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
; Goal:		stm32F103_rtc equates.
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

#ifndef __STM32F103_RTC__
#define __STM32F103_RTC__

// RTC address definitions
// -----------------------

typedef struct {
	volatile	uint16_t	CRH;
	volatile	uint16_t	ESERVED0;
	volatile	uint16_t	CRL;
	volatile	uint16_t	RESERVED1;
	volatile	uint16_t	PRLH;
	volatile	uint16_t	RESERVED2;
	volatile	uint16_t	PRLL;
	volatile	uint16_t	RESERVED3;
	volatile	uint16_t	DIVH;
	volatile	uint16_t	RESERVED4;
	volatile	uint16_t	DIVL;
	volatile	uint16_t	RESERVED5;
	volatile	uint16_t	CNTH;
	volatile	uint16_t	RESERVED6;
	volatile	uint16_t	CNTL;
	volatile	uint16_t	RESERVED7;
	volatile	uint16_t	ALRH;
	volatile	uint16_t	RESERVED8;
	volatile	uint16_t	ALRL;
	volatile	uint16_t	RESERVED9;
} RTC_TypeDef;

#define RTC		((RTC_TypeDef *)RTC_BASE)

// RTC_CRH register

#define	RTC_CRH_SECIE			0x01		//
#define	RTC_CRH_ALRIE			0x02		//
#define	RTC_CRH_OWIE			0x04		//

// RTC_CRL register

#define	RTC_CRL_SECF			0x01		//
#define	RTC_CRL_ALRF			0x02		//
#define	RTC_CRL_OWF				0x04		//
#define	RTC_CRL_RSF				0x08		//
#define	RTC_CRL_CNF				0x10		//
#define	RTC_CRL_RTOFF			0x20		//

// RTC_PRLH register

#define	RTC_PRLH_PRL			0x000F		//

// RTC_PRLL register

#define	RTC_PRLL_PRL			0xFFFF		//

// RTC_DIVH register

#define	RTC_DIVH_RTC_DIV		0x000F		//

// RTC_DIVL register

#define	RTC_DIVL_RTC_DIV		0xFFFF		//

// RTC_CNTH register

#define	RTC_CNTH_RTC_CNT		0xFFFF		//

// RTC_CNTL register

#define	RTC_CNTL_RTC_CNT		0xFFFF		//

// RTC_ALRH register

#define	RTC_ALRH_RTC_ALR		0xFFFF		//

// RTC_ALRL register

#define	RTC_ALRL_RTC_ALR		0xFFFF		//
#endif
