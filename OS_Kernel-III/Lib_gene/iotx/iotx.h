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

#ifndef	__IOTX__
#define	__IOTX__

/*!
 * \addtogroup Lib_gene
 */
/*@{*/

#include 	<modules.h>

#define	KIOTXMAN	(KIOTXNUM<<8)
#define	KIOTXERR	((KIDGENE<<24) | KIOTXMAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

/*!
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
extern	int32_t	iotx_waitString(uint32_t commManager, char_t *ascii, uint32_t size);

/*!
 * \brief Small printf
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    number;
 * 
 * number = iotx_printf(KSYST, “the lazy fox ...\n”);
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
extern	int32_t	iotx_printf(uint32_t commManager, const char_t *format, ...);

#ifdef	__cplusplus
}
#endif

// iotx manager errors
// -------------------

#define	KIOTXNOERR	0				// No error
#define	KIOTXSYCNA	-(KIOTXERR+1)	// System call not available
#define	KIOTXGEERR	-(KIOTXERR+2)	// General error

/*@}*/

#endif
