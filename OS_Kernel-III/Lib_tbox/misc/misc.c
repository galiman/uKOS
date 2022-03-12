/*
; misc.
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
; Goal:		misc manager.
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

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc         misc manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "misc manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_misc_init(void);
extern	int32_t	stub_misc_getMode(uint32_t *mode);
extern	int32_t	stub_misc_onLed(uint8_t ledNb);
extern	int32_t	stub_misc_offLed(uint8_t ledNb);
extern	int32_t	stub_misc_toggleLed(uint8_t ledNb);
extern	int32_t	stub_misc_muteLed(bool_t mute);
extern	int32_t	stub_misc_getAddressExtension(volatile void **extension);
extern	int32_t	stub_misc_getExtension(uint8_t *byte, uint8_t offset);
extern	int32_t	stub_misc_putExtension(uint8_t byte, uint8_t offset);
extern	int32_t	stub_misc_installException(void (*code)(void), uint8_t exceptionNb);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Misc, KIDTBOX, KMISCNUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Get the configuration mode
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    mode;
 *
 *    status = misc_getMode(&mode);
 * \endcode
 *
 * \param[in]	*mode		Ptr on the jumper value
 * \param[out]	KMISCNOERR	OK
 *
 */
int32_t	misc_getMode(uint32_t *mode) {

	_init();
	return (stub_misc_getMode(mode));
}

/*
 * \brief Turn on a LED
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    ledNb;
 *
 *     status = misc_onLed(ledNb);
 * \endcode
 *
 * \param[in]	ledNb		LED number
 * \param[out]	KMISCNOERR  OK
 * \param[out]	KMISCNODEV	The selected LED does not exist
 *
 */
int32_t	misc_onLed(uint8_t ledNb) {

	_init();
	return (stub_misc_onLed(ledNb));
}

/*
 * \brief Turn off a LED
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    ledNb;
 *
 *    status = misc_offLed(ledNb);
 * \endcode
 *
 * \param[in]	ledNb		LED number
 * \param[out]	KMISCNOERR  OK
 * \param[out]	KMISCNODEV	The selected LED does not exist
 *
 */
int32_t	misc_offLed(uint8_t ledNb) {

	_init();
	return (stub_misc_offLed(ledNb));
}

/*
 * \brief Change the state of a LED
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    ledNb;
 *
 *    status = misc_toggleLed(ledNb);
 * \endcode
 *
 * \param[in]	ledNb		LED number
 * \param[out]	KMISCNOERR  OK
 * \param[out]	KMISCNODEV	The selected LED does not exist
 *
 */
int32_t	misc_toggleLed(uint8_t ledNb) {

	_init();
	return (stub_misc_toggleLed(ledNb));
}

/*
 * \brief General control of the LEDs
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = misc_muteLed(TRUE);  //
 *                                  //
 *                                  //
 *    status = misc_muteLed(FALSE); // Portion of code without any LED activity
 * \endcode
 *
 * \param[in]	mute		(TRUE -> turn off all the LEDs, FALSE -> normal mode)
 * \param[out]	KMISCNOERR  OK
 *
 */
int32_t	misc_muteLed(bool_t mute) {

	_init();
	return (stub_misc_muteLed(mute));
}

/*
 * \brief Get the address of the extension bus
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    *extension;
 *
 *    status = misc_getAddressExtension((void **)&extension);
 *    extension = 0x25;
 * \endcode
 *
 * \param[in]	**extension	Ptr on the extension bus
 * \param[out]	KMISCNOERR	OK
 * \param[out]	KMISCSYCNA	System call not available
 *
 */
int32_t	misc_getAddressExtension(volatile void **extension) {

	_init();
	return (stub_misc_getAddressExtension(extension));
}

/*
 * \brief Get a byte from the extension bus
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    byte;
 *
 *    status = misc_getExtension(&byte, 0x25);
 * \endcode
 *
 * \param[in]	*byte		Ptr on the byte
 * \param[in]	offset		Relative extension bus offset
 * \param[out]	KMISCNOERR	OK
 * \param[out]	KMISCSYCNA	System call not available
 *
 */
int32_t	misc_getExtension(uint8_t *byte, uint8_t offset) {

	_init();
	return (stub_misc_getExtension(byte, offset));
}

/*
 * \brief Put a byte to the extension bus
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    byte;
 *
 *    byte = 0x23;
 *    status = misc_putExtension(byte, 0x87);
 * \endcode
 *
 * \param[in]	byte		Byte
 * \param[in]	offset		Relative extension bus offset
 * \param[out]	KMISCNOERR	OK
 * \param[out]	KMISCSYCNA	System call not available
 *
 */
int32_t	misc_putExtension(uint8_t byte, uint8_t offset) {

	_init();
	return (stub_misc_putExtension(byte, offset));
}

/*
 * \brief Install a routine for an exception
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = misc_installException(addressRoutine, 23);
 * \endcode
 *
 * \param[in]	*code		Ptr on the exception function
 * \param[in]	exceptionNb	Exception number
 * \param[out]	KMISCNOERR	OK
 * \param[out]	KMISCNODEV	The selected exception does not exist
 *
 */
int32_t	misc_installException(void (*code)(void), uint8_t exceptionNb) {

	_init();
	return (stub_misc_installException(code, exceptionNb));
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

		stub_misc_init();
	}
}

