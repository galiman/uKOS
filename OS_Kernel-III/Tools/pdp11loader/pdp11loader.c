/*
; pdp11loader.
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
; Goal:		PDP11 DEC loader.
;			This tool allows to download a PDP11 format binary to the memory
;			and to execute it.
;			Here are the PDP11 format definitions:
;
;			HEADER	1	;
;			HEADER	0	; start block
;			NUMBER	L	;
;			NUMBER	H	; Nb. of Bytes to load
;			ADDR	L	;
;			ADDR	H	; load address
;
;			DATA	xx	; data
;			CHEK	yy	; block checksum
;
;			A block can be repeated ...
;			...
;			...
;
;			HEADER	1	;
;			HEADER	0	; start block
;			NUMBER	0	;
;			NUMBER	6	; Nb. of Bytes to load (last block)
;			ADDR	L	;
;			ADDR	H	; execution address if <> 1
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
#define	KNBCARBLK	20

enum	{ KPLOANOERR, KPLOAUMBSY, KPLOAWRARG, KPLOACKERR, KPLOARBFUL, KPLOAERNOI, KPLOAERFRA, KPLOAERPAR, KPLOAERBLK };
enum	{ KRUN, KNORUN };

extern	uint32_t	_stUMemo;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"pdp11loader  PDP11 DEC loader.                         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "PDP11 DEC loader\n"
									"================\n\n"

									"This function module downloads an executable code.\n"
									"This loader is reserved for internal use. The user\n"
									"should use S Motorola loader to download.\n\n"

									"Input format:  pdp11loader {-run | -norun}\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	int32_t	_getCounter(uint16_t *counter, uint8_t *checksum);
static	int32_t	_getAddress(uint8_t **address, uint8_t *checksum);
static	int32_t	_getData(uint16_t *counter, uint8_t *checksum, uint8_t *address);
static	int32_t	_syncHeader(uint8_t *checksum);
static	int32_t	_getByte(uint8_t *byte, uint8_t *checksum);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Pdp11loader, KIDTOOL, KPDP11LOADERNUM, prgm, "1.1", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint8_t		*address = NULL, checksum = 0;
	uint16_t	counter;
	int32_t		status, error = KPLOANOERR, (*code)(void);
	bool_t		terminate = FALSE, equals;
	uint8_t		run = KRUN;

	RESERVE_COMM(KSYST, KDEVREAD);

	iotx_printf(KSYST, "PDP11 format DEC loader mode; waiting for the code.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// Verify if the user memory is available
// --------------------------------------

	if (syos_reserve(KDEVALL) != KSYOSNOERR) {
		terminate = TRUE;
		error = KPLOAUMBSY;
	}

// Analyze the command line
// ------------------------
//
// Example:
//
// run pdp11loader
// run pdp11loader -run
// run pdp11loader -norun

	switch (argc) {
		case 1: {
			run = KRUN;
			break;
		}
		case 2: {
			text_checkAsciiBuffer(argv[1], "-norun", &equals); if (equals) { run = KNORUN; break; }
			text_checkAsciiBuffer(argv[1], "-run",   &equals); if (equals) { run = KRUN;   break; }
			error = KPLOAWRARG;
			terminate = TRUE;
			break;
		}
		default: {
			error = KPLOAWRARG;
			terminate = TRUE;
			break;
		}
	}

// Waiting for the header, the counter, the address, and load the record

	if (error == KPLOANOERR) {
		while (!terminate) {
			counter = 0;
			error = _syncHeader(&checksum);
			if (error == KPLOANOERR) {
				error = _getCounter(&counter, &checksum);
				if (error == KPLOANOERR) {
					error = _getAddress(&address, &checksum);
					if (error == KPLOANOERR) {
						if (counter < 256) {
							if (counter == 6) {
								terminate = TRUE;
							}
							else {
								counter -= 6;
								address = (uint8_t *)((uint32_t)address + (uint32_t)&_stUMemo);
								error = _getData(&counter, &checksum, address);
								if (error != KPLOANOERR) {
									terminate = TRUE;
									break;
								}
							}
						}
						else {
							error = KPLOAERBLK;
							terminate = TRUE;
						}
					}
					else {
						terminate = TRUE;
					}
				}
				else {
					terminate = TRUE;
				}
			}
			else {
				terminate = TRUE;
			}
		}
	}

// Terminated

	misc_offLed(KLED1);

	comm_flush(KSYST);
	RELEASE_COMM(KSYST, KDEVREAD);
	syos_release(KDEVALL);

	switch (error) {
		case KPLOANOERR: { iotx_printf(KSYST, "\nPDP: download terminated.\n\n");
						   address = (uint8_t *)&_stUMemo;
						   syos_setDoLoCode(address);
						   syos_getDoLoCode((void *)&code);
						   if (run == KRUN)	{										  status = (*code)();       break; }
						   else {										  			  status = EXIT_SUCCESS_OS; break; }
		}
		case KPLOAUMBSY: { iotx_printf(KSYST, "\nPDP: The user memory is busy.\n\n"); status = EXIT_FAILURE;    break; }
		case KPLOAWRARG: { iotx_printf(KSYST, "\nPDP: Incorrect arguments.\n\n");     status = EXIT_FAILURE;    break; }
		case KPLOACKERR: { iotx_printf(KSYST, "\nPDP: wrong checksum.\n\n");          status = EXIT_FAILURE;    break; }
		case KPLOARBFUL: { iotx_printf(KSYST, "\nPDP: buffer full error.\n\n");       status = EXIT_FAILURE;    break; }
		case KPLOAERNOI: { iotx_printf(KSYST, "\nPDP: noise error.\n\n");             status = EXIT_FAILURE;    break; }
		case KPLOAERFRA: { iotx_printf(KSYST, "\nPDP: framing error.\n\n");           status = EXIT_FAILURE;    break; }
		case KPLOAERPAR: { iotx_printf(KSYST, "\nPDP: parity error.\n\n");            status = EXIT_FAILURE;    break; }
		case KPLOAERBLK: { iotx_printf(KSYST, "\nPDP: block counter > 256.\n\n");     status = EXIT_FAILURE;    break; }
		default:	     {														      status = EXIT_FAILURE;    break; }
	}
	return (status);
}

// Local routines
// ==============

/*
 * \brief _getByte
 *
 * - Read a Byte
 * - Try to read a burst of KLNBUF Bytes
 *
 */
static	int32_t	_getByte(uint8_t *byte, uint8_t *checksum) {
	#define	KLNBUF		32
	static	uint8_t		buffer[KLNBUF], cptBlinkLED = 0;
	static	uint32_t	i = 0, size = 0;

	if (size == 0) {
		i = 0;
		do {
			kern_switchFast();
			size = KLNBUF;

			switch (comm_read(KSYST, buffer, &size)) {
				case KCOMMRBFUL: { return (KPLOARBFUL); }
				case KCOMMERNOI: { return (KPLOAERNOI); }
				case KCOMMERFRA: { return (KPLOAERFRA); }
				case KCOMMERPAR: { return (KPLOAERPAR); }
				default: {
					break;
				}
			}
		} while (size == 0);
	}

	if (cptBlinkLED++ >= KNBCARBLK) {
		cptBlinkLED = 0;
		misc_toggleLed(KLED1);
	}
	size--;
	*checksum += buffer[i];
	*byte = buffer[i++];
	return (KPLOANOERR);
}

/*
 * \brief _syncHeader
 *
 * - Synchronize to the header
 * - A header is 0x10 followed by 0x00
 *
 */
static	int32_t	_syncHeader(uint8_t *checksum) {
	uint8_t		byte;
	uint16_t	header;
	int32_t		status;

	header = 0xFFFF;
	do {
		status = _getByte(&byte, checksum);
		if (status != KPLOANOERR) {
			return (status);
		}
		header = (header<<8) + (uint16_t)byte;
	} while (header != 0x0100);
	return (KPLOANOERR);
}

/*
 * \brief _getCounter
 *
 * - Read the counter field
 *
 */
static	int32_t	_getCounter(uint16_t *counter, uint8_t *checksum) {
	uint8_t		cptMSB, cptLSB;
	int32_t		status;

	status = _getByte(&cptLSB, checksum);
	if (status != KPLOANOERR) {
		return (status);
	}

	status = _getByte(&cptMSB, checksum);
	if (status != KPLOANOERR) {
		return (status);
	}

	*counter = ((cptMSB<<8) + cptLSB);
	return (KPLOANOERR);
}

/*
 * \brief _getAddress
 *
 * - Read the address field
 *
 */
static	int32_t	_getAddress(uint8_t **address, uint8_t *checksum) {
	uint8_t		addressMsb, addressLsb;
	int32_t		status;
	uint32_t	field = 0;

	status = _getByte(&addressLsb, checksum);
	if (status != KPLOANOERR) {
		return (status);
	}

	status = _getByte(&addressMsb, checksum);
	if (status != KPLOANOERR) {
		return (status);
	}

	field = (uint32_t)(addressMsb<<8) + (uint32_t)addressLsb;
	*address = (uint8_t *)field;
	return (KPLOANOERR);
}

/*
 * \brief _getData
 *
 * - Read the data field
 *
 */
static	int32_t	_getData(uint16_t *counter, uint8_t *checksum, uint8_t *address) {
	uint8_t	i, byte, *load;
	int32_t	status;

	load = address;
	for (i = 0; i < (*counter); i++) {
		status = _getByte(&byte, checksum); 
		if (status != KPLOANOERR) {
			return (status);
		}
		*load++ = byte;
	}
	status = _getByte(&byte, checksum);
	if (status != KPLOANOERR) {
		return (status);
	}

	if (*checksum != 0) {
		status =  KPLOACKERR;
		return (status);
	}
	return (KPLOANOERR);
}

