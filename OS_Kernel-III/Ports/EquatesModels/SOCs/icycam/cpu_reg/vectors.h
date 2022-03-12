/*
; vectors.
; ========

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
; Goal:		Indirect Exception Vectors for icycam.
;			The "reset" exception is not considered. The indirect table
;			is reserved in the first.s file.
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

#ifndef	__VECTORS__
#define	__VECTORS__

// Exception vector table (table of jmp xxx)
// -----------------------------------------

// Format of the jmp
//
// jmp  addr
//
// E8 nnnn F8 with nnnn = addr / 4 (number of long words)

#define	KJUMP			0xE80000F8							// JMP op-code
#define	KADDPOS			6									// Address position

#define	EXCEPTIONS		(volatile uint32_t *)(0x00000000)

#define	KNBEXCEPTIONS	25									// Number of exceptions
#define	KSZEXCEPTIONS	(KNBEXCEPTIONS*8)					// Size of the vector table


#define	KRESETEXEPTION	0									// Restart
#define	KILLOPEXEPTION	1									// Illegal Operations (PSU, DMU, DPU, HDU)
#define	KHWCNTEXEPTION	2									// Hardware Contention (PSU, DMU, DPU, HDU)
#define	KBUERREXEPTION	3									// Misaligned Memory Access (X, Y), Bus Error (P, X, Y)
#define	KHWSTKEXEPTION	4									// PSU HW Stack Empty / Full
#define	KDEBUGEXEPTION	5									// Debug Breakpoint Exception
#define	KPRIVIEXEPTION	6									// Privileged Operation Execution
#define	KTRAP1EXEPTION	7									// Software trap #1
#define	KHDU1EXEPTION	8									// HDU-specific #1
#define	KDPU3EXEPTION	9									// DPU-specific #3
#define	KDPU2EXEPTION	10									// DPU-specific #2
#define	KDMU1EXEPTION	11									// DMU-specific #1
#define	DPU1EXEPTION	12									// DPU-specific #1
#define	KDMU0EXEPTION	13									// DMU-specific #0
#define	KDPU0EXEPTION	14									// DPU-specific #0
#define	KHDU0EXEPTION	15									// HDU-specific #0
#define	KIRQ0EXEPTION	16									// IRQ #0  (Non MaskableInterrupt)
#define	KIRQ1EXEPTION	17									// IRQ #1
#define	KIRQ2EXEPTION	18									// IRQ #2
#define	KIRQ3EXEPTION	19									// IRQ #3
#define	KIRQ4EXEPTION	20									// IRQ #4
#define	KIRQ5EXEPTION	21									// IRQ #5
#define	KIRQ6EXEPTION	22									// IRQ #6
#define	KIRQ7EXEPTION	23									// IRQ #7
#define	KTRAP0EXEPTION	24									// Software trap #0
#endif
