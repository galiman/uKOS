/*
; macros.
; =======

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 161                  $:  Revision of last commit
; $Date:: 2017-06-15 15:01:25#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Important macros.
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

#ifndef	__MACROS__
#define	__MACROS__

// Baudrate macro
// --------------

#define	DIV(ck, baudrate)		((uint32_t)(ck/(baudrate * 16)))
#define	REM(ck, baudrate)		((uint32_t)((((ck/(baudrate * 16.0)) - DIV(ck, baudrate)) * 16.0) + 0.5))
#define	BAUDRATE(ck, baudrate)	(DIV(ck, baudrate)<<4 | REM(ck, baudrate))

// Interruption macros
// -------------------

#define	KINT_IMASK_OFF		2						// Only NMI, SWI

#define	KINT_LEVEL_SWI		1						//
#define	KINT_IMASK_SWI		2						// For all the software interruptions

#define	KINT_LEVEL_COMM		2						//
#define	KINT_IMASK_COMM		3						// For all the COMM peripherals except the normal PERIPH and the kernel timers

#define	KINT_LEVEL_PERIPH	3						//
#define	KINT_IMASK_PERIPH	4						// For all the peripherals except the kernel timers

#define	KINT_LEVEL_TIMERS	4						//
#define	KINT_IMASK_TIMERS	5						// For all the peripherals and the kernel timers

#define	KINT_LEVEL_ALL		6						//
#define	KINT_IMASK_ALL		7						// All the levels

#define	BKERNPREEMPTION		2						// EXTI 2 used for preemption (change the context)
#define	BKERNSWIMSG			3						// EXTI 3 used for SWI kernel messages 

#define	EXCEPTION_VECTOR(vectorNb, address) \
								vExce_indExcVectors[vectorNb] = address;

#define	INTERRUPT_VECTOR(vectorNb, address) \
								vExce_indIntVectors[vectorNb] = address;

#define	INTERRUPTION_SET		__asm__ volatile ("								\n \
								mov			r0,%0								\n \
								msr			basepri,r0"							   \
								:												   \
								: "i" (KINT_IMASK_ALL<<4)						   \
								: "r0"											   \
								);

#define	INTERRUPTION_SET_PERIPH	__asm__ volatile ("								\n \
								mov			r0,%0								\n \
								msr			basepri,r0"							   \
								:												   \
								: "i" (KINT_IMASK_PERIPH<<4)					   \
								: "r0"											   \
								);

#define	INTERRUPTION_OFF		volatile uint32_t	__saveBASEPRI;				   \
								__asm__ volatile ("								\n \
								mrs			r0,basepri							\n \
								str			r0,%0								\n \
								mov			r0,%1								\n \
								msr			basepri,r0"							   \
								:												   \
								: "m" (__saveBASEPRI), "i" (KINT_IMASK_OFF<<4)	   \
								: "r0"											   \
								);

#define	INTERRUPTION_RESTORED	__asm__ volatile ("								\n \
								ldr			r0,%0								\n \
								msr			basepri,r0"							   \
								:												   \
								: "m" (__saveBASEPRI)							   \
								: "r0"											   \
								);

#define	RETURN_INT_RESTORED(status)												   \
								INTERRUPTION_RESTORED;							   \
								return (status);

#define	GOTO_KERN_I(msg)		__asm__ volatile ("								\n \
								cpsid		i									\n \
								movw		r0,%0								\n \
								movt		r0,%1								\n \
								push		{r0}								\n \
								push		{r0}"								   \
								:												   \
								: "i" ((msg) & 0x0000FFFF), "i" ((msg) >> 16)	   \
								: "r0"											   \
								);												   \
								__asm__ volatile ("								\n \
								dmb												\n \
								dsb												\n \
								isb"											   \
								);												   \
								EXTI->SWIER1 = (1<<BKERNSWIMSG);				   \
								__asm__ volatile ("								\n \
								cpsie		i									\n \
								wfi												\n \
								mrs			r0,psp								\n \
								add			r0,r0,#8							\n \
								msr			psp,r0"								   \
								:												   \
								:												   \
								: "r0"											   \
								);

#define	GOTO_KERN_M(msg)		__asm__ volatile ("								\n \
								cpsid		i									\n \
								ldr			r0,%0								\n \
								push		{r0}"								   \
								:												   \
								: "m" (msg)										   \
								: "r0"											   \
								);												   \
								__asm__ volatile ("								\n \
								dmb												\n \
								dsb												\n \
								isb"											   \
								);												   \
								EXTI->SWIER1 = (1<<BKERNSWIMSG);				   \
								__asm__ volatile ("								\n \
								cpsie		i									\n \
								wfi												\n \
								mrs			r0,psp								\n \
								add			r0,r0,#4							\n \
								msr			psp,r0"								   \
								:												   \
								:												   \
								: "r0"											   \
								);

#define	PREEMPTION				EXTI->SWIER1 = (1<<BKERNPREEMPTION);

// Misc assembler macro
// --------------------

#define	NOP 					__asm__ volatile ("								\n \
								nop"											   \
								);

#define	WAITING_INTERRUPTION	__asm__ volatile ("								\n \
								wfi												\n \
								dmb												\n \
								dsb												\n \
								isb"											   \
								);

#define	DATA_SYNC_BARRIER		__asm__ volatile ("								\n \
								dsb"											   \
								);

#define	INST_SYNC_BARRIER		__asm__ volatile ("								\n \
								isb"											   \
								);

#define	MEMO_SYNC_BARRIER		__asm__ volatile ("								\n \
								dmb"											   \
								);

#endif
