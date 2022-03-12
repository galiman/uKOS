/*
; process.
; ========

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
; Goal:		List the installed processes and overlays.
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
#include	"float.h"

typedef	struct	process	process_t;

struct	process {
			bool_t		oValid;											// Process valid
			spec_t		oSpecification;									// Process specification
			work_t		oInternal;										// Process internal stuff
			stts_t		oStatistic;										// uKernel statistic
			uint32_t	oPC;											// Process PC
		};

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"process      List the installed processes.             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "List the installed processes\n"
									"============================\n\n"

									"This tool displays some information concerning\n"
									"the installed processes.\n\n"

									"Input format:  process {-all | -noall}\n"
									"Output format: Process information\n\n";

extern volatile	proc_t		*vKern_current;							// Ptr on the current process

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_printParameter_P0(uint8_t number, const char_t *textID, uint8_t state, float64_t pRatio, float64_t kRatio, float64_t eRatio);
static	void	_printParameter_P1(uint8_t number, uint8_t priority, uint32_t mode, const char_t *textID, const char_t *identifierID);
static	void	_printParameter_T1(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio);
static	void	_printParameter_T2(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio);
static	void	_printParameter_T3(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio);
static	void	_printParameter_N1(uint64_t nbTimes);
static	void	_printParameter_S1(uint64_t nbCalls);
static	void	_printParameter_S2(uint32_t size);
static	void	_compose(const char_t *identifier, const char_t **idSpacer);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Process, KIDTOOL, KPROCESSNUM, prgm, "2.6", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				uint64_t	pRatio, kRatio, eRatio, totalTimeCPU;
				float64_t	pRatioF, kRatioF, eRatioF;
				bool_t		equals;
				enum		{ KALL, KNOALL } all;
				enum		{ KPROCNOERR, KPROCERPAR, KPROCENMEM } error = KPROCNOERR;
				int32_t		status;
				uint8_t		i, *bufSysProcess;
				process_t	*sysProcess;
	volatile	proc_t		*process;
	const		char_t		*functionName;

	iotx_printf(KSYST, "List of the system processes.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// process
// process -all
// process -noall

	switch (argc) {
		case 1: {
			all = KNOALL;
			break;
		}
		case 2: {
			text_checkAsciiBuffer(argv[1], "-noall", &equals); if (equals) { all = KNOALL; break; }
			text_checkAsciiBuffer(argv[1], "-all",   &equals); if (equals) { all = KALL;   break; }
	        error = KPROCERPAR;
	        break;
		}
		default: {
			error = KPROCERPAR;
			break;
		}
	}

	switch (error) {
		case KPROCNOERR: {
			totalTimeCPU = 0;

// Collect the process information usable for the statistics

			bufSysProcess = (uint8_t *)syos_malloc(KINTERNAL, KNBPROCESS*sizeof(process_t));
			if (bufSysProcess != NULL) {

				INTERRUPTION_OFF;
				for (i = 0; i < KNBPROCESS; i++) {
					sysProcess = (process_t *)((uint32_t)bufSysProcess + i*sizeof(process_t));
					sysProcess->oValid = FALSE;
					if (kern_getProcessByNb(i, &process) == KKERNNOERR) {
						sysProcess->oValid = TRUE;

						sysProcess->oSpecification.oIdentifier = process->oSpecification.oIdentifier;
						sysProcess->oSpecification.oText       = process->oSpecification.oText;
						sysProcess->oSpecification.oPriority   = process->oSpecification.oPriority;
						sysProcess->oSpecification.oMode       = process->oSpecification.oMode;

						sysProcess->oInternal.oState       	   = process->oInternal.oState;

						sysProcess->oStatistic.oNbExecutions   = process->oStatistic.oNbExecutions;
						sysProcess->oStatistic.oNbKernCalls    = process->oStatistic.oNbKernCalls;
						sysProcess->oStatistic.oAvStkSup       = process->oStatistic.oAvStkSup;
						sysProcess->oStatistic.oTimePMin       = process->oStatistic.oTimePMin;
						sysProcess->oStatistic.oTimePMax       = process->oStatistic.oTimePMax;
						sysProcess->oStatistic.oTimePAvg       = process->oStatistic.oTimePAvg;
						sysProcess->oStatistic.oTimeKMin       = process->oStatistic.oTimeKMin;
						sysProcess->oStatistic.oTimeKMax       = process->oStatistic.oTimeKMax;
						sysProcess->oStatistic.oTimeKAvg       = process->oStatistic.oTimeKAvg;
						sysProcess->oStatistic.oTimeEMin       = process->oStatistic.oTimeEMin;
						sysProcess->oStatistic.oTimeEMax       = process->oStatistic.oTimeEMax;
						sysProcess->oStatistic.oTimeEAvg       = process->oStatistic.oTimeEAvg;

						if (vKern_current->oSpecification.oStkSup != process->oSpecification.oStkSup) {
							kern_getPC(process, &sysProcess->oPC);
						}
						else {
							sysProcess->oPC = 0;
						}
					}
				}
				INTERRUPTION_RESTORED;

				for (i = 0; i < KNBPROCESS; i++) {
					sysProcess = (process_t *)((uint32_t)bufSysProcess + i*sizeof(process_t));
					if (sysProcess->oValid) {
						if ((sysProcess->oInternal.oState & ((1<<BPROCSUSPTIME) | (1<<BPROCSUSPEVNT) | (1<<BPROCSUSPSIGN) | (1<<BPROCSUSPSEMA) | (1<<BPROCSUSPDEBG))) != 0) {
							sysProcess->oStatistic.oTimePAvg = 0;
							sysProcess->oStatistic.oTimeKAvg = 0;
							sysProcess->oStatistic.oTimeEAvg = 0;
						}
						totalTimeCPU += ((uint64_t)sysProcess->oStatistic.oTimePAvg + \
										 (uint64_t)sysProcess->oStatistic.oTimeKAvg + \
										 (uint64_t)sysProcess->oStatistic.oTimeEAvg) * sysProcess->oStatistic.oNbExecutions;
					}
				}

// Compute the statistic in % of time CPU
// Print the string process

				if (all == KNOALL) {
					iotx_printf(KSYST, " #  Process information                                      State        Used cpu in %%       PC             Routine\n\n");
				}
				else {
					iotx_printf(KSYST, "Process details\n\n");
				}
				for (i = 0; i < KNBPROCESS; i++) {
					sysProcess = (process_t *)((uint32_t)bufSysProcess + i*sizeof(process_t));
					if (sysProcess->oValid) {
						pRatio = sysProcess->oStatistic.oTimePAvg * sysProcess->oStatistic.oNbExecutions * 100;
						kRatio = sysProcess->oStatistic.oTimeKAvg * sysProcess->oStatistic.oNbExecutions * 100;
						eRatio = sysProcess->oStatistic.oTimeEAvg * sysProcess->oStatistic.oNbExecutions * 100;

						pRatioF = (float64_t)pRatio / (float64_t)totalTimeCPU;
						kRatioF = (float64_t)kRatio / (float64_t)totalTimeCPU;
						eRatioF = (float64_t)eRatio / (float64_t)totalTimeCPU;

						if (all == KNOALL) {
							_printParameter_P0(i, sysProcess->oSpecification.oText, sysProcess->oInternal.oState, pRatioF, kRatioF, eRatioF);
							if (sysProcess->oPC != 0) {
								kern_getFunctionName(sysProcess->oPC, &functionName);
								if (functionName == NULL) { iotx_printf(KSYST, " - PC = 0x%X\n",    sysProcess->oPC);               }
								else {                      iotx_printf(KSYST, " - PC = 0x%X %s\n", sysProcess->oPC, functionName); }
							}
							else {
								iotx_printf(KSYST, "\n");
							}
						}
						else {
							_printParameter_P1(i, sysProcess->oSpecification.oPriority, sysProcess->oSpecification.oMode, sysProcess->oSpecification.oText, sysProcess->oSpecification.oIdentifier);
							_printParameter_T1(   sysProcess->oStatistic.oTimePMin,     sysProcess->oStatistic.oTimePMax, sysProcess->oStatistic.oTimePAvg, pRatioF);
							_printParameter_T2(   sysProcess->oStatistic.oTimeKMin,     sysProcess->oStatistic.oTimeKMax, sysProcess->oStatistic.oTimeKAvg, kRatioF);
							_printParameter_T3(   sysProcess->oStatistic.oTimeEMin,     sysProcess->oStatistic.oTimeEMax, sysProcess->oStatistic.oTimeEAvg, eRatioF);
							_printParameter_N1(   sysProcess->oStatistic.oNbExecutions);
							_printParameter_S1(   sysProcess->oStatistic.oNbKernCalls);
							_printParameter_S2(   sysProcess->oStatistic.oAvStkSup);
						}
					}
				}
				syos_free(bufSysProcess);
			}
			else {
				error = KPROCENMEM;
			}
			break;
		}
		default: {
			error = KPROCERPAR;
			break;
		}
	}

	switch (error) {
		case KPROCNOERR: {
			if (all == KNOALL) { 
				iotx_printf(KSYST, "\n");
			}
					   													   status = EXIT_SUCCESS_OS; break; }
		case KPROCENMEM: { iotx_printf(KSYST, "Not enough memory.\n\n");   status = EXIT_FAILURE;    break; }
		case KPROCERPAR: { iotx_printf(KSYST, "Incorrect arguments.\n\n"); status = EXIT_FAILURE;    break; }
		default:	     {												   status = EXIT_FAILURE;    break; }
	}
	return (status);
}

// Local routines
// ==============

/*
 * \brief _printParameter_XYZ
 *
 * - Print a parameter P(riority), T(imes) & N(umbers) ...
 *
 */
static	void	_printParameter_P0(uint8_t number, const char_t *textID, uint8_t state, float64_t pRatio, float64_t kRatio, float64_t eRatio) {
			float64_t	sum;
	const	char_t		*idSpacer;

	sum = pRatio + kRatio + eRatio;

	_compose(textID, &idSpacer);

	if (((state & (1<<BPROCSUSPTIME)) != 0) && ((state & (1<<BPROCSUSPDEBG)) == 0)) {
		iotx_printf(KSYST, "%2d  %s%s - Susp. time - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
		return;
	}

	if (((state & (1<<BPROCSUSPEVNT)) != 0) && ((state & (1<<BPROCSUSPDEBG)) == 0)) {
		iotx_printf(KSYST, "%2d  %s%s - Susp. evnt - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
		return;
	}

	if (((state & (1<<BPROCSUSPSIGN)) != 0) && ((state & (1<<BPROCSUSPDEBG)) == 0)) {
		iotx_printf(KSYST, "%2d  %s%s - Susp. sign - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
		return;
	}

	if (((state & (1<<BPROCSUSPSEMA)) != 0) && ((state & (1<<BPROCSUSPDEBG)) == 0)) {
		iotx_printf(KSYST, "%2d  %s%s - Susp. sema - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
		return;
	}

	if ((state & (1<<BPROCSUSPDEBG)) != 0) {
		iotx_printf(KSYST, "%2d  %s%s - Freezed    - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
		return;
	}

	if ((state & (1<<BPROCRUNNING)) != 0) {
		iotx_printf(KSYST, "%2d  %s%s - Running    - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
		return;
	}
	
	    iotx_printf(KSYST, "%2d  %s%s - Scheduled  - used cpu %3d.%03d%%", number, textID, idSpacer, FLOAT_3(sum));
}

static	void	_printParameter_P1(uint8_t number, uint8_t priority, uint32_t mode, const char_t *textID, const char_t *identifierID) {
	char_t	*space;

	if (mode == KUSER) { space = "User";       }
	else 			   { space = "Supervisor"; }

	iotx_printf(KSYST, "Process id:       %d %s\n", number, identifierID);
	iotx_printf(KSYST, "Process text:     %s\n",     textID);
	iotx_printf(KSYST, "Process space:    %s\n",     space);
	iotx_printf(KSYST, "Process priority: %2d\n",    priority);
}

static	void	_printParameter_T1(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio) {

	iotx_printf(KSYST, "CPU time used by the process,    min: %5dus max: %5dus avg: %5dus  %3d.%03d%%\n", min, max, avg, FLOAT_3(ratio));
}

static	void	_printParameter_T2(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio) {

	iotx_printf(KSYST, "CPU time used by the kernel,     min: %5dus max: %5dus avg: %5dus  %3d.%03d%%\n", min, max, avg, FLOAT_3(ratio));
}

static	void	_printParameter_T3(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio) {

	iotx_printf(KSYST, "CPU time used by the exceptions, min: %5dus max: %5dus avg: %5dus  %3d.%03d%%\n", min, max, avg, FLOAT_3(ratio));
}

static	void	_printParameter_N1(uint64_t nbTimes) {

	iotx_printf(KSYST, "Nb of time that the process was scheduled:   %-ld\n", nbTimes);
}

static	void	_printParameter_S1(uint64_t nbCalls) {

	iotx_printf(KSYST, "Nb of system calls to the uKernel functions: %-ld\n", nbCalls);
}

static	void	_printParameter_S2(uint32_t size) {

	iotx_printf(KSYST, "Available supervisor stack size (bytes):     %-d\n\n", size);
}

/*
 * \brief _compose
 *
 * - input:  "1234567"
 * - output: "     "
 * - to accomodate "1234567     "
 * -               "            "
 *
 * \param[in]	*string		Ptr on a string
 * \param[in]	**idSpacer	Ptr on the idSpace
 * \param[out]	**idSpacer	Ptr on the final idSpace
 *
 */
static	void	_compose(const char_t *identifier, const char_t **idSpacer) {
					uint32_t	len;

// -----------------------------------------|----------------------------------------------------|---
//                                         "Process xyz: this process ...             (c) EFr-2017";
	static	const	char_t		spacer[] = "                                                      ";

	*idSpacer = &spacer[0];
	strg_getBufferSize(identifier, &len);
	if (len <= sizeof(spacer)-1) {
		*idSpacer = &spacer[len];
	}
}
