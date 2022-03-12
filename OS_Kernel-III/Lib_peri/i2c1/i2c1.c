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

#include 	<uKOS.h>

static	bool_t	vReserved  = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"i2c1         i2c1 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "i2c1 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_i2c1_init(void);
extern	int32_t	stub_i2c1_configure(const cnfI2cx_t *configure);
extern	int32_t	stub_i2c1_write(uint8_t address, const uint8_t *buffer, uint16_t size);
extern	int32_t	stub_i2c1_read(uint8_t address, uint8_t *buffer, uint16_t size);
extern	int32_t	stub_i2c1_flush(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(I2c1, KIDPERI, KI2C1NUM, NULL, "1.0", (1<<BSHOW));

/*
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
int32_t	i2c1_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KI2C1CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KI2C1NOERR);
}

/*
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
int32_t	i2c1_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KI2C1NOERR);
}

/*
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
int32_t	i2c1_configure(const cnfI2cx_t *configure) {

	_init();
	return (stub_i2c1_configure(configure));
}

/*
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
int32_t	i2c1_write(uint8_t address, const uint8_t *buffer, uint16_t size) {

	_init();
	return (stub_i2c1_write(address, buffer, size));
}

/*
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
int32_t	i2c1_read(uint8_t address, uint8_t *buffer, uint16_t size) {

	_init();
	return (stub_i2c1_read(address, buffer, size));
}

/*
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
int32_t	i2c1_getIdSemaphore(uint8_t semaphore, char_t **identifier) {

	_init();
	if (semaphore == KSEMARX) { *identifier = KSEMAPHORE_I2C1RX; return (KI2C1NOERR); }
	if (semaphore == KSEMATX) { *identifier = KSEMAPHORE_I2C1TX; return (KI2C1NOERR); }
	return (KI2C1SENOE);
}

/*
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
int32_t	i2c1_flush(void) {

	_init();
	return (stub_i2c1_flush());
}

// Local routines
// ==============

/*
 * \brief _init
 *
 * - This function initializes the manager and
 *   has to be called at least once.
 *
 */
static	void	_init(void) {
	static	bool_t	init = FALSE;

	if (!init) {
		init = TRUE;

		stub_i2c1_init();
	}
}

