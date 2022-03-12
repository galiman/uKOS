/*
; dumptrace.
; ==========

;------------------------------------------------------------------------
; Author:	Koller Jean-Marc.	The 2017-06-01
; Modifs:	Franzi Edo.			The 2017-06-03
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Display the trace fifo captured with syos_trace().
;			Idea, concept and original implementation by Koller Jean-Marc.
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

extern	trace_t		vTraceFifo[KSZFIFO];

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"dumptrace    Dump the trace buffer.                    (c) JMK-2017";
LOC_CONST_STRG(aStrHelp[])        = "Dump the circular trace buffer\n"
									"==============================\n\n"

									"This tool display the content of the fifo trace\n\n"

									"Input format:  dumptrace\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Dumptrace, KIDTOOL, KDUMPNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint32_t	i;
	trace_t		*traceFifo;

	iotx_printf(KSYST, "Trace dump.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) JMK-2017\n\n");

// Reserve the necessary memory

	traceFifo = (trace_t *)syos_malloc(KINTERNAL, KSZFIFO*sizeof(trace_t));
	if (traceFifo == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		return (EXIT_FAILURE);
	}

// Copy the fifo into a memory (to ensure the coherence between the Fifo & the parameter)

	INTERRUPTION_OFF;
	syos_fastCopy(vTraceFifo, traceFifo, KSZFIFO*sizeof(trace_t));
	INTERRUPTION_RESTORED;

	iotx_printf(KSYST, "Start of System trace dump:\nNewest\n");
	for (i = 0; i < KSZFIFO; i++) {
		if (traceFifo[i].oIdentifier != NULL) {
			iotx_printf(KSYST, vTraceFifo[i].oIdentifier, vTraceFifo[i].oParameter);
			iotx_printf(KSYST, "\n");
		}
	}
	iotx_printf(KSYST, "Oldest\nEnd of System trace dump.\n\n");

	syos_free(traceFifo);
	return (EXIT_SUCCESS_OS);
}

