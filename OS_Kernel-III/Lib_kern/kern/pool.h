/*
; pool.
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
; 			Memory pool system calls
; 			------------------------
;
;			void	pool_init(void);
;			int32_t	pool_createPool(const char_t *identifier, volatile pool_t **handle);
;			int32_t	pool_setPool(volatile pool_t *handle, pcnf_t *configure);
;			int32_t	pool_allocateBlock(volatile pool_t *handle, void **address);
;			int32_t	pool_deAllocateBlock(volatile pool_t *handle, void *address);
;			int32_t	pool_killPool(volatile pool_t *handle);
;			int32_t	pool_getPoolById(const char_t *identifier, volatile pool_t **handle);
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

#ifndef	__POOL__
#define	__POOL__

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

extern	void	pool_init(void);
extern	int32_t	pool_createPool(const char_t *identifier, volatile pool_t **handle);
extern	int32_t	pool_setPool(volatile pool_t *handle, pcnf_t *configure);
extern	int32_t	pool_allocateBlock(volatile pool_t *handle, void **address);
extern	int32_t	pool_deAllocateBlock(volatile pool_t *handle, void *address);
extern	int32_t	pool_killPool(volatile pool_t *handle);
extern	int32_t	pool_getPoolById(const char_t *identifier, volatile pool_t **handle);

#ifdef	__cplusplus
}
#endif

// Structure of the memory memory pool
// -----------------------------------

struct	pcnf {
			uint32_t	oNbBlocks;										// Number of blocks
			uint32_t	oBlockSize;										// Block size
			uint32_t	oLocation;										// Memory pool location
};

struct	pool {
	const	char_t		*oIdentifier;									// Memory pool identifier
			uint8_t		oState;											// Status
			#define		BPOOLINSTALLED	0								// Memory pool installed

			uint32_t	oNbBlocks;										// Number of blocks
			uint32_t	oBlockSize;										// Block size
			void		*oBlockArray;									// Block array
			void		**oUsedBlocks;									// Used Blocks
};
#endif
