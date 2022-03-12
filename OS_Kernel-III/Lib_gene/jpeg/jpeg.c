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

#include 	<uKOS.h>

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"jpeg         jpeg manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "iotx manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_jpeg_init
extern	int32_t	stub_jpeg_encode(const uint8_t *image, uint32_t nbRows, uint32_t nbCols, uint8_t *imageEncoded);
extern	int32_t	stub_jpeg_decode(const uint8_t *imageEncoded, uint32_t nbRows, uint32_t nbCols, uint8_t *image);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Ipeg, KIDGENE, KJPEGNUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief Encode a raw image to a jpeg one
 *
 * \param[in]	*image			Ptr on the raw image
 * \param[in]	nbRows			Nb of rows
 * \param[in]	nbCols			Nb of cols
 * \param[in]	*imageEncoded	Ptr on the jpeg encoded image
 * \param[out]	KJPEGNOERR		OK
 *
 */
int32_t	jpeg_encode(const uint8_t *image, uint32_t nbRows, uint32_t nbCols, uint8_t *imageEncoded) {

	_init();
	return (stup_jpeg_encode(image, nbRows, nbCols, imageEncoded));
}

/*
 * \brief Decode a jpeg image to a raw one
 *
 * \param[in]	*imageEncoded	Ptr on the jpeg encoded image
 * \param[in]	nbRows			Nb of rows
 * \param[in]	nbCols			Nb of cols
 * \param[in]	*image			Ptr on the raw image
 * \param[out]	KJPEGNOERR		OK
 *
 */
int32_t	jpeg_decode(const uint8_t *imageEncoded, uint32_t nbRows, uint32_t nbCols, uint8_t *image) {

	_init();
	return (stup_jpeg_decode(imageEncoded, nbRows, nbCols, image));
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

		stub_jpeg_init();
	}
}

