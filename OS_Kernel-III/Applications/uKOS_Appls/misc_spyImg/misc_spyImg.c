/*
; misc_spyImg.
; ============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Once
;				  Read the imager values
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

#define	__LIMITED__

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_spyImg  Example of how to use an imager.          (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_spyImg\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Read the imager values
 *
 */
static	void	aProcess_1(const void *argument) {
			uint8_t		regNumber, i = 0;
			uint16_t	regValue;

	#if (defined(__LIMITED__))
	const	uint8_t		aTabAptina[] = {       0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
										                                                                   0x1B, 0x1C,
										                                                                         0x2C,
										       0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
										                                     0x46, 0x47, 0x48,                   0x4C,
										 0x70,       0x72, 0x73, 0x74,                                                             0x7F,
										 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
										 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
										 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,       0xA8, 0xA9,       0xAB,                   0xAF,
										 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,                               0xBD, 0xBE, 0xBF,
										 0xC0,       0xC2, 0xC3, 0xC4, 0xC5,
										 0xF0,                                                                               0xFE,
										 0x00
						};

	#else
	const	uint8_t		aTabAptina[] = {       0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
										 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
										 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
										 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
										                                     0x46, 0x47, 0x48,                   0x4C,
										 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,                         0x6C,
										 0x70, 0x71, 0x72, 0x73, 0x74,                                                             0x7F,
										 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
										 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
										 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB,                   0xAF,
										 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,                               0xBD, 0xBE, 0xBF,
										 0xC0,       0xC2, 0xC3, 0xC4, 0xC5,
										 0xF0, 0xF1,                                                                         0xFE,
										 0x00
						};
	#endif

	kern_suspendProcess(2000);

	iotx_printf(KSYST, "static  const   mt9v03x_t   aTabxxxyyyz[] = {\n            ");
	while (TRUE) {
		regNumber = aTabAptina[i++];
		if ((i > 0) && (regNumber == 0)) {
			iotx_printf(KSYST, "\n        };\n");
			while (TRUE) { kern_switchFast(); }
		}

		img0_getRegister(regNumber, &regValue);
		iotx_printf(KSYST, "{ 0x%02X, 0x%04X }, ", regNumber, regValue);
		if ((i % 8) == 0) {
			iotx_printf(KSYST, "\n            ");
		}
	}
}

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {
	volatile	proc_t 	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),  0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

