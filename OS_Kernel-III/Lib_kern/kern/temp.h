/*
; temp.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
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

#ifndef	__TEMP__
#define	__TEMP__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	temp_init(void);
extern	int32_t	temp_suspendProcess(uint32_t time);
extern	int32_t	temp_setUnixTime(const atim_t *time);
extern	int32_t	temp_getUnixTime(atim_t *time);
extern	int32_t	temp_switchFast(void);
extern	int32_t	temp_getTiccount(uint64_t *ticcount);
extern	int32_t	temp_waitAtLeast(uint16_t time);
extern	int32_t temp_runPrecise(uint16_t time, void (*code)(void));
extern	void	temp_testEOTime(void);

#ifdef	__cplusplus
}
#endif

// Structure of the absolute time (UNIX time)
// ------------------------------------------

// 0x00000000	->  1970 January 1st, 0h, 0m,  0s
// 0xFFFFFFFF	->  2106 July    2nd, 6h, 28m, 16s

#define LEAPYEAR(year)	(!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)	(LEAPYEAR(year) ? 366 : 365)

#define	KSECSDAY	(24 * 60 * 60)										// Seconds per days
#define KYEAR0		1900												// First year
#define KEPOCHYEAR	1970												// Epoch year
#define KYEARMAX	2106												// Last year

enum { KSUNDAY, KMONDAY, KTUESDAY, KWEDNESDAY, KTHURSDAY, KFRIDAY, KSATURDAY };
enum { KJANUARY = 1, KFEBRUARY, KMARCH, KAPRIL, KMAY, KJUNE, KJULY, KAUGUST, KSEPTEMBER, KOCTOBER, KNOVEMBER, KDECEMBER };

struct	atim {
			uint32_t	oAbsolute;										// Binary absolute time	[0..2^32]
			uint8_t		oSeconds;										// Seconds				[0..59]
			uint8_t		oMinutes;										// Minutes				[0..59]
			uint8_t		oHours;											// Hours				[0..23]
			uint8_t		oWeekDays;										// Week day				[0..6] 0 = Sunday
			uint8_t		oDays;											// Days					[1..28,29,30,31]
			uint8_t		oMonths;										// Months				[1..12]
			uint16_t	oYears;											// Years				[1970..2106]
};
#endif
