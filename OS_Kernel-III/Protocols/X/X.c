/*
; X.
; ==

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
; Goal:		Control the temperature.
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

#define	KNBSAMPLES	128				// Nb. of samples (size of the vector)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"X            Control the temperature.                  (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Control the temperature\n"
									"=======================\n\n"

									"This protocol module allows to operate on the xtmp\n"
									"temperature manager\n\n"

									"Input format:  X\n"
									"Output format: x,t0,t1,t...t127\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(X, KIDPROTOCOL, KXNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				uint16_t	*temperature, i;
	volatile	shar_t		*pack;
	volatile	astp_t		*association;
	volatile	sema_t		*semaphore;

	enum 	{ KERRNOT, KERRASS, KERRSEM } error = KERRNOT;

	if (kern_getAssociationById("Temperature", &association) != KKERNNOERR) { error = KERRASS; }
	if (kern_findAssociation(association, &pack) 			 != KKERNNOERR) { error = KERRASS; }
	if (kern_getSemaphoreById("Vector", &semaphore)     	 != KKERNNOERR) { error = KERRSEM; }

	temperature = (uint16_t *)pack->oGeneral;

	switch (error) {
		case KERRNOT: {
			kern_lockSemaphore(semaphore, -1);
			iotx_printf(KSYST, "x,");
			for (i = 0; i < (KNBSAMPLES-1); i++) {
				iotx_printf(KSYST, "%d,",  temperature[i]);
			}
			iotx_printf(KSYST, "%d\n", temperature[KNBSAMPLES-1]);

			kern_unlockSemaphore(semaphore);
			return (EXIT_SUCCESS_OS);
		}
		case KERRASS: { iotx_printf(KSYST, "z,Association error.\n"); return (EXIT_FAILURE); }
		case KERRSEM: { iotx_printf(KSYST, "z,Semaphore error.\n");   return (EXIT_FAILURE); }
		default:	  {												  return (EXIT_FAILURE); }
	}
}

