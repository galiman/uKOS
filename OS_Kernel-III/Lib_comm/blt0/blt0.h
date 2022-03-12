/*
; blt0.
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
; Goal:		blt0 manager.
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

#ifndef	__BLT0__
#define	__BLT0__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/urtx.h>

#define	KBLT0MAN	(KBLT0NUM<<8)
#define	KBLT0ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KBLT0		(('b'<<24) | ('l'<<16) | ('t'<<8) | '0')

// Semaphores
// ----------

#define	KSEMAPHORE_BLT0RX	"blt0 - RX char"
#define	KSEMAPHORE_BLT0TX	"blt0 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	BLT0_reserve	blt0_reserve
#define	BLT0_release	blt0_release

/*!
 * \brief Reserve the blt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (blt0_reserve(KDEVWRITE) == KCOMMCHBSY) {
 *        kern_switchFast();
 *     }
 *
 *    blt0_xyz();
 *    status = blt0_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KBLT0NOERR	The manager is reserved
 * \param[out]  KBLT0CHBSY 	The manager is busy
 *
 */
extern	int32_t	blt0_reserve(uint8_t mode);

/*!
 * \brief Release the blt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = blt0_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KBLT0NOERR	OK
 *
 */
extern	int32_t	blt0_release(uint8_t mode);

/*!
 * \brief Configure the blt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfBlt0_t    configure = {
 *                             .oBaudRate = KBD57600,
 *                             .oKernSync = (1<<BRXSEMA),
 *                             .oNBits    = K8BIT,
 *                             .oStopBits = K1STBIT,
 *                             .oParity   = KNONE
 *                  };
 *
 *    status = blt0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KBLT0NOERR	OK
 * \param[out]	KBLT0NOCNF	The configuration does not exist
 *
 */
extern	int32_t	blt0_configure(const cnfUrtx_t *configure);

/*!
 * \brief Write a buffer to the blt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = blt0_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KBLT0NOERR	OK
 * \param[out]	KBLT0SEPRO	The sender is busy
 * \param[out]	KBLT0LNBUB	The buffer length is too big
 * \param[out]	KBLT0LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	blt0_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the blt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t	   buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = blt0_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*data		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KBLT0NOERR	OK
 * \param[out]	KBLT0RBUEM	The receiver buffer is empty
 * \param[out]	KBLT0RBFUL	The receiver buffer is full
 * \param[out]	KBLT0EROVR	Overrun error
 * \param[out]	KBLT0ERNOI	Noise error
 * \param[out]	KBLT0ERFRA	Framing error
 * \param[out]	KBLT0ERPAR	Parity error
 *
 */
extern	int32_t	blt0_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = blt0_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = blt0_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore Id
 * \param[out]	KBLT0NOERR		OK
 * \param[out]	KBLT0SENOE		The semaphore does not exist
 *
 */
extern	int32_t	blt0_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the blt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = blt0_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KBLT0NOERR	OK
 *
 */
extern	int32_t	blt0_flush(void);

#ifdef	__cplusplus
}
#endif

// blt0 manager errors & status
// ----------------------------

#define	KBLT0NOERR	KCOMMNOERR		// No error
#define	KBLT0SYCNA	KCOMMSYCNA		// System call not available
#define	KBLT0GEERR	KCOMMGEERR		// General error
#define	KBLT0NODEV	KCOMMNODEV		// The device does not exist
#define	KBLT0NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KBLT0CHBSY	KCOMMCHBSY		// The manager is busy

#define	KBLT0LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KBLT0LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KBLT0SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KBLT0SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KBLT0SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KBLT0SEPRO	KCOMMSEPRO		// the sender is busy

#define	KBLT0RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KBLT0RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KBLT0EROVR	KCOMMEROVR		// Overrun error
#define	KBLT0ERNOI	KCOMMERNOI		// Noise error
#define	KBLT0ERFRA	KCOMMERFRA		// Framing error
#define	KBLT0ERPAR	KCOMMERPAR		// Parity error

#define	KBLT0NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KBLT0NOHAR	KCOMMNOHAR		// Hardware problem
#define	KBLT0TIMEO	KCOMMTIMEO		// Timeout error
#define	KBLT0SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
