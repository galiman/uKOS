/*
; temp.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:	Franzi Edo.	The 2010-10-14	Suspend for a time = 0 is not possible.
;										In this case, time = 1
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Kern - time management.
;
;			This module implements the temporal and the call-back primitives.
;
; 			Temporal system calls
; 			---------------------
;
;			void	temp_init(void);
;			int32_t	temp_suspendProcess(uint32_t time);
;			int32_t	temp_setUnixTime(const atim_t *time);
;			int32_t	temp_getUnixTime(atim_t *time);
;			int32_t	temp_switchFast(void);
;			int32_t	temp_getTiccount(uint64_t *ticcount);
;			int32_t	temp_waitAtLeast(uint16_t time);
;			int32_t temp_runPrecise(uint16_t time, void (*code)(void));
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

extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
extern	volatile	list_t		vKern_listExec;							// Execution list for all the priorities
		volatile	list_t		vKern_listWait;							// Wait list (processes waiting for a time)

static	const uint32_t	aTabMonths[2][12] = {
							{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
							{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
						};

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "temp" manager
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	temp_init(void) {

	TRACE("entry: temp_init");
	chls_initialize(&vKern_listWait);
	TRACE("exit: temp_init OK");
}

/*
 * \brief Suspend the process for a time
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KWAIT312MS    312    // Wait for 312-ms

 * int32_t    status;
 *
 *    status = kern_suspendProcess(KWAIT312MS);
 * \endcode
 *
 * - Disconnect a process from the execution list
 * - Connect the process to the time list
 *
 * \param[in]	time		Suspend the process for a time (1-ms of resolution)
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	temp_suspendProcess(uint32_t time) {

	TRACE("entry: temp_suspendProcess");
	vKern_current->oStatistic.oNbKernCalls++;
	if (time == 0) {
		return (KKERNNOERR);
	}

	INTERRUPTION_OFF;
	vKern_current->oInternal.oTimeout = (time/KTIMEUNIT);

	GOTO_KERN_I(KWAITTIME);
	vKern_current->oInternal.oStatus = KKERNNOERR;
	TRACE("exit: temp_suspendProcess OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Set the UNIX absolute time
 *
 * Call example in C:
 *
 * \code{.c}
 * atim_t     time;
 * int32_t    status;
 *
 *    time.oYear    = 2016;
 *    time.oMonths  = KJANUARY;
 *    time.oDays    = 1;
 *    time.oHours   = 12;
 *    time.oMinutes = 27;
 *    time.oSeconds = 0;
 *    status = kern_setUnixTime(&time);
 * \endcode
 *
 * - From the absolute calendar time compute the absolute binary 32-bit 
 *   00 .. 59				Seconds
 *   00 .. 59				Minutes
 *   00 .. 23				Hours
 *   00 .. 06				Week days -> 0 = Sunday
 *   01 .. 28, 29, 30, 31	Days
 *   01 .. 12				Months
 *   1970 .. 2106
 *
 * \param[in]	*time		Ptr on the time structure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNTIFMT	Time format error
 *
 */
int32_t	temp_setUnixTime(const atim_t *time) {
	uint32_t	days = 0, tmpDays = 0, years = KEPOCHYEAR, tmpYears, months = 0, tmpMonths, unixTime;

	TRACE("entry: temp_setUnixTime");
	vKern_current->oStatistic.oNbKernCalls++;

// Minimal check of the time - calendar format

	if (time->oSeconds > 59)										{ TRACE("exit: temp_setUnixTime KO 1"); return (KKERNTIFMT); }
	if (time->oMinutes > 59)										{ TRACE("exit: temp_setUnixTime KO 2"); return (KKERNTIFMT); }
	if (time->oHours   > 23)										{ TRACE("exit: temp_setUnixTime KO 3"); return (KKERNTIFMT); }
	if ((time->oDays   < 1)          || (time->oDays   > 31))       { TRACE("exit: temp_setUnixTime KO 4"); return (KKERNTIFMT); }
	if ((time->oMonths < 1)          || (time->oMonths > 12))       { TRACE("exit: temp_setUnixTime KO 5"); return (KKERNTIFMT); }
	if ((time->oYears  < KEPOCHYEAR) || (time->oYears  > KYEARMAX)) { TRACE("exit: temp_setUnixTime KO 6"); return (KKERNTIFMT); }

	tmpYears  = (uint32_t)time->oYears;
	tmpMonths = (uint32_t)(time->oMonths - 1);

// Correct the number of days (+1) accordingly to the bissextile year
// A year is bissextile has to be divisible by 4
// With the exception of the century changes (not bissextiles)
// With the exception of the millenary changes (bissextiles)
// Examples: 1980, 1992, 1996 are bissextiles (+1 day) --> divisible by 4
//           2000             are bissextiles (+1 day) --> divisible by 4 and millenary change
//           2100             not bissextile           --> divisible by 4 but century change
//
// The following computation is valid only for the period of time 1970 <= year < 2106

	days  = (tmpYears - years) * 365;
	days += (tmpYears - years) / 4   + ((tmpYears % 4)   && ((tmpYears % 4)   < (years % 4)));		// +n1 days (bissextiles)
	days -= (tmpYears - years) / 100 + ((tmpYears % 100) && ((tmpYears % 100) < (years % 100)));	// -n2 days (bissextiles without the century changes)
	days += (tmpYears - years) / 400 + ((tmpYears % 400) && ((tmpYears % 400) < (years % 400)));	// +n3 days (bissextiles without the century changes but with the millenary changes)

	while (months < tmpMonths) {
		tmpDays += aTabMonths[LEAPYEAR(tmpYears)][months];
		months++;
	}

	tmpDays += (time->oDays - 1);
	days    += tmpDays;

	unixTime = (days * KSECSDAY) + (uint32_t)(time->oHours * 3600) + (uint32_t)(time->oMinutes * 60) + (uint32_t)time->oSeconds;
	stub_kern_setUnixTime(unixTime);
	TRACE("exit: temp_setUnixTime OK");
	return (KKERNNOERR);
}

/*
 * \brief Get the UNIX absolute time
 *
 * Call example in C:
 *
 * \code{.c}
 * atim_t     time;
 * int32_t    status;
 *
 *    status = kern_getUnixTime(&time);
 *    iotx_printf(KSYST, “YYYY %d, MM %d, DD %d, WD %d, hh %d, mm %d, ss %d UNIX time %8X\n”, \
 *                        time.oYear, time.oMonths, time.oDays, time.oWeekDays, \
 *                        time.oHours, time.oMinutes, time.oSeconds, time.oWeekDays, time.oAbsolute);
 * \endcode
 *
 * - From the absolute binary 32-bit compute the absolute calendar time 
 *   00 .. 59				Seconds
 *   00 .. 59				Minutes
 *   00 .. 23				Hours
 *   00 .. 06				Week days -> 0 = Sunday
 *   01 .. 28, 29, 30, 31	Days
 *   01 .. 12				Months
 *   1970 .. 2106
 *
 * \param[in]	*time		Ptr on the time structure
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	temp_getUnixTime(atim_t *time) {
	uint32_t	dayClock, dayNumber, year = KEPOCHYEAR, unixTime;

	TRACE("entry: temp_getUnixTime");
	vKern_current->oStatistic.oNbKernCalls++;

	stub_kern_getUnixTime(&unixTime);
	time->oAbsolute = unixTime;

	dayClock  = unixTime % KSECSDAY;
	dayNumber = unixTime / KSECSDAY;

	time->oSeconds  = (uint8_t)(dayClock % 60);
	time->oMinutes  = (uint8_t)((dayClock % 3600) / 60);
	time->oHours    = (uint8_t)(dayClock / 3600);
	time->oWeekDays = (uint8_t)((dayNumber + 4) % 7);

	while (dayNumber >= YEARSIZE(year)) {
		dayNumber -= YEARSIZE(year);
		year++;
	}

	time->oDays   = (uint8_t)dayNumber;
	time->oMonths = 0;
	time->oYears  = (uint16_t)year;

	while (dayNumber >= aTabMonths[LEAPYEAR(year)][time->oMonths]) {
		dayNumber -= aTabMonths[LEAPYEAR(year)][time->oMonths++];
	}

	time->oMonths++;
	time->oDays = (uint8_t)(dayNumber + 1);
	TRACE("exit: temp_getUnixTime OK");
	return (KKERNNOERR);
}

/*
 * \brief Force the context switching
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = kern_switchFast();
 * \endcode
 *
 * - Force a context switch
 *
 * \param[in]	-
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	temp_switchFast(void) {

	INTERRUPTION_OFF;
	TRACE("entry: temp_switchFast");
	vKern_current->oStatistic.oNbKernCalls++;

	GOTO_KERN_I(KJUMPKERN);
	TRACE("exit: temp_switchFast OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the ticcount (1-us) from the start of the uKernel
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint64_t    ticcount;
 *
 *    status = temp_getTiccount(&ticcount);
 * \endcode
 *
 * - Return the 1-us ticcount value from the start of the uKernel
 *
 * \param[in]	*ticcount	Ptr on the tic-count
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	temp_getTiccount(uint64_t *ticcount) {

	INTERRUPTION_OFF;
	TRACE("entry: temp_getTiccount");
	vKern_current->oStatistic.oNbKernCalls++;
	stub_kern_timeStmp(ticcount);
	TRACE("exit: temp_getTiccount OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Waiting for short times (at least ... a certain time)
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KWAITMIN    120    // Wait at least 120-µs
 *
 * int32_t    status;
 *
 *    status = kern_waitAtLeast(KWAITMIN);
 * \endcode
 *
 * - Return when the time is reached. Max. 65536-uS
 *   This call ensure the minimum time but not the maximum
 *
 * \param[in]	time		Suspend the process for a time (1-us of resolution)
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	temp_waitAtLeast(uint16_t time) {
	uint64_t 	timeStmp[2];
	bool_t		terminate = FALSE;

	TRACE("entry: temp_waitAtLeast");
	stub_kern_timeStmp(&timeStmp[0]);

	do {
		stub_kern_timeStmp(&timeStmp[1]);
		if ((timeStmp[1] - timeStmp[0]) >= (uint64_t)time) {
			terminate = TRUE;
		}
		kern_switchFast();
	} while (!terminate);

	vKern_current->oStatistic.oNbKernCalls++;
	TRACE("exit: temp_waitAtLeast OK");
	return (KKERNNOERR);
}

/*
 * \brief Execute cyclically a routine with a precise time
 *
 * Call example in C:
 *
 * \code{.c}
 * status = kern_runPrecise(100, (void (*)(void))aStateMachine);
 * int32_t    status;
 *
 *    // My callback routine
 *
 *    static    void    aStateMachine(void) {
 *       static    uint32_t    vNb = 0;
 *       static    enum    { KSTATE1, KSTATE2, KSTATE3 } vState = KSTATE1;
 *    
 *       switch (vState) {
 *           case KSTATE1: {
 *               if (++vNb < 1)  {                            break; }
 *               else            { vNb = 0; vState = KSTATE2; break; }
 *           }
 *           case KSTATE2: {
 *               if (++vNb < 10) {                            break; }
 *               else            { vNb = 0; vState = KSTATE3; break; }
 *           }
 *           case KSTATE3: {
 *               if(++vNb < 5)   {                            break; }
 *               else            { vNb = 0; vState = KSTATE1; break; }
 *           }
 *       }
 *    }
 * \endcode
 *
 * - time = 0, stop the execution
 * - time > 0 and <= 6553- us
 *
 * \param[in]	time		Sampling time
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNSYCNA	System call not available
 *
 */
int32_t	temp_runPrecise(uint16_t time, void (*code)(void)) {

	TRACE("entry: temp_runPrecise");
	stub_kern_runPrecise(time, code);
	TRACE("exit: temp_runPrecise OK");
	return (KKERNNOERR);
}

/*
 * \brief Test of the time suspended condition
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	temp_testEOTime(void) {
				uint16_t	i, j;
	volatile	proc_t		*process, *forwardProcess;

	INTERRUPTION_OFF;
	if (vKern_listWait.oNbElements > 0) {
		process = vKern_listWait.oFirst;
		j = vKern_listWait.oNbElements;
		for (i = 0; i < j; i++) {
			forwardProcess = process->oObject.oForward;
			if (--process->oInternal.oTimeout == 0) {

// NEED to be TESTED
				#if 0
				chls_disconnect(&vKern_listWait, process);

				#else
				chls_disconnect(process->oObject.oList, process);
				#endif

				chls_connect(&vKern_listExec, process);
				process->oInternal.oState &= ~(1<<BPROCSUSPTIME);
				process->oInternal.oStatus = KKERNTIMEO;
			}
			process = forwardProcess;
		}
	}
	INTERRUPTION_RESTORED;
}
