/*
; temperature.
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
; Goal:		temperature process; continuous acquisition of the temperature.
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
#include	<stdlib.h>

#define	__SIMULE__

#define	KTIMEACQ	200					// 200-ms
#define	KNBSAMPLES	128					// Nb. of samples (size of the vector)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"temperature  temperature acquisition process           (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "temperature process\n"
									"===================\n\n"

									"Acquisition of the temperature\n\n";

LOC_CONST_STRG(aStrIden[])        = "Process_temperature";
LOC_CONST_STRG(aStrText[])        = "Process temperature: temp acquisition.    (c) EFr-2017";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_process(const void *argument);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Temperature, KIDPROCESS, KTEMPERATURENUM, prgm, "1.0", (1<<BSHOW));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	volatile	proc_t 	*process;

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, _process, aStrIden, KDEF0, (KLOWPRIORITY+7), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR) { exit(EXIT_PANIC); }
	return (EXIT_SUCCESS_OS);
}

/*
 * \brief _process
 *
 * - Temperature acquisitions (5-Hz)
 *   In the simulated table a period is represented by 16 samples
 *   200-ms per sample -> 3.2-s -> 1/3.2 = 0.3225-Hz
 *
 */
static	void	_process(const void *argument) {
						shar_t		pack;
			volatile	astp_t		*association;
			volatile	sema_t		*semaphore;
	static				int16_t		i, vTemperature[KNBSAMPLES];

	#if (defined(__SIMULE__))
	const				int16_t		aSimule[KNBSAMPLES] = {
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100,
										20.23*100, 20.52*100, 21.23*100, 21.87*100, 22.21*100, 22.67*100, 23.12*100, 23.67*100,
										23.78*100, 23.34*100, 22.76*100, 22.09*100, 21.56*100, 21.14*100, 20.55*100, 20.03*100
									};

	#else
						float64_t	temperature;
	#endif

// Create the association "Temperature" and the semaphore "Vector"

	pack.oGeneral = (void *)&vTemperature[0];
	pack.oSize = sizeof(vTemperature);

	if (kern_createAssociation("Temperature", &association) != KKERNNOERR) { exit(EXIT_PANIC); }
	if (kern_publishAssociation(association, &pack) 		!= KKERNNOERR) { exit(EXIT_PANIC); }
	if (kern_createMutxSemaphore("Vector", &semaphore)      != KKERNNOERR) { exit(EXIT_PANIC); }

	#if (defined(__SIMULE__))

	#else
	RESERVE(TMP0, KDEVALL);
	#endif

	while (TRUE) {
		kern_suspendProcess(KTIMEACQ);

// Waiting for the semaphore
// Rearrange the vector & store the new value inside the FIFO
// Release the semaphore
// If we use the __SIMULE__ flag (emulation of the temperature acquisition)
// then, a random noise (-1 .. +1 degree) is added

		kern_lockSemaphore(semaphore, -1);

		#if (defined(__SIMULE__))
		for (i = 0; i < KNBSAMPLES; i++) {
			vTemperature[i] = aSimule[i] + ((int16_t)(rand())>>8);
		}

		#else
		for (i = 0; i < (KNBSAMPLES-1); i++) {
			vTemperature[KNBSAMPLES-1-i] = vTemperature[KNBSAMPLES-2-i];
		}

		tmp0_getTemperature(&temperature);
		vTemperature[0] = (int16_t)(temperature * 100.0);
		#endif

		kern_unlockSemaphore(semaphore);
	}	
}

