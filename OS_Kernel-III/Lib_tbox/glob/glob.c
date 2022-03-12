/*
; glob.
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

#include 	<uKOS.h>
#include	<version.h>

static	volatile	sema_t		*vSemaphore;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"glob         glob manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "glob manager\n"
									"============\n\n"

									"This manager ...\n\n";

LOC_CONST_STRG(aStrApp[])         = uKOS_KBOARD", V."uKOS_VERSION", "uKOS_COMPILER", "VERSIONING_TOOL" "SW_VERSION"\n(c) Franzi Edo., "__DATE__" - "__TIME__"\n";
LOC_CONST_STRG(aStrRev[])         = uKOS_VERSION;

LOC_CONST_STRG(aStrIdSe[])		  = "Critical_EEPROM";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_glob_init(void);
extern	int32_t	stub_glob_getFamily(uint32_t *family);
extern	int32_t	stub_glob_getRND(uint32_t *number);
extern	int32_t	stub_glob_restart(void);
extern	int32_t	stub_glob_blankMemory(const uint32_t address, uint32_t size);
extern	int32_t	stub_glob_eraseMemory(const uint32_t address, uint32_t nbSectors);
extern	int32_t	stub_glob_writeMemory(const uint32_t address, const uint8_t *buffer, uint32_t size);
extern	int32_t	stub_glob_readMemory(const uint32_t address, uint8_t *buffer, uint32_t size);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Glob, KIDTBOX, KGLOBNUM, NULL, "2.1", (1<<BSHOW));

/*
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
int32_t	glob_getIdentifier(const char_t **identifier) {

	_init();
	*identifier = aStrApp;
	return (KGLOBNOERR);
}

/*
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
int32_t	glob_getRevision(const char_t **revision) {

	_init();
	*revision = aStrRev;
	return (KGLOBNOERR);
}

/*
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
int32_t	glob_getFamily(uint32_t *family) {

	_init();
	return (stub_glob_getFamily(family));
}

/*
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
int32_t	glob_getRND(uint32_t *number) {

	_init();
	return (stub_glob_getRND(number));
}

/*
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
int32_t	glob_restart(void) {

	_init();
	return (stub_glob_restart());
}

/*
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
int32_t	glob_blankMemory(uint32_t address, uint32_t size) {
	int32_t		status;

	_init();
	kern_lockSemaphore(vSemaphore, -1);
	status = stub_glob_blankMemory(address, size);
	kern_unlockSemaphore(vSemaphore);
	return (status);
}

/*
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
int32_t	glob_eraseMemory(uint32_t address, uint32_t nbSectors) {
	int32_t		status;

	_init();
	kern_lockSemaphore(vSemaphore, -1);
	status = stub_glob_eraseMemory(address, nbSectors);
	kern_unlockSemaphore(vSemaphore);
	return (status);
}

/*
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
int32_t	glob_writeMemory(uint32_t address, const uint8_t *buffer, uint32_t size) {
	int32_t		status;

	_init();
	kern_lockSemaphore(vSemaphore, -1);
	status = stub_glob_writeMemory(address, buffer, size);
	kern_unlockSemaphore(vSemaphore);
	return (status);
}

/*
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
int32_t	glob_readMemory(uint32_t address, uint8_t *buffer, uint32_t size) {
	int32_t		status;

	_init();
	kern_lockSemaphore(vSemaphore, -1);
	status = stub_glob_readMemory(address, buffer, size);
	kern_unlockSemaphore(vSemaphore);
	return (status);
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

		stub_glob_init();
	}
}

