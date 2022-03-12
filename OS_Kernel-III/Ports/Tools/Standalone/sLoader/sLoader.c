/*
; sLoader.
; ========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Motorola S1-9, S2-8, S3-7 loader.
;			This tool allows to download a S format file to the memory
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

#define	KLED0		0
#define	KLED1		1

// Module strings
// ==============

LOC_CONST_STRG(aStrSLoader[])  = "uKOS S format Motorola loader mode; waiting for the code.\n"
								 __DATE__"  "__TIME__" (c) EFr-2017\n";
LOC_CONST_STRG(aStrNoLoaded[]) = "\nS: Downloading problems.\n";
LOC_CONST_STRG(aStrErCkSum[])  = "\nS: wrong checksum.\n";
LOC_CONST_STRG(aStrSOK[])      = "\nS: Download terminated.\n\n";

extern	const char_t	aStrTarget[];

// Prototypes
// ==========

extern	void	stub_offAllLed(void);
extern	void	stub_configure(uint32_t commManager, uint32_t baudrate);
extern	void	stub_send(uint32_t commManager, const char_t *ascii);
extern	uint8_t	stub_receive(uint32_t commManager, uint8_t ledNb);
static	uint8_t	_getCounter(uint32_t commManager, uint8_t *checksum);
static	bool_t	_getData(uint32_t commManager, uint8_t *counter, uint8_t *checksum, uint8_t *address);
static	uint8_t	_getHexValue(uint32_t commManager);
static	uint8_t	*_getAddress(uint32_t commManager, uint8_t *counter, uint8_t *checksum, uint8_t sType);
static	void	_error(uint32_t commManager, const char_t *ascii);

/*
 * \brief sLoader
 *
 * Motorola S1-9, S2-8, S3-7 loader
 * - This tool allows to download a S format binary to the memory
 * - Here are the S format definitions:
 *	 - Everything is represented in ASCII
 *	 - Block format:
 *	   - Block type:			2 chars
 *	   - Length of the block:	an hex. number of 2 chars
 *	   - Address:				an hex. number of 4, 6 or 8 chars
 *	   - Data:					an hex. 2n chars
 *	   - Checksum:				an hex. number of 2 chars
 *
 *	 - Block type:
 *	   - "S1" data block: for addresses coded over 16-bits
 *	   - "S2" data block: for addresses coded over 24-bits
 *	   - "S3" data block: for addresses coded over 32-bits
 *	   - "S7" end block:  for addresses coded over 32-bits
 *	   - "S8" end block:  for addresses coded over 24-bits
 *	   - "S9" end block:  for addresses coded over 16-bits
 *	   - !!! S0 & S5 are not supported but tolerated
 *
 */
void	sLoader(uint32_t commManager, uint32_t baudrate, bool_t autoboot) {
	uint8_t		*address = 0, byte, checksum, counter;
	void		(*go)(void);

	stub_configure(commManager, baudrate);
	stub_send(commManager, aStrSLoader);
	stub_send(commManager, aStrTarget);

	while (TRUE) {

// Waiting for a header S1, S2, S3 or S7, S8, S9

		do { byte = stub_receive(commManager, KLED0); } while (byte != 'S');

		byte = stub_receive(commManager, KLED1);

		switch (byte) {
			case '1':
			case '2':
			case '3': {
				counter = _getCounter(commManager, &checksum);
				address = _getAddress(commManager, &counter, &checksum, byte);
				if (!_getData(commManager, &counter, &checksum, address)) {
					_error(commManager, aStrErCkSum);
				}
				break;
			}
			case '7':
			case '8':
			case '9': {
				counter = _getCounter(commManager, &checksum);
				address = _getAddress(commManager, &counter, &checksum, byte);
				if (!_getData(commManager, &counter, &checksum, address)) {
					_error(commManager, aStrErCkSum);
				}
				else {
					stub_send(commManager, aStrSOK);
					stub_offAllLed();
					go = address;
					if (autoboot) {
						(*go)();
					}
					else {
						return;
					}
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}

// Local routines
// ==============

/*
 * \brief _getHexValue
 *
 * - Read a hexa value (2 chars)
 * - Ex. 31 is read as '3' (0x33) and '1' (0x31) 2-Bytes
 *
 */
static	uint8_t	_getHexValue(uint32_t commManager) {
			uint8_t		byte, hexValue = 0;
	const	uint8_t		tabAB[] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15 };

	byte = stub_receive(commManager, KLED1);
	if       ((byte >= '0') && (byte <= '9'))									   { hexValue  = (tabAB[byte             - '0']<<4); }
	else if (((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'))) { hexValue  = (tabAB[(byte & (~0x20)) - '0']<<4); }

	byte = stub_receive(commManager, KLED1);
	if       ((byte >= '0') && (byte <= '9'))									   { hexValue += (tabAB[byte             - '0']);    }
	else if (((byte >= 'A') && (byte <= 'F')) || ((byte >= 'a') && (byte <= 'f'))) { hexValue += (tabAB[(byte & (~0x20)) - '0']);    }
	return (hexValue);
}

/*
 * \brief _getCounter
 *
 * - Read the counter field
 *
 */
static	uint8_t	_getCounter(uint32_t commManager, uint8_t *checksum) {
	uint8_t		hexValue;

	hexValue = _getHexValue(commManager);
	*checksum = hexValue;
	return (hexValue);
}

/*
 * \brief _getAddress
 *
 * - Read the address field
 *
 */
static	uint8_t	*_getAddress(uint32_t commManager, uint8_t *counter, uint8_t *checksum, uint8_t sType) {
	uint8_t		i, wide = 2, hexValue;
	uint32_t	offset = 0, field = 0;

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
		hexValue = _getHexValue(commManager);
		field = (field<<8) + (uint32_t)hexValue;
		*checksum += hexValue;
		(*counter)--;
	}
	return ((uint8_t *)(field + offset));
}

/*
 * \brief _getData
 *
 * - Read the data field
 *
 */
static	bool_t	_getData(uint32_t commManager, uint8_t *counter, uint8_t *checksum, uint8_t *address) {
	uint8_t		i, hexValue, *load;

	load = address;
	for (i = 0; i < (*counter-1); i++) {
		hexValue = _getHexValue(commManager);
		*load++ = hexValue;
		*checksum += hexValue;
	}
	if (_getHexValue(commManager) != (uint8_t)(~(*checksum))) {
		return (FALSE);
	}

	return (TRUE);
}

/*
 * \brief _error
 *
 * - Waiting for a reset
 *
 */
static	void	_error(uint32_t commManager, const char_t *ascii) {

	stub_offAllLed();
	stub_send(commManager, ascii);
	while (TRUE);
}

