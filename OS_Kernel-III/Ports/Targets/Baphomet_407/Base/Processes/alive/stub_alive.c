/*
; stub_alive.
; ===========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 184                  $:  Revision of last commit
; $Date:: 2017-07-17 10:50:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		alive process; the system is working.
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
#include	"alive/alive.h"

#define	KLEDALIVE	6
#define	KLEDIDLE	7

// Prototypes
// ==========

static	void	_myRoutine(uint8_t state);

/*
 * \brief stub_alive_process
 *
 */
void	stub_alive_process(const void *argument) {
			uint32_t	time[2];
	const	cnfAlive_t	*configure;

	kern_installCallBack(_myRoutine);
	
	configure = (const cnfAlive_t *)argument;
	time[0] = configure->oTime[0];
	time[1] = configure->oTime[1];

	while (TRUE) {
		misc_onLed(KLEDALIVE);
		kern_suspendProcess(time[0]);
		misc_offLed(KLEDALIVE);
		kern_suspendProcess(time[1]);
	}
}

static	void	_myRoutine(uint8_t state) {

	if (state == KINIDLE) { misc_offLed(KLEDIDLE); }
	else			      { misc_onLed(KLEDIDLE);  }
}

