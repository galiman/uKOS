/*
; test_RTC.
; =========

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
;			Launch 1 processes:
;
;			- P0: some RTC tests
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

#undef	__TEST_1__				// Set the calendar in the RTC
#undef	__TEST_2__				// Read the calendar
#define	__TEST_3__				// Set the interruption wakeup (every 10-s)

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_RTC     test of the RTC.                          (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_RTC\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

extern	void	stub_setRTC(void);
extern	void	stub_setPeriodicInt(void);
static	void	_display(uint32_t date, uint32_t time);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

#if (defined(__TEST_1__))
/*
 * \brief aProcess 0
 *
 * - P0: - Initialize the RTC
 *		 - Every 1000-ms
 *			- Display the calendar
 *
 */
static	void	aProcess_0(const void *argument) {

	stub_setRTC();

	while (TRUE) {
		kern_suspendProcess(1000);
		
		_display(RTC->DR, RTC->TR);
	}
}
#endif

#if (defined(__TEST_2__))
/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *			- Display the calendar
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);

		_display(RTC->DR, RTC->TR);
	}
}
#endif

#if (defined(__TEST_3__))
/*
 * \brief aProcess 0
 *
 * - P0: - Set the wake-up interruption
 *		 - Every 10-s
 *			- Blink the LED0 every 10-s
 *
 */
static	void	aProcess_0(const void *argument) {

	stub_setPeriodicInt();

	while (TRUE) {
		kern_suspendProcess(1000);

		_display(RTC->DR, RTC->TR);
	}
}
#endif

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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KURT0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

// Local routines
// ==============

/*
 * \brief _display
 *
 * - Display the date & the time
 *
 */
static	void	_display(uint32_t date, uint32_t time) {
	uint8_t		years, months, days, hours, minutes, seconds;
	char_t		*asciiWeek, *asciiMonth;

// The calendar
// ------------

	years  = (((date>>20) & 0xF) * 10) + ((date>>16) & 0xF);
	months = (((date>>12) & 0x1) * 10) + ((date>>8)  & 0xF);
	days   = (((date>>4)  & 0x3) * 10) + ((date>>0)  & 0xF);

	switch ((date>>13) & 0x7) {
		case 1:  { asciiWeek = "Monday";       break; }
		case 2:  { asciiWeek = "Tuesday";      break; }
		case 3:  { asciiWeek = "Wednesday";    break; }
		case 4:  { asciiWeek = "Thursday";     break; }
		case 5:  { asciiWeek = "Friday";       break; }
		case 6:  { asciiWeek = "Saturday";     break; }
		case 7:  { asciiWeek = "Sunday";       break; }
		default: { asciiWeek = "Problem";      break; }
	}

	switch (months) {
		case 1:  { asciiMonth = "January";     break; }
		case 2:  { asciiMonth = "February";    break; }
		case 3:  { asciiMonth = "March";       break; }
		case 4:  { asciiMonth = "April";       break; }
		case 5:  { asciiMonth = "May";         break; }
		case 6:  { asciiMonth = "June";        break; }
		case 7:  { asciiMonth = "July";        break; }
		case 8:  { asciiMonth = "August";      break; }
		case 9:  { asciiMonth = "September";   break; }
		case 10: { asciiMonth = "October";     break; }
		case 11: { asciiMonth = "November";    break; }
		case 12: { asciiMonth = "December";    break; }
		default: { asciiMonth = "Problem";     break; }
	}

// The time
// --------

	hours   = (((time>>20) & 0x3) * 10) + ((time>>16) & 0xF);
	minutes = (((time>>12) & 0x7) * 10) + ((time>>8)  & 0xF);
	seconds = (((time>>4)  & 0x7) * 10) + ((time>>0)  & 0xF);

	iotx_printf(KSYST, "%s, %s %2d, 20%2d   %2d : %2d : %2d\n", asciiWeek, 
																asciiMonth,
																days,
																years,
																hours,
																minutes,
																seconds);
}

