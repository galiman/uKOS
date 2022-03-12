/*
; strg.
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
; Goal:		strg manager.
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
#include 	<math.h>
#include 	<float.h>

#define	KNBDIGIT	30		// Number of digits

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"strg         strg manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "strg manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	int32_t		_cnvtDASCIIInt32(const char_t *number);
static	int32_t		_cnvtHASCIIInt32(const char_t *number);
static	char_t		*_cnvtInt32HASCII(int32_t binary, char_t *buffer, uint8_t nbDigits);
static	char_t		*_cnvtInt64DASCII(int64_t binary, char_t *buffer);
static	char_t		*_cnvtInt64HASCII(int64_t binary, char_t *buffer, uint8_t nbDigits);

#if (defined(__STRG_FULL__))
static	char_t		*_cnvtInt32DASCII(int32_t binary, char_t *buffer);
static	float64_t	_cnvtDASCIIFloat64(const char_t *number);
#endif

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Strg, KIDTBOX, KSTRGNUM, NULL, "2.1", (1<<BSHOW));

/*
 * \brief Get the size of an ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * const       char_t    ascii[] = ”This is a string ...\n\r”;
 * int32_t     status;
 * uint32_t    size;
 *
 *    status = strg_getBufferSize(ascii, &size);
 * \endcode
 *
 * - ASCII input buffer:
 * - (ASCII) -2345,2345,+6789CRLF0	out: (.32) 18
 *   - The \0 character is not counted
 *
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KSTRGNOERR	OK
 *
 */
int32_t	strg_getBufferSize(const char_t *ascii, uint32_t *size) {

	*size = 0;
	while (*ascii++ != '\0') {
		*size += 1;
	}
	return (KSTRGNOERR);
}

/*
 * \brief Skip parameters from a formatted ASCII buffer (the separator is ',')
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     ascii[] = ”R,232,234,454,67567,7678\n\r”;
 *
 *    status = strg_skipProtocolParam(ascii, 3);
 * \endcode
 *
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 * \param[out]	KSTRGNOPRM		The number of parameters is = 0
 * \param[out]	KSTRGTMPRM		Too many parameters to skip
 *
 */
int32_t	strg_skipProtocolParam(char_t *ascii, uint16_t nbParameters) {
	uint16_t	i;
	char_t 		*wkAscii;
	bool_t		found;

	if (nbParameters == 0) { return (KSTRGNOPRM); }

// Find the parameters

	wkAscii = ascii;
	for (i = 0; i < nbParameters; i++) {
		found = FALSE;
		do {
			switch (*wkAscii++) {
				case '\0': {
					return (KSTRGTMPRM);
				}
				case ',' : {
					found = TRUE;
					break;
				}
				default: {
					break;
				}
			}
		} while (!found);
	}

// Skip them

	do {
		*ascii++ = *wkAscii;
	} while (*wkAscii++ != '\0');
	return (KSTRGNOERR);
}

/*
 * \brief Convert a formatted decimal ASCII buffer to a int32_t binary array
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * const      char_t    ascii[] = ”-2345,2345,+6789\n\r”;
 * int32_t    status;
 * int32_t    binary[KSIZE];
 *
 *    status = strg_cnvtDecAsciiToBufInt32(binary, ascii, 3);
 * \endcode
 *
 * - ASCII input buffer:			int32_t output array:
 * - (ASCII) -2345,2345,+6789CRLF0	(.32) -0x929 0x929 0x1A85
 *
 * \param[in]	*binary			Ptr on the binary buffer
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 * \param[out]	KSTRGNOPRM		The number of parameters is = 0
 * \param[out]	KSTRGNOASC		The parameter is not an ASCII value
 * \param[out]	KSTRGTODIG		The parameter has too many digits
 *
 */
int32_t	strg_cnvtDecAsciiToBufInt32(int32_t *binary, const char_t *ascii, uint16_t nbParameters) {
	uint16_t	i, j = 0, k;
	char_t		number[KNBDIGIT];
	bool_t		processed;

	if (nbParameters == 0) { return (KSTRGNOPRM); }

// Build the array

	for (i = 0; i < nbParameters; i++) {

// Build a number

		k = 0;
		processed = FALSE;
		do {
			switch (ascii[j]) {
				case '\0':
				case '\r':
				case '\n':
				case ',' :
				case ' ' : {
					number[k] = '\0';
					j++;
					binary[i] = _cnvtDASCIIInt32(number);
					processed = TRUE;
					break;
				}
				default: {

// Decimal number: ____+____-____0 1 2 3 4 5 6 7 8 9____  =>  ____ = Error

					if ((ascii[j] < '+') || (ascii[j] > '9')) { return (KSTRGNOASC); }
					if ((ascii[j] > '+') && (ascii[j] < '-')) { return (KSTRGNOASC); }
					if ((ascii[j] > '-') && (ascii[j] < '0')) { return (KSTRGNOASC); }
					if (k > KNBDIGIT)                         { return (KSTRGTODIG); }

					number[k++] = ascii[j++];
					break;
				}
			}
		} while (!processed);
	}
	return (KSTRGNOERR);
}

/*
 * \brief Convert a formatted hexadecimal ASCII buffer to a int32_t binary array
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * const      char_t    ascii[] = ”-2345,2345,+6789\n\r”;
 * int32_t    status;
 * int32_t    binary[KSIZE];
 *
 *     status = strg_cnvtHexAsciiToBufInt32(binary, ascii, 3);
 * \endcode
 *
 * - ASCII input buffer:			int32_t output array:
 * - (ASCII) -2345,2345,+1A85CRLF0	(.32) -0x2345 0x2345 0x1A85
 *
 * \param[in]	*binary			Ptr on the binary buffer
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 * \param[out]	KSTRGNOPRM		The number of parameters is = 0
 * \param[out]	KSTRGNOASC		The parameter is not an ASCII value
 * \param[out]	KSTRGTODIG		The parameter has too many digits
 *
 */
int32_t	strg_cnvtHexAsciiToBufInt32(int32_t *binary, const char_t *ascii, uint16_t nbParameters) {
	uint16_t	i, j = 0, k;
	char_t		number[KNBDIGIT];
	bool_t		processed;

	if (nbParameters == 0) { return (KSTRGNOPRM); }

// Build the array

	for (i = 0; i < nbParameters; i++) {

// Build a number

		k = 0;
		processed = FALSE;
		do {
			switch (ascii[j]) {
				case '\0':
				case '\r':
				case '\n':
				case ',' :
				case ' ' : {
					number[k] = '\0';
					j++;
					binary[i] = _cnvtHASCIIInt32(number);
					processed = TRUE;
					break;
				}
				default: {

// Hexadecimal number: ____+____-____0 1 2 3 4 5 6 7 8 9____A B C D E F____a b c d e f____  =>  ____ = Error

					if ((ascii[j] < '+') || (ascii[j] > 'f')) { return (KSTRGNOASC); }
					if ((ascii[j] > '+') && (ascii[j] < '-')) { return (KSTRGNOASC); }
					if ((ascii[j] > '-') && (ascii[j] < '0')) { return (KSTRGNOASC); }
					if ((ascii[j] > '9') && (ascii[j] < 'A')) { return (KSTRGNOASC); }
					if ((ascii[j] > 'F') && (ascii[j] < 'a')) { return (KSTRGNOASC); }
					if (k > KNBDIGIT)                         { return (KSTRGTODIG); }

					number[k++] = ascii[j++];
					break;
				}
			}
		} while (!processed);
	}
	return (KSTRGNOERR);
}

#if (defined(__STRG_FULL__))
/*
 * \brief Convert a formatted decimal ASCII buffer to a float64_t array
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    3
 *
 * const        char_t    ascii[] = ”-1e-2,2.98,+80.4e-5\n\r”;
 * int32_t      status;
 * float64_t    binary[KSIZE];
 *
 *    status = strg_cnvtDecAsciiToBufFloat64(binary, ascii, 3);
 * \endcode
 *
 * - ASCII input buffer:				float64_t output array:
 * - (ASCII) -1e-2,2.98,+80.4e-5CRLF0	-1e-2 2.98 +80.4e-5
 *
 * \param[in]	*binary			Ptr on the binary buffer
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 * \param[out]	KSTRGNOPRM		The number of parameters is = 0
 * \param[out]	KSTRGNOASC		The parameter is not an ASCII value
 * \param[out]	KSTRGTODIG		The parameter has too many digits
 *
 */
int32_t	strg_cnvtDecAsciiToBufFloat64(float64_t *binary, const char_t *ascii, uint16_t nbParameters) {
	uint16_t	i, j = 0, k;
	char_t		number[KNBDIGIT];
	bool_t		processed;

	if (nbParameters == 0) { return (KSTRGNOPRM); }

// Build the array

	for (i = 0; i < nbParameters; i++) {

// Build a number

		k = 0;
		processed = FALSE;
		do {
			switch (ascii[j]) {
				case '\0':
				case '\r':
				case '\n':
				case ',' :
				case ' ' : {
					number[k] = '\0';
					j++;
					binary[i] = (float64_t)_cnvtDASCIIFloat64(number);
					processed = TRUE;
					break;
				}
				default: {

// Decimal number: ____+____-.____0 1 2 3 4 5 6 7 8 9____E____e____  =>  ____ = Error

					if ((ascii[j] < '+') || (ascii[j] > 'e')) { return (KSTRGNOASC); }
					if ((ascii[j] > '+') && (ascii[j] < '-')) { return (KSTRGNOASC); }
					if ((ascii[j] > '.') && (ascii[j] < '0')) { return (KSTRGNOASC); }
					if ((ascii[j] > '9') && (ascii[j] < 'E')) { return (KSTRGNOASC); }
					if ((ascii[j] > 'E') && (ascii[j] < 'e')) { return (KSTRGNOASC); }
					if (k > KNBDIGIT)                         { return (KSTRGTODIG); }

					number[k++] = ascii[j++];
					break;
				}
			}
		} while (!processed);
	}
	return (KSTRGNOERR);
}
#endif

/*
 * \brief Convert a int8_t binary array to a formatted hexadecimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * const      int8_t    binary[] = { -0x1,0x2,0xA0 };
 * int32_t    status;
 * char_t     ascii[KSIZE];
 *
 *    status = strg_cnvtBufInt8ToHexAscii(ascii, binary, 3);
 * \endcode
 *
 * - int8_t input array:	ASCII output buffer:
 * - (.8) -0x1 0x2 0xA0		(ASCII) -1,2,+A0CRLF
 *
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	*binary			Ptr on the binary buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 *
 */
int32_t	strg_cnvtBufInt8ToHexAscii(char_t *ascii, const int8_t *binary, uint16_t nbParameters) {
	uint16_t	i;

	if (nbParameters == 0) {
		*ascii++ = '\n';
		*ascii++ = '\0';
		return (KSTRGNOERR);
 	}

// Build the buffer

	for (i = 0; i < nbParameters; i++) {

// Build a number

		ascii = _cnvtInt32HASCII((int32_t)binary[i], ascii, 2);
		*ascii++ = ',';
	}
	ascii--;
	*ascii++ = '\n';
	*ascii++ = '\0';
	return (KSTRGNOERR);
}

#if (defined(__STRG_FULL__))
/*
 * \brief Convert a int32_t binary array to a formatted decimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * const      int32_t    binary[] = { -0x929,0x929,0x1A85 };
 * int32_t    status;
 * char_t     ascii[KSIZE];
 *
 *    status = strg_cnvtBufInt32ToDecAscii(ascii, binary, 3);
 * \endcode
 *
 * - int32_t input array:		ASCII output buffer:
 * - (.32) -0x929 0x929 0x1A85	(ASCII) -2345,2345,+6789CRLF
 *
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	*binary			Ptr on the binary buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 *
 */
int32_t	strg_cnvtBufInt32ToDecAscii(char_t *ascii, const int32_t *binary, uint16_t nbParameters) {
	uint16_t	i;

	if (nbParameters == 0) {
		*ascii++ = '\n';
		*ascii++ = '\0';
		return (KSTRGNOERR);
 	}

// Build the buffer

	for (i = 0; i < nbParameters; i++) {

// Build a number

		ascii = _cnvtInt32DASCII((int32_t)binary[i], ascii);
		*ascii++ = ',';
	}
	ascii--;
	*ascii++ = '\n';
	*ascii++ = '\0';
	return (KSTRGNOERR);
}
#endif

#if (defined(__STRG_FULL__))
/*
 * \brief Convert a int32_t binary array to a formatted hexadecimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * const      int32_t    binary[] = { -0x929,0x929,0x1A85 };
 * int32_t    status;
 * char_t     ascii[KSIZE];
 *
 *     status = strg_cnvtBufInt32ToHexAscii(ascii, binary, 3);
 * \endcode
 *
 * - int32_t input array:			ASCII output buffer:
 * - (.32) -0x2345 0x2345 0x1A85	(ASCII) -2345,2345,+1A85CRLF
 *
 * \param[in]	*ascii			Ptr on the ASCII buffer
 * \param[in]	*binary			Ptr on the binary buffer
 * \param[in]	nbParameters	Nb. of parameters
 * \param[out]	KSTRGNOERR		OK
 *
 */
int32_t	strg_cnvtBufInt32ToHexAscii(char_t *ascii, const int32_t *binary, uint16_t nbParameters) {
	uint16_t	i;

	if (nbParameters == 0) {
		*ascii++ = '\n';
		*ascii++ = '\0';
		return (KSTRGNOERR);
 	}

// Build the buffer

	for (i = 0; i < nbParameters; i++) {

// Build a number

		ascii = _cnvtInt32HASCII((int32_t)binary[i], ascii, 8);
		*ascii++ = ',';
	}
	ascii--;
	*ascii++ = '\n';
	*ascii++ = '\0';
	return (KSTRGNOERR);
}
#endif

#if (defined(__STRG_FULL__))
/*
 * \brief Convert a int32_t binary value to a decimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * int32_t    status;
 * int32_t    value = -233;
 * char_t     ascii[KSIZE], *wkAscii;
 *
 *    status = strg_cnvtValInt32ToDecAscii(ascii, &value, &wkAscii);
 * \endcode
 *
 * - int32_t input value:	ASCII output buffer:
 * - (.32) -0x929			(ASCII) -2345
 *
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	*binary		Ptr on the binary value
 * \param[in]	**buffer	Ptr on the first available position of the ASCII buffer
 * \param[out]	KSTRGNOERR	OK
 *
 */
int32_t	strg_cnvtValInt32ToDecAscii(char_t *ascii, const int32_t *binary, char_t **buffer) {

	*buffer = (_cnvtInt32DASCII(*binary, ascii));
	**buffer = '\0';
	return (KSTRGNOERR);
}
#endif

#if (defined(__STRG_FULL__))
/*
 * \brief Convert a int32_t binary value to a hexadecimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * int32_t    status;
 * int32_t    value = -233;
 * char_t     ascii[KSIZE], *wkAscii;
 *
 *    status = strg_cnvtValInt32ToHexAscii(ascii, &value, &wkAscii);
 * \endcode
 *
 * - int32_t input value:	ASCII output buffer:
 * - (.32) -0x92A			(ASCII) -0x92A
 *
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	*binary		Ptr on the binary value
 * \param[in]	**buffer	Ptr on the first available position of the ASCII buffer
 * \param[out]	KSTRGNOERR	OK
 *
 */
int32_t	strg_cnvtValInt32ToHexAscii(char_t *ascii, const int32_t *binary, char_t **buffer) {

	*buffer = (_cnvtInt32HASCII(*binary, ascii, 8));
	**buffer = '\0';
	return (KSTRGNOERR);
}
#endif

/*
 * \brief Convert a int64_t binary value to a decimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * int32_t    status;
 * int64_t    value = -233;
 * char_t     ascii[KSIZE], *wkAscii;
 *
 *    status = strg_cnvtValInt64ToDecAscii(ascii, &value, &wkAscii);
 * \endcode
 *
 * - int64_t input value:	ASCII output buffer:
 * - (.64) -0x929			(ASCII) -2345
 *
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	*binary		Ptr on the binary value
 * \param[in]	**buffer	Ptr on the first available position of the ASCII buffer
 * \param[out]	KSTRGNOERR	OK
 *
 */
int32_t	strg_cnvtValInt64ToDecAscii(char_t *ascii, const int64_t *binary, char_t **buffer) {

	*buffer = (_cnvtInt64DASCII(*binary, ascii));
	**buffer = '\0';
	return (KSTRGNOERR);
}

/*
 * \brief Convert a int64_t binary value to a hexadecimal ASCII buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * int32_t    status;
 * int64_t    value = -233;
 * char_t     ascii[KSIZE], *wkAscii;
 *
 *    status = strg_cnvtValInt64ToHexAscii(ascii, &value, &wkAscii);
 * \endcode
 *
 * - int64_t input value:	ASCII output buffer:
 * - (.646) -0x92A			(ASCII) -0x92A
 *
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	*binary		Ptr on the binary value
 * \param[in]	**buffer	Ptr on the first available position of the ASCII buffer
 * \param[out]	KSTRGNOERR	OK
 *
 */
int32_t	strg_cnvtValInt64ToHexAscii(char_t *ascii, const int64_t *binary, char_t **buffer) {

	*buffer = (_cnvtInt64HASCII(*binary, ascii, 16));
	**buffer = '\0';
	return (KSTRGNOERR);
}

// Local routines
// ==============

/*
 * \brief _cnvtDASCIIInt32
 *
 * - Conversion from a decimal ASCII buffer to a int32_t value
 *
 */
static	int32_t	_cnvtDASCIIInt32(const char_t *number) {
	int32_t		value = 0;
	bool_t		negative = FALSE;

	while (*number == ' ') {
		number++;
	}
	switch (*number) {
		case '-': {
			negative = TRUE;
			number++;
			break;
		}
		case '+': {
			negative = FALSE;
			number++;
			break;
		}
		default: {
			break;
		}
	}

	while (*number != '\0') {
		value = (value * 10) + (*number++ - '0');
	}
	value = (negative) ? (-value) : (value);
	return (value);
}

/*
 * \brief _cnvtHASCIIInt32
 *
 * - Conversion from a hexadecimal ASCII buffer to a int32_t value
 *
 */
static	int32_t	_cnvtHASCIIInt32(const char_t *number) {
			int32_t		value = 0;
			bool_t		negative = FALSE;
	const	uint32_t	tabAB[] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15 };

	while (*number == ' ') {
		number++;
	}

	switch (*number) {
		case '-': {
			negative = TRUE;
			number++;
			break;
		}
		case '+': {
			negative = FALSE;
			number++;
			break;
		}
		default: {
			break;
		}
	}

	while (*number != '\0') {
		if       ((*number >= '0') && (*number <= '9')) 										   { value = (value * 16) + tabAB[(*number++             - '0')]; }
		else if (((*number >= 'A') && (*number <= 'F')) || ((*number >= 'a') && (*number <= 'f'))) { value = (value * 16) + tabAB[((*number++ & (~0x20)) - '0')]; }
		     else                                                                                  { value = (value * 16); 										  }
	}
	value = (negative) ? (-value): (value);
	return (value);
}

#if (defined(__STRG_FULL__))
/*
 * \brief _cnvtDASCIIFloat64
 *
 * - Conversion from a decimal ASCII buffer to a float64_t value
 *
 */
static	float64_t	_cnvtDASCIIFloat64(const char_t *number) {
	bool_t		negative = FALSE;
	uint16_t	nbDigits = 0;
	int16_t		exponent = 0, exp = 0;
	float64_t	mantissa = 0.0, value = HUGE_VAL, pow10 = 10.0;

	while (*number == ' ') {
		number++;
	}
	switch (*number) {
		case '-': {
			negative = TRUE;
			number++;
			break;
		}
		case '+': {
			negative = FALSE;
			number++;
			break;
		}
		default: {
			break;
		}
	}

// Process the mantissa
// xxxx.yyyyy --> mantissa = xxxyyyyy with exponent = -5

	while ((*number >= '0') && (*number <= '9')) {
		mantissa = (mantissa * 10.0) + (*number++ - '0');
		nbDigits++;
	}

	if (*number == '.') {
		number++;
		while ((*number >= '0') && (*number <= '9')) {
			mantissa = (mantissa * 10.0) + (*number++ - '0');
			nbDigits++;
			exponent--;
		}
	}

// Correct for the sign of the mantissa

	if (negative) mantissa = -mantissa;

// Process the exponent
// xxxx.yyyyyEsexp --> xxxxyyyyyEsrrr with rrr = exponent-exp

	if (*number == 'e' || *number == 'E') {
		negative = FALSE;
		switch (*++number) {
			case '-': {
				negative = TRUE;
				number++;
				break;
			}
			case '+': {
				negative = FALSE;
				number++;
				break;
			}
			default: {
				break;
			}
		}

// Process string of digits

		while ((*number >= '0') && (*number <= '9')) {
			exp = (exp * 10) + (*number++ - '0');
		}

		if (negative) {
			exponent -= exp;
		}
		else {
			exponent += exp;
		}
	}

// Scale the result accordingly to the new exponent

	if ((exponent >= DBL_MIN_EXP) && (exponent <= DBL_MAX_EXP)) {
		value = mantissa;
		exp   = exponent;
		if (exp < 0) {
			exp = -exp;
		}
		while (exp) {
			if (exp & 0x0001) {
				if (exponent < 0) {
					value /= pow10;
				}
				else {
					value *= pow10;
				}
			}
    		exp >>= 1;
    		pow10 *= pow10;
		}
	}
	return (value);
}
#endif

#if (defined(__STRG_FULL__))
/*
 * \brief _cnvtInt32DASCII
 *
 * - Conversion from a int32_t value to a decimal ASCII buffer
 *
 */
static	char_t	*_cnvtInt32DASCII(int32_t binary, char_t *ascii) {
			uint8_t		i, digit;
			bool_t		zero = TRUE;
	const	int32_t		tabDF[] = {
							1000000000,
							100000000,
							10000000,
							1000000,
							100000,
							10000,
							1000,
							100,
							10,
							1
						};

	if (binary < 0) {
		*ascii++ = '-';
		binary = -binary;
	}

// Process only positive numbers of max. 10 digits

	for (i = 0; i < 10; i++) {
		digit = 0;
		do {
			digit++;
			binary -= tabDF[i];
		} while (binary >= 0);
		digit--;
		binary += tabDF[i];
		if (digit != 0) {
			zero = FALSE;
		}
		if (!zero) {
			*ascii++ = (char_t)digit + '0';
		}
	}
	if (zero) {
		*ascii++ = '0';
	}
	return (ascii);
}
#endif

/*
 * \brief _cnvtInt32HASCII
 *
 * - Conversion from a int32_t value to a hexadecimal ASCII buffer
 *
 */
static	char_t	*_cnvtInt32HASCII(int32_t binary, char_t *ascii, uint8_t nbDigits) {
			uint8_t		i, number;
	const	char_t		tabHX[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

	for (i = 0; i < nbDigits; i++) {
		number = (uint8_t)binary & 0xF;
		binary >>= 4;
		ascii[nbDigits-1-i] = tabHX[number];
	}
	return (ascii + nbDigits);
}

/*
 * \brief _cnvtInt64DASCII
 *
 * - Conversion from a int64_t value to a decimal ASCII buffer
 *
 */
static	char_t	*_cnvtInt64DASCII(int64_t binary, char_t *ascii) {
			uint8_t		i, digit;
			bool_t		zero = TRUE;
	const	int64_t		tabDF[] = {
							1000000000000000000LL,
							100000000000000000LL,
							10000000000000000LL,
							1000000000000000LL,
							100000000000000LL,
							10000000000000LL,
							1000000000000LL,
							100000000000LL,
							10000000000LL,
							1000000000LL,
							100000000LL,
							10000000LL,
							1000000LL,
							100000LL,
							10000LL,
							1000LL,
							100LL,
							10LL,
							1LL
						};

	if (binary < 0) {
		*ascii++ = '-';
		binary = -binary;
	}

// Process only positive numbers of max. 19 digits (-9223372036854775807..9223372036854775807)

	for (i = 0; i < 19; i++) {
		digit = 0;
		do {
			digit++;
			binary -= tabDF[i];
		} while (binary >= 0);
		digit--;
		binary += tabDF[i];
		if (digit != 0) {
			zero = FALSE;
		}
		if (!zero) {
			*ascii++ = (char_t)digit + '0';
		}
	}
	if (zero) {
		*ascii++ = '0';
	}
	return (ascii);
}

/*
 * \brief Conversion from a int64_t value to a hexadecimal ASCII buffer
 *
 */
static	char_t	*_cnvtInt64HASCII(int64_t binary, char_t *ascii, uint8_t nbDigits) {
			uint8_t		i, number;
	const	char_t		tabHX[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

	for (i = 0; i < nbDigits; i++) {
		number = (uint8_t)binary & 0xF;
		binary >>= 4;
		ascii[nbDigits-1-i] = tabHX[number];
	}
	return (ascii + nbDigits);
}

