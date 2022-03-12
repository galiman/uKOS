/*
; jpeg.
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
; Goal:		jpeg manager.
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

#ifndef	__JPEG__
#define	__JPEG__

/*!
 * \addtogroup Lib_gene
 */
/*@{*/

#include 	<modules.h>

#define	KJPEGMAN	(KJPEGNUM<<8)
#define	KJPEGERR	((KIDGENE<<24) | KJPEGMAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

/*!
 * \brief Encode a raw image to a jpeg one
 *
 * \param[in]	*image			Ptr on the raw image
 * \param[in]	nbRows			Nb of rows
 * \param[in]	nbCols			Nb of cols
 * \param[in]	*imageEncoded	Ptr on the jpeg encoded image
 * \param[out]	KJPEGNOERR		OK
 *
 */
extern	int32_t	jpeg_encode(const uint8_t *image, uint32_t nbRows, uint32_t nbCols, uint8_t *imageEncoded);

/*!
 * \brief Decode a jpeg image to a raw one
 *
 * \param[in]	*imageEncoded	Ptr on the jpeg encoded image
 * \param[in]	nbRows			Nb of rows
 * \param[in]	nbCols			Nb of cols
 * \param[in]	*image			Ptr on the raw image
 * \param[out]	KJPEGNOERR		OK
 *
 */
extern	int32_t	jpeg_decode(const uint8_t *imageEncoded, uint32_t nbRows, uint32_t nbCols, uint8_t *image);

#ifdef	__cplusplus
}
#endif

// jpeg manager errors
// -------------------

#define	KJPEGNOERR	0				// No error
#define	KJPEGSYCNA	-(KJPEGERR+1)	// System call not available
#define	KJPEGGEERR	-(KJPEGERR+2)	// General error

/*@}*/

#endif
