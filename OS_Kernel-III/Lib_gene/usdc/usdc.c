/*
; usdc.
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

#include	<uKOS.h>

static	bool_t	vReserved = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"usdc         usdc manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "usdc manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_usdc_init(void);
extern	int32_t	stub_usdc_initialize(prmUsdc_t *specification);
extern	int32_t	stub_usdc_read(const prmUsdc_t *specification, uint8_t *buffer, uint32_t sector, uint8_t nbSectors);
extern	int32_t	stub_usdc_write(const prmUsdc_t *specification, const uint8_t *buffer, uint32_t sector, uint8_t nbSectors);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Usdc, KIDGENE, KUSDCNUM, NULL, "2.0", (1<<BSHOW));

/*
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
int32_t	usdc_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KUSDCCHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KUSDCNOERR);
}

/*
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
int32_t	usdc_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KUSDCNOERR);
}

/*
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
int32_t	usdc_initialize(prmUsdc_t *specification) {

	_init();
	return (stub_usdc_initialize(specification));
}

/*
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
int32_t	usdc_readSDCard(uint8_t *buffer, uint32_t nbBytes, uint32_t sector) {
	uint8_t		nbSectorsForThisCall, lastSectorBuffer[KSZFATSECT];
	uint32_t	i, nbSectors, nbBytesLastSector;
	prmUsdc_t	card;
	uint8_t		*memory;

	_init();

// Read the card specifications

	if (usdc_initialize(&card) != KUSDCNOERR) {
		return (KUSDCNOCRD);
	}

// Compute the number of sectors to read.
// We need to treat the last sector specially because
// the input buffer may not fill it fully, in which case
// we need to avoid reading beyond the last element in the buffer

	nbSectors = nbBytes / KSZFATSECT;
	nbBytesLastSector = nbBytes % KSZFATSECT;
	memory = buffer;

// Write the sectors in chunks.

	while (nbSectors > 0) {
		nbSectorsForThisCall = (nbSectors > KMAXSECTORSPERCALL ? KMAXSECTORSPERCALL : nbSectors);
    
		if (usdc_read(&card, memory, sector, nbSectorsForThisCall) != KUSDCNOERR) {
			return (KUSDCTRANT);
		}
    
		memory    += nbSectorsForThisCall * KSZFATSECT;
		sector    += nbSectorsForThisCall;
		nbSectors -= nbSectorsForThisCall;
	}

// Read the last sector's data, which may not fill it fully

	if (nbBytesLastSector > 0) {
		if (usdc_read(&card, lastSectorBuffer, sector, 1) != KUSDCNOERR) {
			return (KUSDCTRANT);
		}

		for (i = 0; i < nbBytesLastSector; i++) {
			*(memory++) = lastSectorBuffer[i];
		}
	}
	return (KUSDCNOERR);
}

/*
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
int32_t	usdc_writeSDCard(const uint8_t *buffer, uint32_t nbBytes, uint32_t sector) {
			uint8_t		nbSectorsForThisCall, lastSectorBuffer[KSZFATSECT];
			uint32_t	i, nbSectors, nbBytesLastSector;
			prmUsdc_t	card;
	const	uint8_t		*memory;

	_init();

// Read the card specifications

	if (usdc_initialize(&card) != KUSDCNOERR) {
		return (KUSDCNOCRD);
	}

// Compute the number of sectors to read.
// We need to treat the last sector specially because
// the input buffer may not fill it fully, in which case
// we need to avoid reading beyond the last element in the buffer

	nbSectors = nbBytes / KSZFATSECT;
	nbBytesLastSector = nbBytes % KSZFATSECT;
	memory = buffer;

// Write the sectors in chunks.

	while (nbSectors > 0) {
		nbSectorsForThisCall = (nbSectors > KMAXSECTORSPERCALL ? KMAXSECTORSPERCALL : nbSectors);
    
		if (usdc_write(&card, memory, sector, nbSectorsForThisCall) != KUSDCNOERR) {
			return (KUSDCTRANT);
		}
    
		memory    += nbSectorsForThisCall * KSZFATSECT;
		sector    += nbSectorsForThisCall;
		nbSectors -= nbSectorsForThisCall;
	}

// Write the last sector's data, which may not fill it fully
// Fill the gap with 0x00 (for test)

	if (nbBytesLastSector > 0) {
		for (i = 0; i < KSZFATSECT; i++) {
			lastSectorBuffer[i] = 0;
		}
		for (i = 0; i < nbBytesLastSector; i++) {
			lastSectorBuffer[i] = *(memory++);
		}
		if (usdc_write(&card, lastSectorBuffer, sector, 1) != KUSDCNOERR) {
			return (KUSDCTRANT);
		}
	}
	return (KUSDCNOERR);
}

/*
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
int32_t	usdc_read(const prmUsdc_t *specification, uint8_t *buffer, uint32_t sector, uint8_t nbSectors) {

	_init();
	return (stub_usdc_read(specification, buffer, sector, nbSectors));
}

/*
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
int32_t	usdc_write(const prmUsdc_t *specification, const uint8_t *buffer, uint32_t sector, uint8_t nbSectors) {

	_init();
	return (stub_usdc_write(specification, buffer, sector, nbSectors));
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

		stub_usdc_init();
	}
}

