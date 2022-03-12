/*
; ole0.
; =====

;------------------------------------------------------------------------
; Author:	Nussbaum Pascal	The 2016-10-08
; Modifs:	Franzi Edo.		The 2006-11-16
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		ole0 manager.
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

#ifndef	__OLE0__
#define	__OLE0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KOLE0MAN	(KOLE0NUM<<8)
#define	KOLE0ERR	((KIDPERI<<24) | KOLE0MAN)

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	OLE0_reserve	ole0_reserve
#define	OLE0_release	ole0_release

/*!
 * \brief Reserve the ole0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (ole0_reserve(KDEVALL) == KOLE0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    ole0_xyz();
 *    status = ole0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KOLE0NOERR	The manager is reserved
 * \param[out]	KOLE0CHBSY 	The manager is busy
 *
 */
extern	int32_t	ole0_reserve(uint8_t mode);

/*!
 * \brief Release the ole0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = ole0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_release(uint8_t mode);

/*!
 * \brief Set the color
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    r = 23, g = 45, b = 77;
 *
 *    status = ole0_setColor(r, g, b);
 * \endcode
 *
 * \param[in]	r			Red component
 * \param[in]	g			Green component
 * \param[in]	b			Blue component
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_setColor(uint8_t r, uint8_t g, uint8_t b);

/*!
 * \brief Draw a text
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     aText[] = "The lazy fox ...";
 *
 *    status = ole0_drawText(&aText[0]);
 * \endcode
 *
 * \param[in]	*text		Ptr on the text
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_drawText(char_t *text);

/*!
 * \brief Draw an image
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint8_t     image[64][64] = { {1, 2, 3, ..}, {11, 22, 33, ..} };
 * uint16_t    w = 64, h = 64;
 *
 *    status = ole0_drawImage(w, h,&image[0][0]);
 * \endcode
 *
 * \param[in]	w			Width of the image
 * \param[in]	h			Heigh of the image
 * \param[in]	*image		Ptr on the image
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_drawImage(uint16_t w, uint16_t h, uint8_t *image);

/*!
 * \brief Draw a pixel
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    x = 13, y = 15;
 *
 *    status = ole0_drawPixel(x, y);
 * \endcode
 *
 * \param[in]	x			X coordinate of the pixel
 * \param[in]	y			Y coordinate of the pixel
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_drawPixel(uint16_t x, uint16_t y);

/*!
 * \brief Draw a line
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    x0 = 13, y0 = 15, x1 = 33, y1 = 44;
 *
 *    status = ole0_drawLine(x0, y0, x1, y1);
 * \endcode
 *
 * \param[in]	x0			X coordinate of the start pixel
 * \param[in]	y0			Y coordinate of the start pixel
 * \param[in]	x1			X coordinate of the end pixel
 * \param[in]	y1			Y coordinate of the end pixel
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/*!
 * \brief Fill a rectangle
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    x0 = 13, y0 = 15, x1 = 33, y1 = 44;
 *
 *    status = ole0_fillRect(x0, y0, x1, y1);
 * \endcode
 *
 * \param[in]	x0			X coordinate of the corner 0
 * \param[in]	y0			Y coordinate of the corner 0
 * \param[in]	x1			X coordinate of the corner 1
 * \param[in]	y1			Y coordinate of the corner 1
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_fillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/*!
 * \brief Copy a rectangle
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    x0 = 13, y0 = 15, x1 = 33, y1 = 44, x2 = 56, y2 = 23;
 *
 *    status = ole0_copyRect(x0, y0, x1, y1, x2, y2);
 * \endcode
 *
 * \param[in]	x0			X coordinate of the corner 0
 * \param[in]	y0			Y coordinate of the corner 0
 * \param[in]	x1			X coordinate of the corner 1
 * \param[in]	y1			Y coordinate of the corner 1
 * \param[in]	x2			X coordinate of the corner 2
 * \param[in]	y2			Y coordinate of the corner 2
 * \param[out]	KOLE0NOERR	OK
 *
 */
extern	int32_t	ole0_copyRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#ifdef	__cplusplus
}
#endif

// ole0 manager errors
// -------------------

#define	KOLE0NOERR	0				// No error
#define	KOLE0SYCNA	-(KOLE0ERR+1)	// System call not available
#define	KOLE0GEERR	-(KOLE0ERR+2)	// General error
#define	KOLE0CHBSY	-(KOLE0ERR+3)	// The manager is busy
#define	KOLE0BADMO	-(KOLE0ERR+4)	// Bad mode for called function

/*@}*/

#endif
