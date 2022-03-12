/*
; mpy.
; ====

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
;			uKOS interface for Micro-Python (www.micropython.com).
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

#define	KSIZEBUFMPY	90000

static				uint32_t	vSize = KSIZEBUFMPY;
static				uint8_t		*vMemory;
static	volatile	uint8_t		vLocation = KEXTERNAL;
static	volatile	uint32_t	vCommManager = KSYST;
static	volatile	bool_t		vLock = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"mpy          Micro-Python for uKOS.                    (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Sysquake Embedded for uKOS\n"
									"==========================\n\n"

									"This tool allows to use Micro-Python\n"
									" (www.micropython.com) in the uKOS workspace.\n\n"

									"Input format:  mpy commManager, {urt0, ... }\n"
							        "                   memoryLocation, {-internal | -external}\n"
							        "                   memorySize\n"
									"Output format: mpy results\n\n";

LOC_CONST_STRG(aStrIden[])        = "Process_mpy";
LOC_CONST_STRG(aStrText[])        = "Process Micro-Python (www.micropython.com)(c) EFr-2017";

// Prototypes
// ==========

static	void	_process(const void *argument);
static	int32_t	prgm(uint32_t argc, const char_t *argv[]);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Mpy, KIDTOOL, KMPYNUM, prgm, "1.3", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				bool_t		equals;
				int32_t		status;
				enum 		{ KERRNOT, KERRINA, KERRNME, KERRLOK } error = KERRNOT;
	volatile	proc_t 		*process;

// Analyze the command line
// ------------------------
//
// Example:
//
// sqe urt1 -internal 62000	use the urt1 for the prompt
//						    execution in the internal memory
//						    SQE workspace of 62000-byte

 	switch (argc) {
		case 4: {
			vCommManager = GET_PTR_32(argv[1]);
			text_checkAsciiBuffer(argv[2], "-internal", &equals); if (equals) { vLocation = KINTERNAL; break; }
			text_checkAsciiBuffer(argv[2], "-external", &equals); if (equals) { vLocation = KEXTERNAL; break; }
			text_checkAsciiBuffer(argv[2], "-ccoupled", &equals); if (equals) { vLocation = KCCOUPLED; break; }
			error = KERRINA;
			break;
		}
		default: {
			error = KERRINA;
			break;
		}
	}

// Only 1 mpy session at time
// Try to reserve the MPY memory segment

	if (vLock) {
		error = KERRLOK;
	}
	else {
		vLock = TRUE;

		strg_cnvtDecAsciiToBufInt32((int32_t *)&vSize, argv[3], 1);
		vMemory = (uint8_t *)syos_malloc(vLocation, vSize);
		if (vMemory == NULL) {
			error = KERRNME;
		}
	}

	switch (error) {
		case KERRNOT: {
			iotx_printf(KSYST, "Micro-Python launcher.\n");
			iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

			PROC_SUPV_STACKMALLOC(0, KINTERNAL, vSpecification_0, aStrText, KSZSTACKMPY, _process, aStrIden, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

			if (kern_createProcess(&vSpecification_0, NULL, &process) != KKERNNOERR) { status = EXIT_FAILURE;    vLock = FALSE; break; }
			else															   		 { status = EXIT_SUCCESS_OS;                break; }
		}
		case KERRINA: { iotx_printf(KSYST, "Incorrect arguments.\n\n");                status = EXIT_FAILURE;    vLock = FALSE; break; }
		case KERRNME: { iotx_printf(KSYST, "Not enough memory.\n\n");                  status = EXIT_FAILURE;    vLock = FALSE; break; }
		case KERRLOK: { iotx_printf(KSYST, "MPY session is running.\n\n");             status = EXIT_FAILURE;    vLock = FALSE; break; }
		default:	  {													               status = EXIT_FAILURE;    vLock = FALSE; break; }
	}
	return (status);
}

// Local routines
// ==============

/*
 * \brief _process
 *
 * - PSQE: - until "quit"
 *  			- Analyse the command line
 *				- Execute the command
 *
 */
static	void	_process(const void *argument) {
				cnfMpyt_t	configure;
	volatile	proc_t 		*process;

// Set the communication channel

	kern_getProcessRun(&process);
	process->oSpecification.oCommManager = vCommManager;

	configure.oSize   = vSize;
	configure.oMemory = vMemory;
	mpyt_configure(&configure);

// Management of the Command Input Line (until "quit")

	MPYT_CMDLINE;

	vLock = FALSE;
	iotx_printf(KSYST, "Terminated\n");
	syos_free((void *)vMemory);
	exit(EXIT_SUCCESS);
}

