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

#include 	<uKOS.h>

static	bool_t	vReserved  = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"tmp0         tmp0 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "tmp0 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_tmp0_init(void);
extern	int32_t	stub_tmp0_getTemperature(float64_t *temperature);
extern	int32_t	stub_tmp0_setTemperature(float64_t temperature);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Tmp0, KIDPERI, KTMP0NUM, NULL, "2.0", (1<<BSHOW));

/*
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
int32_t	tmp0_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KTMP0CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KTMP0NOERR);
}

/*
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
int32_t	tmp0_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KTMP0NOERR);
}

/*
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
int32_t	tmp0_getTemperature(float64_t *temperature) {

	_init();
	return (stub_tmp0_getTemperature(temperature));
}

/*
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
int32_t	tmp0_setTemperature(float64_t temperature) {

	_init();
	return (stub_tmp0_setTemperature(temperature));
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

		stub_tmp0_init();
	}
}

