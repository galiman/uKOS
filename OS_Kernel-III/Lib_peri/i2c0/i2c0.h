/*
; i2c0.
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
; Goal:		i2c0 manager.
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

#ifndef	__I2C0__
#define	__I2C0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_peri/i2cx.h>

#define	KI2C0MAN	(KI2C0NUM<<8)
#define	KI2C0ERR	((KIDPERI<<24) | KI2C0MAN)

// Semaphores
// ----------

#define	KSEMAPHORE_I2C0RX	"i2c0 - RX msg"
#define	KSEMAPHORE_I2C0TX	"i2c0 - TX msb"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	I2C0_reserve	i2c0_reserve
#define	I2C0_release	i2c0_release

/*!
 * \brief Reserve the i2c0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (i2c0_reserve(KDEVALL) == KI2C0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    i2c0_xyz();
 *    status = i2c0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KI2C0NOERR	The manager is reserved
 * \param[out]  KI2C0CHBSY 	The manager is busy
 *
 */
extern	int32_t	i2c0_reserve(uint8_t mode);

/*!
 * \brief Release the i2c0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = i2c0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KI2C0NOERR	OK
 *
 */
extern	int32_t	i2c0_release(uint8_t mode);

/*!
 * \brief Configure the i2c0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfI2cx_t    configure = {
 *                             .oSpeed    = K100KHZ;
 *                             .oKernSync = (1<<BRXSEMA) | (1<<BTXSEMA);
 *                         };
 *
 *    status = i2c0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KI2C0NOERR	OK
 *
 */
extern	int32_t	i2c0_configure(const cnfI2cx_t *configure);

/*!
 * \brief Write a buffer to the i2c0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      uint8_t    buffer[7] = { 1, 2, 3, 4, 5, 6, 7 };
 *
 *    status = i2c0_write(0x34, buffer, sizeof(buffer));
 * \endcode
 *
 * \param[in]	address		i2c device address
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KI2C0NOERR	OK
 * \param[out]	KI2C0TIMEO	Timeout error
 *
 */
extern	int32_t	i2c0_write(uint8_t address, const uint8_t *buffer, uint16_t size);

/*!
 * \brief Read a buffer from the i2c0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    buffer[10];
 *
 * // Example 1: Read 10 bytes
 *
 *    buffer[0] = 0xFF;
 *    status = i2c0_read(0x34, buffer, 10);
 *
 * // Example 2: Write the register 56 and then read 2 bytes
 *
 *    buffer[0] = 56;
 *    status = i2c0_read(0x34, buffer, 2);
 * \endcode
 *
 * \param[in]	address		i2c device address
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KI2C0NOERR	OK
 * \param[out]	KI2C0TIMEO	Timeout error
 *
 */
extern	int32_t	i2c0_read(uint8_t address, uint8_t *data, uint16_t size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = i2c0_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = i2c0_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KI2C0NOERR		OK
 * \param[out]	KI2C0SENOE		The semaphore does not exist
 *
 */
extern	int32_t	i2c0_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the i2c0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = i2c0_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KI2C0NOERR	OK
 *
 */
extern	int32_t	i2c0_flush(void);

#ifdef	__cplusplus
}
#endif

// i2c0 manager errors
// -------------------

#define	KI2C0NOERR	0				// No error
#define	KI2C0CHBSY	-(KI2C0ERR+1)	// The manager is busy
#define	KI2C0TIMEO	-(KI2C0ERR+2)	// Timeout error
#define	KI2C0SENOE	-(KI2C0ERR+3)	// The semaphore does not exist

/*@}*/

#endif
