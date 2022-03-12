/*
; ovly.
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
; Goal:		Kern - Overlay management.
;
;			This module allows to implement overlay processes
;
; 			Overlay system calls
; 			--------------------
;
;			void	ovly_init(void);
;			int32_t	ovly_loadOverlay(uint32_t idModule);
;			int32_t	ovly_getOverlay(void **address);
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

#if (defined(__WITHOVLY__))
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
					uint8_t		vKern_overlay[KSZOVLYRAM];				// Overlay process RAM region for executing the code

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "ovly" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	ovly_init(void) {
	uint16_t	i;
	void		*overlayDst;

	TRACE("entry: ovly_init");
	overlayDst = &vKern_overlay[0];
	for (i = 0; i < KSZOVLYRAM/4; i++) {
		*overlayDst++ = 0;
	}

	TRACE("exit: ovly_init OK");
}

/*
 * \brief Load an overlay process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = kern_ loadOverlay(“OV01”);
 * \endcode
 *
 * - This function loads an overlay process into the execution RAM
 *
 * \param[in]	idModule	"XXYY" module Id
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNOVRLY	The overlay process does not exist
 *
 */
int32_t	ovly_loadOverlay(uint32_t idModule) {
	moduleHeader_t	*moduleCODE;

	TRACE("entry: ovly_loadOverlay");
	vKern_current->oStatistic.oNbKernCalls++;
	moduleCODE = (moduleHeader_t *)&vKern_overlay[0];
	if (moduleCODE->oIdentifier != idModule) { TRACE("exit: ovly_loadOverlay ->"); stub_kern_loadOverlay(idModule); }
	else 									 { TRACE("exit: ovly_loadOverlay OK");                                  }

	return (KKERNNOERR);
}

/*
 * \brief Get the overlay execution RAM address
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * void       *address;
 *
 *    status = kern_ getOverlay(&address);
 * \endcode
 *
 * - This function returns the overlay execution RAM
 *
 * \param[in]	**address	Ptr on the overlay process entry address
 * \param[out]	KKERNNOERR	OK
 *
 */
int32_t	ovly_getOverlay(void **address) {

	TRACE("entry: ovly_getOverlay");
	vKern_current->oStatistic.oNbKernCalls++;
	*address = &vKern_overlay[0];
	TRACE("exit: ovly_getOverlay OK");
	return (KKERNNOERR);
}
#endif
