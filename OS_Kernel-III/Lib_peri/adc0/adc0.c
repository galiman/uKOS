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

#include 	<uKOS.h>

static	bool_t	vReserved = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"adc0         adc0 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "adc0 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_adc0_init(void);
extern	int32_t	stub_adc0_getAnalog(uint8_t channel, float64_t *reference, float64_t *data);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Adc0, KIDPERI, KADC0NUM, NULL, "2.0", (1<<BSHOW));

/*
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
int32_t	adc0_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KADC0CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KADC0NOERR);
}

/*
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
int32_t	adc0_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KADC0NOERR);
}

/*
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
int32_t	adc0_getAnalog(uint8_t channel, float64_t *reference, float64_t *data) {

	_init();
	return (stub_adc0_getAnalog(channel, reference, data));
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

		stub_adc0_init();
	}
}

