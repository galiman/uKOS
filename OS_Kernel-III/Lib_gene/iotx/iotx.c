/*
; iotx.
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
; Goal:		iotx manager.
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

#define	BRIGHT		0			// For paddering
#define	BZERO		1			// For paddering
#define KLNLINBUF	(1+20+2+1)	// place for a 64-bit number; "sign" + "20 digits" + "CR\0"
#define KLNOUTBUF	80			// Length of the output buffer

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"iotx         iotx manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "iotx manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_waitOrder(uint32_t commManager, char_t *ascii, uint32_t size);
static	void	_getChar(uint32_t commManager, char_t *c);
static	int32_t	_print(uint32_t commManager, int32_t *arg, char_t *output, uint8_t *index);
static	int32_t	_printS(uint32_t commManager, const char_t *string, int32_t width, uint8_t pad, char_t *output, uint8_t *index);
static	int32_t	_printI(uint32_t commManager, int64_t value, uint8_t dynamic, int8_t base, bool_t signe, int32_t width, uint8_t pad, char_t radix, char_t *output, uint8_t *index);
static	void	_printBuff(uint32_t commManager, char_t c, char_t *output, uint8_t *index);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Iotx, KIDGENE, KIOTXNUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Waiting for an ASCII string from a communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * int32_t    status;
 * char_t     ascii[KSIZE];
 * 
 *    status = iotx_waitString(KDEF0, ascii, KSIZE);
 * \endcode
 *
 * - Format of the order:
 *   - string,CR,LF a \0 char is added at the end
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	size		Size of the ASCII buffer
 * \param[out]	KIOTXNOERR	OK
 *
 */
int32_t	iotx_waitString(uint32_t commManager, char_t *ascii, uint32_t size) {
				uint32_t	manager;
	volatile	proc_t 		*process;

	switch (commManager) {

// KNOTR: use the default communication manager without to reserve it

		case KNOTR: {
			_waitOrder(KDEF0, ascii, size);
			break;
		}

// KSYS: use the process specified communication manager with its reservation

		case KSYST: {
			kern_getProcessRun(&process);
			manager = process->oSpecification.oCommManager;

			RESERVE_COMM(manager, KDEVREAD);
			_waitOrder(manager, ascii, size);
			RELEASE_COMM(manager, KDEVREAD);

			break;
		}

// KXXX: use the specified communication manager with its reservation

		default: {
			manager = commManager;

			RESERVE_COMM(manager, KDEVREAD);
			_waitOrder(manager, ascii, size);
			RELEASE_COMM(manager, KDEVREAD);

			break;
		}
	}
	return (KIOTXNOERR);
}

/*
 * \brief Small printf
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    number;
 * 
 *    number = iotx_printf(KSYST, “the lazy fox ...\n”);
 * \endcode
 *
 * - Format supported:
 *   - %l.. %d, %s, %c, %x, %X, %u
 *
 * - char_t *ptr = "Hello world!";
 * - char_t *np = 0;
 * - int32_t i = 5;
 * - int64_t j = 5LL;
 * - uint32_t bs  = sizeof(int32_t)*8;
 * - uint64_t lbs = sizeof(int64_t)*8;
 * - int32_t mi;
 * - int64_t lmi;
 * - char_t buf[80];
 *
 * - mi  = (1   << (bs-1))  + 1;
 * - lmi = (1LL << (lbs-1)) + 1LL;
 * - iotx_printf(KSYST, "%s\n", ptr);												//  Hello world!
 * - iotx_printf(KSYST, "printf test\n");											//  printf test
 * - iotx_printf(KSYST, "%s is null pointer\n", np);								//  (null) is null pointer
 * - iotx_printf(KSYST, "%d = 5\n", i);												//  5 = 5
 * - iotx_printf(KSYST, "%ld = 5\n", j);											//  5 = 5
 * - iotx_printf(KSYST, "%d = - max int\n", mi);									//  -2147483647 = - max int32
 * - iotx_printf(KSYST, "%ld = - max int\n", lmi);									//  -9223372036854775807 = - max int64
 * - iotx_printf(KSYST, "char %c = 'a'\n", 'a');									//  char a = 'a'
 * - iotx_printf(KSYST, "hex %x = ff\n", 0xff);										//  hex ff = ff
 * - iotx_printf(KSYST, "hex %02x = 00\n", 0);										//  hex 00 = 00
 * - iotx_printf(KSYST, "signed %d = unsigned %u = hex %x\n", -3, -3, -3);			//  signed -3 = unsigned 4294967293 = hex fffffffd
 * - iotx_printf(KSYST, "signed %ld = unsigned %lu = hex %lx\n", -3LL, -3LL, -3LL);	//  signed -3 = unsigned 18446744073709551613 = hex fffffffffffffffd
 * - iotx_printf(KSYST, "%d %s(s)%", 0, "message");									//  0 message(s)
 * - iotx_printf(KSYST, "\n");														// 
 * - iotx_printf(KSYST, "%d %s(s) with %%\n", 0, "message");						//  0 message(s) with %
 * - iotx_printf(KSYST, "justif: \"%-10s\"\n", "left");								//  justif: "left      "
 * - iotx_printf(KSYST, "justif: \"%10s\"\n", "right");								//  justif: "     right"
 * - iotx_printf(KSYST, " 3: %04d zero padded\n", 3);								//   3: 0003 zero padded
 * - iotx_printf(KSYST, " 3: %-4d left justif.\n", 3);								//   3: 3    left justif.
 * - iotx_printf(KSYST, " 3: %4d right justif.\n", 3);								//   3:    3 right justif.
 * - iotx_printf(KSYST, "-3: %04d zero padded\n", -3);								//  -3: -003 zero padded
 * - iotx_printf(KSYST, "-3: %-4d left justif.\n", -3);								//  -3: -3   left justif.
 * - iotx_printf(KSYST, "-3: %4d right justif.\n", -3);								//  -3:   -3 right justif.
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*format		Printf format
 * \param[out]	number		Number of printed characters
 *
 */
int32_t	iotx_printf(uint32_t commManager, const char_t *format, ...) {
				char_t		output[KLNOUTBUF+2];
				uint8_t		index = 0;
				int32_t		*arg, nbPrintChars;
				uint32_t	manager;
	volatile	proc_t 		*process;

	arg = (int32_t *)&format;
	switch (commManager) {

// KNOTR: use the default communication manager without to reserve it

		case KNOTR: {
			nbPrintChars = _print(KDEF0, arg, output, &index);
			break;
		}

// KSYS: use the process specified communication manager with its reservation

		case KSYST: {
			kern_getProcessRun(&process);
			manager = process->oSpecification.oCommManager;

			RESERVE_COMM(manager, KDEVWRITE);
			nbPrintChars = _print(manager, arg, output, &index);
			RELEASE_COMM(manager, KDEVWRITE);

			break;
		}

// KXXX: use the specified communication manager with its reservation

		default: {
			manager = commManager;

			RESERVE_COMM(manager, KDEVWRITE);
			nbPrintChars = _print(manager, arg, output, &index);
			RELEASE_COMM(manager, KDEVWRITE);

			break;
		}
	}
	return (nbPrintChars);
}

// Local routines
// ==============

/*
 * \brief _waitOrder
 *
 */
static	void	_waitOrder(uint32_t commManager, char_t *ascii, uint32_t size) {
	bool_t		terminate = FALSE;
	char_t		*wkAscii, aChar;
	uint32_t	nbChars = 0;

// Waiting for a new string; first char != CR, LF or BS

	wkAscii = ascii;
	do {
		kern_switchFast();
		_getChar(commManager, &aChar);
		nbChars++;
		switch (aChar) {

// The char is a CR or LF

			case '\n':
			case '\r': {
				if (nbChars == 1) {
					wkAscii = ascii;
					nbChars = 0;
				}
				else {
					*wkAscii++ = aChar;
					*wkAscii++ = '\0';
					nbChars--;
					terminate = TRUE;
				}
				break;
			}

// The char is a back space

			case '\b': {
				if (nbChars == 1) {
					wkAscii = ascii;
					nbChars = 0;
				}
				else {
					wkAscii--;
					nbChars--;
				}
				break;
			}

// ... any other chars
// Maintain 2 byte left in the buffer for the \n & \0

			default: {
				if (nbChars <= (size - 2)) {
					*wkAscii++ = aChar;
				}
				break;
			}
		}
	} while (!terminate);
}

/*
 * \brief _getChar
 *
 */
static	void	_getChar(uint32_t commManager, char_t *c) {
				uint32_t	size;
				char_t		*identifier;
	volatile	sema_t		*semaphore;

	switch (commManager & 0xFFFFFF00) {

// The commManager is a URTx

		case (('u'<<24) | ('r'<<16) | ('t'<<8) | 0x0): {
			comm_getIdSemaphore(commManager, KSEMARX, &identifier);
			kern_getSemaphoreById((const char_t *)identifier, &semaphore);
			kern_waitSemaphore(semaphore, 1, -1);

			size = 1;
			comm_read(commManager, (uint8_t *)c, &size);
			break;
		}

// The commManager is a USBx
// The commManager is a BLTx
// ... or any other managers

		default:
		case (('u'<<24) | ('s'<<16) | ('b'<<8) | 0x0):
		case (('b'<<24) | ('l'<<16) | ('t'<<8) | 0x0): {
			while (TRUE) {
				size = 1;
				if (comm_read(commManager, (uint8_t *)c, &size) == KCOMMNOERR) {
					return;
				}

				kern_suspendProcess(2);
			}
			break;
		}
	}
}

/*
 * \brief _print
 *
 */
static	int32_t	_print(uint32_t commManager, int32_t *arg, char_t *output, uint8_t *index) {
	uint8_t		pad = 0, dynamic;
	int32_t 	width = 0, number = 0, nbArg = 0;
	int64_t		value = 0, msw, lsw;
	char_t		*format, *s, c[2];
	bool_t		skip, processed;

//                            % group      % group
//                              ---        -------
// iotx_printf(manager, "abcd = %8d efgh = %-0005X\n", val1, val2);
//                       ^                             ^
//                       format                        arg

	format = (char_t *)(*arg++);
	while ((*format != '\0')) {

		switch (*format) {

// Found a '%', continue to analyze the line
// Process the % group

			case '%': {
				skip  = FALSE; processed = FALSE;
				width = 0; pad = 0; dynamic = 32;
				nbArg++; format++;
				while ((*format != '\0') && (!processed)) {

					switch (*format) {
	
// Found a '-'
// I.e. %-4d left justif.

						case '-': {
							pad = (1<<BRIGHT);
							format++;
							break;
						}

// Found a '0'; skip all the initial '0'
// I.e. !skip -> %00004d zero padded
// I.e.  skip -> %4d right justif.

						case '0': {
							if (!skip) { pad |= (1<<BZERO);                  }
							else       { width = (width*10) + *format - '0'; }
							format++;
							break;
						}

// Found a '1' .. '9'; build a width
// I.e. %4d right justif.

						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9': {
							skip = TRUE;
							width = (width*10) + *format - '0';
							format++;
							break;
						}

// Found a 's'; print a string

						case 's': {		
							s = *((char_t **)arg++);
							processed = TRUE;
							number += _printS(commManager, (s != NULL) ? s : "(null)", width, pad, output, index);
							format++;
							break;
						}

// Found a 'c'; print a char

						case 'c': {
							c[0] = *arg++;
							c[1] = '\0';
							processed = TRUE;
							number += _printS(commManager, c, width, pad, output, index);
							format++;
							break;
						}

// Found a 'l'
// Case of a 64-bit value. Be sure that the argument is even
// The order of the pointers can change accordingly to the gcc version

						case 'l': {
							dynamic = 64;

							#if (defined(__CORTEX_M3__) || defined(__CORTEX_M4__) || defined(__CORTEX_M7__))
							if ((nbArg & 0x01) == 0) { arg++; }
							nbArg = 0;

							lsw = ((int64_t)*arg++) & 0x00000000FFFFFFFF;
							msw = ((int64_t)*arg++)<<32;
				
							#else
							msw = ((int64_t)*arg++)<<32;
							lsw = ((int64_t)*arg++) & 0x00000000FFFFFFFF;
							#endif

							value = msw | lsw;
							format++;
							break;
						}

// Found a 'd''x''X''u'
// It could be an %ld or a %d
// It could be an %lx or a %x
// It could be an %lX or a %X
// It could be an %lu or a %u

						case 'd': {
							if (dynamic == 32) { value = (int64_t)*arg++; }
							processed = TRUE;
							number += _printI(commManager, value, dynamic, 10, TRUE,  width, pad, 'a', output, index);
							format++;
							break;
						}
						case 'x': {
							if (dynamic == 32) { value = (int64_t)*arg++; }
							processed = TRUE;
							number += _printI(commManager, value, dynamic, 16, FALSE, width, pad, 'a', output, index);
							format++;
							break;
						}
						case 'X': {
							if (dynamic == 32) { value = (int64_t)*arg++; }
							processed = TRUE;
							number += _printI(commManager, value, dynamic, 16, FALSE, width, pad, 'A', output, index);
							format++;
							break;
						}
						case 'u': {
							if (dynamic == 32) { value = (int64_t)*arg++; }
							processed = TRUE;
							number += _printI(commManager, value, dynamic, 10, FALSE, width, pad, 'a', output, index);
							format++;
							break;
						}

// Special case of the '%' (case %%)

						case '%': {
							processed = TRUE;
							_printBuff(commManager, *format, output, index);
							number++;
							format++;
							break;
						}

// Normal char, continue to print

						default: {
							_printBuff(commManager, *format, output, index);
							number++;
							format++;
							break;
						}
					}
				}
				break;
			}

// Normal char, continue to print

			default: {
				_printBuff(commManager, *format, output, index);
				number++;
				format++;
				break;
			}
		}
		_printBuff(commManager, '\0', output, index);
	}
	return (number);
}

/*
 * \brief _printS
 *
 */
static	int32_t	_printS(uint32_t commManager, const char_t *string, int32_t width, uint8_t pad, char_t *output, uint8_t *index) {
			int32_t		number = 0, length = 0;
			char_t		padChar = ' ';
	const	char_t		*ptr;

	if (width > 0) {
		for (ptr = string; *ptr != '\0'; ptr++) {
			length++;
		}
		width = (length >= width) ? 0 : (width - length);
	}
	padChar = (pad & (1<<BZERO)) ? '0' : ' ';

	if (!(pad & (1<<BRIGHT))) { for ( ; width > 0; width--)        { _printBuff(commManager, padChar, output, index); number++; } }
								for ( ; *string != '\0'; string++) { _printBuff(commManager, *string, output, index); number++; }
								for ( ; width > 0; width--)        { _printBuff(commManager, padChar, output, index); number++; }

	return (number);
}

/*
 * \brief _printI
 *
 */
static	int32_t	_printI(uint32_t commManager, int64_t value, uint8_t dynamic, int8_t base, bool_t signe, int32_t width, uint8_t pad, char_t radix, char_t *output, uint8_t *index) {
	char_t		buffer[KLNLINBUF], *s;
	uint8_t		nbDigits = 255; 
	int32_t		number   = 0;
	int64_t 	t;
	uint64_t	uValue;
	bool_t		neg = FALSE;

// Special case, value = 0

	uValue = value;
	if (value == 0) {
		buffer[0] = '0'; buffer[1] = '\0';
		number = _printS(commManager, buffer, width, pad, output, index);
		return (number);
	}

// Cases:
// Signed, base 10 negative
// Not signed, base 10
// Base 16

	if (signe  && (base == 10) && (value < 0))     { neg = TRUE; uValue = -value;          }
	if (!signe && (base == 10) && (dynamic == 32)) { uValue = uValue & 0x00000000FFFFFFFF; }
	if (          (base == 16) && (dynamic == 32)) { nbDigits = 8;                         }

	s  = buffer + KLNLINBUF - 1;
	*s = '\0';

// Build the string equivalent of the "vale"

	while (uValue && nbDigits--) {
		t = uValue % (int64_t)base;
		if (t >= 10) {
			t += (int64_t)radix - '0' - 10;
		}
		*--s = t + '0';
		uValue /= (int64_t)base;
	}

	if (neg) {
		if (width && (pad & (1<<BZERO))) { _printBuff(commManager, '-', output, index); number++; width--; }
		else  							 { *--s = '-'; 													   }
	}
	number += _printS(commManager, s, width, pad, output, index);
	return (number);
}

/*
 * \brief _printBuff
 *
 */
static	void	_printBuff(uint32_t commManager, char_t c, char_t *output, uint8_t *index) {
	int32_t		status;

	if (c != '\0') {
		output[*index] = c;
		*index += 1;
		if (*index < KLNOUTBUF) {
			return;
		}
	}
	
	if (*index != 0) {
		while (TRUE) {
			status = comm_write(commManager, (uint8_t *)output, *index);
			if (status != KCOMMNOERR) { kern_switchFast();  }
			else 					  { *index = 0; return; }
		}
	}
}

