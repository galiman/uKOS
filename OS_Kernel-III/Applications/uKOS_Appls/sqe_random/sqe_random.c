/*
; sqe_random.
; ==========

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
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 1 processes:
;
;			- P0: Launch sqe
;			      Every 1000-ms
;				  Ask sqe for a random 3D array
;				  Toggle LED 0
;
;   (c) 1992-2017, Franzi Edo.
;   --------------------------
;
;   Developed for use with Sysquake Embedded.
;   Sysquake is a trademark of Calerga Sarl, used with permission. 
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

#define	KNBZS		3			// Number of z
#define	KNCOLS		5			// Number of cols
#define	KNROWS		4			// Number of rows

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sqe_random   Example of how to use sqe.                (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sqe_random\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Launch the Sysquake Embedded Process
 *		 - Every 1000-ms
 *			- Ask sqe for a random 3D array
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
	uint32_t	size;
	uint8_t		*memory, z, row, col;
	float64_t	result[KNBZS][KNROWS][KNCOLS];
	cnfSqee_t	configure;
	ioSqee_t	data;

// Try to reserve the SQE memory segment

	size   = 90000;
	memory = (uint8_t *)syos_malloc(KINTERNAL, size);
	if (memory == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	configure.oSize   = size;
	configure.oMemory = memory;
	sqee_configure((cnfSqee_t *)&configure);

	data.oSqeeRef = configure.oSqeeRef;

	while (TRUE) {
		kern_suspendProcess(1000);

// Generate a random 3D array
//  rows  = 5
//  lines = 4
//  z     = 3

		data.oAction 		= KCOMPUTE;					// Compute ...
		data.oSqeeCommand	= "result = rand(4,5,3)";	// The command
		sqee_exchangeData((ioSqee_t *)&data);			// Execution

// Asking for the 3D matrix

		data.oAction 		= KGET;						// Get ...
		data.oNbDimensions	= K3DARRAY;					// 3D array
		data.oDimensions[0]	= KNROWS;					// Rows
		data.oDimensions[1]	= KNCOLS;					// Cols
		data.oDimensions[2]	= KNBZS;					// z
		data.oGetData  		= &result[0][0][0];			// The 3D array result
		data.oSqeeCommand	= "xdata(result);";			// The command
		sqee_exchangeData((ioSqee_t *)&data);			// Execution

// Display the results in integer (result * 1000)

		iotx_printf(KSYST, "Process answer ...\n");
		for (z = 0; z < KNBZS; z++) {
			for (row = 0; row < KNROWS; row++) {
				for (col = 0; col < KNCOLS; col++) {
					iotx_printf(KSYST, "%d ", (int32_t)(result[z][row][col]*1000));
				}
				iotx_printf(KSYST, "\n");
			}
			iotx_printf(KSYST, "\n");
		}
		iotx_printf(KSYST, "\n");
		misc_toggleLed(0);
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
	volatile	proc_t 	*process_0;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKSQE, aProcess_0, aStrIden_0, KURT0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

