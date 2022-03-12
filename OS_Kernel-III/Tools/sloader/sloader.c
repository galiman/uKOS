/*
; sloader.
; ========

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
; Goal:		Motorola S1-9, S2-8, S3-7 loader.
;			This tool allows to download a S format binary to the memory
;			and to execute it.
;			Here are the S format definitions:
;
;			- Everything is represented in ASCII.
;
;			- Block format:
;				Block type:				2 chars.
;				Length of the block:	an hex. number of 2 chars.
;				Address:				an hex. number of 4, 6 or 8 chars.
;				Data:					an hex. 2n chars.
;				Checksum:				an hex. number of 2 chars.
;
;			- Block type:
;				"S1" data block for addresses coded over 16-bits
;				"S2" data block for addresses coded over 24-bits
;				"S3" data block for addresses coded over 32-bits
;				"S7" end block for addresses coded over 32-bits
;				"S8" end block for addresses coded over 24-bits
;				"S9" end block for addresses coded over 16-bits
;				!!! S0 & S5 are not supported but tolerated.
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

enum	{ KSLOANOERR, KSLOAUMBSY, KSLOAWRARG, KSLOACKERR, KSLOARBFUL, KSLOAERNOI, KSLOAERFRA, KSLOAERPAR, KSLOAOSERR };
enum	{ KRUN, KNORUN };

extern	uint32_t	_stUMemo;
extern	uint32_t	_lnUMemo;

// Module strings
// ==============

#if (defined(__MAN_USDC__))
// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sloader      Motorola S1-9, S2-8, S3-7 loader.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Motorola S1-9, S2-8, S3-7 loader\n"
									"================================\n\n"

									"This tool downloads an executable code.\n"
									"The implementation of this loader does not support\n"
									"(but it tolerates) the S0 and S5 labels.\n\n"

									"Input format:  sloader {{-run | -norun}, {-sdcard sector}}\n"
									"Output format: [result]\n\n";

#else
// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sloader      Motorola S1-9, S2-8, S3-7 loader.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Motorola S1-9, S2-8, S3-7 loader\n"
									"================================\n\n"

									"This tool downloads an executable code.\n"
									"The implementation of this loader does not support\n"
									"(but it tolerates) the S0 and S5 labels.\n\n"

									"Input format:  sloader {-run | -norun}\n"
									"Output format: [result]\n\n";
#endif

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	int32_t	_getCounter(uint8_t *counter, uint8_t *checksum);
static	int32_t	_getAddress(uint8_t **address, uint8_t *counter, uint8_t *checksum, uint8_t sType);
static	int32_t	_getData(uint8_t *counter, uint8_t *checksum, uint8_t *address, uint32_t *size);
static	int32_t	_getHexValue(uint8_t *hexValue);
static	int32_t	_getByte(uint8_t *byte);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Sloader, KIDTOOL, KSLOADERNUM, prgm, "3.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	uint8_t		*address = NULL, byte = 0, checksum, counter;
	int32_t		error = KSLOANOERR, (*code)(void);
	bool_t		terminate = FALSE, equals;
	uint8_t		run = KRUN;
	uint32_t	size = 0;

	#if (defined(__MAN_USDC__))
	bool_t		sdcard = FALSE;
	uint32_t	firstSector = 0, nbSectors;
	#endif

	RESERVE_COMM(KSYST, KDEVREAD);

	iotx_printf(KSYST, "S format Motorola loader mode; waiting for the code.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// Verify if the user memory is available
// --------------------------------------

	if (syos_reserve(KDEVALL) != KSYOSNOERR) {
		terminate = TRUE;
		error = KSLOAUMBSY;
	}

// Analyze the command line
// ------------------------
//
// Example:
//
// sloader
// sloader -run
// sloader -norun
// sloader -sdcard sectorNumber (it is also -norun)

	switch (argc) {
		case 1: {
			run = KRUN;
			break;
		}
		case 2: {
			text_checkAsciiBuffer(argv[1], "-norun",    &equals); if (equals) { run = KNORUN; 						   break; }
			text_checkAsciiBuffer(argv[1], "-run",      &equals); if (equals) { run = KRUN;   						   break; }
			error = KSLOAWRARG;
			terminate = TRUE;
			break;
		}

		#if (defined(__MAN_USDC__))
		case 3: {
			text_checkAsciiBuffer(argv[1], "-sdcard",   &equals); if (equals) { run = KNORUN; sdcard = TRUE;                  }
																  else        { error  = KSLOAWRARG; terminate = TRUE; break; }
			strg_cnvtDecAsciiToBufInt32((int32_t *)&firstSector, argv[2], 1);
			break;
		}
		#endif

		default: {
			error = KSLOAWRARG;
			terminate = TRUE;
			break;
		}
	}

	while (!terminate) {

// Waiting for a header S1, S2, S3 or S7, S8, S9

		do { _getByte(&byte); } while (byte != 'S');
		_getByte(&byte);

		switch (byte) {
			case '1':
			case '2':
			case '3': {
				error = _getCounter(&counter, &checksum);                 if (error != KSLOANOERR) { terminate = TRUE; break; }
				error = _getAddress(&address, &counter, &checksum, byte); if (error != KSLOANOERR) { terminate = TRUE; break; }
				error = _getData(&counter, &checksum, address, &size);    if (error != KSLOANOERR) { terminate = TRUE; break; }
				break;
			}
			case '7':
			case '8':
			case '9': {
				error = _getCounter(&counter, &checksum);                 if (error != KSLOANOERR) { terminate = TRUE; break; }
				error = _getAddress(&address, &counter, &checksum, byte); if (error != KSLOANOERR) { terminate = TRUE; break; }
				error = _getData(&counter, &checksum, address, &size); 	  if (error != KSLOANOERR) { terminate = TRUE; break; }
				terminate = TRUE;
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
	syos_release(KDEVALL);

	switch (error) {
		case KSLOANOERR: {
			iotx_printf(KSYST, "\nS: download terminated.\n\n");
			syos_setDoLoCode(address);
			syos_getDoLoCode((void *)&code);
			if (run == KRUN) {
																								return ((*code)());
			}
 
 			#if (defined(__MAN_USDC__))
 			if (sdcard != TRUE)	{
 																								return (EXIT_SUCCESS_OS);
 			}

// Write the data in the SDCard
// ----------------------------

			RESERVE(USDC, KDEVALL);
			if (usdc_writeSDCard((uint8_t *)&_stUMemo, size, firstSector) != KUSDCNOERR) {
				RELEASE(USDC, KDEVALL);
				iotx_printf(KSYST, "\nS: SDCard error.\n\n");
																								return (EXIT_FAILURE);
			}


			nbSectors = (size - 1 + KSZFATSECT) / KSZFATSECT;
			RELEASE(USDC, KDEVALL);
			iotx_printf(KSYST, "\nS: SDCard written. Size = %d, "
							   "First sector = %d, "
							   "Nb. Sectors = %d\n\n",
							   size,
							   firstSector,
							   nbSectors);
							   																	return (EXIT_SUCCESS_OS);
			#else
																								return (EXIT_SUCCESS_OS);
			#endif

		}
		case KSLOAUMBSY:    { iotx_printf(KSYST, "\nS: The user memory is busy.\n\n");       	return (EXIT_FAILURE); }
		case KSLOAWRARG:    { iotx_printf(KSYST, "\nS: Incorrect arguments.\n\n"); 	         	return (EXIT_FAILURE); }
		case KSLOAOSERR:    { iotx_printf(KSYST, "\nS: Download address out of memory.\n\n");	return (EXIT_FAILURE); }
		case KSLOACKERR:    { iotx_printf(KSYST, "\nS: wrong checksum.\n\n");      	         	return (EXIT_FAILURE); }
		case KSLOARBFUL:    { iotx_printf(KSYST, "\nS: buffer full error.\n\n");   	         	return (EXIT_FAILURE); }
		case KSLOAERNOI:    { iotx_printf(KSYST, "\nS: noise error.\n\n");         	         	return (EXIT_FAILURE); }
		case KSLOAERFRA:    { iotx_printf(KSYST, "\nS: framing error.\n\n");       	         	return (EXIT_FAILURE); }
		case KSLOAERPAR:    { iotx_printf(KSYST, "\nS: parity error.\n\n");        	         	return (EXIT_FAILURE); }
		default:	        {														         	return (EXIT_FAILURE); }
	}
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
				case KCOMMRBFUL: { return (KSLOARBFUL); }
				case KCOMMERNOI: { return (KSLOAERNOI); }
				case KCOMMERFRA: { return (KSLOAERFRA); }
				case KCOMMERPAR: { return (KSLOAERPAR); }
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
	return (KSLOANOERR);
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
	if (status != KSLOANOERR) {
		return (status);
	}

	if       ((byte >= '0') && (byte <= '9'))									   { *value  = (tabAB[byte             - '0']<<4); }
	else if (((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'))) { *value  = (tabAB[(byte & (~0x20)) - '0']<<4); }

	status = _getByte(&byte);  if (status != KSLOANOERR) { return (status); }
	if       ((byte >= '0') && (byte <= '9'))									   { *value += (tabAB[byte             - '0']);    }
	else if (((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'))) { *value += (tabAB[(byte & (~0x20)) - '0']);    }
	return (KSLOANOERR);
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
	if (status != KSLOANOERR) {
		return (status);
	}

	*checksum = *counter;
	return (KSLOANOERR);
}

/*
 * \brief _getAddress
 *
 * - Read the address field
 *
 */
static	int32_t	_getAddress(uint8_t **address, uint8_t *counter, uint8_t *checksum, uint8_t sType) {
	uint8_t		i, wide = 2, hexValue;
	int32_t		status;
	uint32_t	stSegment, enSegment, offset = 0;

	stSegment = (uint32_t)&_stUMemo;
	enSegment = (uint32_t)&_stUMemo + (uint32_t)&_lnUMemo;

	switch (sType) {
		case '1':
		case '9': { wide = 2; break; }
		case '2':
		case '8': { wide = 3; break; }
		case '3':
		case '7': { wide = 4; break; }
		default: {
			break;
		}
	}

// Build the address 16, 24 or 32-bit wide

	for (i = 0; i < wide; i++) {
		status = _getHexValue(&hexValue);
		if (status != KSLOANOERR) {
			return (status);
		}

		offset = (offset<<8) + (uint32_t)hexValue;
		*checksum += hexValue;
		(*counter)--;
	}

// Verify the "offset" is inside the UMemo 
// - stSegment <= offset <= enSegment -> address = offset
// - stSegment >  offset >  enSegment -> address = stSegment + offset

	if ((offset >= stSegment) && (offset <= enSegment)) *address = (uint8_t *)offset;
	else 											    *address = (uint8_t *)(stSegment + offset);

	if (((uint32_t)*address < stSegment) || ((uint32_t)*address > enSegment)) {
		return (KSLOAOSERR);
	}
	
	return (KSLOANOERR);
}

/*
 * \brief _getData
 *
 * - Read the data field
 *
 */
static	int32_t	_getData(uint8_t *counter, uint8_t *checksum, uint8_t *address, uint32_t *size) {
	uint8_t		i, notChecksum, hexValue, *load;
	int32_t		status;

	load = address;
	for (i = 0; i < (*counter-1); i++) {
		status = _getHexValue(&hexValue);
		if (status != KSLOANOERR) {
			return (status);
		}

		*load++ = hexValue;
		*size = *size + 1;
		*checksum += hexValue;
	}

	status = _getHexValue(&hexValue);
	if (status != KSLOANOERR) {
		return (status);
	}

	notChecksum = ~(*checksum);
	if (hexValue != notChecksum) {
		return (KSLOACKERR);
	}

	return (KSLOANOERR);
}

