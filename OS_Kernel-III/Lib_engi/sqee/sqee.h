/*
; sqee.
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
; Goal:		sqee manager.
;			uKOS interface for Sysquake Embedded (Calerga Sarl, www.calerga.com).
;
;   (c) 1992-2017, Franzi Edo.
;   --------------------------
;
;   Developed for use with Sysquake Embedded.
;   Sysquake is a trademark of Calerga Sarl, used with permission. 
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

#ifndef	__SQEE__
#define	__SQEE__

/*!
 * \addtogroup Lib_engi
 */
/*@{*/

#include 	<modules.h>

#define	KSQEEMAN	(KSQEENUM<<8)
#define	KSQEEERR	((KIDENGI<<24) | KSQEEMAN)

// sqee access macros
// ------------------

#define	SQEE_COMPUTE(ref, program) \
						ref->oAction 	    = KCOMPUTE; \
						ref->oSqeeCommand	= (char_t *)program; \
						sqee_exchangeData((ioSqee_t *)ref);

#define	SQEE_PUT(ref, nbDimensions, dim_2, dim_1, dim_0, array, program) \
						ref->oAction 		= KPUT; \
						ref->oNbDimensions	= nbDimensions; \
						ref->oDimensions[0]	= dim_0; \
						ref->oDimensions[1]	= dim_1; \
						ref->oDimensions[2]	= dim_2; \
						ref->oPutData  		= (float64_t *)array; \
						ref->oSqeeCommand	= (char_t *)program; \
						sqee_exchangeData((ioSqee_t *)ref);

#define	SQEE_GET(ref, nbDimensions, dim_2, dim_1, dim_0, array, program) \
						ref->oAction 		= KGET; \
						ref->oNbDimensions	= nbDimensions; \
						ref->oDimensions[0]	= dim_0; \
						ref->oDimensions[1]	= dim_1; \
						ref->oDimensions[2]	= dim_2; \
						ref->oGetData  		= (float64_t *)array; \
						ref->oSqeeCommand	= (char_t *)program; \
						sqee_exchangeData((ioSqee_t *)ref);

#define	SQEE_CMDLINE(ref) \
						ref->oAction 	    = KCMDLINE; \
						sqee_exchangeData((ioSqee_t *)ref);

// Configuration structure
// -----------------------

typedef	struct	cnfSqee	cnfSqee_t;
typedef	struct	ioSqee	ioSqee_t;

struct cnfSqee {
			void 		**oSqeeRef;										// Ptr on the Sqee (handle)
			uint32_t	oSize;											// Size of the memory
			uint8_t		*oMemory;										// Ptr on the memory location
};

// I/O structure

struct ioSqee {
			void 		*oSqeeRef;										// Ptr on the Sqee
			uint8_t		oAction;										// Action
			#define		KPUT		0									// Put to sqee
			#define		KCOMPUTE	1									// Compute
			#define		KGET		2									// Get from the sqee
			#define		KCMDLINE	3									// Command line

			uint32_t	oNbDimensions;									// Number of dimensions (0=scalar, -1=null, 1..3=array)
			#define		KSCALAR		0									// Scalar
			#define		K1DARRAY	1									// 1D array
			#define		K2DARRAY	2									// 2D array
			#define		K3DARRAY	3									// 3D array

			uint32_t	oDimensions[3];									// Dimensions (can be NULL if oNbDimensions <= 0)
			float64_t	*oPutData;										// Ptr on the data to put before next sqee call
			float64_t	*oGetData;										// Ptr on the data to get before next sqee call
			char_t		*oSqeeCommand;									// Ptr on the sqee command
};

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

/*!
 * \brief Configure the sqee manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * void         *sqeeHandle;
 * cnfSqee_t    configure;
 * 
 *    configure.oSize = 90000;
 *    configure.oMemory = (uint8_t *)syos_malloc(KINTERNAL, configure.oSize);
 *    if (configure.oMemory == 0) {
 *        exit(EXIT_FAILURE);
 *    }
 *
 *    status = sqee_configure(&configure);
 *    sqeeHandle = configure.oSqeeRef;
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KSQEENOERR	OK
 *
 */
extern	int32_t	sqee_configure(cnfSqee_t *configure);

/*!
 * \brief Exchange data with the sqee manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * float64_t    result[KNBZS][KNROWS][KNCOLS];
 * cnfSqee_t    configure;
 * ioSqee_t     data;
 *
 *    configure.oSize = 90000;
 *    configure.oMemory = (uint8_t *)syos_malloc(KINTERNAL, configure.oSize);
 *    if (configure.oMemory == 0) {
 *        exit(EXIT_FAILURE);
 *    }
 *
 *    sqee_configure((cnfSqee_t *)&configure);
 *
 *    data.oAction = KCOMPUTE;
 *    data.oSqeeCommand = "result = rand(4,5,3)";
 *    sqee_exchangeData((ioSqee_t *)&data);
 * \endcode
 *
 * \param[in]	*sqeeData	Ptr on the sqee data
 * \param[out]	KSQEENOERR	OK
 *
 */
extern	int32_t	sqee_exchangeData(ioSqee_t *sqeData);

#ifdef	__cplusplus
}
#endif

// sqee manager errors
// -------------------

#define	KSQEENOERR	0				// No error
#define	KSQEESYCNA	-(KSQEEERR+1)	// System call not available
#define	KSQEEGEERR	-(KSQEEERR+2)	// General error

/*@}*/

#endif
