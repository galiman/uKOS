/*
; urt4.
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
; Goal:		urt4 manager.
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

#ifndef	__URT4__
#define	__URT4__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/urtx.h>

#define	KURT4MAN	(KURT4NUM<<8)
#define	KURT4ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KURT4		(('u'<<24) | ('r'<<16) | ('t'<<8) | '4')

// Semaphores
// ----------

#define	KSEMAPHORE_URT4RX	"urt4 - RX char"
#define	KSEMAPHORE_URT4TX	"urt4 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	URT4_reserve	urt4_reserve
#define	URT4_release	urt4_release

/*!
 * \brief Reserve the urt4 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (urt4_reserve(KDEVWRITE) == KURT4CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    urt4_xyz();
 *    status = urt4_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT4NOERR	The manager is reserved
 * \param[out]  KURT4CHBSY 	The manager is busy
 *
 */
extern	int32_t	urt4_reserve(uint8_t mode);

/*!
 * \brief Release the urt4 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt4_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT4NOERR	OK
 *
 */
extern	int32_t	urt4_release(uint8_t mode);

/*!
 * \brief Configure the urt4 manager
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
 *    status = urt4_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KURT4NOERR	OK
 * \param[out]	KURT4NOCNF	The configuration does not exist
 *
 */
extern	int32_t	urt4_configure(const cnfUrtx_t *configure);

/*!
 * \brief Write a buffer to the urt4 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = urt4_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KURT4NOERR	OK
 * \param[out]	KURT4SEPRO	The sender is busy
 * \param[out]	KURT4LNBUB	The buffer length is too big
 * \param[out]	KURT4LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	urt4_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the urt4 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = urt4_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KURT4NOERR	OK
 * \param[out]	KURT4RBUEM	The receiver buffer is empty
 * \param[out]	KURT4RBFUL	The receiver buffer is full
 * \param[out]	KURT4EROVR	Overrun error
 * \param[out]	KURT4ERNOI	Noise error
 * \param[out]	KURT4ERFRA	Framing error
 * \param[out]	KURT4ERPAR	Parity error
 *
 */
extern	int32_t	urt4_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = urt4_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = urt4_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KURT4NOERR		OK
 * \param[out]	KURT4SENOE		The semaphore does not exist
 *
 */
extern	int32_t	urt4_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the urt4 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt4_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KURT4NOERR	OK
 *
 */
extern	int32_t	urt4_flush(void);

#ifdef	__cplusplus
}
#endif

// urt4 manager errors & status
// ----------------------------

#define	KURT4NOERR	KCOMMNOERR		// No error
#define	KURT4SYCNA	KCOMMSYCNA		// System call not available
#define	KURT4GEERR	KCOMMGEERR		// General error
#define	KURT4NODEV	KCOMMNODEV		// The device does not exist
#define	KURT4NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KURT4CHBSY	KCOMMCHBSY		// The manager is busy

#define	KURT4LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KURT4LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KURT4SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KURT4SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KURT4SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KURT4SEPRO	KCOMMSEPRO		// the sender is busy

#define	KURT4RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KURT4RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KURT4EROVR	KCOMMEROVR		// Overrun error
#define	KURT4ERNOI	KCOMMERNOI		// Noise error
#define	KURT4ERFRA	KCOMMERFRA		// Framing error
#define	KURT4ERPAR	KCOMMERPAR		// Parity error

#define	KURT4NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KURT4NOHAR	KCOMMNOHAR		// Hardware problem
#define	KURT4TIMEO	KCOMMTIMEO		// Timeout error
#define	KURT4SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
