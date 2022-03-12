/*
; cortex_m3_core.
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
; Goal:		cortex_m3_core equates.
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

#ifndef __CORTEX_M3_CORE__
#define __CORTEX_M3_CORE__

// Register Abstraction
// --------------------

// System Reset

#define NVIC_VECTRESET				0						// Vector Reset Bit
#define NVIC_SYSRESETREQ			2						// System Reset Request
#define NVIC_AIRCR_VECTKEY			(0x5FA<<16)				// AIRCR Key for write access
#define NVIC_AIRCR_ENDIANESS		15						// Endianess
#define CONTROL_ALTERNATE_STACK		(1<<1)					// Set the PSP stack
#define CONTROL_main_STACK			(0<<1)					// Set the MSP stack

// Core Debug

#define CoreDebug_DEMCR_TRCENA		(1<<24)					// DEMCR TRCENA enable
#define ITM_TCR_ITMENA				1						// ITM enable

// M3  mapping
// -----------

#define	SCS_BASE            		0xE000E000				// System Control Space Base Address 
#define	ITM_BASE					0xE0000000				// ITM Base Address
#define	CoreDebug_BASE				0xE000EDF0				// Core Debug Base Address
#define	SysTick_BASE				(SCS_BASE + 0x0010)		// SysTick Base Address
#define	NVIC_BASE					(SCS_BASE + 0x0100)		// NVIC Base Address
#define	SCB_BASE					(SCS_BASE + 0x0D00)		// System Control Block Base Address

// NVIC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	ISER[8];
	volatile	uint32_t	RESERVED0[24];
	volatile	uint32_t	ICER[8];
	volatile	uint32_t	RSERVED1[24];
	volatile	uint32_t	ISPR[8];
	volatile	uint32_t	RESERVED2[24];
	volatile	uint32_t	ICPR[8];
	volatile	uint32_t	RESERVED3[24];
	volatile	uint32_t	IABR[8];
	volatile	uint32_t	RESERVED4[56];
	volatile	uint8_t		IP[240];
	volatile	uint32_t	RESERVED5[644];
	volatile	uint32_t	STIR;
} NVIC_TypeDef;

// SCB address definitions
// -----------------------

typedef struct {
	volatile	const	uint32_t 	CPUID;
	volatile			uint32_t	ICSR;
	volatile			uint32_t	VTOR;
	volatile			uint32_t	AIRCR;
	volatile			uint32_t	SCR;
	volatile			uint32_t	CCR;
	volatile			uint8_t		SHP[12];
	volatile			uint32_t	SHCSR;
	volatile			uint32_t	CFSR;
	volatile			uint32_t	HFSR;
	volatile			uint32_t	DFSR;
	volatile			uint32_t	MMFAR;
	volatile			uint32_t	BFAR;
	volatile			uint32_t 	AFSR;
	volatile	const	uint32_t	PFR[2];
	volatile	const	uint32_t	DFR;
	volatile	const	uint32_t	ADR;
	volatile	const	uint32_t	MMFR[4];
	volatile	const	uint32_t	ISAR[5];
} SCB_TypeDef;

// AIRCR register

#define AIRCR_VECTKEY_MASK			0x05FA0000

#define	SCB_AIRCR_VECTRESET			0x00000001				// System Reset bit
#define	SCB_AIRCR_VECTCLRACTIVE		0x00000002				// Clear active vector bit
#define	SCB_AIRCR_SYSRESETREQ		0x00000004				// Requests chip control logic to generate a reset
#define	SCB_AIRCR_PRIGROUP			0x00000700				// PRIGROUP[2:0] bits (Priority group)
#define	SCB_AIRCR_PRIGROUP_0		0x00000100				// Bit 0
#define	SCB_AIRCR_PRIGROUP_1		0x00000200				// Bit 1
#define	SCB_AIRCR_PRIGROUP_2		0x00000400				// Bit 2

// SYSTICK address definitions
// ---------------------------

typedef struct {
	volatile			uint32_t	CTRL;
	volatile			uint32_t	LOAD;
	volatile			uint32_t	VAL;
	volatile	const	uint32_t	CALIB;
} SysTick_Type;

// ITM address definitions
// -----------------------

typedef struct {
	volatile  union {
		volatile	uint8_t		u8;
		volatile	uint16_t	u16;
		volatile	uint32_t	u32;
	} PORT [32];
	volatile			uint32_t	RESERVED0[864];
	volatile			uint32_t	TER;
	volatile			uint32_t	RESERVED1[15];
	volatile			uint32_t 	TPR;
	volatile			uint32_t	RESERVED2[15];
	volatile			uint32_t	TCR;
	volatile			uint32_t	RESERVED3[29];
	volatile			uint32_t	IWR;
	volatile			uint32_t	IRR;
	volatile			uint32_t	IMCR;
	volatile			uint32_t	RESERVED4[43];
	volatile			uint32_t	LAR;
	volatile			uint32_t	LSR;
	volatile			uint32_t	RESERVED5[6];
	volatile	const	uint32_t	PID4;
	volatile	const	uint32_t	PID5;
	volatile	const	uint32_t	PID6;
	volatile	const	uint32_t	PID7;
	volatile	const	uint32_t	PID0;
	volatile	const	uint32_t	PID1;
	volatile	const	uint32_t	PID2;
	volatile	const	uint32_t	PID3;
	volatile	const	uint32_t	CID0;
	volatile	const	uint32_t	CID1;
	volatile	const	uint32_t	CID2;
	volatile	const	uint32_t	CID3;
} ITM_Type;

// INTERRUPTTYPE address definitions
// ---------------------------------

typedef struct {
	volatile			uint32_t	RESERVED0;
	volatile	const	uint32_t	ICTR;
	volatile			uint32_t	RESERVED1;
} InterruptType_Type;

#define	NVIC			((NVIC_TypeDef *)NVIC_BASE)
#define	SCB				((SCB_TypeDef *)SCB_BASE)
#define	SysTick			((SysTick_Type *)SysTick_BASE)
#define	ITM				((ITM_Type *)ITM_BASE)
#define	InterruptType	((InterruptType_Type *)SCS_BASE)
#endif
