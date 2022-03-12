/*
; glob.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 1999-01-01
; Modifs:	Franzi Edo.	The 2000-01-10
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		glob manager.
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

#ifndef	__GLOB__
#define	__GLOB__

/*!
 * \addtogroup Lib_tbox
 */
/*@{*/

#include 	<modules.h>

#define	KGLOBMAN	(KGLOBNUM<<8)
#define	KGLOBERR	((KIDTBOX<<24) | KGLOBMAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

/*!
 * \brief Get the system ASCII identifier Ptr
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier
 *
 *    status = glob_getIdentifier(&identifier);
 * \endcode
 *
 * - This function returns a char Ptr on the application string
 *
 * \param[in]	**identifier	Ptr on the identifier string
 * \param[out]	KGLOBNOERR		OK
 *
 */
extern	int32_t	glob_getIdentifier(const char_t **identifier);

/*!
 * \brief Get the TBox revision
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *revision
 *
 *    status = glob_getRevision(&revision);
 * \endcode
 *
 * \param[in]	**revision	Ptr on the TBox revision string
 * \param[out]	KGLOBNOERR	OK
 *
 */
extern	int32_t	glob_getRevision(const char_t **revision);

/*!
 * \brief Get the Id of the system family
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    family
 *
 *    status = glob_getFamily(&family);
 * \endcode
 *
 * \param[in]	*family		Ptr on the family Id
 * \param[out]	KGLOBNOERR	OK
 *
 */
extern	int32_t	glob_getFamily(uint32_t *family);

/*!
 * \brief Get a random number
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    number
 *
 *    status = glob_getRND(&number);
 * \endcode
 *
 * \param[in]	*number		Ptr on the number
 * \param[out]	KGLOBNOERR	OK
 * \param[out]	KGLOBSYCNA	System call not available
 *
 */
extern	int32_t	glob_getRND(uint32_t *number);

/*!
 * \brief Restart of the system
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = stub_glob_restart();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KGLOBNOERR	OK
 *
 */
extern	int32_t	glob_restart(void);

/*!
 * \brief Blank check of an arbitrary memory block size
 *
 * Call example in C:
 *
 * \code{.c}
 * uint32_t    size = 0x2000;
 * int32_t     status;
 * uint32_t    memory;
 *
 *    status = glob_blankMemory(memory, size);
 * \endcode
 *
 * \param[in]	address		Memory address
 * \param[in]	size		Size in bytes of the memory block
 * \param[out]	KGLOBNOERR	Memory block empty
 * \param[out]	KGLOBFLNOE	Memory block not empty
 * \param[out]	KGLOBSYCNA	System call not available
 *
 */
extern	int32_t	glob_blankMemory(uint32_t address, uint32_t size);

/*!
 * \brief Erase memory sectors
 *
 * Call example in C:
 *
 * \code{.c}
 * uint32_t    nbSectors = 3;
 * int32_t     status;
 * uint32_t    memory;
 *
 *    memory = 0x10000;
 *    status = glob_eraseMemory(memory, nbSectors);
 * \endcode
 *
 * \param[in]	address		Memory address
 * \param[in]	nbSectors	Number of sector to erase
 * \param[out]	KGLOBNOERR	Memory sector erased
 * \param[out]	KGLOBFLERA	Memory sector not erased
 * \param[out]	KGLOBSYCNA	System call not available
 *
 */
extern	int32_t	glob_eraseMemory(uint32_t address, uint32_t nbSectors);

/*!
 * \brief Write an arbitrary memory block size
 *
 * Call example in C:
 *
 * \code{.c}
 * const       uint8_t    source[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t     status;
 * uint32_t    memory;
 *
 *    memory = 0x10000;
 *    status = glob_writeMemory(memory, source, KSIZE);
 * \endcode
 *
 * - This function writes pages. If a page is not complete
 *   this system call complete it with an unpredictable content
 * - This function should be called only after having erased the
 *   byte. !!! glob_eraseMemory erase a complete sector
 *
 * \param[in]	address		Memory address
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size in byte of the memory block
 * \param[out]	KGLOBNOERR	Memory block written
 * \param[out]	KGLOBFLWRT	Memory block not written
 * \param[out]	KGLOBSYCNA	System call not available
 *
 */
extern	int32_t	glob_writeMemory(uint32_t address, const uint8_t *buffer, uint32_t size);

/*!
 * \brief Read an arbitrary memory block size
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     destination[KSIZE];
 * int32_t     status;
 * uint32_t    memory;
 *
 *    memory = 0x10000;
 *    status = glob_readMemory(memory, destination, KSIZE);
 * \endcode
 *
 * \param[in]	address		Memory address
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size in byte of the memory block
 * \param[out]	KGLOBNOERR	Memory block read
 * \param[out]	KGLOBSYCNA	System call not available
 *
 */
extern	int32_t	glob_readMemory(uint32_t address, uint8_t *buffer, uint32_t size);

#ifdef	__cplusplus
}
#endif

// glob manager errors
// -------------------

#define	KGLOBNOERR	0				// No error
#define	KGLOBSYCNA	-(KGLOBERR+1)	// System call not available
#define	KGLOBGEERR	-(KGLOBERR+2)	// General error
#define	KGLOBFLNOE	-(KGLOBERR+3)	// Memory block not empty
#define	KGLOBFLERA	-(KGLOBERR+4)	// Memory block not erased
#define	KGLOBCHBSY	-(KGLOBERR+5)	// The manager is busy
#define	KGLOBFLWRT	-(KGLOBERR+6)	// Memory block not written

/*@}*/

#endif
