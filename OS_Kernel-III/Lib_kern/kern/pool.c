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

#include 	<uKOS.h>

#if (defined(__WITHPOOL__))
extern	volatile	proc_t		*vKern_current;							// Ptr on the current process
		volatile	pool_t		vKern_pool[KNBPOOLS];					// Pools

/*
 * \brief Initialize the manager
 *
 * - This function initializes the "pool" manager.
 *   Before using the manager functions, it is necessary to
 *   call this function
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	pool_init(void) {
	uint16_t	i;

	TRACE("entry: pool_init");
	for (i = 0; i < KNBPOOLS; i++) {
		vKern_pool[i].oIdentifier = NULL;
		vKern_pool[i].oState      = 0;
		vKern_pool[i].oNbBlocks   = 0;
		vKern_pool[i].oBlockSize  = 0;
		vKern_pool[i].oBlockArray = NULL;
		vKern_pool[i].oUsedBlocks = NULL;
	}
	TRACE("exit: pool_init OK");
}

/*
 * \brief Create a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Xfer buffer”;
 * volatile    pool_t    *memoryPool;
 *
 *    status = kern_createPool(identifier, &memoryPool);
 * \endcode
 *
 * \param[in]	*identifier	Ptr on the memory pool
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR  OK
 * \param[out]	KKERNPOFUL 	No more memory pool
 * \param[out]	KKERNIDPOI	The memory pool Id is already used
 *
 */
int32_t	pool_createPool(const char_t *identifier, volatile pool_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: pool_createPool");
	vKern_current->oStatistic.oNbKernCalls++;
	*handle = NULL;
	for (i = 0; i < KNBPOOLS; i++) {
		if (buff_cmpStrings(vKern_pool[i].oIdentifier, identifier)) { TRACE("exit: pool_createPool KO 1"); RETURN_INT_RESTORED(KKERNIDPOI); }
	}

	for (i = 0; i < KNBPOOLS; i++) {
		if (vKern_pool[i].oIdentifier == NULL) {
			vKern_pool[i].oState      |= (1<<BPOOLINSTALLED);
			vKern_pool[i].oIdentifier = identifier;
			*handle = &vKern_pool[i];
			TRACE("exit: pool_createPool OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: pool_createPool KO 2");
	RETURN_INT_RESTORED(KKERNPOFUL);
}

/*
 * \brief Set a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * pcnf_t      configure;
 * volatile    pool_t    *memoryPool;
 *
 *    configure.oNbBlocks  = 16;
 *    configure.oBlockSize = 256;
 *    configure.oLocation  = KINTERNAL;
 *
 *    status = kern_setPool(memoryPool, &configure);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 * \param[out]	KKERNSTNOI 	The memory pool configuration is not possible
 *
 */
int32_t	pool_setPool(volatile pool_t *handle, pcnf_t *configure) {
				uint16_t	i;
				uint32_t	nbBlocks, blockSize;
				uint8_t		*blockArray;
				void		**usedBlocks;
	volatile	pool_t		*memoryPool;

	INTERRUPTION_OFF;
	TRACE("entry: pool_setPool");
	vKern_current->oStatistic.oNbKernCalls++;
	memoryPool = handle;
	if (memoryPool == NULL)						         			 { TRACE("exit: pool_setPool KO 1"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if (memoryPool->oIdentifier == NULL)			     			 { TRACE("exit: pool_setPool KO 2"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if ((memoryPool->oState & (1<<BPOOLINSTALLED)) == 0) 			 { TRACE("exit: pool_setPool KO 3"); RETURN_INT_RESTORED(KKERNPONOI); }
	if ((configure->oNbBlocks == 0)	|| (configure->oBlockSize == 0)) { TRACE("exit: pool_setPool KO 4"); RETURN_INT_RESTORED(KKERNPOCNF); }

// Consider always multiple of 4 bytes

	nbBlocks  = configure->oNbBlocks;
	blockSize = (configure->oBlockSize + 3) & 0xFFFFFFFC;

// Reserve the "array" and the "used block" vector
// Initialize the vector

	blockArray = (uint8_t *)syos_malloc(configure->oLocation, nbBlocks*blockSize*sizeof(uint8_t));
	if (blockArray == NULL) {
		TRACE("exit: pool_setPool KO 5");
		RETURN_INT_RESTORED(KKERNPOCNF);
	}

	usedBlocks = (void **)syos_malloc(configure->oLocation, nbBlocks*sizeof(uint8_t *));
	if (blockArray == NULL)	{
		syos_free(blockArray);
		TRACE("exit: pool_setPool KO 6");
		RETURN_INT_RESTORED(KKERNPOCNF);
	}

	memoryPool->oNbBlocks   = nbBlocks;
	memoryPool->oBlockSize  = blockSize;
	memoryPool->oBlockArray = blockArray;
	memoryPool->oUsedBlocks = usedBlocks;

	for (i = 0; i < memoryPool->oNbBlocks; i++) {
		memoryPool->oUsedBlocks[i] = NULL;
	}
	TRACE("exit: pool_setPool OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Allocate a block in a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    pool_t    *memoryPool;
 * void        *address;
 *
 *    status = kern_allocateBlock(memoryPool, &address);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 * \param[out]	KKERNBKFUL 	No more block
 *
 */
int32_t	pool_allocateBlock(volatile pool_t *handle, void **address) {
				uint16_t	i;
	volatile	pool_t		*memoryPool;

	INTERRUPTION_OFF;
	TRACE("entry: pool_allocateBlock");
	vKern_current->oStatistic.oNbKernCalls++;
	memoryPool = handle;
	if (memoryPool == NULL)						         { TRACE("exit: pool_allocateBlock KO 1"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if (memoryPool->oIdentifier == NULL)			     { TRACE("exit: pool_allocateBlock KO 2"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if ((memoryPool->oState & (1<<BPOOLINSTALLED)) == 0) { TRACE("exit: pool_allocateBlock KO 3"); RETURN_INT_RESTORED(KKERNPONOI); }

	for (i = 0; i < memoryPool->oNbBlocks; i++) {
		if (memoryPool->oUsedBlocks[i] == NULL) {
			memoryPool->oUsedBlocks[i] = (void *)((uint32_t)memoryPool->oBlockArray + i*memoryPool->oBlockSize);
			*address = memoryPool->oUsedBlocks[i];
			RETURN_INT_RESTORED(KKERNNOERR);

		}
	}
	TRACE("exit: pool_allocateBlock KO 4");
	RETURN_INT_RESTORED(KKERNBKFUL);
}

/*
 * \brief deallocate a block in a memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    pool_t    *memoryPool;
 * void        *address;
 *
 *    status = kern_deAllocateBlock(memoryPool, address);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[in]	*configure	Ptr on the configure
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 * \param[out]	KKERNNOBKI 	The block does not exist
 *
 */
int32_t	pool_deAllocateBlock(volatile pool_t *handle, void *address) {
				uint16_t	i;
	volatile	pool_t		*memoryPool;

	INTERRUPTION_OFF;
	TRACE("entry: pool_deAllocateBlock");
	vKern_current->oStatistic.oNbKernCalls++;
	memoryPool = handle;
	if (memoryPool == NULL)						         { TRACE("exit: pool_deAllocateBlock KO 1"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if (memoryPool->oIdentifier == NULL)			     { TRACE("exit: pool_deAllocateBlock KO 2"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if ((memoryPool->oState & (1<<BPOOLINSTALLED)) == 0) { TRACE("exit: pool_deAllocateBlock KO 3"); RETURN_INT_RESTORED(KKERNPONOI); }

	for (i = 0; i < memoryPool->oNbBlocks; i++) {
		if (memoryPool->oUsedBlocks[i] == address) {
			memoryPool->oUsedBlocks[i] = NULL;
			RETURN_INT_RESTORED(KKERNNOERR);

		}
	}
	TRACE("exit: pool_deAllocateBlock KO 4");
	RETURN_INT_RESTORED(KKERNNOBKI);
}

/*
 * \brief Kill the memory pool
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * volatile    pool_t    *memoryPool;
 *
 *    status = kern_killSoftwareTimer(memoryPool);
 * \endcode
 *
 * \param[in]	*handle		Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 * \param[out]	KKERNPONOI 	The memory pool is not initialized
 *
 */
int32_t	pool_killPool(volatile pool_t *handle) {
	volatile	pool_t	*memoryPool;

	INTERRUPTION_OFF;
	TRACE("entry: pool_killPool");
	vKern_current->oStatistic.oNbKernCalls++;
	memoryPool = handle;
	if (memoryPool == NULL)						         { TRACE("exit: pool_killPool KO 1"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if (memoryPool->oIdentifier == NULL) 			     { TRACE("exit: pool_killPool KO 2"); RETURN_INT_RESTORED(KKERNNOPOI); }
	if ((memoryPool->oState & (1<<BPOOLINSTALLED)) == 0) { TRACE("exit: pool_killPool KO 3"); RETURN_INT_RESTORED(KKERNPONOI); }

	syos_free(memoryPool->oBlockArray);
	syos_free(memoryPool->oUsedBlocks);

	memoryPool->oIdentifier = NULL;
	memoryPool->oState      = 0;
	memoryPool->oNbBlocks   = 0;
	memoryPool->oBlockSize	= 0;
	memoryPool->oBlockArray = NULL;
	memoryPool->oUsedBlocks = NULL;
	TRACE("exit: pool_killPool OK");
	RETURN_INT_RESTORED(KKERNNOERR);
}

/*
 * \brief Get the handle of a memory pool by its Id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * const       char_t    identifier[] = ”Xfer buffer”;
 * volatile    pool_t    *memoryPool;
 *
 *    status = kern_getPoolById(identifier, &memoryPool);
 * \endcode
 *
 * - This function returns the handle of the memory pool
 *
 * \param[in]	*identifier	Ptr on the memory pool Id
 * \param[in]	**handle	Ptr on the handle
 * \param[out]	KKERNNOERR	OK
 * \param[out]	KKERNNOPOI	The memory pool does not exist
 *
 */
int32_t	pool_getPoolById(const char_t *identifier, volatile pool_t **handle) {
	uint16_t	i;

	INTERRUPTION_OFF;
	TRACE("entry: pool_getPoolById");
	vKern_current->oStatistic.oNbKernCalls++;
	for (i = 0; i < KNBPOOLS; i++) {
		if (buff_cmpStrings(vKern_pool[i].oIdentifier, identifier)) {
			*handle = &vKern_pool[i];
			TRACE("exit: pool_getPoolById OK");
			RETURN_INT_RESTORED(KKERNNOERR);
		}

	}
	TRACE("exit: pool_getPoolById KO");
	RETURN_INT_RESTORED(KKERNNOPOI);
}
#endif
