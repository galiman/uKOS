/*
; i2c1.
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
; Goal:		i2c1 manager.
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

#ifndef	__I2C1__
#define	__I2C1__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>
#include 	<Lib_peri/i2cx.h>

#define	KI2C1MAN	(KI2C1NUM<<8)
#define	KI2C1ERR	((KIDPERI<<24) | KI2C1MAN)

// Semaphores
// ----------

#define	KSEMAPHORE_I2C1RX	"i2c1 - RX msg"
#define	KSEMAPHORE_I2C1TX	"i2c1 - TX msg"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	I2C1_reserve	i2c1_reserve
#define	I2C1_release	i2c1_release

/*!
 * \brief Reserve the i2c1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (i2c1_reserve(KDEVALL) == KI2C1CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    i2c1_xyz();
 *    status = i2c1_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KI2C1NOERR	The manager is reserved
 * \param[out]  KI2C1CHBSY 	The manager is busy
 *
 */
extern	int32_t	i2c1_reserve(uint8_t mode);

/*!
 * \brief Release the i2c1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = i2c1_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KI2C1NOERR	OK
 *
 */
extern	int32_t	i2c1_release(uint8_t mode);

/*!
 * \brief Configure the i2c1 manager
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
 *    status = i2c1_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KI2C1NOERR	OK
 *
 */
extern	int32_t	i2c1_configure(const cnfI2cx_t *configure);

/*!
 * \brief Write a buffer to the i2c1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      uint8_t    buffer[7] = { 1, 2, 3, 4, 5, 6, 7 };
 *
 *    status = i2c1_write(0x34, buffer, sizeof(buffer));
 * \endcode
 *
 * \param[in]	address		i2c device address
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KI2C1NOERR	OK
 * \param[out]	KI2C1TIMEO	Timeout error
 *
 */
extern	int32_t	i2c1_write(uint8_t address, const uint8_t *buffer, uint16_t size);

/*!
 * \brief Read a buffer from the i2c1 manager
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
 *    status = i2c1_read(0x34, buffer, 10);
 *
 * // Example 2: Write the register 56 and then read 2 bytes
 *
 *    buffer[0] = 56;
 *    status = i2c1_read(0x34, buffer, 2);
 * \endcode
 *
 * \param[in]	address		i2c device address
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KI2C1NOERR	OK
 * \param[out]	KI2C1TIMEO	Timeout error
 *
 */
extern	int32_t	i2c1_read(uint8_t address, uint8_t *data, uint16_t size);

/*!
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = i2c1_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = i2c1_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KI2C1NOERR		OK
 * \param[out]	KI2C1SENOE		The semaphore does not exist
 *
 */
extern	int32_t	i2c1_getIdSemaphore(uint8_t semaphore, char_t **identifier);

/*!
 * \brief Flush the i2c1 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = i2c1_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KI2C1NOERR	OK
 *
 */
extern	int32_t	i2c1_flush(void);

#ifdef	__cplusplus
}
#endif

// i2c1 manager errors
// -------------------

#define	KI2C1NOERR	0				// No error
#define	KI2C1CHBSY	-(KI2C1ERR+1)	// The manager is busy
#define	KI2C1TIMEO	-(KI2C1ERR+2)	// Timeout error
#define	KI2C1SENOE	-(KI2C1ERR+3)	// The semaphore does not exist

/*@}*/

#endif
