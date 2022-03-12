/*
; stub_misc.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 184                  $:  Revision of last commit
; $Date:: 2017-07-17 10:50:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the "misc" manager module.
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

extern	void	(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
static	bool_t	vMute;

// Prototypes
// ==========

extern	void	pbus_read(uint16_t address, uint8_t *data);
extern	void	pbus_write(uint16_t address, uint8_t data);

/*
 * \brief stub_misc_init
 *
 * - Initialize some specific hardware parts:
 *   - The LEDs state
 *
 */
void	stub_misc_init(void) {

	INTERRUPTION_OFF;
	vMute = FALSE;

	pbus_write(LOCLEDS, 0);
	INTERRUPTION_RESTORED;
}

/*
 * \brief stub_misc_getMode
 *
 * - Get the jumper configuration
 *
 */
int32_t	stub_misc_getMode(uint32_t *mode) {
	uint8_t		switches, rotaryHex;

	INTERRUPTION_OFF;
	pbus_read(LOCSWITCHES,  &switches);
	pbus_read(LOCHEXSWITCH, &rotaryHex);
	*mode = (uint32_t)((rotaryHex & 0x0F)<<8) | (uint32_t)switches;
	RETURN_INT_RESTORED(KMISCNOERR);
}

/*
 * \brief stub_misc_onLed
 *
 * - Turn on a LED
 *
 */
int32_t	stub_misc_onLed(uint8_t ledNb) {
	uint8_t		led;

	INTERRUPTION_OFF;
	if (vMute)           { RETURN_INT_RESTORED(KMISCNOERR); }
	if (ledNb >= KNBLED) { RETURN_INT_RESTORED(KMISCNODEV); }

	pbus_read(LOCLEDS, &led);
	led |= (1<<ledNb);
	pbus_write(LOCLEDS, led);
	RETURN_INT_RESTORED(KMISCNOERR);
}

/*
 * \brief stub_misc_offLed
 *
 * - Turn off a LED
 *
 */
int32_t	stub_misc_offLed(uint8_t ledNb) {
	uint8_t		led;

	INTERRUPTION_OFF;
	if (vMute)           { RETURN_INT_RESTORED(KMISCNOERR); }
	if (ledNb >= KNBLED) { RETURN_INT_RESTORED(KMISCNODEV); }

	pbus_read(LOCLEDS, &led);
	led &= ~(1<<ledNb);
	pbus_write(LOCLEDS, led);
	RETURN_INT_RESTORED(KMISCNOERR);
}

/*
 * \brief stub_misc_toggleLed
 *
 * - Change the state of a LED
 *
 */
int32_t	stub_misc_toggleLed(uint8_t ledNb) {
	uint8_t		led;

	INTERRUPTION_OFF;
	if (vMute)           { RETURN_INT_RESTORED(KMISCNOERR); }
	if (ledNb >= KNBLED) { RETURN_INT_RESTORED(KMISCNODEV); }

	pbus_read(LOCLEDS, &led);
	led ^= (1<<ledNb);
	pbus_write(LOCLEDS, led);
	RETURN_INT_RESTORED(KMISCNOERR);
}

/*
 * \brief stub_misc_muteLed
 *
 * - Control (general) of the LEDs
 *
 */
int32_t	stub_misc_muteLed(bool_t mute) {

	if (!mute) { vMute = FALSE; return (KMISCNOERR); }

	INTERRUPTION_OFF;
	vMute = TRUE;

	pbus_write(LOCLEDS, 0);
	RETURN_INT_RESTORED(KMISCNOERR);
}

/*
 * \brief stub_misc_getAddressExtension
 *
 * - Get the address of the extension bus
 *
 */
int32_t	stub_misc_getAddressExtension(volatile void **extension) {

	*extension = NULL;
	return (KMISCSYCNA);
}

/*
 * \brief misc_getExtension
 *
 * \brief Get a byte from the extension bus
 *
 */
int32_t	stub_misc_getExtension(uint8_t *byte, uint8_t offset) {

	*byte = 0;
	return (KMISCSYCNA);
}

/*
 * \brief misc_putExtension
 *
 * \brief Put a byte to the extension bus
 *
 */
int32_t	stub_misc_putExtension(uint8_t byte, uint8_t offset) {

	return (KMISCSYCNA);
}

/*
 * \brief stub_misc_installException
 *
 * - Install an exception
 *
 */
int32_t	stub_misc_installException(void (*code)(void), uint8_t exceptionNb) {

	INTERRUPTION_OFF;
	if (exceptionNb >= KNBEXCEPTIONS) {
		RETURN_INT_RESTORED(KMISCNODEV);
	}

// Install the exception vector

	INTERRUPT_VECTOR(exceptionNb, code);
	RETURN_INT_RESTORED(KMISCNOERR);
}

