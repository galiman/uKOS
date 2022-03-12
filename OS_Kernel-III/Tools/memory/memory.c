/*
; memory.
; =======

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
; Goal:		Give the memory information.
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

extern	uint32_t	_stTEXT, _enTEXT;
extern	uint32_t	_stDATA, _enDATA;
extern	uint32_t	_stRODATA, _enRODATA;
extern	uint32_t	_stBSS, _enBSS;
extern	uint32_t	_stHeapI, _stHeapE, _stHeapC;
extern	uint32_t	_lnHeapI, _lnHeapE, _lnHeapC;
extern	uint32_t	vSyos_usdMemoryI, vSyos_usdMemoryE, vSyos_usdMemoryC;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"memory       Give the memory section information.      (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Give the memory information\n"
									"===========================\n\n"

									"This tool displays information about the memory\n"
									"section (DATA/CODE) of the system.\n\n"

									"Input format:  memory\n"
									"Output format: Section text:      address: 0x********, length: 0x********\n"
									"               Section rodata:    address: 0x********, length: 0x********\n"
									"               Section data:      address: 0x********, length: 0x********\n"
									"               section bss:       address: 0x********, length: 0x********\n\n"

									"               Internal heap:     address: 0x********, used: x.y%\n"
									"               External heap:     address: 0x********, used: x.y%\n"
									"               Core coupled heap: address: 0x********, used: x.y%\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Memory, KIDTOOL, KMEMORYNUM, prgm, "1.6", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	float64_t	memfI, memfE, memfC;
	int32_t		lengthI, lengthE, lengthC;
	uint32_t	usdMemoryI, usdMemoryE, usdMemoryC;

	iotx_printf(KSYST, "Memory information.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// This because of a gcc bug
// !!! Never use this
//     ... if ((int32_t)&_lnHeapI > 0)
//
//     but use this
//     ... int32_t var
//     ... var = (int32_t)&_lnHeapI
//     ... if (var > 0)

	lengthI = (int32_t)&_lnHeapI;
	lengthE = (int32_t)&_lnHeapE;
	lengthC = (int32_t)&_lnHeapC;

	INTERRUPTION_OFF;
	if (lengthI > 0) { memfI   = ((float64_t)vSyos_usdMemoryI / lengthI) * 100; }
	if (lengthE > 0) { memfE   = ((float64_t)vSyos_usdMemoryE / lengthE) * 100; }
	if (lengthC > 0) { memfC   = ((float64_t)vSyos_usdMemoryC / lengthC) * 100; }

	usdMemoryI = vSyos_usdMemoryI;
	usdMemoryE = vSyos_usdMemoryE;
	usdMemoryC = vSyos_usdMemoryC;
	INTERRUPTION_RESTORED;

					   iotx_printf(KSYST, "Section text:      address: 0x%08X, length: 0x%08X\n",            &_stTEXT,   (&_enTEXT   - &_stTEXT));
					   iotx_printf(KSYST, "Section rodata:    address: 0x%08X, length: 0x%08X\n",            &_stRODATA, (&_enRODATA - &_stRODATA));
					   iotx_printf(KSYST, "Section data:      address: 0x%08X, length: 0x%08X\n",            &_stDATA,   (&_enDATA   - &_stDATA));
					   iotx_printf(KSYST, "Section bss:       address: 0x%08X, length: 0x%08X\n\n",          &_stBSS,    (&_enBSS    - &_stBSS));

	if (lengthI > 0) { iotx_printf(KSYST, "Internal heap:     address: 0x%08X, used:   %08d -> %d.%02d%%\n", &_stHeapI, usdMemoryI, FLOAT_2(memfI)); }
	if (lengthE > 0) { iotx_printf(KSYST, "External heap:     address: 0x%08X, used:   %08d -> %d.%02d%%\n", &_stHeapE, usdMemoryE, FLOAT_2(memfE)); }
	if (lengthC > 0) { iotx_printf(KSYST, "Core Coupled heap: address: 0x%08X, used:   %08d -> %d.%02d%%\n", &_stHeapC, usdMemoryC, FLOAT_2(memfC)); }
					   iotx_printf(KSYST, "\n");
	return (EXIT_SUCCESS_OS);
}

