/*
; vis0.
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
; Goal:		vis0 manager.
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

#ifndef	__VIS0__
#define	__VIS0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KVIS0MAN	(KVIS0NUM<<8)
#define	KVIS0ERR	((KIDPERI<<24) | KVIS0MAN)

// Configuration structure
// -----------------------

typedef	struct	cnfVis0	cnfVis0_t;

struct cnfVis0 {
			uint8_t		oImager;										// Imager
			#define		KLOG		0									// KLOG    = integrate in the logarithmic domain
			#define		KLIN		1									// KLIN    = integrate in the linear domain (encoders)
			#define		KLINLOG		2									// KLINLOG = integrate in the log domain (integration phase, bright pixels) and linear domain (ramp phase, dark pixels)

			uint8_t		oPixMode;										// Pixel mode
			#define		KLUM10		0									// KLUM10    = integrate a 10-bit luminance image
			#define		KLUM8MSB	1									// KLUM8MSB  = integrate an 8-bit (MSB) luminance image
			#define		KLUM8LSB	2									// KLUM8LSB  = integrate an 8-bit (LSB) luminance image
			#define		KCONTRAST	3									// KCONTRAST = integrate a contrast image
			#define		KORICON		4									// KORICON   = integrate an oricon image
			#define		KEXPERT		5									// KEXPERT   = user defined

			uint8_t		oAcqPage;										// Acquisition storage page or device
			#define		KPPI		0									// Acquisition result on the PPI
			#define		KNOXFER		1									// Acquisition result stays in the imager
			#define		KPAGE0		2									// Acquisition result on the page 0
			#define		KPAGE1		3									// Acquisition result on the page 1

			uint16_t	oExposureTime;									// Exposure time	Default: 700
			uint8_t		oVrefRange;										// Dac8 VRef range	Default: 200
			uint8_t		oLogShift;										// Log shift		Default: 2
			uint8_t		oDac0;											// Dac 0			Default: 10
			uint8_t		oDac4;											// Dac 4			Default: 50
			uint8_t		oDac9;											// Dac 9			Default: 100
			uint8_t		oVrefSteps;										// VRef steps		Default: 5

			uint8_t		oKernSync;										// uKernel synchro
			#define		BIMSEMA		0									// IM semaphore
};

#define	KNBROWSQVGA		240												// Number of rows
#define	KNBCOLSQVGA		320												// Number of columns

// Semaphores
// ----------

#define	KSEMAPHORE_VIS0IM	"vis0 - img OK"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	VIS0_reserve	vis0_reserve
#define	VIS0_release	vis0_release

/*!
 * \brief Reserve the vis0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (vis0_reserve(KDEVALL) == KVIS0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    vis0_xyz();
 *    status = vis0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KVIS0NOERR	The manager is reserved
 * \param[out]	KVIS0CHBSY 	The manager is busy
 *
 */
extern	int32_t	vis0_reserve(uint8_t mode);

/*!
 * \brief Release the vis0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = vis0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KVIS0NOERR	OK
 *
 */
extern	int32_t	vis0_release(uint8_t mode);

/*!
 * \brief Configure the vis0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfVisx_t	configure = {
 *                          .oCommand  = KLUM8MSB;
 *                          .oAcqPage  = KPAGE0;
 *                          .oKernSync = (1<<BIMSEMA);
 *                      };
 *
 *    status = vis0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KVIS0NOERR	OK
 *
 */
extern	int32_t	vis0_configure(const cnfVis0_t *configure);

/*!
 * \brief Acquisition of an image
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = vis0_acquisition();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KVIS0NOERR	OK
 * \param[out]	KVIS0TROPR 	Integration on progress
 *
 */
extern	int32_t	vis0_acquisition(void);

/*!
 * \brief Get the image pointer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    *image;
 *
 *     status = vis0_getImage(KPAGE0, &image);
 * \endcode
 *
 * \param[in]	page		The page number (KPAGE0, KPAGE1, KPPI, KNOXFER)
 * \param[in]	**image		Ptr on the image
 * \param[out]	KVIS0NOERR	OK
 * \param[out]	KVIS0PGNOE	The image page does not exist
 *
 */
extern	int32_t	vis0_getImage(uint8_t page, void **image);

#ifdef	__cplusplus
}
#endif

// vis0 manager errors
// -------------------

#define	KVIS0NOERR	0				// No error
#define	KVIS0SYCNA	-(KVIS0ERR+1)	// System call not available
#define	KVIS0GEERR	-(KVIS0ERR+2)	// General error
#define	KVIS0CHBSY	-(KVIS0ERR+3)	// The manager is busy
#define	KVIS0TROPR	-(KVIS0ERR+4)	// Transfer on progress
#define	KVIS0CMNSU	-(KVIS0ERR+5)	// Command not supported
#define	KVIS0PGNOE	-(KVIS0ERR+6)	// The image page does not exist

/*@}*/

#endif
