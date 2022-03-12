/*
; usdc.
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
; Goal:		usdc manager.
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

#ifndef	__USDC__
#define	__USDC__

/*!
 * \addtogroup Lib_gene
 */
/*@{*/

#include 	<modules.h>

#define	KUSDCMAN	(KUSDCNUM<<8)
#define	KUSDCERR	((KIDGENE<<24) | KUSDCMAN)

#define	KSZFATSECT			512											// Sector size in bytes
#define	KMAXSECTORSPERCALL	128											// Max. number of sectors per call

// SDCard parameters structure
// ---------------------------

typedef	struct	prmUsdc	prmUsdc_t;

struct prmUsdc {
			uint64_t	oC_SIZE;										// Device size
			uint32_t	oPSN;											// Product Serial Number
			uint16_t	oVoltage;										// Card Voltage
			uint8_t		oMID;											// Manufacturer ID
			uint8_t		oPNM[5+1];										// Product Name
			uint8_t		oCCS;											// Card Capacity Status
			#define 	KCT_MMC		0x01								// MMC card Ver 3 (not supported)
			#define 	KCT_SD1		0x02								// SDC card Ver 1 (not supported)
			#define 	KCT_SD2		0x04								// SDC card Ver 2
			#define 	KCT_BLOCK	0x08								// Block addressing
};

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	USDC_reserve	usdc_reserve
#define	USDC_release	usdc_release

/*!
 * \brief Reserve the usdc manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (uscd_reserve(KDEVALL) == KUSCDCHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    uscd_xyz();
 *    status = uscd_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KUSDCNOERR	The manager is reserved
 * \param[out]	KUSDCCHBSY 	The manager is busy
 *
 */
extern	int32_t	usdc_reserve(uint8_t mode);

/*!
 * \brief Release the usdc manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = uscd_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KUSDCNOERR	OK
 *
 */
extern	int32_t	usdc_release(uint8_t mode);

/*!
 * \brief Initialize the SDCard
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * prmUsdc_t    sdCard;
 *
 *    status = usdc_initialize(&sdCard);
 *    if (status != KUSDCNOERR) {
 *        exit(EXIT_FAILURE);
 *    }
 *
 *    if (sdCard.oCCS == KCT_SD1) card = "SDCard V.1";
 *    else                        card = "SDCard V.2";
 * \endcode
 *
 * \param[in]	*specification	Ptr on the SDCard specification
 * \param[out]	KUSDCNOERR		OK
 * \param[out]	KUSDCNOCRD		No SDCard
 * \param[out]	KUSDCCANRE		SDCard not recognized
 *
 */
extern	int32_t	usdc_initialize(prmUsdc_t *parameters);

/*!
 * \brief Read 1 or more sectors from the SDCard
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KNBBYTES    53423
 *
 * int32_t    status;
 * int8_t     readBuffer[KNBBYTES];
 *
 *    status = usdc_readSDCard(&readBuffer[], KNBBYTES, 5);
 *    if (status != KUSDCNOERR) {
 *        exit(EXIT_FAILURE);
 *    }
 * \endcode
 *
 * \param[in]	*buffer			Ptr on the buffer
 * \param[in]	nbBytes			Number of bytes to read
 * \param[in]	sector			Start sector
 * \param[out]	KUSDCNOERR		OK
 * \param[out]	KUSDCNOCRD		No SDCard
 * \param[out]	KUSDCCANRE		SDCard not recognized
 * \param[out]	KUSDCLNBU0		The number of sectors is = 0
 * \param[out]	KUSDCTRANT		Transfer not terminated
 *
 */
extern	int32_t	usdc_readSDCard(uint8_t *buffer, uint32_t nbBytes, uint32_t sector);

/*!
 * \brief Write 1 or more sectors to the SDCard
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KNBBYTES    53423
 *
 * int32_t    status;
 * const      int8_t    writeBuffer[KNBBYTES];
 *
 *    status = usdc_writeSDCard(&writeBuffer[], KNBBYTES, 5);
 *    if (status != KUSDCNOERR) {
 *        exit(EXIT_FAILURE);
 *    }
 * \endcode
 *
 * \param[in]	*buffer			Ptr on the buffer
 * \param[in]	nbBytes			Number of bytes to write
 * \param[in]	sector			Start sector
 * \param[out]	KUSDCNOERR		OK
 * \param[out]	KUSDCNOCRD		No SDCard
 * \param[out]	KUSDCCANRE		SDCard not recognized
 * \param[out]	KUSDCLNBU0		The number of sectors is = 0
 * \param[out]	KUSDCTRANT		Transfer not terminated
 *
 */
extern	int32_t	usdc_writeSDCard(const uint8_t *buffer, uint32_t size, uint32_t sector);

/*!
 * \brief Read 1 or more sectors (<= 128) from the SDCard
 *
 * Call example in C:
 *
 * \code{.c}
 * #define     KNBBYTES    5
 *
 * int32_t      status;
 * prmUsdc_t    sdCard;
 * static       int8_t    buffer[KNBSECTORS*KSZFATSECT];
 *
 *    status = usdc_initialize(&sdCard);
 *    status = usdc_read(&sdCard, buffer, 45678, KNBSECTORS);
 * \endcode
 *
 * \param[in]	*specification	Ptr on the SDCard specification
 * \param[in]	*buffer			Ptr on the buffer
 * \param[in]	sector			Start sector
 * \param[in]	nbSectosr		Number of sector to read
 * \param[out]	KUSDCNOERR		OK
 * \param[out]	KUSDCNOCRD		No SDCard
 * \param[out]	KUSDCLNBU0		The number of sectors is = 0
 * \param[out]	KUSDCTRANT		Transfer not terminated
 *
 */
extern	int32_t	usdc_read(const prmUsdc_t *parameters, uint8_t *buffer, uint32_t sector, uint8_t nbSectors);

/*!
 * \brief Write 1 or more sectors (<= 128) to the SDCard
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KNBBYTES    5
 *
 * int32_t      status;
 * prmUsdc_t    sdCard;
 * const        int8_t    buffer[KNBSECTORS*KSZFATSECT] = { 0, 1, ... };
 *
 *    status = usdc_initialize(&sdCard);
 *    status = usdc_write(&sdCard, buffer, 35, KNBSECTORS);
 * \endcode
 *
 * \param[in]	*specification	Ptr on the SDCard specification
 * \param[in]	*buffer			Ptr on the buffer
 * \param[in]	sector			Start sector
 * \param[in]	nbSectors		Number of sector to write
 * \param[out]	KUSDCNOERR		OK
 * \param[out]	KUSDCNOCRD		No SDCard
 * \param[out]	KUSDCLNBU0		The number of sectors is = 0
 * \param[out]	KUSDCTRANT		Transfer not terminated
 *
 */
extern	int32_t	usdc_write(const prmUsdc_t *parameters, const uint8_t *buffer, uint32_t sector, uint8_t nbSectors);

#ifdef	__cplusplus
}
#endif

// usdc manager errors
// -------------------

#define	KUSDCNOERR	0				// No error
#define	KUSDCSYCNA	-(KUSDCERR+1)	// System call not available
#define	KUSDCGEERR	-(KUSDCERR+2)	// General error
#define	KUSDCCHBSY	-(KUSDCERR+3)	// The manager is busy
#define	KUSDCNOCRD	-(KUSDCERR+4)	// No SDCard
#define	KUSDCCANRE	-(KUSDCERR+5)	// SDCard not recognized
#define	KUSDCLNBU0	-(KUSDCERR+6)	// The number of sectors is = 0
#define	KUSDCTRANT	-(KUSDCERR+7)	// Transfer not terminated

/*@}*/

#endif
