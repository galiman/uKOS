/*
; urt3.
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
; Goal:		urt3 manager.
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

#ifndef	__URT3__
#define	__URT3__

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_comm/urtx.h>

#define	KURT3MAN	(KURT3NUM<<8)
#define	KURT3ERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KURT3		(('u'<<24) | ('r'<<16) | ('t'<<8) | '3')

// Semaphores
// ----------

#define	KSEMAPHORE_URT3RX	"urt3 - RX char"
#define	KSEMAPHORE_URT3TX	"urt3 - TX buff"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	URT3_reserve	urt3_reserve
#define	URT3_release	urt3_release

/*!
 * \brief Reserve the urt3 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (urt3_reserve(KDEVWRITE) == KURT3CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    urt3_xyz();
 *    status = urt3_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT3NOERR	The manager is reserved
 * \param[out]  KURT3CHBSY 	The manager is busy
 *
 */
extern	int32_t	urt3_reserve(uint8_t mode);

/*!
 * \brief Release the urt3 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt3_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT3NOERR	OK
 *
 */
extern	int32_t	urt3_release(uint8_t mode);

/*!
 * \brief Configure the urt3 manager
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
 *    status = urt3_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KURT3NOERR	OK
 * \param[out]	KURT3NOCNF	The configuration does not exist
 *
 */
extern	int32_t	urt3_configure(const cnfUrtx_t *configure);

/*!
 * \brief Write a buffer to the urt3 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = urt3_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KURT3NOERR	OK
 * \param[out]	KURT3SEPRO	The sender is busy
 * \param[out]	KURT3LNBUB	The buffer length is too big
 * \param[out]	KURT3LNBU0	The buffer length is = 0
 *
 */
extern	int32_t	urt3_write(const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the urt3 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = urt3_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KURT3NOERR	OK
 * \param[out]	KURT3RBUEM	The receiver buffer is empty
 * \param[out]	KURT3RBFUL	The receiver buffer is full
 * \param[out]	KURT3EROVR	Overrun error
 * \param[out]	KURT3ERNOI	Noise error
 * \param[out]	KURT3ERFRA	Framing error
 * \param[out]	KURT3ERPAR	Parity error
 *
 */
extern	int32_t	urt3_read(uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = urt3_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = urt3_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KURT3NOERR		OK
 * \param[out]	KURT3SENOE		The semaphore does not exist
 *
 */
extern	int32_t	urt3_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the urt3 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt3_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KURT3NOERR	OK
 *
 */
extern	int32_t	urt3_flush(void);

#ifdef	__cplusplus
}
#endif

// urt3 manager errors & status
// ----------------------------

#define	KURT3NOERR	KCOMMNOERR		// No error
#define	KURT3SYCNA	KCOMMSYCNA		// System call not available
#define	KURT3GEERR	KCOMMGEERR		// General error
#define	KURT3NODEV	KCOMMNODEV		// The device does not exist
#define	KURT3NOCHA	KCOMMNOCHA		// The channel does not exist
#define	KURT3CHBSY	KCOMMCHBSY		// The manager is busy

#define	KURT3LNBU0	KCOMMLNBU0		// The buffer length is = 0
#define	KURT3LNBUB	KCOMMLNBUB		// The buffer length is too big
#define	KURT3SBUEM	KCOMMSBUEM		// The sender buffer is empty
#define	KURT3SBUNE	KCOMMSBUNE		// The sender buffer is not empty
#define	KURT3SBFUL	KCOMMSBFUL		// The sender buffer is full
#define	KURT3SEPRO	KCOMMSEPRO		// the sender is busy

#define	KURT3RBUEM	KCOMMRBUEM		// The receiver buffer is empty
#define	KURT3RBFUL	KCOMMRBFUL		// The receiver buffer is full
#define	KURT3EROVR	KCOMMEROVR		// Overrun error
#define	KURT3ERNOI	KCOMMERNOI		// Noise error
#define	KURT3ERFRA	KCOMMERFRA		// Framing error
#define	KURT3ERPAR	KCOMMERPAR		// Parity error

#define	KURT3NOCNF	KCOMMNOCNF		// The configuration does not exist
#define	KURT3NOHAR	KCOMMNOHAR		// Hardware problem
#define	KURT3TIMEO	KCOMMTIMEO		// Timeout error
#define	KURT3SENOE	KCOMMSENOE		// The semaphore does not exist

/*@}*/

#endif
