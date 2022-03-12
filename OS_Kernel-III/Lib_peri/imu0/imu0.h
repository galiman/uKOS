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

#ifndef	__IMU0__
#define	__IMU0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KIMU0MAN	(KIMU0NUM<<8)
#define	KIMU0ERR	((KIDPERI<<24) | KIMU0MAN)

// Configuration structure
// -----------------------

typedef	struct	cnfImu0		cnfImu0_t;
typedef	struct	accePack	accePack_t;
typedef	struct	gyroPack	gyroPack_t;
typedef	struct	magnPack	magnPack_t;

struct cnfImu0 {
			uint8_t		oAcceMode;										// Acceleration mode
			#define		K2G			0									// +/- 2-g
			#define		K4G			1									// +/- 4-g
			#define		K8G			2									// +/- 8-g
			#define		K16G		3									// +/- 16-g

			uint8_t		oGyroMode;										// Gyroscope mode
			#define		K245DPS		0									// +/- 245-dps
			#define		K500DPS		1									// +/- 500-dps
			#define		K2000DPS	2									// +/- 2000-dps

			uint8_t		oMagnMode;										// Magnetometer mode
			#define		K4GAUSS		0									// +/- 4-gauss
			#define		K8GAUSS		1									// +/- 8-gauss
			#define		K12GAUSS	2									// +/- 12-gauss
			#define		K16GAUSS	3									// +/- 16-gauss
};

// Data access

struct accePack {
			float64_t	oAcce_X;										// X Accelerometer
			float64_t	oAcce_Y;										// Y Accelerometer
			float64_t	oAcce_Z;										// Z Accelerometer
};

struct gyroPack {
			float64_t	oGyro_X;										// X gyroscope
			float64_t	oGyro_Y;										// Y gyroscope
			float64_t	oGyro_Z;										// Z gyroscope
};

struct magnPack {
			float64_t	oMagn_X;										// X magnetometer
			float64_t	oMagn_Y;										// Y magnetometer
			float64_t	oMagn_Z;										// Z magnetometer
};

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	IMU0_reserve	imu0_reserve
#define	IMU0_release	imu0_release

/*!
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
extern	int32_t	imu0_reserve(uint8_t mode);

/*!
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
extern	int32_t	imu0_release(uint8_t mode);

/*!
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
extern	int32_t	imu0_configure(const cnfImu0_t *configure);

/*!
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
extern	int32_t	imu0_getImu(accePack_t *accelerometer, gyroPack_t *gyroscope, magnPack_t *magnetometer);

#ifdef	__cplusplus
}
#endif

// imu0 manager errors
// -------------------

#define	KIMU0NOERR	0				// No error
#define	KIMU0SYCNA	-(KIMU0ERR+1)	// System call not available
#define	KIMU0GEERR	-(KIMU0ERR+2)	// General error
#define	KIMU0CHBSY	-(KIMU0ERR+3)	// The manager is busy

/*@}*/

#endif
