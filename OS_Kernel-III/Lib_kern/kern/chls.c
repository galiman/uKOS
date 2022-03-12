/*
; chls.
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
; Goal:		Kern - List management.
;
;			This module manages the list primitives.
;
; 			List system calls
; 			-----------------
;
;			void	chls_init(void);
;			void	chls_initialize(volatile list_t *list);
;			void	chls_connect(volatile list_t *list, volatile proc_t *handle);
;			void	chls_disconnect(volatile list_t *list, volatile proc_t *handle);
;
;			A process descriptor
;			--------------------
;
;			31					 			0
;			+-------------------------------+
;			| Ptr on proper list			+
;			+-------------------------------+
;			| Ptr on the back process		+
;			+-------------------------------+
;			| Ptr on the forward process	+
;			+-------------------------------+
;			| Specifications				+
;			+-------------------------------+
;			| Internal						+
;			+-------------------------------+
;			| Statistic 					+
;			+-------------------------------+
;			| Synchro						+
;			+-------------------------------+
;
;			A list ...
;			----------
;
;			31								0
;			+-------------------------------+
;			| Ptr on the first process		+
;			+-------------------------------+
;			| Ptr on the last process		+
;			+-------------------------------+
;			| Ptr on the current process	+
;			+-------------------------------+
;			| Number of processes linked	+
;			+-------------------------------+
;
;			Example of linked list ...
;			--------------------------
;
;			31								    0
;			+-----------------------------------+ X
;			| Ptr on the first process	 = A	+ ------+
;			+-----------------------------------+		|
;			| Ptr on the last process	 = B	+ ------|-------+
;			+-----------------------------------+		|		|
;			| Number of processes linked = 3	+		|		|
;			+-----------------------------------+		|		|
;													<---+		|
; Decs 1 	+-----------------------------------+ A <-------+	|
;			| Ptr on proper list		 = X 	+			|	|
;			+-----------------------------------+			|	|
;			| Ptr on the back process	 = 0	+			|	|
;			+-----------------------------------+			|	|
;			| Ptr on the forward process = C	+ ------+	|	|
;			+-----------------------------------+		|	|	|
;													<---|---|---+
; Desc 3	+-----------------------------------+ B	<---|---|---+
;			| Ptr on proper list		 = X 	+		|	|	|
;			+-----------------------------------+		|	|	|
;			| Ptr on the back process	 = C	+ ------|---|---|---+
;			+-----------------------------------+		|	|	|	|
;			| Ptr on the forward process = 0	+		|	|	|	|
;			+-----------------------------------+		|	|	|	|
;													<---|	|	|	|
; Desc 2 	+-----------------------------------+ C <-------|---|---+
;			| Ptr on proper list		 = X 	+			|	|
;			+-----------------------------------+			|	|
;			| Ptr on the back process    = A	+ ----------+	|
;			+-----------------------------------+				|
;			| Ptr on the forward process = B	+ --------------+
;			+-----------------------------------+
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

enum	{
		KLISTEMPTY, 													// The list is empty
		KLISTNORMAL,													// The list is not empty (normal)
		KLISTALONE,														// Only one process connected
		KLISTFIRST,														// The process is the first of the list
		KLISTMIDDLE,													// The process is in the middle of the list
		KLISTLAST														// The process is the last of the list
		};

// Prototypes
// ==========

static	void	_nextAction(uint8_t action, volatile list_t *list, volatile proc_t *handle);

/*
 * \brief Initialize a list
 *
 * \param[in]	*list	Ptr on the list
 * \param[out]	-
 *
 */
void	chls_initialize(volatile list_t *list) {

	list->oFirst	  = NULL;
	list->oLast		  = NULL;
	list->oNbElements = 0;
}

/*
 * \brief Connect a process to a list
 *
 * 2 possible cases:
 * - Case 1: IF (The list is empty)
 *           - THEN connect the "process" at the begin of the "list"
 *
 * - Case 2: IF (The list is not empty)
 *           - THEN connect the "process" to the last "process" of the "list"
 *
 * \param[in]	*list	Ptr on the list
 * \param[in]	*handle	Ptr on the handle
 * \param[out]	-
 *
 */
void	chls_connect(volatile list_t *list, volatile proc_t *handle) {

	INTERRUPTION_OFF;

// Determine the next action

	(list->oFirst == NULL) ? _nextAction(KLISTEMPTY, list, handle) : _nextAction(KLISTNORMAL, list, handle);

	handle->oObject.oList    = list;
	handle->oObject.oForward = NULL;
	list->oNbElements++;
	INTERRUPTION_RESTORED;
}

/*
 * \brief Disconnect a process from a list
 *
 * 4 possible cases:
 * - Case 1: IF (The process is alone)
 *           - THEN disconnect the "process" from the "list"
 *
 * - Case 2: IF (The process is the first)
 *           - THEN disconnect the "process" from the "list"
 *		       the next "process" becomes the "first"
 *
 * - Case 3: IF (The process is in the middle)
 *           - THEN disconnect the "process" from the "list"
 *		       connect the next "process" to the previous one
 *
 * - Case 4: IF (The process is at the end)
 *           - THEN disconnect the "process" from the "list"
 *		       the previous "process" becomes the "last" one
 *
 * \param[in]	*list	Ptr on the list
 * \param[in]	*handle	Ptr on the handle
 * \param[out]	-
 *
 */
void	chls_disconnect(volatile list_t *list, volatile proc_t *handle) {

	INTERRUPTION_OFF;

// Determine the next action

	if (handle->oObject.oBack == NULL) {
		(handle->oObject.oForward == NULL) ? _nextAction(KLISTALONE, list, handle) : _nextAction(KLISTFIRST,  list, handle);
	}
	else {
		(handle->oObject.oForward == NULL) ? _nextAction(KLISTLAST,  list, handle) : _nextAction(KLISTMIDDLE, list, handle);
	}

	handle->oObject.oBack    = NULL;
	handle->oObject.oForward = NULL;
	handle->oObject.oList    = NULL;
	list->oNbElements--;
	INTERRUPTION_RESTORED;
}

/*
 * \brief _nextAction
 *
 * - The next action could be:
 *   - For the connection:
 *     - KLISTEMPTY:	The list is empty
 *     - KLISTNORMAL:	The list is not empty (normal)
 *   - For the disconnection:
 *     - KLISTALONE:	Only one process connected
 *     - KLISTFIRST:	The process is the first of the list
 *     - KLISTMIDDLE:	The process is in the middle of the list
 *     - KLISTLAST:		The process is the last of the list
 *
 */
static	void	_nextAction(uint8_t action, volatile list_t *list, volatile proc_t *handle) {

	switch (action) {

// Connect
// - The list is empty

		case KLISTEMPTY: {
			list->oFirst          = handle;
			list->oLast           = handle;
			handle->oObject.oBack = NULL;
			break;
		}

// Connect
// - The list is not empty (normal case)

		case KLISTNORMAL: {
			list->oLast->oObject.oForward = handle;
			handle->oObject.oBack         = list->oLast;
			list->oLast                   = handle;
			break;
		}

// Disconnect
// - Only one process connected

		case KLISTALONE: {
			list->oFirst = NULL;
			list->oLast  = NULL;
			break;
		}

// Disconnect
// - The process is the first of the list

		case KLISTFIRST: {
			list->oFirst                            = handle->oObject.oForward;
			handle->oObject.oForward->oObject.oBack = NULL;
			break;
		}

// Disconnect
// - The process is in the middle of the list

		case KLISTMIDDLE: {
			handle->oObject.oForward->oObject.oBack = handle->oObject.oBack;
			handle->oObject.oBack->oObject.oForward = handle->oObject.oForward;
			break;
		}

// Disconnect
// - The process is the last of the list

		case KLISTLAST: {
			list->oLast                             = handle->oObject.oBack;
			handle->oObject.oBack->oObject.oForward = NULL;
			break;
		}
		default: {
			break;
		}
	}
}
