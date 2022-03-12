/*
; asso_smpl.
; ==========

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
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 3 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Publish an association named "counter" with the ticcount
;			      Every 1000-ms
;				  Read the ticcount
;				  After 20 loops, P1 will commit a suicide
;
;			- P2: Look for an association named "counter"
;			      Every 500-ms
;				  Display the information
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

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"userapp      Example of how to use associations.       (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  userapp\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *      	- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Publish an association named "counter" with the ticcount
 *       - Every 1000-ms
 *  		- Read the ticcount
 * 			- After 20 loops, P1 will commit a suicide
 *
 */
static	void	aProcess_1(const void *argument) {
				uint8_t		i;
				uint64_t	counter;
				shar_t		pack;
	volatile	proc_t		*process;
	volatile	astp_t		*association;

	kern_getTiccount(&counter);

	pack.oGeneral = &counter;
	pack.oSize = sizeof(uint64_t);

	if (kern_createAssociation("Counter", &association) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_publishAssociation(association, &pack)     != KKERNNOERR) { exit(EXIT_FAILURE); }

	for (i = 0; i < 20; i++) {
		kern_suspendProcess(1000);
		misc_toggleLed(1);
		kern_getTiccount(&counter);
	}

// It is time to commit a suicide ...

	kern_getProcessRun(&process);
	kern_killProcess(process);
	while (TRUE);
}

/*
 * \brief aProcess 2
 *
 * - P2: - Look for an association named "counter"
 *       - Every 500-ms
 *  		- Display the information
 *
 */
static	void	aProcess_2(const void *argument) {
				uint64_t	*counter;
	volatile	shar_t		*pack;
	volatile	astp_t		*association;

// Waiting for the association OK

	while (kern_getAssociationById("Counter", &association) != KKERNNOERR) { kern_switchFast(); }
	while (kern_findAssociation(association, &pack)			!= KKERNNOERR) { kern_switchFast(); }

	counter = (uint64_t *)pack->oGeneral;

// The association is effective; link a pointer on it

	while (TRUE) {
		kern_suspendProcess(500);
		iotx_printf(KSYST, "Counter = %ld\n", *counter);
	}
}

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {
	volatile	proc_t 	*process_0, *process_1, *process_2;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrIden_2[]) =    "Process_User_2";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_2[]) =    "Process user 2.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(2, vSpecification_2, aStrText_2, KSZSTACKMIN, aProcess_2, aStrIden_2, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_2, NULL, &process_2) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

