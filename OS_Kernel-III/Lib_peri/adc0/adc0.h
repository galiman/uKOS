/*
; adc0.
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
; Goal:		adc0 manager.
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

#ifndef	__ADC0__
#define	__ADC0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KADC0MAN	(KADC0NUM<<8)
#define	KADC0ERR	((KIDPERI<<24) | KADC0MAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	ADC0_reserve	adc0_reserve
#define	ADC0_release	adc0_release

/*!
 * \brief Reserve the adc0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (adc0_reserve(KDEVALL) == KADC0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    adc0_xyz();
 *    status = adc0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KADC0NOERR	The manager is reserved
 * \param[out]	KADC0CHBSY 	The manager is busy
 *
 */
extern	int32_t	adc0_reserve(uint8_t mode);

/*!
 * \brief Release the adc0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = adc0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KADC0NOERR	OK
 *
 */
extern	int32_t	adc0_release(uint8_t mode);

/*!
 * \brief Get an analog channel
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * float64_t    reference, result[6];
 * uint8_t      channel;
 *
 *    for (channel = 0; channel < 6; channel++) {
 *        status = adc0_getAnalog(channel, &reference, &result[channel]);
 *        if (status != KADC0NOERR) {
 *            return (status);
 *        }
 *    }
 * \endcode
 *
 * \param[in]	channel		Channel (0..n)
 * \param[in]	*reference	Ptr on the reference of the A/D
 * \param[in]	*data		Ptr on the conversion result
 * \param[out]	KADC0NOERR	OK
 * \param[out]	KADC0NODEV	The selected channel does not exist
 *
 */
extern	int32_t	adc0_getAnalog(uint8_t channel, float64_t *reference, float64_t *data);

#ifdef	__cplusplus
}
#endif

// adc0 manager errors
// -------------------

#define	KADC0NOERR	0				// No error
#define	KADC0SYCNA	-(KADC0ERR+1)	// System call not available
#define	KADC0GEERR	-(KADC0ERR+2)	// General error
#define	KADC0CHBSY	-(KADC0ERR+3)	// The manager is busy
#define	KADC0NODEV	-(KADC0ERR+4)	// The device does not exist

/*@}*/

#endif
