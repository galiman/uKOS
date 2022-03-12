/*
; test.
; =====

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
; Goal:		Minimal test of the hardware.
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

enum	{ KNOERR, KPRERR, KMER08, KMER16, KMER32 };

extern	uint32_t	_stEXRAM, _lnEXRAM;

#define	KSTART		0
#define	KEND		((uint32_t)&_lnEXRAM)


// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test         Minimal test of the hardware.             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Minimal test of the hardware\n"
									"============================\n\n"

									"This tool tests the following hardware parts:\n"
									"- Leds.\n"
									"- External memory.\n\n"

									"Input format:  test number_tests\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_display(int32_t error, void *add, uint32_t expe, uint32_t read);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Test, KIDTOOL, KTESTNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint32_t	led, add;
	uint8_t		pattern_08, expe_08, read_08, *memory_08;
	uint16_t	pattern_16, expe_16, read_16, *memory_16;
	uint32_t	pattern_32, expe_32, read_32, *memory_32;
	int32_t		tests, nbTests;

	iotx_printf(KSYST, "Sytsm tests.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc == 2) {
		strg_cnvtDecAsciiToBufInt32(&nbTests, argv[1], 1);

// Test 1: LEDs
// ------------

		iotx_printf(KSYST, "Test LEDs\n");

		for (led = 0; led < KNBLED; led++) { misc_offLed(led); 							}
		for (led = 0; led < KNBLED; led++) { misc_onLed(led); kern_suspendProcess(100); }
		for (led = 0; led < KNBLED; led++) { misc_offLed(led);							}

// Test 2: Memory 8-bits
// ---------------------

// Test a: fill the memory with (uint8_t)0xAA - (uint8_t)0x55 and verify

		memory_08 = (uint8_t *)&_stEXRAM;
		iotx_printf(KSYST, "\n8-bits:  Test Memory: fill 0xAA - 0x55\n");
		for (tests = 0; tests < nbTests; tests++) {
			iotx_printf(KSYST, ".");

			for (add = KSTART; add < (KEND/1); add++) {
				*(memory_08+add)  = 0xAA;
			}

			for (add = KSTART; add < (KEND/1); add++) {
				if (*(memory_08+add) != 0xAA) {
					expe_08 = 0xAA;
					read_08 = *(memory_08+add);
					_display(KMER08, (memory_08+add), (uint32_t)expe_08, (uint32_t)read_08);
					return (EXIT_FAILURE);

				}
			}

			for (add = KSTART; add < (KEND/1); add++) {
				*(memory_08+add)  = 0x55;
			}

			for (add = KSTART; add < (KEND/1); add++) {
				if (*(memory_08+add) != 0x55) {
					expe_08 = 0x55;
					read_08 = *(memory_08+add);
					_display(KMER08, (memory_08+add), (uint32_t)expe_08, (uint32_t)read_08);
					return (EXIT_FAILURE);

				}
			}
		}

// Test b: circular; fill the memory with an incremental pattern and verify

		iotx_printf(KSYST, "\n8-bits:  Test Memory: fill with an incremental pattern\n");
		for (tests = 0; tests < nbTests; tests++) {
			iotx_printf(KSYST, ".");
	
			pattern_08 = 0;
			for (add = KSTART; add < (KEND/1); add++) {
				*(memory_08+add) = pattern_08++;
			}
	
			pattern_08 = 0;
			for (add = KSTART; add < (KEND/1); add++) {
				if (*(memory_08+add) != pattern_08++) {
					expe_08 = --pattern_08;
					read_08 = *(memory_08+add);
					_display(KMER08, (void *)(memory_08+add), (uint32_t)expe_08, (uint32_t)read_08);
					return (EXIT_FAILURE);

				}
			}
		}

// Test 3: Memory 16-bits
// ----------------------

// Test a: fill the memory with (uint8_t)0xAA55 - (uint8_t)0x55AA and verify

		memory_16 = (uint16_t *)&_stEXRAM;
		iotx_printf(KSYST, "\n16-bits: Test Memory: fill 0xAA55 - 0x55AA\n");
		for (tests = 0; tests < nbTests; tests++) {
			iotx_printf(KSYST, ".");
	
			for (add = KSTART; add < (KEND/2); add++) {
				*(memory_16+add)  = 0xAA55;
			}
	
			for (add = KSTART; add < (KEND/2); add++) {
				if (*(memory_16+add) != 0xAA55) {
					expe_16 = 0xAA55;
					read_16 = *(memory_16+add);
					_display(KMER16, (memory_16+add), (uint32_t)expe_16, (uint32_t)read_16);
					return (EXIT_FAILURE);

				}
			}
	
			for (add = KSTART; add < (KEND/2); add++) {
				*(memory_16+add)  = 0x55AA;
			}
	
			for (add = KSTART; add < (KEND/2); add++) {
				if (*(memory_16+add) != 0x55AA) {
					expe_16 = 0x55AA;
					read_16 = *(memory_16+add);
					_display(KMER16, (memory_16+add), (uint32_t)expe_16, (uint32_t)read_16);
					return (EXIT_FAILURE);

				}
			}
		}

// Test b: circular; fill the memory with an incremental pattern and verify

		iotx_printf(KSYST, "\n16-bits: Test Memory: fill with an incremental pattern\n");
		for (tests = 0; tests < nbTests; tests++) {
			iotx_printf(KSYST, ".");
	
			pattern_16 = 0;
			for (add = KSTART; add < (KEND/2); add++) {
				*(memory_16+add) = pattern_16++;
			}
	
			pattern_16 = 0;
			for (add = KSTART; add < (KEND/2); add++) {
				if (*(memory_16+add) != pattern_16++) {
					expe_16 = --pattern_16; 
					read_16 = *(memory_16+add);
					_display(KMER16, (memory_16+add), (uint32_t)expe_16, (uint32_t)read_16);
					return (EXIT_FAILURE);

				}
			}
		}

// Test 4: Memory 32-bits
// ----------------------

// Test a: fill the memory with (uint8_t)0xAA55AA55 - (uint8_t)0x55AA55AA and verify

		memory_32 = (uint32_t *)&_stEXRAM;
		iotx_printf(KSYST, "\n32-bits: Test Memory: fill 0xAA55AA55 - 0x55AA55AA\n");
		for (tests = 0; tests < nbTests; tests++) {
			iotx_printf(KSYST, ".");
	
			for (add = KSTART; add < (KEND/4); add++) {
				 *(memory_32+add)  = 0xAA55AA55;
			}
	
			for (add = KSTART; add < (KEND/4); add++) {
				if (*(memory_32+add) != 0xAA55AA55)	{
					expe_32 = 0xAA55AA55;
					read_32 = *(memory_32+add);
					_display(KMER32, (memory_32+add), (uint32_t)expe_32, (uint32_t)read_32);
					return (EXIT_FAILURE);

				}
			}
	
			for (add = KSTART; add < (KEND/4); add++) {
				*(memory_32+add)  = 0x55AA55AA;
			}
	
			for (add = KSTART; add < (KEND/4); add++) {
				if (*(memory_32+add) != 0x55AA55AA)	{
					expe_32 = 0x55AA55AA;
					read_32 = *(memory_32+add);
					_display(KMER32, (memory_32+add), (uint32_t)expe_32, (uint32_t)read_32);
					return (EXIT_FAILURE);

				}
			}
		}

// Test b: circular; fill the memory with an incremental pattern and verify

		iotx_printf(KSYST, "\n32-bits: Test Memory: fill with an incremental pattern\n");
		for (tests = 0; tests < nbTests; tests++) {
			iotx_printf(KSYST, ".");
	
			pattern_32 = 0;
			for (add = KSTART; add < (KEND/4); add++) {
				*(memory_32+add) = pattern_32++;
			}
	
			pattern_32 = 0;
			for (add = KSTART; add < (KEND/4); add++) {
				if (*(memory_32+add) != pattern_32++) {
					expe_32 = --pattern_32;
					read_32 = *(memory_32+add);
					_display(KMER32, (memory_32+add), (uint32_t)expe_32, (uint32_t)read_32);
					return (EXIT_FAILURE);

				}
			}
		}
		_display(KNOERR, NULL, 0, 0);
		return (EXIT_SUCCESS_OS);
	}
	_display(KPRERR, NULL, 0, 0);
	return (EXIT_FAILURE);
}

// Local routines
// ==============

/*
 * \brief _display
 *
 */
static	void	_display(int32_t error, void *add, uint32_t expe, uint32_t read) {

	switch (error) {
		case KNOERR: { iotx_printf(KSYST, "\nTest terminated\n");																		  break; }
		case KPRERR: { iotx_printf(KSYST, "Protocol error.\n\n");																		  break; }
		case KMER08: { iotx_printf(KSYST, "\nMemory error: address 0x%08X, Data expected 0x%02X, Data read 0x%02X\n\n", add, expe, read); break; }
		case KMER16: { iotx_printf(KSYST, "\nMemory error: address 0x%08X, Data expected 0x%04X, Data read 0x%04X\n\n", add, expe, read); break; }
		case KMER32: { iotx_printf(KSYST, "\nMemory error: address 0x%08X, Data expected 0x%08X, Data read 0x%08X\n\n", add, expe, read); break; }
		default:     { iotx_printf(KSYST, "\nHardware problem.\n\n");																	  break; }
	}
}

