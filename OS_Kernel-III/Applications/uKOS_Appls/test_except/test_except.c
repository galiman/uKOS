/*
; test_except.
; ============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 1 processes:
;
;			- P0: Generate an exception
;				  Display the registers
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

#include	<uKOS.h>

#define LOCCRASH	(volatile uint32_t *)(0x001FFFFF)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_except  uKernel test of the exceptions.           (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_except\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Generate an exception
 * 			- Display the registers
 *
 */
static	void	aProcess_0(const void *argument) {
	uint32_t	i;

	iotx_printf(KSYST,"The achine will crash in 20 seconds!!\n");

	for (i = 0; i < 20; i++) {
		//        --|-------------------------------------|--
		syos_trace("--> Process 0: value %d                ", i);
		kern_suspendProcess(1000);
	}

	//        --|-------------------------------------|--
	syos_trace("--> Process 0: Enter                   ", 0x01010101);
	kern_suspendProcess(1000);

// Load the registers

	//        --|-------------------------------------|--
	syos_trace("--> Process 0: Load the registers      ", 0x02020202);

	__asm__ volatile (
	"cpsid	i						\n"
 
 // CPU
 
	"mov	r0,  #0x0000			\n"
	"movt	r0,  #0xAAA0			\n"
	"mov	r1,  #0x0001			\n"
	"movt	r1,  #0xBBB1			\n"
	"mov	r2,  #0x0002			\n"
	"movt	r2,  #0xCCC2			\n"
	"mov	r3,  #0x0003			\n"
	"movt	r3,  #0xDDD3			\n"
	"mov	r4,  #0x0004			\n"
	"movt	r4,  #0xEEE4			\n"
	"mov	r5,  #0x0005			\n"
	"movt	r5,  #0xFFF5			\n"
	"mov	r6,  #0x0006			\n"
	"movt	r6,  #0xABA6			\n"
	"mov	r7,  #0x0007			\n"
	"movt	r7,  #0xACA7			\n"
	"mov	r8,  #0x0008			\n"
	"movt	r8,  #0xADA8			\n"
	"mov	r9,  #0x0009			\n"
	"movt	r9,  #0xAEA9			\n"
	"mov	r10, #0x000A			\n"
	"movt	r10, #0xAFAA			\n"
	"mov	r11, #0x000B			\n"
	"movt	r11, #0xBABB			\n"
	"mov	r12, #0x000C			\n"
	"movt	r12, #0xBBBC			\n"
	"mov	r14, #0x000D			\n"
	"movt	r14, #0xBBBE			\n"

	#if (defined(__CORTEX_M4__) || defined(__CORTEX_M7__))

// FPU

	"vmov.F32 s0,   #1.00000000		\n"
	"vmov.F32 s1,   #2.00000000		\n"
	"vmov.F32 s2,   #3.00000000		\n"
	"vmov.F32 s3,   #4.00000000		\n"
	"vmov.F32 s4,   #5.00000000		\n"
	"vmov.F32 s5,   #6.00000000		\n"
	"vmov.F32 s6,   #7.00000000		\n"
	"vmov.F32 s7,   #8.00000000		\n"
	"vmov.F32 s8,   #9.00000000		\n"
	"vmov.F32 s9,  #10.00000000		\n"
	"vmov.F32 s10, #11.00000000		\n"
	"vmov.F32 s11, #12.00000000		\n"
	"vmov.F32 s12, #13.00000000		\n"
	"vmov.F32 s13, #14.00000000		\n"
	"vmov.F32 s14, #15.00000000		\n"
	"vmov.F32 s15, #16.00000000		\n"
	"vmov.F32 s16, #17.00000000		\n"
	"vmov.F32 s17, #18.00000000		\n"
	"vmov.F32 s18, #19.00000000		\n"
	"vmov.F32 s19, #20.00000000		\n"
	"vmov.F32 s20, #21.00000000		\n"
	"vmov.F32 s21, #22.00000000		\n"
	"vmov.F32 s22, #23.00000000		\n"
	"vmov.F32 s23, #24.00000000		\n"
	"vmov.F32 s24, #25.00000000		\n"
	"vmov.F32 s25, #26.00000000		\n"
	"vmov.F32 s26, #27.00000000		\n"
	"vmov.F32 s27, #28.00000000		\n"
	"vmov.F32 s28, #29.00000000		\n"
	"vmov.F32 s29, #30.00000000		\n"
	"vmov.F32 s30, #31.00000000		\n"
	"vmov.F32 s31, #-31.00000000	\n"
	#endif
	);

// Crash the system

	//        --|-------------------------------------|--
	syos_trace("--> Process 0: Generate the crash      ", 0x03030303);
	*(LOCCRASH);
}

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {
	volatile	proc_t 	*process_0;

	//        --|-------------------------------------|--
	syos_trace("--> Main: Enter                        ", 0x04040404);

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";

// Specifications for the processes

	//        --|-------------------------------------|--
	syos_trace("--> Main: Process specifications       ", 0x05050505);
	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	//        --|-------------------------------------|--
	syos_trace("--> Main: Process launched             ", 0x06060606);
	return (EXIT_SUCCESS_OS);
}

