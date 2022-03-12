/*
; sqe_separe.
; ===========

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
;				  Compute a NN (simple 2 class separator)
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

#define	KL1NBIN		2			// Number of inputs of the layer 1
#define	KL1NBOUT	5			// Number of neurons of the layer 1
#define	KL2NBIN		KL1NBOUT	// Number of inputs of the layer 2
#define	KL2NBOUT	2			// Number of outputs

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sqe_separe   Example of how to use sqe.                (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sqe_separe\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Launch the Sysquake Embedded Process
 *		 - Every 1000-ms
 *			- Compute a NN (simple 2 class separator)
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
	uint32_t	size;
	uint8_t		*memory;
	float64_t	input[KL1NBIN];
	float64_t	output[KL2NBOUT];
	char_t		*result;
	cnfSqee_t	configure;
	ioSqee_t	data, *ref;
	float64_t	aL1_Weights[KL1NBOUT][KL1NBIN+1] = {
					{ -0.8037,  0.5713, -0.1455    },
					{  0.7032, -0.2452,  0.1388    },
					{ -2.0442,  2.1044, -4.4518e-2 },
					{  1.3805, -1.5087,  4.5242e-2 },
					{ -1.7718,  1.837,  -4.6865e-2 }
				};
	float64_t	aL2_Weights[KL2NBOUT][KL2NBIN+1] = {
					{ -0.8634,  0.3433, -1.8402,  1.2924, -2.1184, -0.1777 },
					{  0.4872, -0.6045,  2.3261, -1.6216,  1.3935,  0.1588 }
				};

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

	ref = &data;
	ref->oSqeeRef = configure.oSqeeRef;

// Load the program

	SQEE_COMPUTE(ref, "define    KL1NBIN       = 2;\n"
					  "define    KL1NBOUT      = 5;\n"
					  "define    KL2NBIN       = KL1NBOUT;\n"
					  "define    KL2NBOUT      = 2;\n"

					  "L1_input                = zeros(KL1NBIN+1, 1);\n"
					  "L1_input(KL1NBIN+1, 1)  = -1;\n"
					  "L1_activity             = zeros(KL1NBOUT, 1);\n"
					  "L1_output               = zeros(KL1NBOUT, 1);\n"
					  "L1_weight               = zeros(size(L1_output, 1), size(L1_input, 1));\n"

					  "L2_input                = zeros(KL2NBIN+1, 1);\n"
					  "L2_input(KL2NBIN+1, 1)  = -1;\n"
					  "L2_activity             = zeros(KL2NBOUT, 1);\n"
					  "L2_output               = zeros(KL2NBOUT, 1);\n"
					  "L2_weight               = zeros(size(L2_output, 1), size(L2_input, 1));\n"

					  "function (input, output, activity) = _layer(input, weight, data);\n"
					  "    input(1:size(input, 1)-1, 1) = data;\n"
					  "    activity                     = weight * input;\n"
					  "    output                       = tanh(activity);\n";);

// Load the weight set of the layer 1
// Load the weight set of the layer 2

	SQEE_PUT(ref, K2DARRAY, 0, KL1NBIN+1, KL1NBOUT, &aL1_Weights[0][0], "L1_weight = xdata;");
	SQEE_PUT(ref, K2DARRAY, 0, KL2NBIN+1, KL2NBOUT, &aL2_Weights[0][0], "L2_weight = xdata;");

	while (TRUE) {
		kern_suspendProcess(1000);

// Generate a random input vector -1..+1
// Compute the NN

		SQEE_COMPUTE(ref, "data = (rand(2,1)-0.5)*2;");

		SQEE_COMPUTE(ref, "(L1_input, L1_output, L1_activity) = _layer(L1_input, L1_weight, data);\n"
		                  "(L2_input, L2_output, L2_activity) = _layer(L2_input, L2_weight, L1_output);");

// Read the I/O of the NN

		SQEE_GET(ref, K1DARRAY, 0, 0, KL1NBIN,  &input[0],  "xdata(data);");
		SQEE_GET(ref, K1DARRAY, 0, 0, KL2NBOUT, &output[0], "xdata(L2_output);");

		if      ((output[0] >  0.2) && (output[1] < -0.2)) { result = "Class A";          }
		else if ((output[0] < -0.2) && (output[1] >  0.2)) { result = "Class B";          }
		else if ((output[0] > -0.2) && (output[1] <  0.2)) { result = "Not well classed"; }
		else 											   { result = "Undetermined";     }

// Inputs from -1 .. +1, so, we multiply input . 1000

		iotx_printf(KSYST, "inputs x=%5d.%03d, y=%5d.%03d, result: (%4d.%03d%% %4d.%03d%% ) %s\n", FLOAT_3(input[0] * 1000),
																         		 	               FLOAT_3(input[1] * 1000),
																         			               FLOAT_3(output[0] * 100),
																         			               FLOAT_3(output[1] * 100),
																        			               result);
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

