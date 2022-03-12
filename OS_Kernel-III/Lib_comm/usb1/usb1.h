/*
; usb1.
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
; Goal:		usb1 manager.
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

#ifndef	__USB1__
#define	__USB1__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/usbx.h>

#define	KUSB1MAN	(KUSB1NUM<<8)
#define	KUSB1ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KUSB1		(('u'<<24) | ('s'<<16) | ('b'<<8) | '1')

// Semaphores
// ----------

#define	KSEMAPHORE_USB1RX	"usb1 - RX char"
#define	KSEMAPHORE_USB1TX	"usb1 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	USB1_reserve	usb1_reserve
#define	USB1_release	usb1_release

/*!
 * \brief Reserve the usb1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (usb1_reserve(KDEVWRITE) == KUSB1CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    usb1_xyz();
 *    status = usb1_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KUSB1NOERR	The manager is reserved
 * \param[out]	KUSB1CHBSY 	The manager is busy
 *
 */
extern	int32_t	usb1_reserve(uint8_t mode);

/*!
 * \brief Release the usb1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = usb1_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KUSB1NOERR	OK
 *
 */
extern	int32_t	usb1_release(uint8_t mode);

/*!
 * \brief Configure the usb1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfUsbx_t    configure = {
 *                             .oKernSync = (1<<BRXSEMA) | (1<<BTXSEMA)
 *                         };
 *
 *    status = usb1_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KUSB1NOERR	OK
 * \param[out]	KUSB1NOCNF	The configuration does not exist
 *
 */
extern	int32_t	usb1_configure(const cnfUsbx_t *configure);

/*!
 * \brief Write a buffer to the usb1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = usb1_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KUSB1NOERR	OK
 * \param[out]	KUSB1SEPRO	The sender is busy
 * \param[out]	KUSB1LNBUB	The buffer length is too big
 * \param[out]	KUSB1LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	usb1_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the usb1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = usb1_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KUSB1NOERR	OK
 * \param[out]	KUSB1RBUEM	The receiver buffer is empty
 * \param[out]	KUSB1RBFUL	The receiver buffer is full
 * \param[out]	KUSB1EROVR	Overrun error
 * \param[out]	KUSB1ERNOI	Noise error
 * \param[out]	KUSB1ERFRA	Framing error
 * \param[out]	KUSB1ERPAR	Parity error
 *
 */
extern	int32_t	usb1_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = usb1_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = usb1_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KUSB1NOERR		OK
 * \param[out]	KUSB1SENOE		The semaphore does not exist
 *
 */
extern	int32_t	usb1_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the usb1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = usb1_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KUSB1NOERR	OK
 *
 */
extern	int32_t	usb1_flush(void);

#ifdef	__cplusplus
}
#endif

// usb1 manager errors & status
// ----------------------------

#define	KUSB1NOERR	KCOMMNOERR		// No error
#define	KUSB1SYCNA	KCOMMSYCNA		// System call not available
#define	KUSB1GEERR	KCOMMGEERR		// General error
#define	KUSB1NODEV	KCOMMNODEV		// The device does not exist
#define	KUSB1NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KUSB1CHBSY	KCOMMCHBSY		// The manager is busy

#define	KUSB1LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KUSB1LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KUSB1SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KUSB1SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KUSB1SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KUSB1SEPRO	KCOMMSEPRO		// the sender is busy

#define	KUSB1RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KUSB1RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KUSB1EROVR	KCOMMEROVR		// Overrun error
#define	KUSB1ERNOI	KCOMMERNOI		// Noise error
#define	KUSB1ERFRA	KCOMMERFRA		// Framing error
#define	KUSB1ERPAR	KCOMMERPAR		// Parity error

#define	KUSB1NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KUSB1NOHAR	KCOMMNOHAR		// Hardware problem
#define	KUSB1TIMEO	KCOMMTIMEO		// Timeout error
#define	KUSB1SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
