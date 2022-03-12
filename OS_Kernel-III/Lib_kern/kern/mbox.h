/*
; mbox.
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
; Goal:		Kern - mailbox management.
;
;			This module implements the mailbox primitives.
;
; 			Mailbox system calls
; 			--------------------
;
;			void	mbox_init(void);
;			int32_t	mbox_createMailbox(const char_t *identifier, volatile mbox_t **handle);
;			int32_t	mbox_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size);
;			int32_t	mbox_writeQueue(volatile mbox_t *handle, uint32_t message);
;			int32_t	mbox_readMailbox(volatile mbox_t *handle, uint8_t **message, uint16_t *size);
;			int32_t	mbox_readQueue(volatile mbox_t *handle, uint32_t *message);
;			int32_t	mbox_killMailbox(volatile mbox_t *handle);
;			int32_t	mbox_getMailboxById(const char_t *identifier, volatile mbox_t **handle);
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

#ifndef	__MBOX__
#define	__MBOX__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	mbox_init(void);
extern	int32_t	mbox_createMailbox(const char_t *identifier, volatile mbox_t **handle);
extern	int32_t	mbox_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size);
extern	int32_t	mbox_writeQueue(volatile mbox_t *handle, uint32_t message);
extern	int32_t	mbox_readMailbox(volatile mbox_t *handle, uint8_t **message, uint16_t *size);
extern	int32_t	mbox_readQueue(volatile mbox_t *handle, uint32_t *message);
extern	int32_t	mbox_killMailbox(volatile mbox_t *handle);
extern	int32_t	mbox_getMailboxById(const char_t *identifier, volatile mbox_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the pack and the mailbox
// -------------------------------------

struct	pack {
				uint8_t		*oBuff;										// Ptr on the memory message
				uint16_t	oSize;										// Size of the message
};

struct	mbox {
	const		char_t		*oIdentifier;								// MBox identifier
				uint8_t		oState;										// Status
				#define		BMBOXINSTALLED	0							// Mailbox installed
				#define		BMBOXEMPTY		1							// Mailbox empty
				#define		BMBOXFULL		2							// Mailbox full

				pack_t		oFIFO[KNBPACKS];							// MBox (fifo of packs)
	volatile	pack_t		*oWrite;									// Fifo write pointer
	volatile	pack_t		*oRead;										// Fifo read pointer
};
#endif
