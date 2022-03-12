/*
; stub_glob.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the "glob" manager module.
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

#define	KFAMILY		Discovery_476

/*
 * \brief stub_glob_init
 *
 * - Initialize some specific CPU parts
 *
 */
void	stub_glob_init(void) {

	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;

// Turn on the RNG
// Read the first random number: it has to be considered as "wrong" 

	RNG->CR = RNG_CR_RNGEN;
	kern_waitAtLeast(10);
	RNG->DR;
}

/*
 * \brief stub_glob_getFamily
 *
 * - Return the family Id
 *
 */
int32_t	stub_glob_getFamily(uint32_t *family) {

	*family = KFAMILY;
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_getRND
 *
 * - Return the random number
 *
 */
int32_t	stub_glob_getRND(uint32_t *number) {

	while (!(RNG->SR & (RNG_SR_DRDY))) {
		kern_suspendProcess(1);
	}

	*number = RNG->DR;
	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_restart
 *
 * - Disable all the system interruption
 * - Restart
 *
 */
int32_t	stub_glob_restart(void) {

// Stop all the interruptions and restart

	INTERRUPTION_OFF;
	*((volatile uint32_t *)0xE000ED0C) = 0x05FA0004;
	while (TRUE);

	return (KGLOBNOERR);
}

/*
 * \brief stub_glob_blankMemory
 *
 * - Blank checking
 *
 */
int32_t	stub_glob_blankMemory(uint32_t address, uint32_t size) {

	return (KGLOBSYCNA);
}

/*
 * \brief stub_glob_eraseMemory
 *
 * - Erase sectors
 *
 */
int32_t	stub_glob_eraseMemory(uint32_t address, uint32_t nbSectors) {

	return (KGLOBSYCNA);
}

/*
 * \brief stub_glob_writeMemory
 *
 * - Write pages
 * - This function writes pages. If a page is not complete
 *   this system call complete it with an unpredictable content
 *
 */
int32_t	stub_glob_writeMemory(uint32_t address, const uint8_t *buffer, uint32_t size) {

	return (KGLOBSYCNA);
}

/*
 * \brief stub_glob_readMemory
 *
 * - Read bytes
 *
 */
int32_t	stub_glob_readMemory(uint32_t address, uint8_t *buffer, uint32_t size) {

	return (KGLOBSYCNA);
}

