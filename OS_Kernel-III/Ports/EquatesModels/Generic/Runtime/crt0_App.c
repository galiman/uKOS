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
;                       EPROM
; _stTEXT        +-----------------+
;                |                 |
;                | .text           |
;                | .eh_frame       |
;                | .rodata         |
;				 |                 |                   DATA
; _enRODATA      +-----------------+ .......... +-----------------+ _stGOT
;                |                 |            |                 |
;                | .got            |            | .got            |
;                | .data           |            | .data           |
;                |                 |            |                 |
;                +-----------------+ .......... +-----------------+ _enDATA
;                                               |                 | _stBSS
;                                               | .bss            |
;                                               | COMMON          |
;                                               |                 |
;                                               +-----------------+ _enBSS
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

#if (defined(__CPLUSPLUS__))
typedef	void		(*fnct_t)(void);
typedef	fnct_t 		*fnctCpp_t;

extern	fnct_t		_stCTOR, _enCTOR;
extern	fnct_t		_stDTOR, _enDTOR;
#endif

extern	uint32_t	_stDATA, _enDATA, _enRODATA;
extern	uint32_t	_stBSS, _enBSS;

/*
 * \brief _start
 *
 * - Copy the initialized data from the CODE to the DATA region
 * - Initialize the BSS region
 * - Call the main
 *
 */
int32_t	_start(void) {
	uint8_t		*regionDATA, *regionCODE, *regionBSS;
	int32_t		nbBytes, status;
	uint32_t	ckSumm;
	
	#if (defined(__CPLUSPLUS__))
	fnctCpp_t	ctor, dtor;
	#endif

// Copy the initialized data from the CODE region to the DATA one

	regionDATA = (uint8_t *)&_stDATA; regionCODE = (uint8_t *)&_enRODATA;
	nbBytes = (int32_t)((uint32_t)&_enDATA - (uint32_t)&_stDATA);
	while (nbBytes-- > 0) { *regionDATA++ = *regionCODE++; }

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

// Call the constructors

	#if (defined(__CPLUSPLUS__))
	ctor = &_stCTOR;
	while (ctor < &_enCTOR) { (*ctor)(); ctor++; }
	#endif

// Verify if the user memory is available

	RESERVE(SYOS, KDEVALL);
	status = main();
	
// Call the destructors

	#if (defined(__CPLUSPLUS__))
	dtor = &_stDTOR;
	while (dtor < &_enDTOR) { (*dtor)(); dtor++; }
	#endif

	return (status);
}

