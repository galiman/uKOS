/*
; asso.
; =====

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

#ifndef	__ASSO__
#define	__ASSO__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	asso_init(void);
extern	int32_t	asso_createAssociation(const char_t *identifier, volatile astp_t **handle);
extern	int32_t	asso_publishAssociation(volatile astp_t *handle, shar_t *pack);
extern	int32_t	asso_findAssociation(volatile astp_t *handle, volatile shar_t **pack);
extern	int32_t	asso_killAssociation(volatile astp_t *handle);
extern	int32_t	asso_getAssociationById(const char_t *identifier, volatile astp_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the string to pointer association
// ----------------------------------------------

struct	shar {
			void		*oGeneral;										// The general Ptr
			uint32_t	oSize;											// Size of the area
};

struct	astp {
	const	char_t		*oIdentifier;									// Association identifier
			uint8_t		oState;											// Status
			#define		BASSOINSTALLED	0								// Association installed

			shar_t		oPack;											// Data pack association
};
#endif
