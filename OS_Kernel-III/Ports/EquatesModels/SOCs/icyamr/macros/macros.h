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

#define	BAUDRATE(baudrate)		switch (baudrate) { \
									case 2400:   *UART_BAUD =(DIV14 | DIVPR32 | 15); break; \
									case 4800:   *UART_BAUD =(DIV14 | DIVPR16 | 15); break; \
									case 9600:   *UART_BAUD =(DIV14 | DIVPR8  | 15); break; \
									case 19200:  *UART_BAUD =(DIV14 | DIVPR4  | 15); break; \
									case 38400:  *UART_BAUD =(DIV14 | DIVPR2  | 15); break; \
									case 57600:  *UART_BAUD =(DIV14 | DIVPR2  | 10); break; \
									case 115200: *UART_BAUD =(DIV16 | DIVPR1  | 8);  break; \
									case 230400: *UART_BAUD =(DIV14 | DIVPR1  | 4);  break; \
									case 460800: *UART_BAUD =(DIV16 | DIVPR1  | 1);  break; \
									default:     *UART_BAUD =(DIV16 | DIVPR1  | 8);  break; \
								}

// Interruption macros
// -------------------

#define	KINT_IMASK_OFF			0			// Only NMI

#define	KINT_LEVEL_COMM			1			//
#define	KINT_LEVEL_VISENS		2			//
#define	KINT_LEVEL_PERIPH		3			//
#define	KINT_IMASK_PERIPH		4			// For all the peripherals except the kernel timers

#define	KINT_LEVEL_TIMERS		5			//
#define	KINT_IMASK_TIMERS		6			// For all the peripherals and the kernel timers

#define	KINT_IMASK_ALL			7			// All the levels

#define	KINT_SYS_LEVEL			LEVEL0_5	// Level 0, 1, 2, 3, 4, 5

#define	EXCEPTION_VECTOR(vectorNb, address) \
								uint32_t	param; \
								param = KJUMP | (((uint32_t)(address))<<KADDPOS); \
								*(EXCEPTIONS + 2*(vectorNb)) = param;

#define	INTERRUPTION_SET		__asm__ volatile ("							\n \
								movil.h 	r7.l,#%0						\n \
								pmov 		iel,r7.l"						   \
								:											   \
								: "i" (KINT_IMASK_ALL)						   \
								: "r7"										   \
								);

#define	INTERRUPTION_SET_PERIPH __asm__ volatile ("							\n \
								movil.h 	r7.l,#%0						\n \
								pmov 		iel,r7.l"						   \
								:											   \
								: "i" (KINT_IMASK_PERIPH)					   \
								: "r7"										   \
								);

#define	INTERRUPTION_OFF		volatile uint16_t	__saveIEL;				   \
								__asm__ volatile ("							\n \
								pmov 		r7.l,iel						\n \
								movbl.h		%0,r7.l							\n \
								movil.h 	r7.l,#%1						\n \
								pmov 		iel,r7.l"						   \
								:											   \
								: "m" (__saveIEL), "i" (KINT_IMASK_OFF)		   \
								: "r7"										   \
								);

#define	INTERRUPTION_OFF_KERNEL	volatile uint16_t	__saveIEL;				   \
								__asm__ volatile ("							\n \
								pmov 		r7.l,iel						\n \
								movbl.h		%0,r7.l							\n \
								movil.h 	r7.l,#%1						\n \
								pmov 		iel,r7.l"						   \
								:											   \
								: "m" (__saveIEL), "i" (KINT_IMASK_PERIPH)	   \
								: "r7" 										   \
								);

#define	INTERRUPTION_RESTORED 	__asm__ volatile ("							\n \
								movbl.h		r7.l,%0							\n \
								pmov 		iel,r7.l"						   \
								:											   \
								: "m" (__saveIEL)							   \
								: "r7"										   \
								);

#define	RETURN_INT_RESTORED(status)											   \
								INTERRUPTION_RESTORED;						   \
								return (status);

#define	GOTO_KERN_I(msg)		__asm__ volatile ("							\n \
								movil.h 	r7.u,#sval.u(%0)				\n \
								movil.h 	r7.l,#sval.l(%0)				\n \
								movbl 		+(px7,-4),r7					\n \
								trap1										\n \
								pxaddi		px7,#4"							   \
								:											   \
								: "i" (msg)									   \
								: "r7"										   \
								);

#define	GOTO_KERN_M(msg)		__asm__ volatile ("							\n \
								movbl 		r7,%0							\n \
								movbl 		+(px7,-4),r7					\n \
								trap1										\n \
								pxaddi		px7,#4"							   \
								:											   \
								: "m" (msg)									   \
								: "r7"										   \
								);

#define	PREEMPTION				kern_switchFast();

// Misc assembler macro
// --------------------

#define	NOP 					__asm__ volatile ("							\n \
								nop"										   \
								);

#define	WAITING_INTERRUPTION	__asm__ volatile ("							\n \
								nop											\n \
								sleep										\n \
								nop"										   \
								);

#endif
