/*
; sign.
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
; Goal:		Kern - Signal management.
;
;			This module implements the signals primitives.
;
; 			Software signal system calls
; 			----------------------------
;
;			void	sign_init(void);
;			int32_t	sign_createSignal(const char_t *identifier, volatile sign_t **handle);
;			int32_t	sign_getBitSignal(volatile sign_t *handle, uint32_t *bitSignal);
;			int32_t	sign_signalSignal(volatile sign_t *handle, uint32_t bitSignal, volatile proc_t *process, uint32_t mode);
;			int32_t	sign_waitSignal(uint32_t *bitSignal, uint32_t timeout);
;			int32_t	sign_killSignal(volatile sign_t *handle);
;			int32_t	sign_getSignalById(const char_t *identifier, volatile sign_t **handle);
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

#if (defined(__WITHSIGN__))
extern	volatile	proc_t		vKern_process[KNBPROCESS];				// User processes, Id = 1..2^31; Id = IDLE if 1
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
extern	volatile	list_t		vKern_listExec;							// Execution list for all the priorities
		volatile	list_t		vKern_listSign;							// Signal list (processes waiting for a software signal)
		volatile	sign_t		vKern_signal[KNBSIGNALS];				// Signals

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "signal" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	sign_init(void) {
	uint16_t	i;

	TRACE("entry: sign_init");
	for (i = 0; i < KNBSIGNALS; i++) {
		vKern_signal[i].oIdentifier = NULL;
		vKern_signal[i].oState      = 0;
		vKern_signal[i].oBitSignal  = 0;
	}

	chls_initialize(&vKern_listSign);
	TRACE("exit: sign_init OK");
}

/*
 * \brief Create a signal
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”BTLE message”;
 * volatile    sign_t    *signal;
 *
 *    status = kern_createSignal(identifier, &signal);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the signal Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSIFUL 	No more signal
 * \param[out]	KKERNIDSIO	The signal Id is already used
 *
 */
int32_t	sign_createSignal(const char_t *identifier, volatile sign_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: sign_createSignal");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBSIGNALS; i++) {
		if (buff_cmpStrings(vKern_signal[i].oIdentifier, identifier)) { TRACE("exit: sign_createSignal KO 1"); RETURN_INT_RESTORED(KKERNIDSIO); }
	}

	for (i = 0; i < KNBSIGNALS; i++) {
		if (vKern_signal[i].oIdentifier == NULL) {
			vKern_signal[i].oState      |= (1<<BSIGNINSTALLED);
			vKern_signal[i].oIdentifier  = identifier;
			*handle = &vKern_signal[i];
			TRACE("exit: sign_createSignal OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}
	}
	TRACE("exit: sign_createSignal KO 2");
	RETURN_INT_RESTORED(KKERNSIFUL);
}

/*
 * \brief Get the bit signal
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    bitSignal;
 * volatile    sign_t    *signal;
 *
 *    status = kern_signalSignal(signal, &bitSignal);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*bitSignal	Ptr on the bitSignal
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 * \param[out]	KKERNSINOI 	The signal is not initialized
 *
 */
int32_t	sign_getBitSignal(volatile sign_t *handle, uint32_t *bitSignal) {
	volatile	sign_t		*signal;

	INTERRUPTION_OFF;
	TRACE("entry: sign_getBitSignal");
	vKern_current->oStatistic.oNbKernCalls++;
	signal = handle;
	if (signal == NULL)							     { TRACE("exit: sign_getBitSignal KO 1"); RETURN_INT_RESTORED(KKERNNOSIO); }
	if (signal->oIdentifier == NULL)				 { TRACE("exit: sign_getBitSignal KO 2"); RETURN_INT_RESTORED(KKERNNOSIO); }
	if ((signal->oState & (1<<BSIGNINSTALLED)) == 0) { TRACE("exit: sign_getBitSignal KO 3"); RETURN_INT_RESTORED(KKERNSINOI); }

	*bitSignal = signal->oBitSignal;
	TRACE("exit: sign_getBitSignal OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Signal signals
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    bitSignal;
 * volatile    sign_t    *signal;
 * volatile    proc_t    *process;
 *
 *    while (kern_getProcessById(“My process”, &process) != KKERNNOERR) {
 *        kern_suspendProcess(1);
 *    }
 *
 * // Mode selective
 *
 *    status = kern_signalSignal(signal, bitSignal, process, KSYNCCONT);
 *
 * // Mode broadcast
 *
 *    status = kern_signalSignal(signal, bitSignal, NULL, KSYNCCONT);
 * \endcode
 *
 * - If (mode == KSIGNSWTC)
 *   - With the context switching
 *
 * - If (mode == KSIGNCONT)
 *   - Without the context switching
 *
 * - Scan all the installed processes
 *   If the process is not suspended
 *   then, save the new signal in the process->oSynchro.oListSignals
 *   else, if the process->oSynchro.oListSignals & signal !=
 *         then, cancel the matched signal and connect the processes to the execution list
 *         else, next 
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	bitSignal	Signal (1 or more)
 * \param[in]	*process	Ptr on the process handle (selective signal)
 * \param[in]				NULL, broadcast to all the installed processes the signals
 * \param[in]	mode		KSIGNSWTC -> (synchro with the context switching)
 * \param[in]				KSIGNCONT -> (synchro without the context switching)
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 * \param[out]	KKERNSINOI 	The signal is not initialized
 * \param[out]	KKERNNOPRO 	The process does not exist
 *
 */
int32_t	sign_signalSignal(volatile sign_t *handle, uint32_t bitSignal, volatile proc_t *process, uint32_t mode) {
				uint16_t	i;
				uint32_t	theSignal;
	volatile	sign_t		*signal;

	INTERRUPTION_OFF;
	TRACE("entry: sign_signalSignal");
	vKern_current->oStatistic.oNbKernCalls++;
	signal = handle;
	if (signal == NULL)							     { TRACE("exit: sign_getBitSignal KO 1"); RETURN_INT_RESTORED(KKERNNOSIO); }
	if (signal->oIdentifier == NULL)				 { TRACE("exit: sign_getBitSignal KO 2"); RETURN_INT_RESTORED(KKERNNOSIO); }
	if ((signal->oState & (1<<BSIGNINSTALLED)) == 0) { TRACE("exit: sign_getBitSignal KO 3"); RETURN_INT_RESTORED(KKERNSINOI); }
	signal->oBitSignal = bitSignal;

	if (process == NULL) {

// Process broadcast signal
// ------------------------

		for (i = 0; i < KNBPROCESS; i++) {
			if ((vKern_process[i].oInternal.oState & (1<<BPROCINSTALLED)) != 0) {
				vKern_process[i].oSynchro.oListSignalsPending |= bitSignal;
				if ((vKern_process[i].oInternal.oState & (1<<BPROCSUSPSIGN)) != 0) {
					theSignal = vKern_process[i].oSynchro.oListSignals & bitSignal;
					if (theSignal != 0) {
						vKern_process[i].oInternal.oState &= ~(1<<BPROCSUSPSIGN);

// NEED to be TESTED
						#if 0
						chls_disconnect(&vKern_listSign, &vKern_process[i]);

						#else
						chls_disconnect(vKern_process[i].oObject.oList, &vKern_process[i]);
						#endif

						chls_connect(&vKern_listExec, &vKern_process[i]);
						vKern_process[i].oInternal.oStatus = KKERNNOERR;

// mode == KSIGNSWTC
// Context switching (preemption when "INTERRUPTION_RESTORED")

						if (mode == KSIGNSWTC) {
							PREEMPTION;
						}
					}
				}
			}
		}
	}
	else {

// Process selective signal
// ------------------------

		if ((process->oInternal.oState & (1<<BPROCINSTALLED)) == 0) { TRACE("exit: sign_signalSignal KO 2"); RETURN_INT_RESTORED(KKERNNOPRO); }

		process->oSynchro.oListSignalsPending |= bitSignal;
		if ((process->oInternal.oState & (1<<BPROCSUSPSIGN)) != 0) {
			theSignal = process->oSynchro.oListSignals & bitSignal;
			if (theSignal != 0) {
				process->oInternal.oState &= ~(1<<BPROCSUSPSIGN);

// NEED to be TESTED
				#if 0
				chls_disconnect(&vKern_listSign, process);

				#else
				chls_disconnect(process->oObject.oList, process);
				#endif

				chls_connect(&vKern_listExec, process);
				process->oInternal.oStatus = KKERNNOERR;

// mode == KSIGNSWTC
// Context switching (preemption when "INTERRUPTION_RESTORED")

				if (mode == KSIGNSWTC) {
							PREEMPTION;
				}
			}
		}
	}
	TRACE("exit: sign_signalSignal OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Wait for signals
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIGNAL_MSG    0x00080000
 *
 * int32_t     status;
 * volatile    sign_t    *signal;
 *
 *    status = kern_getSignalById(“message OK”, &signal);
 *    status = kern_waitSignal(KSIGNAL_MSG, KSIGNCONT);
 * \endcode
 *
 * - Disconnect a process from the execution list
 * - Connect the process to the signal list
 *
 * \param[in]	*bitSignal	Ptr on the signal (1 or more)
 * \param[in]	timeout		Timeout
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNSIKIL  The signal has been killed (with processes in its list)
 * \param[out]	KKERNTIMEO 	Timeout
 *
 */
int32_t	sign_waitSignal(uint32_t *bitSignal, uint32_t timeout) {
	uint32_t	theSignal;

	INTERRUPTION_OFF;
	TRACE("entry: sign_waitSignal");
	vKern_current->oStatistic.oNbKernCalls++;

	theSignal = vKern_current->oSynchro.oListSignalsPending & *bitSignal;

// The signal already occurred
// Cancel it and return the signal

	if (theSignal != 0) {
		vKern_current->oSynchro.oListSignalsPending &= ~theSignal;
		*bitSignal = theSignal;
		RETURN_INT_RESTORED(KKERNNOERR);
	}

// Waiting for the signal
// Add the signal in the vKern_current->oSynchro.oListSignals
//
// When the process is scheduled, cancel it and return the signal

	else {
		if (timeout > 0) {
			vKern_current->oSynchro.oListSignals |= *bitSignal;
			vKern_current->oInternal.oTimeout = (timeout/KTIMEUNIT);

			GOTO_KERN_I(KWAITSIGN);
			theSignal = vKern_current->oSynchro.oListSignalsPending & *bitSignal;
			*bitSignal = theSignal;
			vKern_current->oSynchro.oListSignals        &= ~theSignal;
			vKern_current->oSynchro.oListSignalsPending &= ~theSignal;

			TRACE("exit: sign_waitSignal ->");
			RETURN_INT_RESTORED(vKern_current->oInternal.oStatus);
		}
		else {
			TRACE("exit: sign_waitSignal KO 1");
			RETURN_INT_RESTORED(KKERNTIMEO);
		}

	}
}

/*
 * \brief Kill the signal
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    sign_t    *signal;
 *
 *    status = kern_killSignal(“message OK”, &signal);
 * \endcode
 *
 * - If (process still connected to the signal list)
 *   - Then return error
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 * \param[out]	KKERNSINOI 	The signal is not initialized
 *
 */
int32_t	sign_killSignal(volatile sign_t *handle) {
				uint16_t	i;
	volatile	proc_t		*process;
	volatile	sign_t		*signal;

	INTERRUPTION_OFF;
	TRACE("entry: sign_killSignal");
	vKern_current->oStatistic.oNbKernCalls++;
	signal = handle;
	if (signal == NULL)							     { TRACE("exit: sign_killSignal KO 1"); RETURN_INT_RESTORED(KKERNNOSIO); }
	if (signal->oIdentifier == NULL)				 { TRACE("exit: sign_killSignal KO 2"); RETURN_INT_RESTORED(KKERNNOSIO); }
	if ((signal->oState & (1<<BSIGNINSTALLED)) == 0) { TRACE("exit: sign_killSignal KO 3"); RETURN_INT_RESTORED(KKERNSINOI); }

	if (vKern_listSign.oNbElements > 0) {
		for (i = 0; i < vKern_listSign.oNbElements; i++) {
			process = vKern_listSign.oFirst;
			process->oSynchro.oListSignals        &= ~(signal->oBitSignal);
			process->oSynchro.oListSignalsPending &= ~(signal->oBitSignal);
			if (process->oSynchro.oListSignals == 0) {
				process->oInternal.oState &= ~(1<<BPROCSUSPSIGN);
				chls_disconnect(&vKern_listSign, process);
				chls_connect(&vKern_listExec, process);
				process->oInternal.oStatus = KKERNSIKIL;
			}
		}
	}
	signal->oIdentifier = NULL;
	signal->oState      = 0;
	TRACE("exit: sign_killSignal OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of an signal by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Signal 0”;
 * volatile    sign_t    *signal;
 *
 *    status = kern_getSignalById(identifier, &signal);
 * \endcode
 *
 * - This function returns the handle of the signal
 *
 * \param[in]	*identifier	Ptr on the signal Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOSIO 	The signal does not exist
 *
 */
int32_t	sign_getSignalById(const char_t *identifier, volatile sign_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: sign_getSignalById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBSIGNALS; i++) {
		if (buff_cmpStrings(vKern_signal[i].oIdentifier, identifier)) {
			*handle = &vKern_signal[i];
			TRACE("exit: sign_getSignalById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: sign_getSignalById KO");
	RETURN_INT_RESTORED(KKERNNOSIO);
}
#endif
