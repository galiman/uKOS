/*
; crt0.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2007-05-27
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		crt0 for the uKOS system.
;			See "ld.pdf" file, "Using LD, the GNU linker" page 48.
;
;                       EPROM
; _stTEXT        +-----------------+
;                |                 |
;                | .text           |
;                | .eh_frame       |
;                | .rodata         |
;				 |                 |                   DATA
; _enRODATA      +-----------------+ .......... +-----------------+ _stGOT
;                |                 |            |                 |
;                | .got            |            | .got            |
;                | .data           |            | .data           |
;                |                 |            |                 |
;                +-----------------+ .......... +-----------------+ _enDATA
;                                               |                 | _stBSS
;                                               | .bss            |
;                                               | COMMON          |
;                                               |                 |
;                                               +-----------------+ _enBSS
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

extern	uint32_t	_stDATA, _enDATA, _enRODATA;
extern	uint32_t	_stBSS, _enBSS;
extern	uint32_t	_stSCode, _lnSCode;
extern	uint32_t	_stHeapI, _lnHeapI, _stHeapE, _lnHeapE, _stHeapC, _lnHeapC;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"crt0         Runtime crt0.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Crt0\n"
									"====\n\n"

									"Runtime crt0 of the system compiled the " __DATE__" - "__TIME__".\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Crt0, KIDSTARTUP, KCRT0NUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief exit
 *
 * code = EXIT_SUCCESS			--->		Success, commit a suicide
 * code = EXIT_SUCCESS_OS		--->		Success, commit a suicide
 * code = EXIT_FAILURE			--->		Failure; commit a suicide
 * code = EXIT_PANIC			--->		Failure; panic. System is stopped
 * code = default				--->		Failure; panic. System is stopped
 *
 * \param[in]	number	Exit number
 * \param[out]	-
 *
 */
void	exit(int number) {
	volatile	proc_t	*process;

	switch (number) {
		case EXIT_SUCCESS: {
			kern_getProcessRun(&process);
			kern_killProcess(process);
			while (TRUE);
			break;
		}
		case EXIT_SUCCESS_OS: {
			kern_getProcessRun(&process);
			kern_killProcess(process);
			while (TRUE);
			break;
		}
		case EXIT_FAILURE: {
			kern_getProcessRun(&process);
			kern_killProcess(process);
			while (TRUE);
			break;
		}
		case EXIT_PANIC: {

			INTERRUPTION_OFF;
			cmns_send(KSYST, "Panic: system stopped\n");
			while (TRUE);
			break;
		}
		default: {

			INTERRUPTION_OFF;
			cmns_send(KSYST, "Panic: system stopped\n");
			while (TRUE);
			break;
		}
	}
}

void	_exit(int number) {

	exit(number);
}

/*
 * \brief _start
 *
 * - Copy the initialized data from the CODE to the DATA region
 * - Initialize the BSS region
 * - Call the main
 *
 */
int32_t	_start(void) {
	uint8_t		*regionDATA, *regionCODE, *regionBSS, *regionHeap;
	int32_t		nbBytes;

// Initialize the LOW level (!!! No static variables !!!)

	__asm__ volatile (
	"ldr		r0,=_stStPSP			\n"		//
	"msr		psp,r0					\n"		// r0 = stack PSP
	"movs		r0,%0					\n"		//
	"msr		control,r0				\n"		// Set PSP as a default stack
	"isb								\n"		//
	:
	: "i" (CONTROL_ALTERNATE_STACK)
	:
	);

	init_init();
	cmns_init();

// Copy the initialized data from the CODE region to the DATA one

	regionDATA = (uint8_t *)&_stDATA; regionCODE = (uint8_t *)&_enRODATA;
	nbBytes = (int32_t)((uint32_t)&_enDATA - (uint32_t)&_stDATA);
	while (nbBytes-- > 0) { *regionDATA++ = *regionCODE++; }

// Initialize the BSS region

	regionBSS = (uint8_t *)&_stBSS;
	nbBytes = (int32_t)((uint32_t)&_enBSS - (uint32_t)&_stBSS);
	while (nbBytes-- > 0) { *regionBSS++ = 0; }

// Initialize the Heap regions

	regionHeap = (uint8_t *)&_stHeapI;
	nbBytes = (int32_t)&_lnHeapI;
	while (nbBytes-- > 0) { *regionHeap++ = 0; }

	regionHeap = (uint8_t *)&_stHeapE;
	nbBytes = (int32_t)&_lnHeapE;
	while (nbBytes-- > 0) { *regionHeap++ = 0; }

	regionHeap = (uint8_t *)&_stHeapC;
	nbBytes = (int32_t)&_lnHeapC;
	while (nbBytes-- > 0) { *regionHeap++ = 0; }

// Initialize the interruption and exception vectors
// Go to the main

	exce_init();
	exit(boot());
}

