/*
; echo.
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
; Goal:		echo tool; echo from a communication manager.
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

#define	KLED1		1
#define	KCOMMTO5S	5000000
#define	KCOMMTO10S	10000000

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"echo         echo function.                            (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "echo\n"
									"====\n\n"

									"This tool performs an echo on a communication manager\n\n"

									"Input format:  echo commManager1, commManager2\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	bool_t	_getByte(uint32_t commManager, uint8_t *buffer, uint64_t timeout);
static	void	_putByte(uint32_t commManager, uint8_t *buffer);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Echo, KIDTOOL, KECHONUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
			uint8_t		data;
			int32_t		status;
			uint32_t	commManager0, commManager1;
			uint64_t	timeout;
			bool_t		terminate = FALSE;
			enum 		{ KERRNOT, KERRINA, KERRCHA, KERRBSY } error = KERRNOT;
	const	char_t		*comm = "", *comm0, *comm1;

	iotx_printf(KSYST, "Communication echo.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc < 2) {
		error = KERRINA;
	}
	else {
		comm0 = argv[1];
		comm1 = argv[2];
		commManager0 = GET_PTR_32(comm0);
		commManager1 = GET_PTR_32(comm1);

// Reserve the commManager0 and commManager1 managers

		switch (comm_reserve(commManager0, KDEVREAD)) {
			case KCOMMNODEV: {
				error = KERRCHA;
				comm = comm0;
				terminate = TRUE;
				break;
			}
			case KCOMMCHBSY: {
				error = KERRBSY;
				comm = comm0;
				terminate = TRUE;
				break;
			}
			default : {
				break;
			}
		}

		switch (comm_reserve(commManager1, KDEVWRITE)) {
			case KCOMMNODEV: {
				error = KERRCHA;
				comm = comm1;
				terminate = TRUE;
				RELEASE_COMM(commManager0, KDEVREAD);
				break;
			}
			case KCOMMCHBSY: {
				error = KERRBSY;
				comm = comm1;
				terminate = TRUE;
				RELEASE_COMM(commManager0, KDEVREAD);
				break;
			}
			default : {
				break;
			}
		}

		comm_flush(commManager0);
		timeout = KCOMMTO10S;

		while (!terminate) {

// Read commManager0 under a 10s or 5s timeout (10s for the first char) and write on the commManager1

			if (!_getByte(commManager0, &data, timeout)) {
				error = KERRNOT;
				comm_flush(commManager0);
				RELEASE_COMM(commManager0, KDEVREAD);
				RELEASE_COMM(commManager1, KDEVWRITE);
				_putByte(commManager1, &data);
				terminate = TRUE;
			}
			else {
				_putByte(commManager1, &data);
				timeout = KCOMMTO5S;
			}
		}
	}

	misc_offLed(KLED1);

	switch (error) {
		case KERRNOT: {	iotx_printf(KSYST, "\n");										status = EXIT_SUCCESS_OS; break; }
		case KERRINA: { iotx_printf(KSYST, "Incorrect arguments.\n\n");					status = EXIT_FAILURE;    break; }
		case KERRCHA: { iotx_printf(KSYST, "Bad communication manager: %s\n\n", comm);	status = EXIT_FAILURE;    break; }
		case KERRBSY: { iotx_printf(KSYST, "Communication manager busy: %s\n\n", comm);	status = EXIT_FAILURE;    break; }
		default:	  {																	status = EXIT_FAILURE;    break; }
	}
	return (status);
}

// Local routines
// ==============

/*
 * \brief _getByte
 *
 * - Read a Byte
 *
 */
static	bool_t	_getByte(uint32_t commManager, uint8_t *buffer, uint64_t timeout) {
	int32_t		status;
	uint32_t	size;
	uint64_t	time[2];

	kern_getTiccount(&time[0]);

	do {
		kern_getTiccount(&time[1]);
		if ((time[1] - time[0]) >= timeout) {
			return (FALSE);
		}

		kern_switchFast();
		size = 1;
		status = comm_read(commManager, buffer, &size);
	} while (status != KCOMMNOERR);

	misc_toggleLed(KLED1);
	return (TRUE);
}

/*
 * \brief _putByte
 *
 * - Write a Byte
 *
 */
static	void	_putByte(uint32_t commManager, uint8_t *buffer) {
	int32_t		status;

	do {
		kern_switchFast();

		status = comm_write(commManager, buffer, 1);
	} while (status != KCOMMNOERR);
}

