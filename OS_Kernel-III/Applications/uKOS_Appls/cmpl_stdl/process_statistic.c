/*
; process_statistic.
; ==================

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
; Goal:		Process: display the system statistic.
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
#include	"hard.h"

#define	KTSAMPLE	2000

typedef	struct	process	process_t;

struct process {
			bool_t		oValid;											// Process valid
			spec_t		oSpecification;									// Process specification
			work_t		oInternal;										// Process internal stuff
			stts_t		oStatistic;										// uKernel statistic
		};

// Prototypes
// ==========

static	void	aProcess(const void *argument);
static	void	_printParameter_P1(uint8_t number, uint8_t priority, uint32_t mode, const char_t *textID, const char_t *identifierID);
static	void	_printParameter_T1(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio);
static	void	_printParameter_T2(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio);
static	void	_printParameter_T3(uint32_t min, uint32_t max, uint32_t avg, float64_t ratio);
static	void	_printParameter_N1(uint64_t nbTimes);
static	void	_printParameter_S1(uint64_t nbCalls);
static	void	_printParameter_S2(uint32_t size);

/*
 * \brief Install & launch the process
 *
 */
void	installaProcess_statistic(void) {
	volatile	proc_t 	*process;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden[]) =      "User_Process_statistic";
	LOC_CONST_STRG(aStrText[]) =      "Process statistic.                        (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, aProcess, aStrIden, KDEF0, (KLOWPRIORITY+7), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR) { exit(EXIT_FAILURE); }
}

/*
 * \brief aProcess
 *
 */
static	void	aProcess(const void *argument) {
				uint64_t	pRatio, kRatio, eRatio, totalTimeCPU;
				float64_t	pRatioF, kRatioF, eRatioF;
				uint8_t		i, *bufSysProcess;
				process_t	*sysProcess;
	volatile	proc_t		*process;

	bufSysProcess = (uint8_t *)syos_malloc(KINTERNAL, KNBPROCESS*sizeof(process_t));
	if (bufSysProcess == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	while (TRUE) {
		kern_suspendProcess(KTSAMPLE);

		totalTimeCPU = 0;

// Collect the process information usable for the statistics

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
				sysProcess->oStatistic.oTimeKAvg       = process->oStatistic.oTimeEAvg;
				sysProcess->oStatistic.oTimeEMin       = process->oStatistic.oTimeEMin;
				sysProcess->oStatistic.oTimeEMax       = process->oStatistic.oTimeEMax;
				sysProcess->oStatistic.oTimeEAvg       = process->oStatistic.oTimeEAvg;
			}
		}

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

		for (i = 0; i < KNBPROCESS; i++) {
			sysProcess = (process_t *)((uint32_t)bufSysProcess + i*sizeof(process_t));
			if (sysProcess->oValid) {
				pRatio = sysProcess->oStatistic.oTimePAvg * sysProcess->oStatistic.oNbExecutions * 100;
				kRatio = sysProcess->oStatistic.oTimeKAvg * sysProcess->oStatistic.oNbExecutions * 100;
				eRatio = sysProcess->oStatistic.oTimeEAvg * sysProcess->oStatistic.oNbExecutions * 100;

				pRatioF = (float64_t)pRatio / totalTimeCPU;
				kRatioF = (float64_t)kRatio / totalTimeCPU;
				eRatioF = (float64_t)eRatio / totalTimeCPU;

// Print the string process

				_printParameter_P1(i, sysProcess->oSpecification.oPriority,  sysProcess->oSpecification.oMode, sysProcess->oSpecification.oText, sysProcess->oSpecification.oIdentifier);
				_printParameter_T1(   sysProcess->oStatistic.oTimePMin,      sysProcess->oStatistic.oTimePMax, sysProcess->oStatistic.oTimePAvg, pRatioF);
				_printParameter_T2(   sysProcess->oStatistic.oTimeKMin,      sysProcess->oStatistic.oTimeKMax, sysProcess->oStatistic.oTimeKAvg, kRatioF);
				_printParameter_T3(   sysProcess->oStatistic.oTimeEMin,      sysProcess->oStatistic.oTimeEMax, sysProcess->oStatistic.oTimeEAvg, eRatioF);
				_printParameter_N1(   sysProcess->oStatistic.oNbExecutions);
				_printParameter_S1(   sysProcess->oStatistic.oNbKernCalls);
				_printParameter_S2(   sysProcess->oStatistic.oAvStkSup);
			}
		}
	}
}

// Local routines
// ==============

/*
 * \brief _printParameter_XYZ
 *
 */
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

