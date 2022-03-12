/*
; vis0.
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
; Goal:		vis0 manager.
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

LOC_CONST_STRG(aStrApplication[]) =	"vis0         vis0 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "vis0 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_vis0_init(void);
extern	int32_t	stub_vis0_configure(const cnfVis0_t *configure);
extern	int32_t	stub_vis0_acquisition(void);
extern	int32_t	stub_vis0_getImage(uint8_t page, void **image);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Vis0, KIDPERI, KVIS0NUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Reserve the vis0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (vis0_reserve(KDEVALL) == KVIS0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    vis0_xyz();
 *    status = vis0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KVIS0NOERR	The manager is reserved
 * \param[out]	KVIS0CHBSY 	The manager is busy
 *
 */
int32_t	vis0_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KVIS0CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KVIS0NOERR);
}

/*
 * \brief Release the vis0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = vis0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KVIS0NOERR	OK
 *
 */
int32_t	vis0_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KVIS0NOERR);
}

/*
 * \brief Configure the vis0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfVisx_t	configure = {
 *                          .oCommand  = KLUM8MSB;
 *                          .oAcqPage  = KPAGE0;
 *                          .oKernSync = (1<<BIMSEMA);
 *                      };
 *
 *    status = vis0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KVIS0NOERR	OK
 *
 */
int32_t	vis0_configure(const cnfVis0_t *configure) {

	_init();
	return (stub_vis0_configure(configure));
}

/*
 * \brief Acquisition of an image
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = vis0_acquisition();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KVIS0NOERR	OK
 * \param[out]	KVIS0TROPR 	Integration on progress
 *
 */
int32_t	vis0_acquisition(void) {

	_init();
	return (stub_vis0_acquisition());
}

/*
 * \brief Get the image pointer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    *image;
 *
 *     status = vis0_getImage(KPAGE0, &image);
 * \endcode
 *
 * \param[in]	page		The page number (KPAGE0, KPAGE1, KPPI, KNOXFER)
 * \param[in]	**image		Ptr on the image
 * \param[out]	KVIS0NOERR	OK
 * \param[out]	KVIS0PGNOE	The image page does not exist
 *
 */
int32_t	vis0_getImage(uint8_t page, void **image) {

	_init();
	return (stub_vis0_getImage(page, image));
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

		stub_vis0_init();
	}
}

