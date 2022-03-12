/*
; conf_kernel.
; ============

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
; Goal:		uKernel configuration.
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

#ifndef	__CONF_KERNEL__
#define	__CONF_KERNEL__

// uKernel behavior
// ----------------

#undef	__WITHMBOX__				// With mail box functions
#define	__WITHSEMA__				// With semaphore functions
#define	__WITHASSO__				// With association functions
#define	__WITHEVNT__				// With event functions
#undef	__WITHSIGN__				// With signal functions
#define	__WITHSTAT__				// With statistic functions
#undef	__WITHDEBG__				// With debug functions
#undef	__WITHSTIM__				// With software timer functions
#undef	__WITHPOOL__				// With memory pool functions
#undef	__WITHOVLY__				// With overlay functions
#undef	__DAEMKILL__				// Define = allow to kill the system daemons
									// Undef  = for normal operations

// Time units and misc
// -------------------

#define	KTIMEUNIT		1			// Time unit in ms
#define	KFREQTICRATE	1000		// Tic rate in Hz
#define	KTIMEPROC		20			// Time slot for a process in ms (timeout)
#define	KLNSTRID		(31+1)		// Strings length for all the identifiers + \0 !!! Always aligned !!!

// Daemons
// -------

#define	KTIMETIMEOUT	5			// Time for the timeout management
#define	KTIMESTACKIN	500			// Time for the verification of the stack integrity
#define	KTIMESOFTTIMER	1			// Time for the software timer management

// uKernel features
// ----------------

#define	KNBPROCESS		16			// Number of process
#define	KNBPRIORITIES	(1<<5)		// Number of priorities 0..31
#define	KMSPRIORITIES	((1<<5)-1)	// Mask for priorities 0..31
#define	KHIGHPRIORITY	0			// 1..7  priority for very reactive process	--> i.e. OS, system, communications
#define	KMEDIUMPRIORITY	8			// 8..15 priority for reactive process		--> i.e. Rapid algo and processing
#define	KNORMALPRIORITY	16			// 16..23 priority for normal process		--> i.e. Human interactions
#define	KLOWPRIORITY	24			// 24..31 priority for slow process			--> i.e. Slow reactions

#define	KNBASTP			8			// Number of string to pointer associations
#define	KNBMBOX			8			// Number of mailboxes
#define	KNBPACKS		16			// Number of packets in a mailbox
#define	KNBSEMAPHO		8			// Number of semaphores
#define	KNBSTIM			8			// Number of software timers
#define	KNBSIGNALS		4			// Number of signals pools (4 x 32 signals)
#define	KNBPOOLS		4			// Number of memory pools

#define	KSZOVLYRAM		4096		// Size of the overlay RAM
#define	KSZSTACKMIN		200			// Minimal stack size of the process (32.bits)
#define	KSZSTACKNLB		(200+600)	// Newlib stack size of the process (32.bits)

// Check the limits
// ----------------

#if		(KNBASTP > 255)
#error	"*** The number of associations has to be less than 256"
#endif
#if		(KNBPROCESS > 255)
#error	"*** The number of processes has to be less than 256"
#endif
#if		(KNBPRIORITIES > 254)
#error	"*** The number of priorities has to be less than 255 - 1"
#endif
#if		(KNBMBOX > 255)
#error	"*** The number of mailboxes has to be less than 256"
#endif
#if		(KNBPACKS > 65535)
#error	"*** The number of packs has to be less than 65536"
#endif
#if		(KNBSEMAPHO > 255)
#error	"*** The number of semaphores has to be less than 256"
#endif
#if		(KNBSTIM > 255)
#error	"*** The number of software timers has to be less than 256"
#endif
#if		(KNBSIGNALS > 255)
#error	"*** The number of signal pools has to be less than 256"
#endif
#if		(KNBPOOLS > 255)
#error	"*** The number of memory pools has to be less than 256"
#endif
#if		(KSZOVLYRAM > 8192)
#error	"*** The size of the overlay RAM is over than 8-KBytes"
#endif
#if		(KSZSTACKMIN < 100)
#error	"*** The size of the stack is too low"
#endif
#endif
