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

#include 	<uKOS.h>

#if (defined(__WITHMBOX__))
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	mbox_t		vKern_mailBox[KNBMBOX];					// Mailboxes

// Prototypes
// ==========

static	int32_t	_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size);
static	int32_t	_readMailbox(volatile mbox_t *handle, void **message, uint16_t *size);

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "mbox" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	mbox_init(void) {
	uint16_t	i, j;

	TRACE("entry: mbox_init");
	for (i = 0; i < KNBMBOX; i++) {
		for (j = 0; j < KNBPACKS; j++) {
			vKern_mailBox[i].oFIFO[j].oBuff = NULL;
			vKern_mailBox[i].oFIFO[j].oSize = 0;
		}

		vKern_mailBox[i].oIdentifier = NULL;
		vKern_mailBox[i].oState = 0;
		vKern_mailBox[i].oWrite = &vKern_mailBox[i].oFIFO[0];
		vKern_mailBox[i].oRead  = &vKern_mailBox[i].oFIFO[0];
	}
	TRACE("exit: mbox_init OK");
}

/*
 * \brief Create a mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Mailbox 0”;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_createMailbox(identifier, &mailBox);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the mailbox Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNMBFUL 	No more mailbox
 * \param[out]	KKERNIDMBO	The mailbox Id is already used
 *
 */
int32_t	mbox_createMailbox(const char_t *identifier, volatile mbox_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: mbox_createMailbox");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBMBOX; i++) {
		if (buff_cmpStrings(vKern_mailBox[i].oIdentifier, identifier)) { TRACE("exit: mbox_createMailbox KO 1"); RETURN_INT_RESTORED(KKERNIDMBO); }
	}

	for (i = 0; i < KNBMBOX; i++) {
		if (vKern_mailBox[i].oIdentifier == NULL) {
			vKern_mailBox[i].oState      |= ((1<<BMBOXINSTALLED) | (1<<BMBOXEMPTY));
			vKern_mailBox[i].oIdentifier  = identifier;
			vKern_mailBox[i].oWrite       = &vKern_mailBox[i].oFIFO[0];
			vKern_mailBox[i].oRead        = &vKern_mailBox[i].oFIFO[0];
			*handle = &vKern_mailBox[i];
			TRACE("exit: mbox_createMailbox OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: mbox_createMailbox KO 2");
	RETURN_INT_RESTORED(KKERNMBFUL);
}

/*
 * \brief Write a message in the mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * #define     KSIZE    5
 * int32_t     status;
 * uint8_t     message[KSIZE] = {1,2,3,4,5};
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_writeMessage(mailBox, message, KSIZE);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*message	Ptr on the message
 * \param[in]	size		Size of the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNTOPAC 	The mailbox is full
 *
 */
int32_t	mbox_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _writeMailbox(handle, message, size);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Write a message in the queue
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint8_t     message = 0x2345;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_writeQueue(mailBox, message);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	message		Message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNTOPAC 	The mailbox is full
 *
 */
int32_t	mbox_writeQueue(volatile mbox_t *handle, uint32_t message) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _writeMailbox(handle, (void *)message, 1);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Read a message from the mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint8_t     size, *message:
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_readMessage(mailBox, &message, &size);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	**message	Ptr on the message
 * \param[in]	*size		Ptr on the	size of the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNNOPAC 	The mailbox is empty
 *
 */
int32_t	mbox_readMailbox(volatile mbox_t *handle, uint8_t **message, uint16_t *size) {
	int32_t		status;

	INTERRUPTION_OFF;
	status = _readMailbox(handle, (void *)message, size);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Read a message from the queue
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    message:
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_readQueue(mailBox, &message);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*message	Ptr on the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNNOPAC 	The mailbox is empty
 *
 */
int32_t	mbox_readQueue(volatile mbox_t *handle, uint32_t *message) {
	int32_t		status;
	uint16_t 	size;

	size = 1;
	INTERRUPTION_OFF;
	status = _readMailbox(handle, (void *)message, &size);
	RETURN_INT_RESTORED(status);
}

/*
 * \brief Kill the mailbox
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_killMailbox(mailBox);
 * \endcode
 *
 * - If (mailbox still contain messages)
 *   Then return error
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNPAPRP 	The mailbox cannot be removed; a message is still inside the mailbox
 *
 */
int32_t	mbox_killMailbox(volatile mbox_t *handle) {
				uint16_t	i;
	volatile	mbox_t		*mailBox;

	INTERRUPTION_OFF;
	TRACE("entry: mbox_killMailbox");
	vKern_current->oStatistic.oNbKernCalls++;
	mailBox = handle;
	if (mailBox == NULL) 				  		      { TRACE("exit: mbox_killMailbox KO 1"); RETURN_INT_RESTORED(KKERNNOMBO); }
	if (mailBox->oIdentifier == NULL)	  		      { TRACE("exit: mbox_killMailbox KO 2"); RETURN_INT_RESTORED(KKERNNOMBO); }
	if ((mailBox->oState & (1<<BMBOXINSTALLED)) == 0) { TRACE("exit: mbox_killMailbox KO 3"); RETURN_INT_RESTORED(KKERNMBNOI); }
	if ((mailBox->oState & (1<<BMBOXEMPTY))     != 0) { TRACE("exit: mbox_killMailbox KO 4"); RETURN_INT_RESTORED(KKERNPAPRP); }

	for (i = 0; i < KNBPACKS; i++) {
		mailBox->oFIFO[i].oBuff = NULL;
		mailBox->oFIFO[i].oSize = 0;
	}

	mailBox->oIdentifier = NULL;
	mailBox->oState      = 0;
	mailBox->oWrite      = &mailBox->oFIFO[0];
	mailBox->oRead       = &mailBox->oFIFO[0];
	TRACE("exit: mbox_killMailbox OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of a mailbox by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Mailbox 0”;
 * volatile    mbox_t    *mailBox;
 *
 *    status = kern_getMailboxById(identifier, &mailBox);
 * \endcode
 *
 * - This function returns the handle of the mailbox
 *
 * \param[in]	*identifier	Ptr on the mailbox Id
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 *
 */
int32_t	mbox_getMailboxById(const char_t *identifier, volatile mbox_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: mbox_getMailboxById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBMBOX; i++) {
		if (buff_cmpStrings(vKern_mailBox[i].oIdentifier, identifier)) {
			*handle = &vKern_mailBox[i];
			TRACE("exit: mbox_getMailboxById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: mbox_getMailboxById KO");
	RETURN_INT_RESTORED(KKERNNOMBO);
}

// Local routines
// ==============

/*
 * \brief _writeMailbox
 *
 * - Generic write to a mailbox or queue
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*message	Ptr on the message
 * \param[in]	size		Size of the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNTOPAC 	The mailbox is full
 *
 */
static	int32_t	_writeMailbox(volatile mbox_t *handle, void *message, uint16_t size) {
	volatile	mbox_t	*mailBox;

	TRACE("entry: mbox_writeMailbox");
	vKern_current->oStatistic.oNbKernCalls++;
	mailBox = handle;
	if (mailBox == NULL) 				 		      { TRACE("exit: mbox_writeMailbox KO 1"); return (KKERNNOMBO); }
	if (mailBox->oIdentifier == NULL)	 		      { TRACE("exit: mbox_writeMailbox KO 2"); return (KKERNNOMBO); }
	if ((mailBox->oState & (1<<BMBOXINSTALLED)) == 0) { TRACE("exit: mbox_writeMailbox KO 3"); return (KKERNMBNOI); }
	if ((mailBox->oState & (1<<BMBOXFULL))      != 0) { TRACE("exit: mbox_writeMailbox KO 4"); return (KKERNTOPAC); }

	mailBox->oWrite->oBuff = message;
	mailBox->oWrite->oSize = size;
	mailBox->oWrite++;

	if (mailBox->oWrite == (&mailBox->oFIFO[KNBPACKS])) {
		mailBox->oWrite = &mailBox->oFIFO[0];
	}
	mailBox->oState &= ~(1<<BMBOXEMPTY);
	if (mailBox->oWrite == mailBox->oRead) {
		mailBox->oState |= (1<<BMBOXFULL);
	}
	TRACE("exit: mbox_writeMailbox OK");
	return (KKERNNOERR);
}

/*
 * \brief _readMailbox
 *
 * - Generic read from a mailbox or queue
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	**message	Ptr on the message
 * \param[in]	*size		Ptr on the	size of the message
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNNOMBO 	The mailbox does not exist
 * \param[out]	KKERNMBNOI 	The mailbox is not initialized
 * \param[out]	KKERNNOPAC 	The mailbox is empty
 *
 */
static	int32_t	_readMailbox(volatile mbox_t *handle, void **message, uint16_t *size) {
	volatile	mbox_t	*mailBox;

	TRACE("entry: mbox_readMailbox");
	vKern_current->oStatistic.oNbKernCalls++;
	mailBox = handle;
	if (mailBox == NULL) 				  		      { TRACE("exit: mbox_readMailbox KO 1"); return (KKERNNOMBO); }
	if (mailBox->oIdentifier == NULL)	  		      { TRACE("exit: mbox_readMailbox KO 2"); return (KKERNNOMBO); }
	if ((mailBox->oState & (1<<BMBOXINSTALLED)) == 0) { TRACE("exit: mbox_readMailbox KO 3"); return (KKERNMBNOI); }
	if ((mailBox->oState & (1<<BMBOXEMPTY))     != 0) { TRACE("exit: mbox_readMailbox KO 4"); return (KKERNNOPAC); }

	*message = mailBox->oRead->oBuff;
	*size = mailBox->oRead->oSize;
	mailBox->oRead++;

	if (mailBox->oRead == (&mailBox->oFIFO[KNBPACKS])) {
		mailBox->oRead = &mailBox->oFIFO[0];
	}
	mailBox->oState &= ~(1<<BMBOXFULL);
	if (mailBox->oWrite == mailBox->oRead) {
		mailBox->oState |= (1<<BMBOXEMPTY);
	}
	TRACE("exit: mbox_readMailbox OK");
	return (KKERNNOERR);
}
#endif
