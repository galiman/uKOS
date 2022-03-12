/*
; stat.
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
; Goal:		Kern - Statistic management.
;
;			This module is responsible for computing the statistic of
;			the uKernel.
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

#include 	<uKOS.h>

#if (defined(__WITHSTAT__))
#define	KNMEAN		3													// 2^^3 number of samples for the mean

extern	volatile	proc_t		*vKern_current;							// Ptr on the current process

/*
 *   time
 *
 *          uKernel   process                  uKernel
 *    ----+         +------------------------+         +-------------------------
 *        |         |                        |         |
 *        +---------+                        +---------+
 *                ^                            ^     ^
 *                LastStart                    Stop  Start
 *    ............--------------------------------->----------------------.......
 *                      backward                       forward
 *
 *    ..............---xx---xxxx----x-x----xx-..........---xx---xxx-------.......
 *                      exceptions                       
 *
 * Durations:
 *  - uKernel   time = Start - Stop
 *  - Process   time = Stop - LastStart
 *  - Exception time = Sum ( x )
 *
 */

/*
 * \brief Statistic
 *
 * For each process:
 * - PMin PAvg PMax in [us] (P stand for Process)
 * - KMin KAvg KMax in [us] (K stand for Kernel)
 * - EMin EAvg EMax in [us] (E stand for Exception)
 *
 * - The average time is computed in this way:
 *   - KNMEAN = 3
 *   - PAvg[k+1] = ( (PAvg[k] x ((1<<KNMEAN)-1)) + PIns[k] ) / (1<<KNMEAN)
 *   - KAvg[k+1] = ( (KAvg[k] x ((1<<KNMEAN)-1)) + KIns[k] ) / (1<<KNMEAN)
 *   - EAvg[k+1] = ( (EAvg[k] x ((1<<KNMEAN)-1)) + EIns[k] ) / (1<<KNMEAN)
 *
 * \param[in]	*backwardProcess	Ptr on the backward process
 * \param[in]	timeStart			Time when the process was scheduled
 * \param[in]	timeStop			Time when the process was stopped
 * \param[in]	timeLastStart		Time when the process was started (previous cycle)
 * \param[in]	timeE				Time spent in the exceptions (int managers)
 * \param[out]	-
 *
 */
void	stat_statistic(volatile proc_t *backwardProcess, uint16_t timeStart, uint16_t timeStop, uint16_t timeLastStart, uint16_t timeE) {
	uint16_t	timeP, timeK;
 
	vKern_current->oStatistic.oNbExecutions++;

// Instantaneous CPU time used by the process and the uKernel

	timeP = (timeStop < timeLastStart) ? ((65535 - timeLastStart) + timeStop) - timeE : (timeStop  - timeLastStart) - timeE;
	timeK = (timeStart < timeStop)	   ? ((65535 - timeStop)      + timeStart)        : (timeStart - timeStop);

	if ((backwardProcess->oStatistic.oTimePMin > timeP) || (backwardProcess->oStatistic.oTimePMin == 0)) { backwardProcess->oStatistic.oTimePMin = timeP; }
	if ((backwardProcess->oStatistic.oTimeKMin > timeK) || (backwardProcess->oStatistic.oTimeKMin == 0)) { backwardProcess->oStatistic.oTimeKMin = timeK; }
	if ((backwardProcess->oStatistic.oTimeEMin > timeE) || (backwardProcess->oStatistic.oTimeEMin == 0)) { backwardProcess->oStatistic.oTimeEMin = timeE; }
	if ( backwardProcess->oStatistic.oTimePMax < timeP) 								 			     { backwardProcess->oStatistic.oTimePMax = timeP; }
	if ( backwardProcess->oStatistic.oTimeKMax < timeK) 								 			     { backwardProcess->oStatistic.oTimeKMax = timeK; }
	if ( backwardProcess->oStatistic.oTimeEMax < timeE) 								 			     { backwardProcess->oStatistic.oTimeEMax = timeE; }
	if ( backwardProcess->oStatistic.oTimePAvg == 0) 									 		         { backwardProcess->oStatistic.oTimePAvg = timeP; }
	if ( backwardProcess->oStatistic.oTimeKAvg == 0) 									 		         { backwardProcess->oStatistic.oTimeKAvg = timeK; }
	if ( backwardProcess->oStatistic.oTimeEAvg == 0) 									 		         { backwardProcess->oStatistic.oTimeEAvg = timeE; }

	backwardProcess->oStatistic.oTimePCum += (uint64_t)(timeP);
	backwardProcess->oStatistic.oTimeKCum += (uint64_t)(timeK);
	backwardProcess->oStatistic.oTimeECum += (uint64_t)(timeE);
	
	backwardProcess->oStatistic.oTimePAvg  = (uint16_t)((((uint32_t)backwardProcess->oStatistic.oTimePAvg * ((1<<KNMEAN) - 1)) + (uint32_t)(timeP))>>KNMEAN);
	backwardProcess->oStatistic.oTimeKAvg  = (uint16_t)((((uint32_t)backwardProcess->oStatistic.oTimeKAvg * ((1<<KNMEAN) - 1)) + (uint32_t)(timeK))>>KNMEAN);
	backwardProcess->oStatistic.oTimeEAvg  = (uint16_t)((((uint32_t)backwardProcess->oStatistic.oTimeEAvg * ((1<<KNMEAN) - 1)) + (uint32_t)(timeE))>>KNMEAN);
}
#endif
