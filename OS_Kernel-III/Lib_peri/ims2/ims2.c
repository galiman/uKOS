/*
; ims2.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2016-05-06
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		ims2 manager (QVGA).
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

static	bool_t	vReserved = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"ims2         ims2 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "ims2 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	int32_t	_init(void);
extern	int32_t	stub_ims2_init(void);
extern	int32_t	stub_ims2_configure(const cnfIms2_t *configure);
extern	int32_t	stub_ims2_acquisition(void);
extern	int32_t	stub_ims2_getImage(volatile void **image);
extern	int32_t	stub_ims2_ctrlLeds(uint8_t ledState);
extern	int32_t	stub_ims2_getRegister(S2Order_t *order);
extern	int32_t	stub_ims2_putRegister(const S2Order_t *order);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Ims2, KIDPERI, KIMS2NUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief Reserve the ims2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (ims2_reserve(KDEVALL) == KIMS2CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    ims2_xyz();
 *    status = ims2_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KIMS2NOERR	The manager is reserved
 * \param[out]	KIMS2CHBSY 	The manager is busy
 * \param[out]	KIMS2TIMEO	Timeout error
 *
 */
int32_t	ims2_reserve(uint8_t mode) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KIMS2CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KIMS2NOERR);
}

/*
 * \brief Release the ims2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = ims2_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KIMS2NOERR	OK
 * \param[out]	KIMS2TIMEO	Timeout error
 *
 */
int32_t	ims2_release(uint8_t mode) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KIMS2NOERR);
}

/*
 * \brief Configure the ims2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * // Callback routine
 *
 * static    void    _myRoutine(void) {
 *
 * }
 *
 * // Initialisation table
 *
 * int32_t    status;
 * const      cnfIms2_t    configure = {
 *                             .oAcqMode  = KCONT;
 *                             .oPixMode  = KCONTRAST10;
 *                             .oAcqPage  = KEXTERNAL;
 *                             .oNbRows   = 200;
 *                             .oNbCols   = 200;
 *                             .oKernSync = (1<<BIMSEMA) | (1<<BVSSEMA);
 *                             .oHSync    = 0;
 *                             .oFrame    = 0;
 *                             .oVSync    = 0;
 *                             .oDMAEc    = _myRoutine;
 *                         };
 *
 *    status = ims2_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KIMS2NOERR	OK
 * \param[out]	KIMS2TIMEO	Timeout error
 * \param[out]	KIMS2NOMEM	Not enough memory
 *
 */
int32_t	ims2_configure(const cnfIms2_t *configure) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	return (stub_ims2_configure(configure));
}

/*
 * \brief Acquisition of an image
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = ims2_acquisition();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KIMS2NOERR	OK
 * \param[out]	KIMS2TIMEO	Timeout error
 *
 */
int32_t	ims2_acquisition(void) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	return (stub_ims2_acquisition());
}

/*
 * \brief Get the image pointer
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * uint8_t    *image;
 *
 *    status = ims2_getImage(&image);
 * \endcode
 *
 * \param[in]	**image		Ptr on the image
 * \param[out]	KIMS2NOERR	OK
 *
 */
int32_t	ims2_getImage(volatile void **image) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	return (stub_ims2_getImage(image));
}

/*
 * \brief Control of the LEDs
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = ims2_ctrlLeds(0b00001010);
 * \endcode
 *
 * \param[in]	ledState	State of the LEDs
 * \param[out]	KIMS2NOERR	OK
 *
 */
int32_t	ims2_ctrlLeds(uint8_t ledState) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	return (stub_ims2_ctrlLeds(ledState));
}

/*
 * \brief Get the value of a S2 register
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    i, vTabLUTDay[1024];
 *
 * // Read the LUT
 *
 * S2Order_t    vOrder_RLU = {
 *                  SPI_CMD_READ_4_BYTE_P2,    // The command
 *                  0x60000,                   // The register address
 *                  SPI_CMD_READ_4_BYTE_P4,    // P4 command
 *                  TRUE,                      // Dummy read
 *                  4,                         // 4 bytes
 *                  -1                         // result
 *              };
 *
 *    for (i = 0; i < 1024; i++) {
 *        vOrder_RLU.oRegister = 0x60000 + (i*4);
 *        if (ims2_getRegister(&vOrder_RLU) == FALSE) {
 *            return (KIMS2TIMEO);
 *        }
 *        aTabLUTDay[i] = vOrder_RLU.oParam;
 *    }
 * \endcode
 *
 * \param[in]	*order			Ptr on the order
 * \param[out]	KIMS2NOERR		OK
 * \param[out]	KIMS2TIMEO		Timeout error
 *
 */
int32_t	ims2_getRegister(S2Order_t *order) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	return (stub_ims2_getRegister(order));
}

/*
 * \brief Put a value into a S2 register
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 * // Write the H blanking order
 *
 * S2Order_t    vOrder_HOR = {
 *                  SPI_CMD_WRITE_2_BYTE,    // The command
 *                  REG_DAU_HORIZ_BLANK,     // The register address
 *                  -1,                      // P4 command
 *                  FALSE,                   // Dummy read
 *                  2,                       // 4 bytes
 *                  -1                       // Value
 *              };
 *
 *    vOrder_HOR.oParam = 512;
 *    status = ims2_getRegister(&vOrder_HOR);
 * \endcode
 *
 * \param[in]	*order			Ptr on the order
 * \param[out]	KIMS2NOERR		OK
 * \param[out]	KIMS2TIMEO		Timeout error
 *
 */
int32_t	ims2_putRegister(const S2Order_t *order) {
	int32_t		status;

	status = _init();
	if (status != KIMS2NOERR) {
		return (status);
	}

	return (stub_ims2_putRegister(order));
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
static	int32_t	_init(void) {
			int32_t	status = KIMS2NOERR;
	static	bool_t	init = FALSE;

	if (!init) {
		init = TRUE;

		status = stub_ims2_init();
	}
	return (status);
}

