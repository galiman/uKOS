/*
; init.
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
; Goal:		Low level init for the uKOS CSEM_icycam module.
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

LOC_CONST_STRG(aStrApplication[]) =	"init         First hardware initializations.           (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Init\n"
									"====\n\n"

									"This code places in a quite state the hardware resources.\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Init, KIDSTARTUP, KINITNUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief init_init
 *
 * - Initialize some basic periphs
 * - GPIO, watchdog, SDRAM
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	init_init(void) {
	uint16_t	bus;

// Initialize the GPIO (for the pbus)

	*GPIO_IRQ_CONFIG_1 = 0x0000;										// Irqs level sensitive
	*GPIO_PULL_UP = 0x0000;												// Disconnect the pull-ups
	bus = *GPIO_DATAIN | ((1<<BRW) & ~(1<<BCS));						// Bus state
	*GPIO_DATAOUT_EN = KDIRIN;											// Direction IN
	*GPIO_DATAOUT = bus;												// Place the RD

// Disable the watchdog

	*WDTIMER_UNLOCK	= UNLOCK_WORD;										// Magic word
	*WDTIMER_CTRL = 0x00;												// Disable the watchdog
	
// Initialization of the SDRAM

  	*SDRAMCTRL_MR = CAS_LETENCY_2;										// 00 = base mode register
																		// 0 = programmed burst length
																		// 00 = normal operation
																		// 2 = cas latency
																		// 0 = Sequential
																		// 000 = 1
  	*SDRAMCTRL_EMR = MODE_EXTENDED;										// 10 = Extended mode register
																		// 00 = full-strength driver
																		// 000 = full array
  	*SDRAMCTRL_IRQ0_ENABLE = (1<<BIRQ_INIT_DONE);						// Power-up sequence done
  	*SDRAMCTRL_CTRL = (SIZE_9 | (1<<BSDRAM_INIT) | (1<<BSDRAM_ENABLE));	// 00 = 9-bit
																		// Start Initialize sequence
																		// Enable SDRAM controller

	while (!(*SDRAMCTRL_IRQ_STATUS & (1<<BIRQ_INIT_DONE)));
  	*SDRAMCTRL_IRQ0_ENABLE = 0x00;										// Power-up sequence done
}

