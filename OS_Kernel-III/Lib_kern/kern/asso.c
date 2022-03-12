/*
; asso.
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
; Goal:		Kern - Association management.
;
;			This module implements the association primitives.
;
; 			Association system calls
; 			------------------------
;
;			void	asso_init(void);
;			int32_t	asso_createAssociation(const char_t *identifier, volatile astp_t **handle);
;			int32_t	asso_publishAssociation(volatile astp_t *handle, shar_t *pack);
;			int32_t	asso_findAssociation(volatile astp_t *handle, volatile shar_t **pack);
;			int32_t	asso_killAssociation(volatile astp_t *handle);
;			int32_t	asso_getAssociationById(const char_t *identifier, volatile astp_t **handle);
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

#if (defined(__WITHASSO__))
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	astp_t		vKern_association[KNBASTP];				// Id to pointer associations

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "asso" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	asso_init(void) {
	uint16_t	i;

	TRACE("entry: asso_init");
	for (i = 0; i < KNBASTP; i++) {
		vKern_association[i].oIdentifier    = NULL;
		vKern_association[i].oState         = 0;
		vKern_association[i].oPack.oGeneral = NULL;
		vKern_association[i].oPack.oSize 	= 0;
	}
	TRACE("exit: asso_init OK");
}

/*
 * \brief Create an association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Association 0”;
 * volatile    astp_t    *association;
 *
 *    status = kern_createAssociation(identifier, &association);
 * \endcode
 *
 * - Associate a string to a generic pointer
 *
 * \param[in]	*identifier	Ptr on the association Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNASFUL 	No more association
 * \param[out]	KKERNIDASS	The association Id is already used
 *
 */
int32_t	asso_createAssociation(const char_t *identifier, volatile astp_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: asso_createAssociation");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBASTP; i++) {
		if (buff_cmpStrings(vKern_association[i].oIdentifier, identifier)) { TRACE("exit: asso_createAssociation KO 1"); RETURN_INT_RESTORED(KKERNIDASS); }
	}

	for (i = 0; i < KNBASTP; i++) {
		if (vKern_association[i].oIdentifier == NULL) {
			vKern_association[i].oState      |= (1<<BASSOINSTALLED);
			vKern_association[i].oIdentifier = identifier;
			*handle = &vKern_association[i];
			TRACE("exit: asso_createAssociation OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: asso_createAssociation KO 2");
	RETURN_INT_RESTORED(KKERNASFUL);
}

/*
 * \brief Publish an association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    sharedBuffer[123];
 * shar_t      pack;
 * volatile    astp_t    *association;
 *
 *    pack.oGeneral = (void *)&sharedBuffer[0];
 *    pack.oSize = sizeof(sharedBuffer);
 *    status = kern_publishAssociation(association, &pack);
 * \endcode
 *
 * - Publish the association
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*pack		Ptr on the data pack association
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 * \param[out]	KKERNASNOI 	The association is not initialized
 *
 */
int32_t	asso_publishAssociation(volatile astp_t *handle, shar_t *pack) {
	volatile	astp_t	*association;

	INTERRUPTION_OFF;
	TRACE("entry: asso_sharedAssociation");
	vKern_current->oStatistic.oNbKernCalls++;
	association = handle;
	if (association == NULL)						      { TRACE("exit: asso_sharedAssociation KO 1"); RETURN_INT_RESTORED(KKERNNOASS); }
	if (association->oIdentifier == NULL)			      { TRACE("exit: asso_sharedAssociation KO 2"); RETURN_INT_RESTORED(KKERNNOASS); }
	if ((association->oState & (1<<BASSOINSTALLED)) == 0) { TRACE("exit: asso_sharedAssociation KO 3"); RETURN_INT_RESTORED(KKERNASNOI); }

	association->oPack.oGeneral = pack->oGeneral;
	association->oPack.oSize    = pack->oSize;
	TRACE("exit: asso_sharedAssociation OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Find an association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    *counter, size;
 * shar_t      pack;
 * volatile    astp_t    *association;
 *
 *    status = kern_findAssociation(association, &pack);
 *    counter = (uint32_t *)pack->oGeneral;
 *    size = pack->oSize;
 * \endcode
 *
 * - Find an association (check for the string reference)
 * - Return the associated pointer
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	**pack		Ptr on the data pack association
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 * \param[out]	KKERNASNOI 	The association is not initialized
 *
 */
int32_t	asso_findAssociation(volatile astp_t *handle, volatile shar_t **pack) {
	volatile	astp_t	*association;

	INTERRUPTION_OFF;
	TRACE("entry: asso_findAssociation");
	vKern_current->oStatistic.oNbKernCalls++;
	association = handle;
	if (association == NULL)						      { TRACE("exit: asso_findAssociation KO 1"); RETURN_INT_RESTORED(KKERNNOASS); }
	if (association->oIdentifier == NULL)			      { TRACE("exit: asso_findAssociation KO 2"); RETURN_INT_RESTORED(KKERNNOASS); }
	if ((association->oState & (1<<BASSOINSTALLED)) == 0) { TRACE("exit: asso_findAssociation KO 3"); RETURN_INT_RESTORED(KKERNASNOI); }
	if (association->oPack.oGeneral == NULL)		      { TRACE("exit: asso_findAssociation KO 4"); RETURN_INT_RESTORED(KKERNNOASS); }
	if (association->oPack.oSize == 0)				      { TRACE("exit: asso_findAssociation KO 5"); RETURN_INT_RESTORED(KKERNNOASS); }

	*pack = &association->oPack;
	TRACE("exit: asso_findAssociation OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Kill the association
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    astp_t    *association;
 *
 *    status = kern_killAssociation(association);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 * \param[out]	KKERNASNOI 	The association is not initialized
 *
 */
int32_t	asso_killAssociation(volatile astp_t *handle) {
	volatile	astp_t	*association;

	INTERRUPTION_OFF;
	TRACE("entry: asso_killAssociation");
	vKern_current->oStatistic.oNbKernCalls++;
	association = handle;
	if (association == NULL)						      { TRACE("exit: asso_killAssociation KO 1"); RETURN_INT_RESTORED(KKERNNOASS); }
	if (association->oIdentifier == NULL) 			      { TRACE("exit: asso_killAssociation KO 2"); RETURN_INT_RESTORED(KKERNNOASS); }
	if ((association->oState & (1<<BASSOINSTALLED)) == 0) { TRACE("exit: asso_killAssociation KO 3"); RETURN_INT_RESTORED(KKERNASNOI); }

	association->oIdentifier    = NULL;
	association->oState         = 0;
	association->oPack.oGeneral = NULL;
	association->oPack.oSize    = 0;
	TRACE("exit: asso_killAssociation OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of an association by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Association 0”;
 * volatile    astp_t    *association;
 *
 *    status = kern_getAssociationById(identifier, &association);
 * \endcode
 *
 * - This function returns the handle of the association
 *
 * \param[in]	*identifier	Ptr on the association Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOASS	The association does not exist
 *
 */
int32_t	asso_getAssociationById(const char_t *identifier, volatile astp_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: asso_getAssociationById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBASTP; i++) {
		if (buff_cmpStrings(vKern_association[i].oIdentifier, identifier)) {
			*handle = &vKern_association[i];
			TRACE("exit: asso_getAssociationById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: asso_getAssociationById KO");
	RETURN_INT_RESTORED(KKERNNOASS);
}
#endif
