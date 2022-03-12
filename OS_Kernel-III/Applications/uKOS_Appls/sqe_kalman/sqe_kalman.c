/*
; sqe_kalman.
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
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Every 10-ms
;				  Display the Kalman result a = f(accelerometer, gyroscope)
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
#include	<stdio.h>
#include	<math.h>

#define 	KPI		(3.14159265358979)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"sqe_kalman   Example of how to use the Kalman / SQE.   (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  sqe_kalman\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

#if (defined(__ROMABLE__))
static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
#endif

// Module specifications (See the uKOS_module.h)
// =============================================

#if (defined(__ROMABLE__))
MODULE(UserAppl, KIDTOOL,        KROMABLE0,       prgm,   "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

#else
MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));
#endif

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- Toggle LED 0
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
 * - P1: - Every 10-ms
 *  		- Display the Kalman result a = f(acce, gyro)
 *
 */
static	void	aProcess_1(const void *argument) {
		uint32_t	size, sample = 0;
		uint8_t		*memory;
		cnfImu0_t	configureIMU0;
		accePack_t	accelerometer;
		gyroPack_t	gyroscope;
		magnPack_t	magnetometer;
		float64_t	bias, angle, acceAngle;
		cnfSqee_t	configure;
		ioSqee_t	data, *ref;

	NLIB_PROCESS_INIT;

// Configure the IMU

	configureIMU0.oAcceMode = K4G;
	configureIMU0.oGyroMode = K2000DPS;
	configureIMU0.oMagnMode = K8GAUSS;
	imu0_configure(&configureIMU0);

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

	SQEE_COMPUTE(ref, "define    KQangle       = 0.01;\n"
					  "define    KQgyro        = 0.03;\n"
					  "define    KRangle       = 0.02;\n"
					  "define    KTSAMPLING    = 0.0010;\n"
					  "define    KINIANGLE     = -90;\n"
					  "define    KINIBIAS      = 0;\n"

					  "A = [1, -KTSAMPLING; 0, 1];\n"
					  "B = [KTSAMPLING; 1];\n"
					  "H = [1, 0];\n"
					  "Q = [KQangle, 0; 0, KQgyro];\n"
					  "I = [1, 0; 0, 1];\n"
					  "P = [0, 0; 0, 0];\n"
					  "X = [KINIANGLE; KINIBIAS];\n"

					  "function (P, X) = _kalman(A, B, H, Q, I, P, X, acceAngle, gyroRate);\n"

					  "// Prediction step;\n"
					  "    X = A * X + B * gyroRate;\n"
					  "    P = A * P * A' + Q;\n"

					  "// Observation step;\n"
					  "    Y = acceAngle - H * X;\n"
					  "    S = H * P * H' + KRangle;\n"

					  "// Update step;\n"
					  "    K = (P * H') / S;\n"
					  "    X = X + K * Y;\n"
					  "    P = (I - K * H) * P;\n";);

	while (TRUE) {
		kern_suspendProcess(10);

// Get the IMU data & compute the angle

		imu0_getImu(&accelerometer, &gyroscope, &magnetometer);
		acceAngle = atan2(accelerometer.oAcce_X,
					      sqrt((accelerometer.oAcce_Y * accelerometer.oAcce_Y) +
					           (accelerometer.oAcce_Z * accelerometer.oAcce_Z))) * 180.0 / KPI;

// Fed the SQE model

		SQEE_PUT(ref, K1DARRAY, 0, 0, 1, &acceAngle,         "acceAngle = xdata;");
		SQEE_PUT(ref, K1DARRAY, 0, 0, 1, &gyroscope.oGyro_Y, "gyroRate  = xdata;");

// Compute the filter

		SQEE_COMPUTE(ref, "(P, X) = _kalman(A, B, H, Q, I, P, X, acceAngle, gyroRate);\n");

// Get the results & print them

		SQEE_GET(ref, K1DARRAY, 0, 0, 1, &angle, "xdata(X(1,:));");
		SQEE_GET(ref, K1DARRAY, 0, 0, 1, &bias,  "xdata(X(2,:));");
		printf("%ld %f %f %f\n", ++sample, acceAngle, bias, angle);
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
#if (defined(__ROMABLE__))
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {

#else
int		main(void) {
#endif

	volatile	proc_t 	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

	NLIB_INIT;		// Initialize the newlib ptr

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKNLB, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKSQE, aProcess_1, aStrIden_1, KURT1, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

