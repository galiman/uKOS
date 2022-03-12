/*
; stm32L476_exti.
; ===============

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
; Goal:		stm32L476_exti equates.
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

#ifndef __STM32L476_EXTI__
#define __STM32L476_EXTI__

// EXTI address definitions
// ------------------------

typedef struct {
	volatile	uint32_t	IMR1;
	volatile	uint32_t	EMR1;
	volatile	uint32_t	RTSR1;
	volatile	uint32_t	FTSR1;
	volatile	uint32_t	SWIER1;
	volatile	uint32_t	PR1;
	volatile	uint32_t	RESERVED1;
	volatile	uint32_t	RESERVED2;
	volatile	uint32_t	IMR2;
	volatile	uint32_t	EMR2;
	volatile	uint32_t	RTSR2;
	volatile	uint32_t	FTSR2;
	volatile	uint32_t	SWIER2;
	volatile	uint32_t	PR2;
} EXTI_TypeDef;

#define	EXTI	((EXTI_TypeDef *)EXTI_BASE)

#define	EXTI_Mode_Interrupt		0x00000000
#endif
