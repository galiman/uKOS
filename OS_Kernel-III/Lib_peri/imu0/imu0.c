/*
; imu0.
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
; Goal:		imu0 manager.
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

LOC_CONST_STRG(aStrApplication[]) =	"imu0         imu0 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "imu0 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_imu0_init(void);
extern	int32_t	stub_imu0_configure(const cnfImu0_t *configure);
extern	int32_t	stub_imu0_getImu(accePack_t *accelerometer, gyroPack_t *gyroscope, magnPack_t *magnetometer);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Imu0, KIDPERI, KIMU0NUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Reserve the imu0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (imu0_reserve(KDEVALL) == KIMU0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    imu0_xyz();
 *    status = imu0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KIMU0NOERR	The manager is reserved
 * \param[out]  KIMU0CHBSY 	The manager is busy
 *
 */
int32_t	imu0_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KIMU0CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KIMU0NOERR);
}

/*
 * \brief Release the imu0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = imu0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KIMU0NOERR	OK
 *
 */
int32_t	imu0_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KIMU0NOERR);
}

/*
 * \brief Configure the imu0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfImux_t    configure = {
 *                             .oAcceMode = K4G;
 *                             .oGyroMode = K2000DPS;
 *                             .oMagnMode = K8GAUSS;
 *                         };
 *
 *    status = imu0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KIMU0NOERR	OK
 *
 */
int32_t	imu0_configure(const cnfImu0_t *configure) {

	_init();
	return (stub_imu0_configure(configure));
}

/*
 * \brief Get the imu0 pack
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t       status;
 * accePack_t    accelerometer;
 * gyroPack_t    gyroscope;
 * magnPack_t    magnetometer;
 *
 *    status = imu0_getImu(&accelerometer, &gyroscope, &magnetometer);
 *    printf(“%3f %3f %3f,   %3f %3f %3f,   %3f %3f %3f\n”, \
 *                                                          accelerometer.oAcce_X, \
 *                                                          accelerometer.oAcce_Y, \
 *                                                          accelerometer.oAcce_Z, \
 *                                                          gyroscope.oGyro_X, \
 *                                                          gyroscope.oGyro_Y, \
 *                                                          gyroscope.oGyro_Z, \
 *                                                          magnetometer.oMagn_X, \
 *                                                          magnetometer.oMagn_Y, \
 *                                                          magnetometer.oMagn_Z);
 * \endcode
 *
 * \param[in]	*temperature	Ptr on the temperature
 * \param[out]	KIMU0NOERR		OK
 *
 */
int32_t	imu0_getImu(accePack_t *accelerometer, gyroPack_t *gyroscope, magnPack_t *magnetometer) {

	_init();
	return (stub_imu0_getImu(accelerometer, gyroscope, magnetometer));
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

		stub_imu0_init();
	}
}

