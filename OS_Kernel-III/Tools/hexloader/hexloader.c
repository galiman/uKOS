/*
; hexloader.
; ==========

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
; Goal:		Intel hex+ loader.
;			This tool allows to download a Intel hex+ format binary
;			to the memory and to execute it.
;			Here are the Intel hex+ format definitions:
;
;			- Everything is represented in ASCII.
;
;			- Block format:
;				Mark:					1 char
;				Length of the block:	an hex. number of 2 chars.
;				Address:				an hex. number of 4 chars.
;				Block type:				2 chars.
;				Data:					an hex. 2n chars.
;				Checksum:				an hex. number of 2 chars.
;
;			- Mark:
;				":"
;			- Block type:
;				"00" normal data record
;				"01" end of file record
;				"02" not supported
;				"03" not supported
;				"04" extended linear address (for 32-bit CPU)
;				"05" execution address
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
enum	{ KHLOANOERR, KHLOAUMBSY, KHLOAWRARG, KHLOACKERR, KHLOARBFUL, KHLOAERNOI, KHLOAERFRA, KHLOAERPAR };
enum	{ KRUN, KNORUN };

extern	uint32_t	_stUMemo;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"hexloader    Intel hex+ (32-bit) loader.               (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Intel hex+ (32-bit) loader\n"
									"==========================\n\n"

									"This tool downloads an executable code\n"
									"in a Intel hex+ format (32-bit address range).\n\n"

									"Input format:  hexloader {-run | -norun}\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	int32_t	_getCounter(uint8_t *counter, uint8_t *checksum);
static	int32_t	_getType(uint8_t *type, uint8_t *checksum);
static	int32_t	_getOffset(uint32_t *offset, uint8_t *checksum);
static	int32_t	_getExecAddress(uint8_t **address, uint8_t *checksum);
static	int32_t	_getData(uint8_t *counter, uint8_t *checksum, uint8_t *address);
static	int32_t	_getHexValue(uint8_t *value);
static	int32_t	_getByte(uint8_t *byte);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Hexloader, KIDTOOL, KHEXLOADERNUM, prgm, "1.1", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint8_t		*address = NULL, byte, hexValue, type = 0, checksum, counter;
	int32_t		status, error = KHLOANOERR, (*code)(void);
	uint32_t	page = 0, offset = 0;
	bool_t		terminate = FALSE, equals;
	uint8_t		run = KRUN;

	RESERVE_COMM(KSYST, KDEVREAD);

	iotx_printf(KSYST, "Hex format Intel loader mode; waiting for the code.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// Verify if the user memory is available
// --------------------------------------

	if (syos_reserve(KDEVALL) != KSYOSNOERR) {
		terminate = TRUE;
		error = KHLOAUMBSY;
	}

// Analyze the command line
// ------------------------
//
// Example:
//
// run hexloader
// run hexloader -run
// run hexloader -norun

	switch (argc) {
		case 1: {
			run = KRUN;
			break;
		}
		case 2: {
			text_checkAsciiBuffer(argv[1], "-norun", &equals); if (equals) { run = KNORUN; break; }
			text_checkAsciiBuffer(argv[1], "-run",   &equals); if (equals) { run = KRUN;   break; }
			error = KHLOAWRARG;
			terminate = TRUE;
			break;
		}
		default: {
			error = KHLOAWRARG;
			terminate = TRUE;
			break;
		}
	}

	while (!terminate) {

// Waiting for the mark ":"

		do { _getByte(&byte); } while (byte != ':');

		error = _getCounter(&counter, &checksum); 			    if (error != KHLOANOERR) { terminate = TRUE; break; }
		error = _getOffset(&offset, &checksum);  			    if (error != KHLOANOERR) { terminate = TRUE; break; }
		error = _getType(&type, &checksum);						if (error != KHLOANOERR) { terminate = TRUE; break; }

		switch (type) {
			case 0: {
				address = (uint8_t *)(page + offset);
				error = _getData(&counter, &checksum, address);	if (error != KHLOANOERR) { terminate = TRUE; break; }
				error = _getHexValue(&hexValue);           		if (error != KHLOANOERR) { terminate = TRUE; break; }
				if (hexValue != (uint8_t)(~checksum + 1))         { error  = KHLOACKERR;   terminate = TRUE; break; }
				break;
			}
			case 1: {
				error = _getHexValue(&hexValue);           		if (error != KHLOANOERR) { terminate = TRUE; break; }
				if (hexValue != (uint8_t)(~checksum + 1))         { error  = KHLOACKERR;   terminate = TRUE; break; }
				terminate = TRUE;
				break;
			}
			case 4: {
				error = _getOffset(&page, &checksum); 			if (error != KHLOANOERR) { terminate = TRUE; break; }
				page <<= 16;
				if (page == 0) page = (uint32_t)&_stUMemo;
				error = _getHexValue(&hexValue);        		if (error != KHLOANOERR) { terminate = TRUE; break; }
				if (hexValue != (uint8_t)(~checksum + 1))         { error  = KHLOACKERR;   terminate = TRUE; break; }
				break;
			}
			case 5: {
				error = _getExecAddress(&address, &checksum); 	if (error != KHLOANOERR) { terminate = TRUE; break; }
				error = _getHexValue(&hexValue);       			if (error != KHLOANOERR) { terminate = TRUE; break; }
				if (hexValue != (uint8_t)(~checksum + 1))         { error  = KHLOACKERR;   terminate = TRUE; break; }
				break;
			}
			default: {
				break;
			}
		}
	}

// Terminated

	misc_offLed(KLED1);

	comm_flush(KSYST);
	RELEASE_COMM(KSYST, KDEVREAD);

	if (error != KHLOAUMBSY) {
		syos_release(KDEVALL);
	}

	switch (error) {
		case KHLOANOERR: { iotx_printf(KSYST, "\nHex: download terminated.\n\n");
						   syos_setDoLoCode(address);
						   syos_getDoLoCode((void *)&code);
						   if (run == KRUN)	{										  status = (*code)();       break; }
						   else {													  status = EXIT_SUCCESS_OS; break; }
		}
		case KHLOAUMBSY: { iotx_printf(KSYST, "\nHex: The user memory is busy.\n\n"); status = EXIT_FAILURE;    break; }
		case KHLOAWRARG: { iotx_printf(KSYST, "\nHex: Incorrect arguments.\n\n");     status = EXIT_FAILURE;    break; }
		case KHLOACKERR: { iotx_printf(KSYST, "\nHex: wrong checksum.\n\n");          status = EXIT_FAILURE;    break; }
		case KHLOARBFUL: { iotx_printf(KSYST, "\nHex: buffer full error.\n\n");       status = EXIT_FAILURE;    break; }
		case KHLOAERNOI: { iotx_printf(KSYST, "\nHex: noise error.\n\n");             status = EXIT_FAILURE;    break; }
		case KHLOAERFRA: { iotx_printf(KSYST, "\nHex: framing error.\n\n");           status = EXIT_FAILURE;    break; }
		case KHLOAERPAR: { iotx_printf(KSYST, "\nHex: parity error.\n\n");            status = EXIT_FAILURE;    break; }
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
static	int32_t	_getByte(uint8_t *byte) {
	#define	KLNBUF		32
	static	uint8_t		buffer[KLNBUF], cptBlinkLED = 0;
	static	uint32_t	i = 0, size = 0;

	if (size == 0) {
		i = 0;
		do {
			kern_switchFast();
			size = KLNBUF;

			switch (comm_read(KSYST, buffer, &size)) {
				case KCOMMRBFUL: { return (KHLOARBFUL); }
				case KCOMMERNOI: { return (KHLOAERNOI); }
				case KCOMMERFRA: { return (KHLOAERFRA); }
				case KCOMMERPAR: { return (KHLOAERPAR); }
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
	*byte = buffer[i++];
	return (KHLOANOERR);
}

/*
 * \brief _getHexValue
 *
 * - Read a hexa value (2 chars)
 * - Ex. 31 is read as '3' (0x33) and '1' (0x31) 2-Bytes
 *
 */
static	int32_t	_getHexValue(uint8_t *value) {
			uint8_t		byte;
			int32_t		status;
	const	uint8_t		tabAB[] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15 };

	status = _getByte(&byte);
	if (status != KHLOANOERR) {
		return (status);
	}

	if       ((byte >= '0') && (byte <= '9'))									   { *value  = (tabAB[byte             - '0']<<4); }
	else if (((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'))) { *value  = (tabAB[(byte & (~0x20)) - '0']<<4); }

	status = _getByte(&byte);  if (status != KHLOANOERR) { return (status); }
	if       ((byte >= '0') && (byte <= '9'))									   { *value += (tabAB[byte             - '0']);    }
	else if (((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'))) { *value += (tabAB[(byte & (~0x20)) - '0']);    }
	return (KHLOANOERR);
}

/*
 * \brief _getCounter
 *
 * - Read the counter field
 *
 */
static	int32_t	_getCounter(uint8_t *counter, uint8_t *checksum) {
	int32_t		status;

	status = _getHexValue(counter);
	if (status != KHLOANOERR) {
		return (status);
	}

	*checksum = *counter;
	return (KHLOANOERR);
}

/*
 * \brief _getOffset
 *
 * - Read the offset field
 *
 */
int32_t	_getOffset(uint32_t *offset, uint8_t *checksum) {
	uint8_t		i, hexValue;
	int32_t		status;

// Build the offset 16-bit wide

	*offset = 0;
	for (i = 0; i < 2; i++) {
		status = _getHexValue(&hexValue);
		if (status != KHLOANOERR) {
			return (status);
		}

		*offset = (*offset<<8) + (uint32_t)hexValue;
		*checksum += hexValue;
	}
	return (KHLOANOERR);
}

/*
 * \brief _getExecAddress
 *
 * - Read the execution address field
 *
 */
static	int32_t	_getExecAddress(uint8_t **address, uint8_t *checksum) {
	uint8_t		i, hexValue;
	int32_t		status;
	uint32_t	field = 0;

// Build the address 32-bit wide

	for (i = 0; i < 4; i++) {
		status = _getHexValue(&hexValue);
		if (status != KHLOANOERR) {
			return (status);
		}

		field = (field<<8) + (uint32_t)hexValue;
		*checksum += hexValue;
	}
	*address = (uint8_t *)field;
	return (KHLOANOERR);
}

/*
 * \brief _getType
 *
 * - Read the type field
 *
 */
static	int32_t	_getType(uint8_t *type, uint8_t *checksum) {
	int32_t		status;

	status = _getHexValue(type);
	if (status != KHLOANOERR) {
		return (status);
	}

	*checksum += *type;
	return (KHLOANOERR);
}

/*
 * \brief _getData
 *
 * - Read the data field
 *
 */
static	int32_t	_getData(uint8_t *counter, uint8_t *checksum, uint8_t *address) {
	uint8_t		i, hexValue, *load;
	int32_t		status;

	load = address;
	for (i = 0; i < (*counter); i++) {
		status = _getHexValue(&hexValue);
		if (status != KHLOANOERR) {
			return (status);
		}

		*load++ = hexValue;
		*checksum += hexValue;
	}
	return (KHLOANOERR);
}

