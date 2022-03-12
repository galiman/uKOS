/*
; text.
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
; Goal:		text manager.
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

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"text         text manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "text manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Text, KIDGENE, KTEXTNUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Get the arguments from a command line
 *
 * Call example in C:
 *
 * \code{.c}
 * const    char_t      commandLine[KLNINPBUF] = ”Line: 3224.5 test 123”;
 *          char_t      *argv[KLNINPBUF];
 *          uint32_t    argc;
 *          int32_t     status;
 *
 *    status = text_getArgs(commandLine, KLNINPBUF, argv, &argc);
 *
 *    iotx_printf(KSYST, “%d\n”, argc);     // --> 4
 *    iotx_printf(KSYST, “%d\n”, argv[0]);  // --> line
 *    iotx_printf(KSYST, “%d\n”, argv[1]);  // --> 4.5
 *    iotx_printf(KSYST, “%d\n”, argv[2]);  // --> test
 *    iotx_printf(KSYST, “%d\n”, argv[3]);  // --> 123
 * \endcode
 *
 * - The char "_" is used for space!
 *   - Ex. buffer1 R________
 *         buffer2 RXYZCRLF\0
 *
 * \param[in]	*ascii		Ptr on the ASCII buffer
 * \param[in]	size		Size of the buffer
 * \param[in]	*args		Ptr on the ASCII argument buffer
 * \param[in]	*nbArgs		Ptr on the number of ASCII arguments
 * \param[out]	KTEXTNOERR	OK
 *
 */
int32_t	text_getArgs(char_t *ascii, uint32_t size, char_t *args[], uint32_t *nbArgs) {
	uint8_t		i, j = 0;
	bool_t		terminate = FALSE, start = FALSE;

// First char is \0

	if (ascii[0] == '\0') {
		*nbArgs = 0;
		return (KTEXTNOERR);
	}

// 1st pass; replace the ' ' with '\0'
// Ex. in:  abc def   werw0wer rtz rtzrz0
//     out: abc0def000werw000000000000000

	for (i = 0; i < size; i++) {
		if (terminate) { ascii[i] = '\0'; }
		else {
			if (ascii[i] == '\0') { terminate = TRUE; }
			if (ascii[i] == '\r') { ascii[i] = '\0';  }
			if (ascii[i] == '\n') { ascii[i] = '\0';  }
			if (ascii[i] == ' ' ) { ascii[i] = '\0';  }
		}
	}

// 2nd pass; determine the argument pointers
// Ex. in:  abc0def000werw000000000000000
//     out: |   |     |
//          0   1     2 --> vArg

	args[j++] = ascii; *nbArgs = 1;
	for (i = 0; i < size; i++) {
		if (ascii[i] == '\0') {
			start = TRUE;
		}
		else {
			if (start && (ascii[i] != '\0')) {
				args[j++] = (ascii + i);
				*nbArgs += 1;
				start = FALSE;
			}
		}
	}
	return (KTEXTNOERR);
}

/*
 * \brief Check if 2 ASCII buffers are identical
 *
 * Call example in C:
 *
 * \code{.c}
 * const    char_t     ascii1[] = ”This is the buffer 1”;
 * const    char_t     ascii2[] = ”This is the buffer 2”;
 *          bool_t     *equal;
 *          int32_t    status;
 *
 *    status = text_checkAsciiBuffer(ascii1, ascii2, equals);
 * \endcode
 *
 * \param[in]	*ascii1		Ptr on the ASCII buffer 1
 * \param[in]	*ascii2		Ptr on the ASCII buffer 2
 * \param[in]	*equals		The 2 ASCII buffers are identical (TRUE) or not (FALSE)
 * \param[out]	KTEXTNOERR	OK
 *
 */
int32_t	text_checkAsciiBuffer(const char_t *ascii1, const char_t *ascii2, bool_t *equals) {

	do {
		if (*ascii1++ != *ascii2++) {
			*equals = FALSE;
			return (KTEXTNOERR);
		}

	} while ((*ascii1 != ' ') && (*ascii1 != '\0'));

	switch (*ascii2) {
		case ',':
		case ' ':
		case '\n':
		case '\r':
		case '\0': {
			*equals = TRUE;
			return (KTEXTNOERR);
		}
		default: {
		}
	}
	*equals = FALSE;
	return (KTEXTNOERR);
}

/*
 * \brief Copy 2 ASCII buffers (\0 is copied)
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * const    char_t     asciiS[] = ”This is the buffer 2”;
 *          char_      asciiD[KSIZE];
 *          int32_t    status;
 *
 *    status = text_copyBufferZ(asciiD, asciiS);
 * \endcode
 *
 * - The char "_" is used for space!
 *   - Ex. buffer1 R________
 *         buffer2 RXYZCRLF\0
 *
 * \param[in]	*ascii1		Ptr on the ASCII buffer 1
 * \param[in]	*ascii2		Ptr on the ASCII buffer 2
 * \param[in]	*equals		The 2 ASCII buffers are identical (TRUE) or not (FALSE)
 * \param[out]	KTEXTNOERR	OK
 *
 */
int32_t	text_copyAsciiBufferZ(char_t *asciiD, const char_t *asciiS) {
	uint32_t	i, size;

	strg_getBufferSize(asciiS, &size);
	if (size == 0) {
		return (KTEXTNOERR);
	}

	for (i = 0; i < size; i++) {
		*asciiD++ = *asciiS++;
	}
	*asciiD++ = '\0';
	return (KTEXTNOERR);
}

/*
 * \brief Copy 2 ASCII buffers (\0 is not copied)
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    256
 *
 * const    char_t     asciiS[] = ”This is the buffer 2”;
 *          char_t     asciiD[KSIZE];
 *          int32_t    status;
 *
 *    status = text_copyBufferN(asciiD, asciiS);
 * \endcode
 *
 * \param[in]	*asciiD		Ptr on the ASCII destination buffer
 * \param[in]	*asciiS		Ptr on the ASCII source buffer
 * \param[out]	KTEXTNOERR	OK
 *
 */
int32_t	text_copyAsciiBufferN(char_t *asciiD, const char_t *asciiS) {
	uint32_t	i, size;

	strg_getBufferSize(asciiS, &size);
	if (size == 0) {
		return (KTEXTNOERR);
	}

	for (i = 0; i < size; i++) {
		*asciiD++ = *asciiS++;
	}
	return (KTEXTNOERR);
}

