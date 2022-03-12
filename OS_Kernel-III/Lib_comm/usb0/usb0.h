/*
; usb0.
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
; Goal:		usb0 manager.
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

#ifndef	__USB0__
#define	__USB0__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/usbx.h>

#define	KUSB0MAN	(KUSB0NUM<<8)
#define	KUSB0ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KUSB0		(('u'<<24) | ('s'<<16) | ('b'<<8) | '0')

// Semaphores
// ----------

#define	KSEMAPHORE_USB0RX	"usb0 - RX char"
#define	KSEMAPHORE_USB0TX	"usb0 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	USB0_reserve	usb0_reserve
#define	USB0_release	usb0_release

/*!
 * \brief Reserve the usb0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (usb0_reserve(KDEVWRITE) == KUSB0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    usb0_xyz();
 *    status = usb0_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KUSB0NOERR	The manager is reserved
 * \param[out]	KUSB0CHBSY 	The manager is busy
 *
 */
extern	int32_t	usb0_reserve(uint8_t mode);

/*!
 * \brief Release the usb0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = usb0_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KUSB0NOERR	OK
 *
 */
extern	int32_t	usb0_release(uint8_t mode);

/*!
 * \brief Configure the usb0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfUsbx_t    configure = {
 *                             .oKernSync = (1<<BRXSEMA) | (1<<BTXSEMA)
 *                         };
 *
 *    status = usb0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KUSB0NOERR	OK
 * \param[out]	KUSB0NOCNF	The configuration does not exist
 *
 */
extern	int32_t	usb0_configure(const cnfUsbx_t *configure);

/*!
 * \brief Write a buffer to the usb0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = usb0_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KUSB0NOERR	OK
 * \param[out]	KUSB0SEPRO	The sender is busy
 * \param[out]	KUSB0LNBUB	The buffer length is too big
 * \param[out]	KUSB0LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	usb0_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the usb0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = usb0_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KUSB0NOERR	OK
 * \param[out]	KUSB0RBUEM	The receiver buffer is empty
 * \param[out]	KUSB0RBFUL	The receiver buffer is full
 * \param[out]	KUSB0EROVR	Overrun error
 * \param[out]	KUSB0ERNOI	Noise error
 * \param[out]	KUSB0ERFRA	Framing error
 * \param[out]	KUSB0ERPAR	Parity error
 *
 */
extern	int32_t	usb0_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = usb0_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = usb0_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KUSB0NOERR		OK
 * \param[out]	KUSB0SENOE		The semaphore does not exist
 *
 */
extern	int32_t	usb0_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the usb0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = usb0_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KUSB0NOERR	OK
 *
 */
extern	int32_t	usb0_flush(void);

#ifdef	__cplusplus
}
#endif

// usb0 manager errors & status
// ----------------------------

#define	KUSB0NOERR	KCOMMNOERR		// No error
#define	KUSB0SYCNA	KCOMMSYCNA		// System call not available
#define	KUSB0GEERR	KCOMMGEERR		// General error
#define	KUSB0NODEV	KCOMMNODEV		// The device does not exist
#define	KUSB0NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KUSB0CHBSY	KCOMMCHBSY		// The manager is busy

#define	KUSB0LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KUSB0LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KUSB0SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KUSB0SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KUSB0SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KUSB0SEPRO	KCOMMSEPRO		// the sender is busy

#define	KUSB0RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KUSB0RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KUSB0EROVR	KCOMMEROVR		// Overrun error
#define	KUSB0ERNOI	KCOMMERNOI		// Noise error
#define	KUSB0ERFRA	KCOMMERFRA		// Framing error
#define	KUSB0ERPAR	KCOMMERPAR		// Parity error

#define	KUSB0NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KUSB0NOHAR	KCOMMNOHAR		// Hardware problem
#define	KUSB0TIMEO	KCOMMTIMEO		// Timeout error
#define	KUSB0SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
