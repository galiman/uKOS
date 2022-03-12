/*
; misc_adco.
; ==========

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
;			Waiting 10-s, then , launch the processes
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Every 20-ms
;				  Read the ADC channel 0, 1, 2
;				  Display the values
;
;			Used for the scope observation of the task slot used by
;			the processes.
;
;			1. rm -rf /tmp/pipe
;			2. dump -d2x FTWS6VG0D 460800 10000 -display -pipe /tmp/pipe
;			3. By eclipse, connect Impulse to /tmp/pipe
;			4. Download a uKOS application (i.e. misc_adco)
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

#if (defined(__Discovery_407__))
#define	CHANN	KURT1
#endif

#if (defined(__Discovery_429__))
#define	CHANN	KURT1
#endif

#if (defined(__LAP_icycam_103__))
#define	CHANN	KURT0
#endif


// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_adco    Example of how to use the ADC.            (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_adco\n"
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
 * - P1: - Every 20-ms
 *			- Read the ADC channel 0, 1, 2
 *			- Display the values
 *
 */
static	void	aProcess_1(const void *argument) {
	float64_t	reference, channel_0, channel_1, channel_2;
	float64_t	vBat, vRef, vSen, temperature;
	uint32_t	nbData = 1;

	while (TRUE) {
		kern_suspendProcess(20);

		adc0_getAnalog(0, &reference, &channel_0);
		adc0_getAnalog(1, &reference, &channel_1);
		adc0_getAnalog(2, &reference, &channel_2);

		#if (defined(__STM32F103__))

// Now compute the temperature of the core
//
// T(C) = [(V25 - VSen)/Avg_Slope] + 25
// With:
// V25		 = 1.43V
// VSen		 = measure
// Avg_Slope = 0.0043 V/C
//
// VBat internal = VBat external

		vBat = channel_2;
		vRef = channel_1;
		vSen = channel_0;
		temperature = ((1.43 - vSen)/0.0043) + 25.0;
		#endif
	
		#if (defined(__STM32F407__))

// Now compute the temperature of the core
//
// T(C) = [(VSen - V25)/Avg_Slope] + 25
// With:
// V25		 = 0.76V
// VSen		 = measure
// Avg_Slope = 0.0025 V/C
//
// VBat internal = VBat external / 2

		vBat = channel_2 * 2;
		vRef = channel_1;
		vSen = channel_0;
		temperature = ((vSen - 0.76)/0.0025) + 25.0;
		#endif

		#if (defined(__STM32F429__))

// Now compute the temperature of the core
//
// T(C) = [(VSen - V25)/Avg_Slope] + 25
// With:
// V25		 = 0.76V
// VSen		 = measure
// Avg_Slope = 0.0025 V/C
// VBat internal = VBat external / 4

		vBat = channel_2 * 4;
		vRef = channel_1;
		vSen = channel_0;
		temperature = ((vSen - 0.76)/0.0025) + 25.0;
		#endif

		iotx_printf(CHANN, "%d %d.%02d %d.%02d %d.%02d\n", nbData++, FLOAT_2(vBat), FLOAT_2(temperature), FLOAT_2(vRef));
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
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKNLB, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKNLB, aProcess_1, aStrIden_1, CHANN, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

