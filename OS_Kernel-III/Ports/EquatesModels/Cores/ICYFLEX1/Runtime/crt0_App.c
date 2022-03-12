/*
; crt0_App.
; =========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2007-05-27
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		crt0 for the uKOS applications.
;			See "ld.pdf" file, "Using LD, the GNU linker" page 48.
;
;                       CODE
; _stRODATA      +-----------------+
;                |                 |
;                | .rodata         |
; _enRODATA      |                 |
; _stTEXT        +-----------------+ 
;                |                 |
;                | .text           |
; _enTEXT 	     |                 |
; _stDATA        +-----------------+ 
;                |                 |
;                | .data           |
; _enDATA	     |                 | 
; _stBSS         +-----------------+ 
;                |                 |
;                | .bss            |
; _enBSS	     |                 |
; _end           +-----------------+
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

#include	<uKOS.h>
#include	<EPROM.ck>

extern	uint32_t	_stBSS, _enBSS;

/*
 * \brief start
 *
 * - The copy the initialized data from the CODE to the DATA region
 *   is not necessary (full RAM system)
 * - Initialize the BSS region
 * - Call the main
 *
 */
void	__main(void) {
}

int32_t	start(void) {
	uint8_t		*regionBSS;
	int32_t		nbBytes;
	uint32_t	ckSumm;

// Initialize the BSS region

	regionBSS = (uint8_t *)&_stBSS;
	nbBytes = (int32_t)((uint32_t)&_enBSS - (uint32_t)&_stBSS);
	while (nbBytes-- > 0) { *regionBSS++ = 0; }

// Verify if the application is compatible with the burned OS

	#if (defined(__CHECK_OS_VERSION__))
	syos_getCKSumOS(&ckSumm);
	if (ckSumm != KOSCKSUMM) {
		return (EXIT_FAILURE_CRT0);
	}
	#endif

// Verify if the user memory is available

	RESERVE(SYOS, KDEVALL);
	return (main());
}

