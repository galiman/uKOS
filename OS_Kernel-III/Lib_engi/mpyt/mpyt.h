/*
; mpyt.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2015-02-15
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		mpyt manager.
;			uKOS interface for Micro-Python (www.micropython.com).
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

#ifndef	__MPYT__
#define	__MPYT__

/*!
 * \addtogroup Lib_engi
 */
/*@{*/

#include 	<modules.h>

#define	KMPYTMAN	(KMPYTNUM<<8)
#define	KMPYTERR	((KIDENGI<<24) | KMPYTMAN)

// mpyt access macros
// ------------------

#define	MPYT_CMDLINE	mpyt_exchangeData();

// Configuration structure
// -----------------------

typedef	struct	cnfMpyt	cnfMpyt_t;

struct cnfMpyt {
			uint32_t	oSize;											// Size of the memory
			uint8_t		*oMemory;										// Ptr on the memory location
};

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

/*!
 * \brief Configure the mpyt manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * cnfMpyt_t    configure;
 * 
 *    configure.oSize = 90000;
 *    configure.oMemory = (uint8_t *)syos_malloc(KINTERNAL, configure.oSize);
 *    if (configure.oMemory == 0) {
 *        exit(EXIT_FAILURE);
 *    }
 *
 *    status = mpyt_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KMPYTNOERR	OK
 *
 */
extern	int32_t	mpyt_configure(cnfMpyt_t *configure);

/*!
 * \brief Exchange data with the mpyt manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = mpyt_exchangeData();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KMPYTNOERR	OK
 *
 */
extern	int32_t	mpyt_exchangeData(void);

#ifdef	__cplusplus
}
#endif

// sqee manager errors
// -------------------

#define	KMPYTNOERR	0				// No error
#define	KMPYTSYCNA	-(KMPYTERR+1)	// System call not available
#define	KMPYTGEERR	-(KMPYTERR+2)	// General error

/*@}*/

#endif
