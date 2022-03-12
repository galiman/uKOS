/*
; debg.
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
; Goal:		Kern - Debug list management.
;
;			This module implements the debug list primitives.
;
; 			Software debug system calls
; 			---------------------------
;
;			void	debg_init(void);
;			int32_t	debg_stopProcess(volatile proc_t *handle);
;			int32_t	debg_reactivateProcess(volatile proc_t *handle);
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

#if (defined(__WITHDEBG__))
static	volatile	list_t		*vKern_savelist = NULL;					// Save the list
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	list_t		vKern_listDebg;							// Debug list

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "debg" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	debg_init(void) {

	TRACE("entry: debg_init");
	chls_initialize(&vKern_listDebg);
	TRACE("exit: debg_init OK");
}

/*
 * \brief Stop a process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_stopProcess(process);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 * \param[out]	KKERNDBGER	The process is already in the debug list
 *
 */
int32_t	debg_stopProcess(volatile proc_t *handle) {
	volatile	proc_t		*process;

	INTERRUPTION_OFF;
	TRACE("entry: debg_stopProcess");
	vKern_current->oStatistic.oNbKernCalls++;
	process = handle;
	if (process == NULL)                           			 	{ TRACE("exit: debg_stopProcess KO 1"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((process->oInternal.oState & (1<<BPROCINSTALLED)) == 0) { TRACE("exit: debg_stopProcess KO 2"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((process->oInternal.oState & (1<<BPROCSUSPDEBG))  != 0) { TRACE("exit: debg_stopProcess KO 3"); RETURN_INT_RESTORED(KKERNDBGER); }

	vKern_savelist = process->oObject.oList;
	process->oInternal.oState |= (1<<BPROCSUSPDEBG);
	chls_disconnect(process->oObject.oList, process);
	chls_connect(&vKern_listDebg, process);

	TRACE("exit: debg_stopProcess OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Reactivate a process
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    proc_t    *process;
 *
 *    status = kern_reactivateProcess(process);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOPRO 	The process does not exist
 * \param[out]	KKERNDBGER	The process is not in the debug list
 *
 */
int32_t	debg_reactivateProcess(volatile proc_t *handle) {
	volatile	proc_t		*process;

	INTERRUPTION_OFF;
	TRACE("entry: debg_reactivateProcess");
	vKern_current->oStatistic.oNbKernCalls++;
	process = handle;
	if (process == NULL)                           			 	{ TRACE("exit: debg_reactivateProcess KO 1"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((process->oInternal.oState & (1<<BPROCINSTALLED)) == 0) { TRACE("exit: debg_reactivateProcess KO 2"); RETURN_INT_RESTORED(KKERNNOPRO); }
	if ((process->oInternal.oState & (1<<BPROCSUSPDEBG))  == 0) { TRACE("exit: debg_reactivateProcess KO 3"); RETURN_INT_RESTORED(KKERNDBGER); }

	process->oInternal.oState &= ~(1<<BPROCSUSPDEBG);

// NEED to be TESTED
	#if 0
	chls_disconnect(&vKern_listDebg, process);

	#else
	chls_disconnect(process->oObject.oList, process);
	#endif

	chls_connect(vKern_savelist, process);

	TRACE("exit: debg_reactivateProcess OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}
#endif
