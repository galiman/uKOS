/*
; semaphore.
; ==========

;------------------------------------------------------------------------
; Author:	Koller Jean-Marc.	The 2017-07-17
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		List the created semaphores and show the processes waiting on each.
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

#if (defined(__WITHSEMA__))
extern	volatile	sema_t		vKern_semaphore[KNBSEMAPHO];			// Semaphores

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"semaphore    List semaphores.                          (c) JMK-2017";
LOC_CONST_STRG(aStrHelp[])        = "Show all created semaphores"
									"===========================\n\n"

									"This tool displays the name of all the semaphores used\n"
									"with the associated process.\n\n"

									"Input format:  semaphore\n"
									"Output format: semaphore information\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, char_t *argv[]);
static	void	_compose(const char_t *identifier, const char_t **idSpacer);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Semaphore, KIDTOOL, KDUMPNUM, prgm, "1.1", ((1<<BSHOW) | (1<<BEXECONSOLE)));

static	int32_t	prgm(uint32_t argc, char_t *argv[]) {
				int32_t		status;
				uint32_t	counter;
				uint8_t		i, j, nbAttached;
				enum		{ KSEMANOERR, KSEMAENMEM } error = KSEMANOERR;
				char_t		kind, *buffer;
	const		char_t		*idBuffer[KNBPROCESS], *identifier, *idSpacerS, *owner, *idSpacerO;
	volatile	proc_t		*process;

	iotx_printf(KSYST, "List of the system semaphores.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) JMK-2017\n\n");
	iotx_printf(KSYST, " #  Semaphore Id         Kind  Counter  Owner Id                 Waiting\n\n");

	buffer = (char_t *)syos_malloc(KINTERNAL, KNBPROCESS*KLNSTRID);
	if (buffer != NULL) {
		for (i = 0; i < KNBSEMAPHO; i++) {
			if ((vKern_semaphore[i].oState & (1<<BSEMAINSTALLED)) != 0) {

// Prepare the generic printing characteristics
// for all the semaphores (id, spacer, kind, counter & owner)

				INTERRUPTION_OFF;
				identifier = vKern_semaphore[i].oIdentifier;
				_compose(identifier, &idSpacerS);
				kind = ((vKern_semaphore[i].oState & (1<<BSEMASYNCHRO)) != 0) ? ('S') : ('M');
				counter = vKern_semaphore[i].oCounter;
				owner = (vKern_semaphore[i].oOwner == NULL) ? ("") : (vKern_semaphore[i].oOwner->oSpecification.oIdentifier);
				_compose(owner, &idSpacerO);

// Scann the semaphore list and collect
// the name of all the attached processes

				nbAttached = 0;
				if (vKern_semaphore[i].oList.oNbElements > 0) {
					process = vKern_semaphore[i].oList.oFirst;
					buffer[0] = '\0';
					for (j = 0; j < vKern_semaphore[i].oList.oNbElements; j++) {
						if (process == NULL) {

// This should not happen!
// Need better implementation

							break;
						}

// Save the names of all the attached
// processes

						idBuffer[j] = process->oSpecification.oIdentifier;
						process = process->oObject.oForward;
					}
					nbAttached = j;
				}
				INTERRUPTION_RESTORED;

				iotx_printf( KSYST, "%2d  %s%s  %c   %3d", i, identifier, idSpacerS, kind, counter);

// Mode mutex:
// Display the owner and eventually all the suspended processes

				if (kind == 'M') {
					if ((nbAttached == 0) && (counter == 0)) {
						iotx_printf(KSYST, "       %s\n", owner);
					}
					if (nbAttached > 0) {
						iotx_printf(KSYST, "       %s%s     %s\n", owner, idSpacerO, idBuffer[0]);
						for (j = 1; j < nbAttached; j++) {
							iotx_printf(KSYST, "                                                                 %s\n", idBuffer[j]);
						}
					}
				}
				iotx_printf(KSYST, "\n");
			}
		}
		iotx_printf(KSYST, "\n");
		syos_free(buffer);
	}
	else {
		error = KSEMAENMEM;
	}

	switch (error) {
		case KSEMANOERR: {
					   													   status = EXIT_SUCCESS_OS; break; }
		case KSEMAENMEM: { iotx_printf(KSYST, "Not enough memory.\n\n");   status = EXIT_FAILURE;    break; }
		default:	     {												   status = EXIT_FAILURE;    break; }
	}
	return (status);
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
	static	const	char_t		spacer[] = "                    ";

	*idSpacer = &spacer[0];
	strg_getBufferSize(identifier, &len);
	if (len <= sizeof(spacer)-1) {
		*idSpacer = &spacer[len];
	}
}
#endif
