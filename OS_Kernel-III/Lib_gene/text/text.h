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

#ifndef	__TEXT__
#define	__TEXT__

/*!
 * \addtogroup Lib_gene
 */
/*@{*/

#include 	<modules.h>

#define	KTEXTMAN	(KTEXTNUM<<8)
#define	KTEXTERR	((KIDGENE<<24) | KTEXTMAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

/*!
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
extern	int32_t	text_getArgs(char_t *ascii, uint32_t size, char_t *args[], uint32_t *nbArgs);

/*!
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
extern	int32_t	text_copyAsciiBufferZ(char_t *asciiD, const char_t *asciiS);

/*!
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
extern	int32_t	text_copyAsciiBufferN(char_t *asciiD, const char_t *asciiS);

/*!
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
extern	int32_t	text_checkAsciiBuffer(const char_t *ascii1, const char_t *ascii2, bool_t *equals);

#ifdef	__cplusplus
}
#endif

// text manager errors
// -------------------

#define	KTEXTNOERR	0				// No error
#define	KTEXTSYCNA	-(KTEXTERR+1)	// System call not available
#define	KTEXTGEERR	-(KTEXTERR+2)	// General error

/*@}*/

#endif
