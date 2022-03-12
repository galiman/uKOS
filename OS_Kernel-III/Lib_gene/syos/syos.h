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

#ifndef	__SYOS__
#define	__SYOS__

/*!
 * \addtogroup Lib_gene
 */
/*@{*/

#include 	<modules.h>

#define	KSYOSMAN	(KSYOSNUM<<8)
#define	KSYOSERR	((KIDGENE<<24) | KSYOSMAN)

#define	KMEM0		(('m'<<24) | ('m'<<16) | ('s'<<8) | '0')
#define	KMEM1		(('m'<<24) | ('m'<<16) | ('s'<<8) | '1')

// syos_malloc memory locations
// ----------------------------

#define	KINTERNAL	0													// The internal heap block
#define	KEXTERNAL	1													// The external heap block
#define	KCCOUPLED	2													// The core coupled heap block

// Trace fifo
// ----------

#define	KSZFIFO		10													// Size of the fifo

typedef	struct	trace	trace_t;

struct trace {
	const	char_t		*oIdentifier;									// Element identifier
    		uint32_t	oParameter;										// Element parameter
};

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	SYOS_reserve	syos_reserve
#define	SYOS_release	syos_release

/*!
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
extern	int32_t	syos_reserve(uint8_t mode);

/*!
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
extern	int32_t	syos_release(uint8_t mode);

/*!
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
extern	int32_t	syos_getCKSumOS(uint32_t *checksum);

/*!
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
extern	int32_t	syos_getIdModule(uint32_t mms, uint32_t idModule, uint16_t *index, const module_t **module);

/*!
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
extern	int32_t	syos_getNameModule(uint32_t mms, const char_t *name, uint16_t *index, const module_t **module);

/*!
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
extern	int32_t	syos_getFamilyModule(uint32_t mms, uint8_t family, uint32_t *idModule, uint16_t *index, const module_t **module);

/*!
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
extern	int32_t	syos_eraseBlock(uint32_t mms, const uint16_t *index);

/*!
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
extern	int32_t	syos_writeBlock(uint32_t mms, const uint16_t *index);

/*!
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
extern	int32_t	syos_setDoLoCode(void *address);

/*!
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
extern	int32_t	syos_getDoLoCode(void **address);

/*!
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
extern	int32_t	syos_trace(const char_t *message, uint32_t parameter);

/*!
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
extern	int32_t	syos_fastCopy(const void *source, void *destination, uint32_t size);

/*!
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
extern	void	*syos_malloc(uint8_t location, uint32_t size);

/*!
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
extern	void	syos_free(void *address);

#ifdef	__cplusplus
}
#endif

// syos manager errors
// -------------------

#define	KSYOSNOERR	0				// No error
#define	KSYOSSYCNA	-(KSYOSERR+1)	// System call not available
#define	KSYOSGEERR	-(KSYOSERR+2)	// General error
#define	KSYOSNOMOD	-(KSYOSERR+3)	// The module does not exist
#define	KSYOSNOFAM	-(KSYOSERR+4)	// The family does not exist
#define	KSYOSNOMEM	-(KSYOSERR+5)	// The memory unit does not exist
#define	KSYOSNOBLK	-(KSYOSERR+6)	// The memory block does not exist
#define	KSYOSCHBSY	-(KSYOSERR+7)	// The manager is busy

/*@}*/

#endif
