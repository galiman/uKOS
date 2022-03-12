/*
; urt2.
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
; Goal:		urt2 manager.
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

#ifndef	__URT2__
#define	__URT2__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/urtx.h>

#define	KURT2MAN	(KURT2NUM<<8)
#define	KURT2ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KURT2		(('u'<<24) | ('r'<<16) | ('t'<<8) | '2')

// Semaphores
// ----------

#define	KSEMAPHORE_URT2RX	"urt2 - RX char"
#define	KSEMAPHORE_URT2TX	"urt2 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	URT2_reserve	urt2_reserve
#define	URT2_release	urt2_release

/*!
 * \brief Reserve the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (urt2_reserve(KDEVWRITE) == KURT2CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    urt2_xyz();
 *    status = urt2_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT2NOERR	The manager is reserved
 * \param[out]  KURT2CHBSY 	The manager is busy
 *
 */
extern	int32_t	urt2_reserve(uint8_t mode);

/*!
 * \brief Release the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt2_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT2NOERR	OK
 *
 */
extern	int32_t	urt2_release(uint8_t mode);

/*!
 * \brief Configure the urt2 manager
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
 *    status = urt2_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KURT2NOERR	OK
 * \param[out]	KURT2NOCNF	The configuration does not exist
 *
 */
extern	int32_t	urt2_configure(const cnfUrtx_t *configure);

/*!
 * \brief Write a buffer to the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = urt2_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KURT2NOERR	OK
 * \param[out]	KURT2SEPRO	The sender is busy
 * \param[out]	KURT2LNBUB	The buffer length is too big
 * \param[out]	KURT2LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	urt2_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = urt2_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KURT2NOERR	OK
 * \param[out]	KURT2RBUEM	The receiver buffer is empty
 * \param[out]	KURT2RBFUL	The receiver buffer is full
 * \param[out]	KURT2EROVR	Overrun error
 * \param[out]	KURT2ERNOI	Noise error
 * \param[out]	KURT2ERFRA	Framing error
 * \param[out]	KURT2ERPAR	Parity error
 *
 */
extern	int32_t	urt2_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = urt2_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = urt2_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KURT2NOERR		OK
 * \param[out]	KURT2SENOE		The semaphore does not exist
 *
 */
extern	int32_t	urt2_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt2_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KURT2NOERR	OK
 *
 */
extern	int32_t	urt2_flush(void);

#ifdef	__cplusplus
}
#endif

// urt2 manager errors & status
// ----------------------------

#define	KURT2NOERR	KCOMMNOERR		// No error
#define	KURT2SYCNA	KCOMMSYCNA		// System call not available
#define	KURT2GEERR	KCOMMGEERR		// General error
#define	KURT2NODEV	KCOMMNODEV		// The device does not exist
#define	KURT2NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KURT2CHBSY	KCOMMCHBSY		// The manager is busy

#define	KURT2LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KURT2LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KURT2SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KURT2SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KURT2SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KURT2SEPRO	KCOMMSEPRO		// the sender is busy

#define	KURT2RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KURT2RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KURT2EROVR	KCOMMEROVR		// Overrun error
#define	KURT2ERNOI	KCOMMERNOI		// Noise error
#define	KURT2ERFRA	KCOMMERFRA		// Framing error
#define	KURT2ERPAR	KCOMMERPAR		// Parity error

#define	KURT2NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KURT2NOHAR	KCOMMNOHAR		// Hardware problem
#define	KURT2TIMEO	KCOMMTIMEO		// Timeout error
#define	KURT2SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
