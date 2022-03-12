/*
; xfer.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 1999-01-01
; Modifs:	Franzi Edo.	The 2000-01-10
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		xfer manager.
;			Idea, concept and original implementation by Nussbaum Pascal.
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

#ifndef	__XFER__
#define	__XFER__

/*!
 * \addtogroup Lib_gene
 */
/*@{*/

#include 	<modules.h>

#define	KXFERMAN	(KXFERNUM<<8)
#define	KXFERERR	((KIDGENE<<24) | KXFERMAN)

// Xfer parameters structure
// -------------------------

typedef	struct	cnfXfer	cnfXfer_t;

struct cnfXfer {
			void		*oXferBuffer;									// Ptr on the transfer buffer
			uint32_t	oPacketSize;									// Size of the packet
			uint32_t	oNbPackets;										// Number of packets
			void		(*oEndXfer)(void);								// Ptr on the callback EndXfer routine
};

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	XFER_reserve	xfer_reserve
#define	XFER_release	xfer_release

/*!
 * \brief Reserve the xfer manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (xfer_reserve(KDEVALL) == KXFERCHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    xfer_xyz();
 *    status = xfer_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KXFERNOERR	The manager is reserved
 * \param[out]	KXFERCHBSY 	The manager is busy
 *
 */
extern	int32_t	xfer_reserve(uint8_t mode);

/*!
 * \brief Release the xfer manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = xfer_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KXFERNOERR	OK
 *
 */
extern	int32_t	xfer_release(uint8_t mode);

/*!
 * \brief Configure the xfer manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KNBCOLS        480                              // Number of cols
 * #define    KNBROWS        480                              // Number of rows
 * #define    KNBPACKETS     80                               // Number of packs
 * #define    KNBPACKMIO     (KNBPACKETS-1)                   // Number of packs minus 1
 * #define    KPACKETSIZE    ((KNBCOLS*KNBROWS)/KNBPACKETS)   // Packet size
 * 
 * int32_t      status;
 * cnfXfer_t    configureXFER;
 * uint8_t      *image;
 * bool_t       imageReady = FALSE;
 * 
 * // Transfer buffer = image buffer, Size of the transfer
 * // Call back after the transfer of all the packets
 * 
 *    img0_getImage((void *)&image);
 * 
 *    configureXFER.oXferBuffer = image;
 *    configureXFER.oPacketSize = KPACKETSIZE;
 *    configureXFER.oNbPackets  = KNBPACKETS;
 *    configureXFER.oEndXfer    = _transferSPI;
 * 
 *    status = xfer_configure(&configureXFER);
 *    if (status != KXFERNOERR) {
 *        iotx_printf(KSYST, "xfer manager problem\n");
 *    }
 * 
 * // Start the first acquisition
 * 
 *    img0_acquisition();
 *    xfer_startXfer();
 *    while (TRUE) {
 *        while (imageReady == FALSE) {
 *            kern_switchFast();
 *        }
 *        vImageReady = FALSE;
 *        img0_acquisition();
 *    }
 * 
 *    static    void    _transferSPI(void) {
 *        uint8_t    *image;
 * 
 *        img0_getImage((void *)&image);
 *        xfer_setXferBuffer(image);
 *        imageReady = TRUE;
 *    }
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KXFERNOERR	OK
 *
 */
extern	int32_t	xfer_configure(const cnfXfer_t *parameters);

/*!
 * \brief Set the xfer buffer
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KNBSECTORS    5;
 *
 * int32_t    status;
 * static     uint8_t    buffer[752*480];
 *
 *    status = xfer_setXferBuffer(buffer);
 * \endcode
 * 
 * \param[in]	*xferBuffer	Ptr on the xfer buffer
 * \param[out]	KXFERNOERR	OK
 *
 */
extern	int32_t	xfer_setXferBuffer(void *xferBuffer);

/*!
 * \brief Start the transfer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = xfer_startXfer();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KXFERNOERR	OK
 *
 */
extern	int32_t	xfer_startXfer(void);

/*!
 * \brief Stop the transfer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = xfer_stopXfer();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KXFERNOERR	OK
 *
 */
extern	int32_t	xfer_stopXfer(void);

#ifdef	__cplusplus
}
#endif

// Xfer manager errors
// -------------------

#define	KXFERNOERR	0				// No error
#define	KXFERSYCNA	-(KXFERERR+1)	// System call not available
#define	KXFERGEERR	-(KXFERERR+2)	// General error
#define	KXFERCHBSY	-(KXFERERR+3)	// The manager is busy

/*@}*/

#endif
