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

#ifndef	__STRG__
#define	__STRG__

/*!
 * \addtogroup Lib_tbox
 */
/*@{*/

#include 	<modules.h>

#define	KSTRGMAN	(KSTRGNUM<<8)
#define	KSTRGERR	((KIDTBOX<<24) | KSTRGMAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#if (defined(__STRG_FULL__))
/*!
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
extern	int32_t	strg_cnvtDecAsciiToBufFloat64(float64_t *binary, const char_t *ascii, uint16_t nbParameters);

/*!
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
extern	int32_t	strg_cnvtBufInt32ToDecAscii(char_t *ascii, const int32_t *binary, uint16_t nbParameters);

/*!
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
 *    status = strg_cnvtBufInt32ToHexAscii(ascii, binary, 3);
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
extern	int32_t	strg_cnvtBufInt32ToHexAscii(char_t *ascii, const int32_t *binary, uint16_t nbParameters);

/*!
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
extern	int32_t	strg_cnvtValInt32ToDecAscii(char_t *ascii, const int32_t *binary, char_t **buffer);

/*!
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
extern	int32_t	strg_cnvtValInt32ToHexAscii(char_t *ascii, const int32_t *binary, char_t **buffer);
#endif

/*!
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
extern	int32_t	strg_getBufferSize(const char_t *ascii, uint32_t *size);

/*!
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
extern	int32_t	strg_skipProtocolParam(char_t *ascii, uint16_t nbParameters);

/*!
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
extern	int32_t	strg_cnvtDecAsciiToBufInt32(int32_t *binary, const char_t *ascii, uint16_t nbParameters);

/*!
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
extern	int32_t	strg_cnvtHexAsciiToBufInt32(int32_t *binary, const char_t *ascii, uint16_t nbParameters);

/*!
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
extern	int32_t	strg_cnvtBufInt8ToHexAscii(char_t *ascii, const int8_t *binary, uint16_t nbParameters);

/*!
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
extern	int32_t	strg_cnvtValInt64ToDecAscii(char_t *ascii, const int64_t *binary, char_t **buffer);

/*!
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
extern	int32_t	strg_cnvtValInt64ToHexAscii(char_t *ascii, const int64_t *binary, char_t **buffer);

#ifdef	__cplusplus
}
#endif

// strg manager errors
// -------------------

#define	KSTRGNOERR	0				// No error
#define	KSTRGSYCNA	-(KSTRGERR+1)	// System call not available
#define	KSTRGGEERR	-(KSTRGERR+2)	// General error
#define	KSTRGNOPRM	-(KSTRGERR+3)	// The number of parameter is = 0
#define	KSTRGNOASC	-(KSTRGERR+4)	// The parameter is not an ASCII value
#define	KSTRGTODIG	-(KSTRGERR+5)	// The parameter has too many digits
#define	KSTRGTMPRM	-(KSTRGERR+6)	// Too many parameters to skip

/*@}*/

#endif
