/*
; chls.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 178                  $:  Revision of last commit
; $Date:: 2017-07-15 21:15:17#$:  Date of last commit
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
; Desc 1 	+-----------------------------------+ A <-------+	|
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

#ifndef	__CHLS__
#define	__CHLS__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	chls_init(void);
extern	void	chls_initialize(volatile list_t *list);
extern	void	chls_connect(volatile list_t *list, volatile proc_t *handle);
extern	void	chls_disconnect(volatile list_t *list, volatile proc_t *handle);

#ifdef	__cplusplus
}
#endif

// Structure of the linked list & object
// -------------------------------------

struct	list {
	volatile	proc_t		*oFirst;									// Ptr on the first process
	volatile	proc_t		*oLast;										// Ptr on the last process
				uint16_t	oNbElements;								// Number of elements of the list
};

struct	obje {
	volatile	list_t		*oList;										// Ptr on the list where the process is linked
	volatile	proc_t		*oBack;										// Ptr on the back process
	volatile	proc_t		*oForward;									// Ptr on the forward process
};

#endif
