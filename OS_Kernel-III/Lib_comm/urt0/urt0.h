/*
; urt0.
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
; Goal:		urt0 manager.
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

#ifndef	__URT0__
#define	__URT0__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/urtx.h>

#define	KURT0MAN	(KURT0NUM<<8)
#define	KURT0ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KURT0		(('u'<<24) | ('r'<<16) | ('t'<<8) | '0')

// Semaphores
// ----------

#define	KSEMAPHORE_URT0RX	"urt0 - RX char"
#define	KSEMAPHORE_URT0TX	"urt0 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	URT0_reserve	urt0_reserve
#define	URT0_release	urt0_release

/*!
 * \brief Reserve the urt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (urt0_reserve(KDEVWRITE) == KURT0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    urt0_xyz();
 *    status = urt0_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT0NOERR	The manager is reserved
 * \param[out]  KURT0CHBSY 	The manager is busy
 *
 */
extern	int32_t	urt0_reserve(uint8_t mode);

/*!
 * \brief Release the urt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt0_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT0NOERR	OK
 *
 */
extern	int32_t	urt0_release(uint8_t mode);

/*!
 * \brief Configure the urt0 manager
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
 *    status = urt0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KURT0NOERR	OK
 * \param[out]	KURT0NOCNF	The configuration does not exist
 *
 */
extern	int32_t	urt0_configure(const cnfUrtx_t *configure);

/*!
 * \brief Write a buffer to the urt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = urt0_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KURT0NOERR	OK
 * \param[out]	KURT0SEPRO	The sender is busy
 * \param[out]	KURT0LNBUB	The buffer length is too big
 * \param[out]	KURT0LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	urt0_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the urt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = urt0_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KURT0NOERR	OK
 * \param[out]	KURT0RBUEM	The receiver buffer is empty
 * \param[out]	KURT0RBFUL	The receiver buffer is full
 * \param[out]	KURT0EROVR	Overrun error
 * \param[out]	KURT0ERNOI	Noise error
 * \param[out]	KURT0ERFRA	Framing error
 * \param[out]	KURT0ERPAR	Parity error
 *
 */
extern	int32_t	urt0_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = urt0_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = urt0_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KURT0NOERR		OK
 * \param[out]	KURT0SENOE		The semaphore does not exist
 *
 */
extern	int32_t	urt0_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the urt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt0_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KURT0NOERR	OK
 *
 */
extern	int32_t	urt0_flush(void);

#ifdef	__cplusplus
}
#endif

// urt0 manager errors & status
// ----------------------------

#define	KURT0NOERR	KCOMMNOERR		// No error
#define	KURT0SYCNA	KCOMMSYCNA		// System call not available
#define	KURT0GEERR	KCOMMGEERR		// General error
#define	KURT0NODEV	KCOMMNODEV		// The device does not exist
#define	KURT0NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KURT0CHBSY	KCOMMCHBSY		// The manager is busy

#define	KURT0LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KURT0LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KURT0SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KURT0SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KURT0SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KURT0SEPRO	KCOMMSEPRO		// the sender is busy

#define	KURT0RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KURT0RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KURT0EROVR	KCOMMEROVR		// Overrun error
#define	KURT0ERNOI	KCOMMERNOI		// Noise error
#define	KURT0ERFRA	KCOMMERFRA		// Framing error
#define	KURT0ERPAR	KCOMMERPAR		// Parity error

#define	KURT0NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KURT0NOHAR	KCOMMNOHAR		// Hardware problem
#define	KURT0TIMEO	KCOMMTIMEO		// Timeout error
#define	KURT0SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
