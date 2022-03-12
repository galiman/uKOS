/*
; stim.
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
; Goal:		Kern - Software timers.
;
;			This module implements the software primitives.
;
; 			Software timer system calls
; 			---------------------------
;
;			void	stim_init(void);
;			int32_t	stim_createSoftwareTimer(const char_t *identifier, volatile stim_t **handle);
;			int32_t	stim_setSoftwareTimer(volatile stim_t *handle, tspc_t *configure);
;			int32_t	stim_killSoftwareTimer(volatile stim_t *handle);
;			int32_t	stim_getSoftwareTimerById(const char_t *identifier, volatile stim_t **handle);
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

#if (defined(__WITHSTIM__))
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	stim_t		vKern_softTimer[KNBSTIM];				// Software timers

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "stim" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	stim_init(void) {
	uint16_t	i;

	TRACE("entry: stim_init");
	for (i = 0; i < KNBSTIM; i++) {
		vKern_softTimer[i].oIdentifier          = NULL;
		vKern_softTimer[i].oState               = 0;
		vKern_softTimer[i].oInitCounter         = 0;
		vKern_softTimer[i].oCounter	            = 0;
		vKern_softTimer[i].oTimerSpec.oMode     = KSTOP;
		vKern_softTimer[i].oTimerSpec.oIniTime  = 0;
		vKern_softTimer[i].oTimerSpec.oTime     = 0;
		vKern_softTimer[i].oTimerSpec.oCode     = NULL;
		vKern_softTimer[i].oTimerSpec.oArgument = NULL;
	}
	TRACE("exit: stim_init OK");
}

/*
 * \brief Create a software timer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Timer acquisition”;
 * volatile    stim_t    *softwareTimer;
 *
 *    status = kern_createSoftwareTimer(identifier, &softwareTimer);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the software timer
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSTFUL 	No more software timer
 * \param[out]	KKERNIDSTI	The software timer Id is already used
 *
 */
int32_t	stim_createSoftwareTimer(const char_t *identifier, volatile stim_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: stim_createSoftwareTimer");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBSTIM; i++) {
		if (buff_cmpStrings(vKern_softTimer[i].oIdentifier, identifier)) { TRACE("exit: stim_createSoftwareTimer KO 1"); RETURN_INT_RESTORED(KKERNIDSTI); }
	}

	for (i = 0; i < KNBSTIM; i++) {
		if (vKern_softTimer[i].oIdentifier == NULL) {
			vKern_softTimer[i].oState      |= (1<<BSTIMINSTALLED);
			vKern_softTimer[i].oIdentifier = identifier;
			*handle = &vKern_softTimer[i];
			TRACE("exit: stim_createSoftwareTimer OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: stim_createSoftwareTimer KO 2");
	RETURN_INT_RESTORED(KKERNSTFUL);
}

/*
 * \brief Set a software timer
 *
 * Call example in C:
 *
 * \code{.c}
 * static      void    _changeStateLed(void);
 *
 * int32_t     status;
 * tspc_t      configure;
 * volatile    stim_t    *softwareTimer;
 *
 *    configure.oMode     = KCONTINUE;
 *    configure.oIniTime  = 10000;
 *    configure.oTime     = 1000;
 *    configure.oArgument = NULL;
 *    configure.oCode     = _changeStateLed;
 *
 *    status = kern_setSoftwareTimer(&softwareTimer, &configure);
 *
 *    // My callback routine
 *
 *    static    void    _changeStateLed(void) {
 *
 *        misc_toggleLed(0);
 *    }
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSTI	The software timer does not exist
 * \param[out]	KKERNSTNOI 	The software timer is not initialized
 *
 */
int32_t	stim_setSoftwareTimer(volatile stim_t *handle, tspc_t *configure) {
	volatile	stim_t	*softwareTimer;

	INTERRUPTION_OFF;
	TRACE("entry: stim_setSoftwareTimer");
	vKern_current->oStatistic.oNbKernCalls++;
	softwareTimer = handle;
	if (softwareTimer == NULL)						        { TRACE("exit: stim_setSoftwareTimer KO 1"); RETURN_INT_RESTORED(KKERNNOSTI); }
	if (softwareTimer->oIdentifier == NULL)			        { TRACE("exit: stim_setSoftwareTimer KO 2"); RETURN_INT_RESTORED(KKERNNOSTI); }
	if ((softwareTimer->oState & (1<<BSTIMINSTALLED)) == 0) { TRACE("exit: stim_setSoftwareTimer KO 3"); RETURN_INT_RESTORED(KKERNSTNOI); }

	softwareTimer->oTimerSpec.oMode     = configure->oMode;
	softwareTimer->oTimerSpec.oIniTime  = configure->oIniTime;
	softwareTimer->oTimerSpec.oTime     = configure->oTime;
	softwareTimer->oTimerSpec.oCode     = configure->oCode;
	softwareTimer->oTimerSpec.oArgument = configure->oArgument;
	softwareTimer->oInitCounter 	    = configure->oIniTime;
	softwareTimer->oCounter 	   	    = configure->oTime;

	softwareTimer->oState			   &= ~(1<<BSTIMRUNNING);
	softwareTimer->oState			   |= (configure->oMode != KSTOP) ? ((1<<BSTIMCONFIGURED) | (1<<BSTIMRUNNING)) : ((1<<BSTIMCONFIGURED));
	TRACE("exit: stim_setSoftwareTimer OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Kill the software timer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    stim_t    *softwareTimer;
 *
 *    status = kern_killSoftwareTimer(softwareTimer);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSTI	The software timer does not exist
 * \param[out]	KKERNSTNOI 	The software timer is not initialized
 *
 */
int32_t	stim_killSoftwareTimer(volatile stim_t *handle) {
	volatile	stim_t	*softwareTimer;

	INTERRUPTION_OFF;
	TRACE("entry: stim_killSoftwareTimer");
	vKern_current->oStatistic.oNbKernCalls++;
	softwareTimer = handle;
	if (softwareTimer == NULL)						        { TRACE("exit: stim_killSoftwareTimer KO 1"); RETURN_INT_RESTORED(KKERNNOSTI); }
	if (softwareTimer->oIdentifier == NULL) 			    { TRACE("exit: stim_killSoftwareTimer KO 2"); RETURN_INT_RESTORED(KKERNNOSTI); }
	if ((softwareTimer->oState & (1<<BSTIMINSTALLED)) == 0) { TRACE("exit: stim_killSoftwareTimer KO 3"); RETURN_INT_RESTORED(KKERNSTNOI); }

	softwareTimer->oIdentifier          = NULL;
	softwareTimer->oState               = 0;
	softwareTimer->oInitCounter         = 0;
	softwareTimer->oCounter	            = 0;
	softwareTimer->oTimerSpec.oMode     = KSTOP;
	softwareTimer->oTimerSpec.oIniTime  = 0;
	softwareTimer->oTimerSpec.oTime     = 0;
	softwareTimer->oTimerSpec.oCode     = NULL;
	softwareTimer->oTimerSpec.oArgument = NULL;
	TRACE("exit: stim_killSoftwareTimer OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of a software timer by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Timer acquisition”;
 * volatile    stim_t    *softwareTimer;
 *
 *    status = kern_getSoftwareTimerById(identifier, &softwareTimer);
 * \endcode
 *
 * - This function returns the handle of the software timer
 *
 * \param[in]	*identifier	Ptr on the software timer Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSTI	The software timer does not exist
 *
 */
int32_t	stim_getSoftwareTimerById(const char_t *identifier, volatile stim_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: stim_getSoftwareTimerById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBSTIM; i++) {
		if (buff_cmpStrings(vKern_softTimer[i].oIdentifier, identifier)) {
			*handle = &vKern_softTimer[i];
			TRACE("exit: stim_getSoftwareTimerById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: stim_getSoftwareTimerById KO");
	RETURN_INT_RESTORED(KKERNNOSTI);
}
#endif
