/*
; calendar.
; =========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2009-01-08
; Modifs:
; Version:	1.2
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 172                  $:  Revision of last commit
; $Date:: 2017-07-05 15:05:12#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Initial date & time settings.
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

#ifndef	__CALENDAR__
#define	__CALENDAR__

// Today BCD date & time

#define	K24H			0x0		// 24-H format
#define	KHOURS			0x13	// Hours
#define	KMINUTES		0x27	// Minutes
#define	KSECONDS		0x00	// Seconds

#define	KYEARS			0x17	// Years
#define	KMONTHS			0x07	// Months
#define	KDAYS			0x05	// Days
#define	KASCIIWEEK		0x3		// Week day (1 = Monday, 7 = Sunday)
#endif
