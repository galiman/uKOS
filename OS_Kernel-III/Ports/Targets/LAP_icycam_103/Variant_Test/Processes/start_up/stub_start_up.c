/*
; stub_start_up.
; ==============

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
; Goal:		start_up process; execute some important initializations
;			before jumping to the selected function.
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

// Bootstrap function table
// ------------------------

typedef	struct	boot	boot_t;

struct	boot {
				uint8_t		oSW;										// Switch value
		const	char_t		*oFunction;									// Ptr on the function
				uint8_t		oBaudrate;									// Baudrate
				uint32_t	oCommManager;								// Default communication manager
				uint8_t		oArgC;										// Number of arguments
		const	char_t		**oArgV;									// Ptr on the arguments
		};

static	const	char_t	*argv_cnsUrt0[] = { "console", "urt0" };
static	const	char_t	*argv_sloader[] = { "sloader", "-run" };

static	const	boot_t	aFunction[] = {
							{ 0x00, "console", KBD460800, KURT0, 2, argv_cnsUrt0 },
							{ 0x01, "sloader", KBD460800, KURT0, 2, argv_sloader }
						};

#define	KDEFCOMM	KURT0
#define	KNBFNCT		(sizeof(aFunction)/sizeof(boot_t))

// Module strings
// ==============

GLB_CONST_STRG(aStrHelp[]) =	"Start-up process\n"
								"================\n\n"

								"Start_up switch action. The default settings are:\n"
								"460800-bits/s, 8-bits, 2-stop-bits, no parity.\n\n"

								"   SW1\n"
								"    0   KDEF0, console (460800-bit/s).\n"
								"    1   KDEF0, sloader (460800-bit/s).\n\n";

LOC_CONST_STRG(aStrLogo[]) = 	KSTRLOGO

/*
 * \brief stub_start_up_launch
 *
 */
void	stub_start_up_launch(void) {
			uint8_t		i;
			uint16_t	index;
			uint32_t	mode;
			cnfUrtx_t	configureURTx;
			bool_t		error = FALSE;
	const	module_t	*module;
	const	char_t		*identifier;

// Configure by default all the communication managers
// Set the default communication device (KSYST)

	comm_setCommDev(KDEFCOMM);

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBDDEFAULT;
	configureURTx.oKernSync = (1<<BRXSEMA);
	urt0_configure(&configureURTx);

	misc_getMode(&mode); mode >>= 8;
	if (mode >= KNBFNCT) {
		mode = 0;
	}

// Bootstrap ...
// -------------

// Launch all the possible applications

	glob_getIdentifier(&identifier);
	iotx_printf(KSYST, "%s", aStrLogo);
	iotx_printf(KSYST, "%ssw = %X\n\n", identifier, mode);
	kern_suspendProcess(500);

	for (i = 0; i < KNBFNCT; i++) {
		if (aFunction[i].oSW == mode) {

// The communication
// The last available function in the table is considered for the default communication manager

			comm_setCommDev(aFunction[i].oCommManager);
			switch (aFunction[i].oCommManager) {
				case KURT0: { configureURTx.oBaudRate = aFunction[i].oBaudrate; urt0_configure(&configureURTx); break; }
				default: {
					break;
				}
			}

// The mode exist
// Found a module; execute it or error

			if (syos_getNameModule(KMEM0, aFunction[i].oFunction, &index, &module) != KSYOSNOERR) {
				if (syos_getNameModule(KMEM1, aFunction[i].oFunction, &index, &module) != KSYOSNOERR) {
					error = TRUE;
				}
			}

			if (error) {
				iotx_printf(KSYST, "Module not found or user memory busy by a running application.\n\n");
				while (TRUE) { kern_switchFast(); }
			}
			else {

				switch (module->oExecution(aFunction[i].oArgC, aFunction[i].oArgV)) {
					case EXIT_FAILURE_CRT0: {
						iotx_printf(KSYST, "Incompatible OS!!!\nReload the latest OS inside the target.\n");
						break;
					}
					default: {
						break;
					}
				}
			}
		}
	}

// Terminated; kill the start-up

	exit(EXIT_SUCCESS);
}

