/*
; comm.
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
; Goal:		comm manager.
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

#ifndef	__COMM___
#define	__COMM___

/*!
 * \addtogroup Lib_comm
 */
/*@{*/

#include 	<modules.h>

#define	KCOMMMAN	(KCOMMNUM<<8)
#define	KCOMMERR	((KIDCOMM<<24) | KCOMMMAN)

#define	KNOTR		(('n'<<24) | ('o'<<16) | ('t'<<8) | 'R')
#define	KSYST		(('s'<<24) | ('y'<<16) | ('s'<<8) | 't')
#define	KDEF0		(('d'<<24) | ('e'<<16) | ('f'<<8) | '0')

// Semaphores
// ----------

#define	KSEMARX		0				// Semaphore RX
#define	KSEMATX		1				// Semaphore TX

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	COMM_reserve	comm_reserve
#define	COMM_release	comm_release

/*!
 * \brief Reserve the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (comm_reserve(KDEF0, KDEVWRITE) == KCOMMCHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    comm_xyz();
 *    status = comm_release(KDEF0, KDEVWRITE);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
extern	int32_t	comm_reserve(uint32_t commManager, uint8_t mode);

/*!
 * \brief Release the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = comm_release(KDEF0, KDEVWRITE);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
extern	int32_t	comm_release(uint32_t commManager, uint8_t mode);

/*!
 * \brief Configure the communication manager
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
 *    status = comm_configure(KURT0, (void *)&configure);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
extern	int32_t	comm_configure(uint32_t commManager, const void *configure);

/*!
 * \brief Write a buffer to the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = comm_write(KDEF0, buffer, KSIZE);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
extern	int32_t	comm_write(uint32_t commManager, const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read a buffer from the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = comm_read(KDEF0, buffer, &size);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*data		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
extern	int32_t	comm_read(uint32_t commManager, uint8_t *data, uint32_t *size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = comm_getIdSemaphore(KURT0, KSEMARX, &identifier[0];
 *    status = comm_getIdSemaphore(KURT0, KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	commManager		Communication manager
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore Id
 * \param[out]	-				Depends on the "xxxx" communication manager
 *
 */
extern	int32_t	comm_getIdSemaphore(uint32_t commManager, uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = comm_flush(KDEF0);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[out]	KCOMMNOERR	OK
 * \param[out]	KCOMMNODEV	The commManager does not exist
 *
 */
extern	int32_t	comm_flush(uint32_t commManager);

/*!
 * \brief Set the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = comm_setCommDev(KURT0);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[out]	KCOMMNOERR	OK
 *
 */
extern	int32_t	comm_setCommDev(uint32_t commManager);

/*!
 * \brief Get the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    commManager;
 *
 *    status = comm_getCommDev(&commManager);
 * \endcode
 *
 * \param[in]	*commManager	Ptr on the communication manager
 * \param[out]	KCOMMNOERR		OK
 *
 */
extern	int32_t	comm_getCommDev(uint32_t *commManager);

#ifdef	__cplusplus
}
#endif

// comm manager errors
// -------------------

#define	KCOMMNOERR	0				// No error
#define	KCOMMSYCNA	-(KCOMMERR+1)	// System call not available
#define	KCOMMGEERR	-(KCOMMERR+2)	// General error
#define	KCOMMNODEV	-(KCOMMERR+3)	// The device does not exist
#define	KCOMMNOCHA	-(KCOMMERR+4)	// The channel does not exist
#define	KCOMMCHBSY	-(KCOMMERR+5)	// The manager is busy

#define	KCOMMLNBU0	-(KCOMMERR+6)	// The buffer length is = 0
#define	KCOMMLNBUB	-(KCOMMERR+7)	// The buffer length is too big
#define	KCOMMSBUEM	-(KCOMMERR+8)	// The sender buffer is empty
#define	KCOMMSBUNE	-(KCOMMERR+9)	// The sender buffer is not empty
#define	KCOMMSBFUL	-(KCOMMERR+10)	// The sender buffer is full
#define	KCOMMSEPRO	-(KCOMMERR+11)	// the sender is busy

#define	KCOMMRBUEM	-(KCOMMERR+12)	// The receiver buffer is empty
#define	KCOMMRBFUL	-(KCOMMERR+13)	// The receiver buffer is full
#define	KCOMMEROVR	-(KCOMMERR+14)	// Overrun error
#define	KCOMMERNOI	-(KCOMMERR+15)	// Noise error
#define	KCOMMERFRA	-(KCOMMERR+16)	// Framing error
#define	KCOMMERPAR	-(KCOMMERR+17)	// Parity error

#define	KCOMMNOCNF	-(KCOMMERR+18)	// The configuration does not exist
#define	KCOMMNOHAR	-(KCOMMERR+19)	// Hardware problem
#define	KCOMMTIMEO	-(KCOMMERR+20)	// Timeout error
#define	KCOMMSENOE	-(KCOMMERR+21)	// The semaphore does not exist

/*@}*/

#endif
