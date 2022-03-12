/*
; urt1.
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
; Goal:		urt1 manager.
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

#ifndef	__URT1__
#define	__URT1__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/urtx.h>

#define	KURT1MAN	(KURT1NUM<<8)
#define	KURT1ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KURT1		(('u'<<24) | ('r'<<16) | ('t'<<8) | '1')

// Semaphores
// ----------

#define	KSEMAPHORE_URT1RX	"urt1 - RX char"
#define	KSEMAPHORE_URT1TX	"urt1 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	URT1_reserve	urt1_reserve
#define	URT1_release	urt1_release

/*!
 * \brief Reserve the urt1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (urt1_reserve(KDEVWRITE) == KURT1CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    urt1_xyz();
 *    status = urt1_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT1NOERR	The manager is reserved
 * \param[out]  KURT1CHBSY 	The manager is busy
 *
 */
extern	int32_t	urt1_reserve(uint8_t mode);

/*!
 * \brief Release the urt1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt1_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT1NOERR	OK
 *
 */
extern	int32_t	urt1_release(uint8_t mode);

/*!
 * \brief Configure the urt1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfUrtx_t    configure = {
 *                             .oBaudRate = KBD57600,
 *                             .oKernSync = (1<<BRXSEMA),
 *                             .oNBits    = K8BIT,
 *                             .oStopBits = K1STBIT,
 *                             .oParity   = KNONE
 *                         };
 *
 *    status = urt1_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KURT1NOERR	OK
 * \param[out]	KURT1NOCNF	The configuration does not exist
 *
 */
extern	int32_t	urt1_configure(const cnfUrtx_t *configure);

/*!
 * \brief Write a buffer to the urt1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = urt1_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KURT1NOERR	OK
 * \param[out]	KURT1SEPRO	The sender is busy
 * \param[out]	KURT1LNBUB	The buffer length is too big
 * \param[out]	KURT1LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	urt1_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the urt1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = urt1_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KURT1NOERR	OK
 * \param[out]	KURT1RBUEM	The receiver buffer is empty
 * \param[out]	KURT1RBFUL	The receiver buffer is full
 * \param[out]	KURT1EROVR	Overrun error
 * \param[out]	KURT1ERNOI	Noise error
 * \param[out]	KURT1ERFRA	Framing error
 * \param[out]	KURT1ERPAR	Parity error
 *
 */
extern	int32_t	urt1_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = urt1_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = urt1_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KURT1NOERR		OK
 * \param[out]	KURT1SENOE		The semaphore does not exist
 *
 */
extern	int32_t	urt1_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the urt1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt1_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KURT1NOERR	OK
 *
 */
extern	int32_t	urt1_flush(void);

#ifdef	__cplusplus
}
#endif

// urt1 manager errors & status
// ----------------------------

#define	KURT1NOERR	KCOMMNOERR		// No error
#define	KURT1SYCNA	KCOMMSYCNA		// System call not available
#define	KURT1GEERR	KCOMMGEERR		// General error
#define	KURT1NODEV	KCOMMNODEV		// The device does not exist
#define	KURT1NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KURT1CHBSY	KCOMMCHBSY		// The manager is busy

#define	KURT1LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KURT1LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KURT1SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KURT1SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KURT1SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KURT1SEPRO	KCOMMSEPRO		// the sender is busy

#define	KURT1RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KURT1RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KURT1EROVR	KCOMMEROVR		// Overrun error
#define	KURT1ERNOI	KCOMMERNOI		// Noise error
#define	KURT1ERFRA	KCOMMERFRA		// Framing error
#define	KURT1ERPAR	KCOMMERPAR		// Parity error

#define	KURT1NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KURT1NOHAR	KCOMMNOHAR		// Hardware problem
#define	KURT1TIMEO	KCOMMTIMEO		// Timeout error
#define	KURT1SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
