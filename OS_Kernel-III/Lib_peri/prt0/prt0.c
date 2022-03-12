/*
; prt0.
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
; Goal:		prt0 SLAVE manager.
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

LOC_CONST_STRG(aStrApplication[]) =	"prt0         prt0 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "prt0 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_prt0_init(void);
extern	int32_t	stub_prt0_writeCommand(const cnfPrt0_t *command);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Prt0, KIDPERI, KPRT0NUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Reserve the prt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (prt0_reserve(KDEVALL) == KPRT0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    prt0_xyz();
 *    status = prt0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KPRT0NOERR	The manager is reserved
 * \param[out]	KPRT0CHBSY 	The manager is busy
 *
 */
int32_t	prt0_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KPRT0CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KPRT0NOERR);
}

/*
 * \brief Release the prt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = prt0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KPRT0NOERR	OK
 *
 */
int32_t	prt0_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KPRT0NOERR);
}

/*
 * \brief Write a command to the prt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * uint8_t      buffer[256];
 * cnfPrt0_t    command = {
 *                  &buffer[0],
 *                  256,
 *                  KWTPAYLOAD
 *              };
 *
 *    status = prt0_writeCommand(&command);
 * \endcode
 *
 * \param[in]	*command	Ptr on the prt0 command
 * \param[out]	KPRT0NOERR	OK
 * \param[out]	KPRT0LNBUW	The buffer length is wrong
 * \param[out]	KPRT0LNBUB	The buffer length is too big
 * \param[out]	KPRT0LNBU0	The buffer length is = 0
 * \param[out]	KPRT0PALPH	The pay-load phase is on progress
 * \param[out]	KPRT0CMNSU	Command not supported
 *
 */
int32_t	prt0_writeCommand(cnfPrt0_t *command) {

	_init();
	return (stub_prt0_writeCommand(command));
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

		stub_prt0_init();
	}
}

