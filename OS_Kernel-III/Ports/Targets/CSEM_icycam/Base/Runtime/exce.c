/*
; exce.
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
; Goal:		Exceptions for the CSEM_icycam module.
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

#include	<uKOS.h>

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"exce         Exception management.                     (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Exce\n"
									"====\n\n"

									"This code manages the spurious exceptions.\n\n";

// Prototypes
// ==========

extern	void	pbus_write(uint16_t address, uint8_t data);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Exce, KIDSTARTUP, KEXCENUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief exce_display
 *
 * - In case of an exception occurs, its number (0..xx) is displayed
 *   on the leds
 *
 * \param[in]	exceptionNb	Number of the exception
 * \param[out]	-
 *
 */
void	exce_display(uint8_t exceptionNb) {
	uint32_t	time;

	while (TRUE) {
		for (time = 0; time < 1000000; time++) { NOP; }
		pbus_write(LOCLEDS, (0x80 + exceptionNb));

		for (time = 0; time < 1000000; time++) { NOP; }
		pbus_write(LOCLEDS, 0x00);
	}
}

