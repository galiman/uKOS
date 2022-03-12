/*
; syos.
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
; Goal:		syos manager.
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
#include	<filer.h>

#define		KLNTEXTBLKICY	0x8000										// The checksum for the icyflex cpu (only RAM system) is 
																		// arbitrary limited to 32-K

// malloc structure
// ----------------

typedef	struct	mab		mab_t;

struct mab {
    		uint32_t	oSzAllocated;									// Block size (allocated)
    		uint32_t	oSzAvailable;									// Block size (available)
    		mab_t       *oPtrPreBlock;									// Ptr on the previous block
    		mab_t       *oPtrNexBlock;									// Ptr on the next block
		};

// Fifo trace
// ----------

					trace_t		vTraceFifo[KSZFIFO];
static				bool_t		vReserved        = FALSE;
static				void		*vDoLoCode       = NULL;
static	volatile	sema_t		*vSemaphore;

extern		uint32_t	_stUMemo;
extern		uint32_t	_stSCode, _lnSCode;
extern		uint32_t	_stHeapI, _lnHeapI;
extern		uint32_t	_stHeapE, _lnHeapE;
extern		uint32_t	_stHeapC, _lnHeapC;
static		uint32_t	vNbBlocksI       = 0;
static		uint32_t	vNbBlocksE       = 0;
static		uint32_t	vNbBlocksC       = 0;
			uint32_t	vSyos_usdMemoryI = 0;
			uint32_t	vSyos_usdMemoryE = 0;
			uint32_t	vSyos_usdMemoryC = 0;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"syos         syos manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "syos manager\n"
									"============\n\n"

									"This manager ...\n\n";

LOC_CONST_STRG(aStrIdSe[])		  = "Critical_malloc";

// Prototypes
// ==========

static	void	_init(void);
extern	int32_t	stub_syos_fastCopy(const void *source, void *destination, uint32_t size);

#if (defined(__MEM_MMX1__))
static	int32_t	_getIdModule(uint32_t idModule, uint16_t *index, const module_t **module);
static	int32_t	_getNameModule(const char_t *identifier, uint16_t *index, const module_t **module);
static	int32_t	_getFaModule(uint8_t family, uint32_t *idModule, uint16_t *index, const module_t **module);
#endif

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Syos, KIDGENE, KSYOSNUM, NULL, "3.0", (1<<BSHOW));

/*
 * \brief Reserve the syos manager (only for the user memory accesses)
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (syos_reserve(KDEVALL) == KSYOSCHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    syos_xyz();
 *    status = syos_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KSYOSNOERR	The manager is reserved
 * \param[out]	KSYOSCHBSY 	The manager is busy
 *
 */
int32_t	syos_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KSYOSCHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KSYOSNOERR);
}

/*
 * \brief Release the syos manager (only for the user memory accesses)
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = syos_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KSYOSNOERR	OK
 *
 */
int32_t	syos_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KSYOSNOERR);
}

/*
 * \brief Get the checksum of the OS
 *
 * Call example in C:
 *
 * \code{.c}
 * uint32_t    checksum;
 *
 *    checksum = syos_getCKSumOS(&checksum);
 * \endcode
 *
 * \param[in]	*checksum	Ptr on the checksum
 * \param[out]	KSYOSNOERR	OK
 *
 */
int32_t	syos_getCKSumOS(uint32_t *checksum) {
	uint8_t		*memory;
	uint32_t	i, nbBytes;

	_init();

	#if (defined(__icycam__) || defined(__icyamr__))
	memory = (uint8_t *)&_stSCode + KSZEXCEPTIONS;
	nbBytes = KLNTEXTBLKICY - KSZEXCEPTIONS;

	#else
	memory = (uint8_t *)&_stSCode;
	nbBytes = (uint32_t)&_lnSCode;
	#endif

	*checksum = 0;
	for (i = 0; i < nbBytes; i++) {
		*checksum += *memory++;
	}

	return (KSYOSNOERR);
}

/*
 * \brief Get the module handle by its Id (short)
 *
 * Call example in C:
 *
 * \code{.c}
 * #define  KIDMODULE  ((KIDPROCESS<<16) | KSTARTUPNUM)
 *
 * int32_t     status;
 * module_t    *module;
 * uint16_t    index;
 *
 *    status = syos_getIdModule(KMEM0, KIDMODULE, &index, &module);
 *    if (status == KSYOSNOERR) {
 *        module->oExecution(0, 0);
 *    }
 * \endcode
 *
 * \param[in]	mms			The Memory Mass Storage (KMEM0..KMEM1)
 * \param[in]	idModule	The module Id
 * \param[in]	*index		Ptr on the filer index
 * \param[in]	**module	Ptr on the module
 * \param[out]	KSYOSNOERR	OK
 * \param[out]	KSYOSNOMEM 	The memory unit does not exist
 * \param[out]	KSYOSNOMOD 	The module does not exist
 *
 */
int32_t	syos_getIdModule(uint32_t mms, uint32_t idModule, uint16_t *index, const module_t **module) {

	_init();

	*index = 0;
	switch (mms) {

// In system modules

		case KMEM0: {
			while (aDirectory[*index].oWhere != KNOMODULE) {
				if (aDirectory[*index].oModule->oIdModule == idModule) {
					*module = aDirectory[*index].oModule;
					return (KSYOSNOERR);
				}
				*index += 1;
			}
			return (KSYOSNOMOD);
		}

// In EEPROM modules

		#if (defined(__MEM_MMX1__))
		case KMEM1: return (_getIdModule(idModule, index, module));

		#else
		case KMEM1: return (KSYOSNOMOD);
		#endif

		default:    return (KSYOSNOMEM);
	}
}

/*
 * \brief Get the module handle by its name
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * module_t    *module;
 * uint16_t    index;
 *
 *    status = syos_getNameModule(KMEM0, “sloader”, &index, &module);
 *    if (status == KSYOSNOERR) {
 *        module->oExecution(0, 0);
 *    }
 * \endcode
 *
 * \param[in]	mms			The Memory Mass Storage (KMEM0..KMEM1)
 * \param[in]	*name		Ptr on the module name
 * \param[in]	*index		Ptr on the filer index
 * \param[in]	**module	Ptr on the module
 * \param[out]	KSYOSNOERR	OK
 * \param[out]	KSYOSNOMEM 	The memory unit does not exist
 * \param[out]	KSYOSNOMOD 	The module does not exist
 *
 */
int32_t	syos_getNameModule(uint32_t mms, const char_t *name, uint16_t *index, const module_t **module) {
	bool_t	equals;

	_init();

	*index = 0;
	switch (mms) {

// In system modules

		case KMEM0: {
			while (aDirectory[*index].oWhere != KNOMODULE) {
				text_checkAsciiBuffer(aDirectory[*index].oModule->oStrApplication, name, &equals);
				if (equals) {
					*module = aDirectory[*index].oModule;
					return (KSYOSNOERR);
				}
				*index += 1;
			}
			return (KSYOSNOMOD);
		}

// In EEPROM modules

		#if (defined(__MEM_MMX1__))
		case KMEM1: return (_getNameModule(name, index, module));

		#else
		case KMEM1: return (KSYOSNOMOD);
		#endif

		default:    return (KSYOSNOMEM);
	}
}

/*
 * \brief Get a module handle by its family and index
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KIDPRC    KIDPROCESS
 *
 * int32_t     status;
 * uint32_t    idModule;
 * module_t    *module;
 * uint16_t    index;
 *
 *    index = 0;
 *    while (syos_getFamilyModule(KMEM0, KIDPRC, &idModule, &index, &module) == KSYOSNOERR) {
 *        if (status == KSYOSNOERR) {
 *            module->oExecution(0, 0);
 *        }
 *    index++;
 *    }

 * \endcode
 *
 * \param[in]	mms			The Memory Mass Storage (KMEM0..KMEM1)
 * \param[in]	family		The module family
 * \param[in]	*idModule	Ptr on the module Id
 * \param[in]	*index		Ptr on the filer index
 * \param[in]	**module	Ptr on the module
 * \param[out]	KSYOSNOERR	OK
 * \param[out]	KSYOSNOMEM 	The memory unit does not exist
 * \param[out]	KSYOSNOFAM 	The family does not exist
 *
 */
int32_t	syos_getFamilyModule(uint32_t mms, uint8_t family, uint32_t *idModule, uint16_t *index, const module_t **module) {

	_init();

	switch (mms) {

// In system modules

		case KMEM0: {
			while (aDirectory[*index].oWhere != KNOMODULE) {
				if ((aDirectory[*index].oModule->oIdModule>>24) == family) {
					*idModule = aDirectory[*index].oModule->oIdModule;
					*module = aDirectory[*index].oModule;
					return (KSYOSNOERR);
				}
				*index += 1;
			}
			return (KSYOSNOFAM);
		}

// In EEPROM modules

		#if (defined(__MEM_MMX1__))
		case KMEM1: return (_getFaModule(family, idModule, index, module));

		#else
		case KMEM1: return (KSYOSNOMEM);
		#endif

		default:    return (KSYOSNOMEM);
	}
}

/*
 * \brief Erase a block
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    index;
 *
 *    index = 3;
 *    status = syos_eraseBlock(KMEM0, &index);
 * \endcode
 *
 * \param[in]	mms			The Memory Mass Storage (KMEM0..KMEM1)
 * \param[in]	*index		Ptr on the filer index
 * \param[out]	KSYOSNOERR	OK
 * \param[out]	KSYOSNOMEM 	The memory unit does not exist
 * \param[out]	KSYOSNOBLK 	The memory block does not exist
 *
 */
int32_t	syos_eraseBlock(uint32_t mms, const uint16_t *index) {

	_init();

	#if (defined(__MEM_MMX1__))
	uint32_t	address;

	if (mms != KMEM1) {
		return (KSYOSNOMEM);
	}

	if (*index >= KNBEEPROMAPPL) {
		return (KSYOSNOBLK);
	}

	address = KSZUKOSOS + *index*KSZEEPROMAPPL;
	glob_eraseMemory(address, KNBEEPROMSEC);

	return (KSYOSNOERR);

	#else
	return (KSYOSNOMEM);
	#endif
}

/*
 * \brief Write a block
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    index;
 *
 *    index = 3;
 *    status = syos_writeBlock(KMEM0, &index);
 * \endcode
 *
 * \param[in]	mms			The Memory Mass Storage (KMEM0..KMEM1)
 * \param[in]	*index		Ptr on the filer index
 * \param[out]	KSYOSNOERR	OK
 * \param[out]	KSYOSNOMEM 	The memory unit does not exist
 * \param[out]	KSYOSNOBLK 	The memory block does not exist
 *
 */
int32_t	syos_writeBlock(uint32_t mms, const uint16_t *index) {

	_init();

	#if (defined(__MEM_MMX1__))
	const	uint8_t		*source;
			uint32_t	length, address;
			header_t	*header;

	if (mms != KMEM1) {
		return (KSYOSNOMEM);
	}

	if (*index >= KNBEEPROMAPPL) {
		return (KSYOSNOBLK);
	}

	header  = (header_t *)&_stUMemo;
	address = KSZUKOSOS + *index*KSZEEPROMAPPL;
	source  = (uint8_t *)&_stUMemo;
	length  = header->oLnApplication;

	glob_writeMemory(address, source, length);
	return (KSYOSNOERR);

	#else
	return (KSYOSNOMEM);
	#endif
}

/*
 * \brief Set the downloaded code address
 *
 * Call example in C:
 *
 * \code{.c}
 * void    *address;
 *
 *    address = 0x12345678;
 *    status = syos_setDoLoCode(address);
 * \endcode
 *
 * \param[in]	*address	Ptr on the downloaded code
 * \param[out]	KSYOSNOERR	OK
 *
 */
int32_t	syos_setDoLoCode(void *address) {

	_init();

	vDoLoCode = address;
	return (KSYOSNOERR);
}

/*
 * \brief Get the downloaded code address
 *
 * Call example in C:
 *
 * \code{.c}
 * void    *address;
 *
 *    status = syos_getDoLoCode(&address);
 * \endcode
 *
 * \param[in]	**address	Ptr on the downloaded code
 * \param[out]	KSYOSNOERR	OK
 *
 */
int32_t	syos_getDoLoCode(void **address) {

	_init();

	*address = vDoLoCode;
	return (KSYOSNOERR);
}

/*
 * \brief Trace the code
 *
 * Call example in C:
 *
 * \code{.c}
 *    syos_trace(“Program call the routine _xyz\n”, 0x55);
 *    _xyz();
 *
 *    syos_trace(“Program write\n”, 0x24);
 * \endcode
 *
 * \param[in]	*message	Ptr on the message
 * \param[in]	parameter	Parameter
 * \param[out]	KSYOSNOERR	OK
 * \param[out]	KSYOSSYCNA 	System call not available
 *
 */
int32_t	syos_trace(const char_t *message, uint32_t parameter) {
	#if (defined(__FIFO_TRACE__))
	uint32_t	i;

	_init();

	INTERRUPTION_OFF;
	for (i = 0; i < (KSZFIFO-1); i++) {
		vTraceFifo[KSZFIFO-1-i].oIdentifier = vTraceFifo[KSZFIFO-2-i].oIdentifier;
		vTraceFifo[KSZFIFO-1-i].oParameter  = vTraceFifo[KSZFIFO-2-i].oParameter;
	}
		
	vTraceFifo[0].oIdentifier = message;
	vTraceFifo[0].oParameter  = parameter;
	RETURN_INT_RESTORED(KSYOSNOERR);

	#else
	return (KSYOSSYCNA);
	#endif
}

/*
 * \brief Fast copy
 *
 * Call example in C:
 *
 * \code{.c}
 * const    uint8_t    src8[123]   = { 0, 1, ...34, 56 };
 *          uint8_t    dst8[123];
 * const    uint16_t   src16[123]  = { 0, 1, ...34, 56 };
 *          uint16_t   dst16[123];
 * const    uint32_t   src32[123]  = { 0, 1, ...34, 56 };
 *          uint32_t   dst32[123];
 *
 *    syos_fastCopy(src8,  dst8,  123);
 *    syos_fastCopy(src16, dst16, (2*123));
 *    syos_fastCopy(src32, dst32, (4*123));
 * \endcode
 *
 * \param[in]	*source			Ptr on the source
 * \param[in]	*destination	Ptr on the destination
 * \param[in]	size			Number of bytes
 * \param[out]	KSYOSNOERR		OK
 *
 */
int32_t	syos_fastCopy(const void *source, void *destination, uint32_t size) {

	_init();
	return (stub_syos_fastCopy(source, destination, size));
}

/*
 * \brief Memory allocation
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t    *memory;
 *
 *    memory = (uint8_t *)syos_malloc(KINTERNAL, 1234);
 * \endcode
 *
 * \param[in]	location	Internal (KINTERNAL), external (KEXTERNAL), coupled (KCOUPLED) memory
 * \param[in]	size		Size of the memory to allocate
 * \param[out]	0			Memory block not allocated
 * \param[out]	address		Memory address of the allocated block
 *
 */
void	*syos_malloc(uint8_t location, uint32_t size) {
    mab_t       *curBlock, *newBlock, *nexBlock;
    uint32_t    nb, available, *nbBlocks, *usdMemory;
    uint32_t	lnHeap = 0;

	_init();

	kern_lockSemaphore(vSemaphore, -1);

// Consider always multiple of 4 bytes

	size = (size + 3) & 0xFFFFFFFC;

// From the start of the heap memory ...
// Verify if there are enough place for a new memory allocation

// Prepare some pointers to take in account the heap location (internal or external)

	switch (location) {
		default:
		case KINTERNAL: {
			curBlock  = (mab_t *)&_stHeapI;
			lnHeap    = (uint32_t)&_lnHeapI;
			usdMemory = &vSyos_usdMemoryI;
			nbBlocks  = &vNbBlocksI;
			break;
		}
		case KEXTERNAL: {
			curBlock  = (mab_t *)&_stHeapE;
			lnHeap    = (uint32_t)&_lnHeapE;
			usdMemory = &vSyos_usdMemoryE;
			nbBlocks  = &vNbBlocksE;
			break;
		}
		case KCCOUPLED: {
			curBlock  = (mab_t *)&_stHeapC;
			lnHeap    = (uint32_t)&_lnHeapC;
			usdMemory = &vSyos_usdMemoryC;
			nbBlocks  = &vNbBlocksC;
			break;
		}
	}

// Verify if the selected heap exist

	if (lnHeap == 0) {
		kern_unlockSemaphore(vSemaphore);
		return (NULL);
	}

// Looking for a memory hole that can accommodate
// the new allocation demand
    
    if (*nbBlocks > 0) {
        for (nb = 0; nb < *nbBlocks; nb++) {
            if ((sizeof(mab_t) + size) <= curBlock->oSzAvailable) {
                available = curBlock->oSzAvailable - sizeof(mab_t) - size;

                curBlock->oSzAvailable = 0;

                newBlock = (mab_t *)((uint32_t)curBlock + (uint32_t)sizeof(mab_t) + (uint32_t)curBlock->oSzAllocated);
                newBlock->oSzAllocated = size;
                newBlock->oSzAvailable = available;

// Cross link (current <--> new)
// Adjust the number of blocks and the available memory

                newBlock->oPtrPreBlock = curBlock;
                nexBlock = curBlock->oPtrNexBlock;
                curBlock->oPtrNexBlock = newBlock;

                if (nexBlock != NULL) { nexBlock->oPtrPreBlock = newBlock; newBlock->oPtrNexBlock = nexBlock; }
                else                  {                                    newBlock->oPtrNexBlock = NULL;     }

                *nbBlocks  += 1;
                *usdMemory += (size + sizeof(mab_t));
				kern_unlockSemaphore(vSemaphore);
                return ((void *)((uint32_t)newBlock + (uint32_t)sizeof(mab_t)));
            }
            curBlock = (mab_t *)((uint32_t)curBlock + (uint32_t)sizeof(mab_t) + (uint32_t)curBlock->oSzAllocated + (uint32_t)curBlock->oSzAvailable);
        }
		kern_unlockSemaphore(vSemaphore);
		return (NULL);
    }

// Special case, no blocks

    if ((sizeof(mab_t) + size) <= lnHeap) {
        curBlock->oSzAllocated = size;
        curBlock->oSzAvailable = lnHeap - sizeof(mab_t) - size;
 
// No cross link (first block)
// Adjust the number of blocks and the available memory

        curBlock->oPtrPreBlock = NULL;
     	curBlock->oPtrNexBlock = NULL;
  
       	*nbBlocks  += 1;
       	*usdMemory += (size + sizeof(mab_t));
		kern_unlockSemaphore(vSemaphore);
		return ((void *)((uint32_t)curBlock + (uint32_t)sizeof(mab_t)));
    }

	kern_unlockSemaphore(vSemaphore);
	return (NULL);
}

/*
 * \brief Free the memory allocation
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t    *memory;
 *
 *    syos_free(memory);
 * \endcode
 *
 * \param[in]	*address	Ptr on the address bloc to release
 * \param[out]	-
 *
 */
void	syos_free(void *address) {
    mab_t		*stHeap, *preBlock, *curBlock, *nexBlock;
    uint32_t	*usdMemory, *nbBlocks, released;
    uint32_t	beginI, beginE, beginC, endI, endE, endC;
	enum		{ INTERNAL, EXTERNAL, COUPLED, OUT } region = OUT;

	_init();

	kern_lockSemaphore(vSemaphore, -1);
    curBlock = (mab_t *)((uint32_t)address - (uint32_t)sizeof(mab_t));

// This because of a gcc bug
// !!! Never use this
//     ... if ((int32_t)&_stHeapI > 0)
//
//     but use this
//     ... int32_t var
//     ... var = (int32_t)&_stHeapI
//     ... if (var > 0)

	beginI = (uint32_t)&_stHeapI;
	beginE = (uint32_t)&_stHeapE;
	beginC = (uint32_t)&_stHeapC;

	endI   = (uint32_t)&_stHeapI + (uint32_t)&_lnHeapI;
	endE   = (uint32_t)&_stHeapE + (uint32_t)&_lnHeapE;
	endC   = (uint32_t)&_stHeapC + (uint32_t)&_lnHeapC;

	if (((uint32_t)curBlock >= beginI) && ((uint32_t)curBlock < endI)) { region = INTERNAL; }
	if (((uint32_t)curBlock >= beginE) && ((uint32_t)curBlock < endE)) { region = EXTERNAL; }
	if (((uint32_t)curBlock >= beginC) && ((uint32_t)curBlock < endC)) { region = COUPLED;  }

// Prepare some pointers to take in account the heap location (internal or external)

	switch (region) {
		case INTERNAL: {
        	stHeap    = (mab_t *)&_stHeapI;
        	usdMemory = &vSyos_usdMemoryI;
        	nbBlocks  = &vNbBlocksI;
        	break;
		}
		case EXTERNAL: {
        	stHeap    = (mab_t *)&_stHeapE;
        	usdMemory = &vSyos_usdMemoryE;
        	nbBlocks  = &vNbBlocksE;
        	break;
		}
		case COUPLED: {
        	stHeap    = (mab_t *)&_stHeapC;
        	usdMemory = &vSyos_usdMemoryC;
        	nbBlocks  = &vNbBlocksC;
        	break;
		}
		default: {
			kern_unlockSemaphore(vSemaphore);
			return;
		}
	}

// Release the memory block

	released = (curBlock->oSzAllocated + sizeof(mab_t));
	if (curBlock == stHeap) {
        curBlock->oSzAvailable = (curBlock->oSzAllocated + curBlock->oSzAvailable);
        curBlock->oSzAllocated = 0;
    }
    else {
		nexBlock = curBlock->oPtrNexBlock;
		preBlock = curBlock->oPtrPreBlock;
		preBlock->oSzAvailable += (curBlock->oSzAllocated + curBlock->oSzAvailable + sizeof(mab_t));
        preBlock->oPtrNexBlock = nexBlock;
		if (nexBlock != NULL) { nexBlock->oPtrPreBlock = preBlock; }
    }

// Adjust the number of blocks and the available memory

	*nbBlocks  -= 1;
	*usdMemory -= released;
	kern_unlockSemaphore(vSemaphore);
}

// Local routines
// ==============

/*
 * \brief _init
 *
 * - This function initializes the manager and
 *   has to be called at least once.
 *
 */
static	void	_init(void) {
	static	bool_t	init = FALSE;

	if (!init) {
		init = TRUE;

		if (kern_createMutxSemaphore(aStrIdSe, &vSemaphore) != KKERNNOERR) {
			exit(EXIT_PANIC);
		}

	}
}

#if (defined(__MEM_MMX1__))
/*
 * \brief _getIdModule
 *
 * - This function returns the pointer on the module specifications
 *
 */
static	int32_t	_getIdModule(uint32_t idModule, uint16_t *index, const module_t **module) {
	uint8_t		*destination;
	uint32_t	source, length, *value;
	header_t	*header;

	destination = (uint8_t  *)&_stUMemo;
	value		= (uint32_t *)&_stUMemo;
	header		= (header_t *)&_stUMemo;

	*index = 0;
	while (*index < KNBEEPROMAPPL) {
		source = KSZUKOSOS + *index*KSZEEPROMAPPL;

// Read the header

		glob_readMemory(source, destination, sizeof(header_t));
		if (*value == (('m'<<24) | ('e'<<16) | ('m'<<8) | '1')) {

// Module available

			length = header->oLnApplication;
			glob_readMemory(source, destination, length);

			if (header->oModule->oIdModule == idModule) {
				*module = header->oModule;
				return (KSYOSNOERR);
			}
		}
		*index += 1;
	}
	return (KSYOSNOMOD);
}

/*
 * \brief _getNameModule
 *
 * - This function returns the pointer on the module specifications
 *
 */
static	int32_t	_getNameModule(const char_t *name, uint16_t *index, const module_t **module) {
	uint8_t		*destination;
	uint32_t	source, length, *value;
	header_t	*header;
	bool_t		equals;

	destination = (uint8_t  *)&_stUMemo;
	value		= (uint32_t *)&_stUMemo;
	header		= (header_t *)&_stUMemo;

	*index = 0;
	while (*index < KNBEEPROMAPPL) {
		source = KSZUKOSOS + *index*KSZEEPROMAPPL;

// Read the header

		glob_readMemory(source, destination, sizeof(header_t));
		if (*value == (('m'<<24) | ('e'<<16) | ('m'<<8) | '1')) {

// Module available

			length = header->oLnApplication;
			glob_readMemory(source, destination, length);

			text_checkAsciiBuffer(header->oModule->oStrApplication, name, &equals);
			if (equals) {
				*module = header->oModule;
				return (KSYOSNOERR);
			}
		}
		*index += 1;
	}
	return (KSYOSNOMOD);
}

/*
 * \brief _getFaModule
 *
 * - This function returns the pointer on the module specifications
 *
 */
static	int32_t	_getFaModule(uint8_t family, uint32_t *idModule, uint16_t *index, const module_t **module) {
	uint8_t		*destination;
	uint32_t	source, length, *value;
	header_t	*header;

	destination	= (uint8_t  *)&_stUMemo;
	value		= (uint32_t *)&_stUMemo;
	header		= (header_t *)&_stUMemo;

	while (*index < KNBEEPROMAPPL) {
		source = KSZUKOSOS + *index*KSZEEPROMAPPL;

// Read the header

		glob_readMemory(source, destination, sizeof(header_t));
		if (*value == (('m'<<24) | ('e'<<16) | ('m'<<8) | '1')) {

// Module available

			length = header->oLnApplication;
			glob_readMemory(source, destination, length);

			if ((header->oModule->oIdModule>>24) == family) {
				*idModule = header->oModule->oIdModule;
				*module = header->oModule;
				return (KSYOSNOERR);
			}
		}
		*index += 1;
	}
	return (KSYOSNOFAM);
}
#endif

