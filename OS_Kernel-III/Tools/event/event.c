/*
; event.
; ======

;------------------------------------------------------------------------
; Author:	Koller Jean-Marc.	The 2017-07-17
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		List the created events and the processes waiting on each.
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

#include 	<uKOS.h>

#if (defined(__WITHEVNT__))
extern	volatile	evnt_t		vKern_event[KNBEVENTS];					// Events
extern	volatile	proc_t		vKern_process[KNBPROCESS];				// User processes, Id = 1..2^31; Id = IDLE if 1

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"event        List events.                              (c) JMK-2017";
LOC_CONST_STRG(aStrHelp[])        = "Show all events"
									"===============\n\n"

									"This tool displays the name of all the event used\n"
									"with the associated process.\n\n"

									"Input format:  event\n"
									"Output format: event information\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, char_t *argv[]);
static	void	_compose(const char_t *identifier, const char_t **idSpacer);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Event, KIDTOOL, KDUMPNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

static	int32_t	prgm(uint32_t argc, char_t *argv[]) {
			uint8_t		i, j;
	const	char_t		*idSpacer;

	iotx_printf(KSYST, "List of the system events.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) JMK-2017\n\n");
	iotx_printf(KSYST, " #  Event Id            Process\n\n");

	for (i = 0; i < KNBEVENTS; i++) {
		if ((vKern_event[i].oState & (1<<BEVNTINSTALLED)) != 0) {
			_compose(vKern_event[i].oIdentifier, &idSpacer);
			iotx_printf(KSYST, "%2d  %s%s  ", i, vKern_event[i].oIdentifier, idSpacer);

			for (j = 0; j < KNBPROCESS; j++) {
				if ((vKern_process[j].oInternal.oState & (1<<BPROCINSTALLED)) != 0) {
					if ((vKern_process[j].oInternal.oState & (1<<BPROCSUSPEVNT)) != 0) {
						if ((vKern_process[j].oSynchro.oListEvents & vKern_event[i].oBitEvent) != 0) {
							iotx_printf(KSYST, "%s ", vKern_process[j].oSpecification.oIdentifier);
						}
					}
				}
			}
			iotx_printf(KSYST, "\n");
		}
	}
	return (EXIT_SUCCESS_OS);
}

// Local routines
// ==============

/*
 * \brief _compose
 *
 * - input:  "1234567"
 * - output: "     "
 * - to accomodate "1234567     "
 * -               "            "
 *
 * \param[in]	*string		Ptr on a string
 * \param[in]	**idSpacer	Ptr on the idSpace
 * \param[out]	**idSpacer	Ptr on the final idSpace
 *
 */
static	void	_compose(const char_t *identifier, const char_t **idSpacer) {
					uint32_t	len;
	static	const	char_t		spacer[] = "                  ";

	*idSpacer = &spacer[0];
	strg_getBufferSize(identifier, &len);
	if (len <= sizeof(spacer)-1) {
		*idSpacer = &spacer[len];
	}
}
#endif
