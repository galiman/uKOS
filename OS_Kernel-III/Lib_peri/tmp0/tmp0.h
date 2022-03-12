/*
; tmp0.
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
; Goal:		tmp0 manager.
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

#ifndef	__TMP0__
#define	__TMP0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KTMP0MAN	(KTMP0NUM<<8)
#define	KTMP0ERR	((KIDPERI<<24) | KTMP0MAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	TMP0_reserve	tmp0_reserve
#define	TMP0_release	tmp0_release

/*!
 * \brief Reserve the tmp0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (tmp0_reserve(KDEVALL) == KTMP0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    tmp0_xyz();
 *    status = tmp0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KTMP0NOERR	The manager is reserved
 * \param[out]  KTMP0CHBSY 	The manager is busy
 *
 */
extern	int32_t	tmp0_reserve(uint8_t mode);

/*!
 * \brief Release the tmp0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = tmp0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KTMP0NOERR	OK
 *
 */
extern	int32_t	tmp0_release(uint8_t mode);

/*!
 * \brief Get the temperature
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * float64_t    temperature;
 * uint16_t    temperatureInt, temperatureFrc;
 *
 *    status = tmp0_getTemperature(&temperature);
 *    temperatureInt = (uint16_t)temperature;
 *    temperatureFrc = (uint16_t)((temperature - (float64_t)(temperatureInt)) * 100);
 *
 *    iotx_printf(KSYST, "Temp = %d.%d\n", temperatureInt, temperatureFrc);
 * \endcode
 *
 * \param[in]	*temperature	Ptr on the temperature
 * \param[out]	KTMP0NOERR		OK
 *
 */
extern	int32_t	tmp0_getTemperature(float64_t *temperature);

/*!
 * \brief Set the temperature
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * float64_t    temperature = 37.6;
 * uint16_t     temperatureInt, temperatureFrc;
 *
 *    status = tmp0_setTemperature(temperature);
 * \endcode
 *
 * \param[in]	temperature	The temperature
 * \param[out]	KTMP0NOERR	OK
 *
 */
extern	int32_t	tmp0_setTemperature(float64_t temperature);

#ifdef	__cplusplus
}
#endif

// tmp0 manager errors
// -------------------

#define	KTMP0NOERR	0				// No error
#define	KTMP0SYCNA	-(KTMP0ERR+1)	// System call not available
#define	KTMP0GEERR	-(KTMP0ERR+2)	// General error
#define	KTMP0CHBSY	-(KTMP0ERR+3)	// The manager is busy

/*@}*/

#endif
