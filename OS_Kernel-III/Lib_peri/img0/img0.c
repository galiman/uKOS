/*
; img0.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.		The 2006-06-28
; Modifs:	Nussbaum Pascal	The 2014-08-22	Added the "true" snap mode
; Modifs:	Nussbaum Pascal	The 2014-08-28	Added the cntrl of the standby
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		img0 manager.
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

LOC_CONST_STRG(aStrApplication[]) =	"img0         img0 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "img0 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	int32_t	_init(void);
extern	int32_t	stub_img0_init(void);
extern	int32_t	stub_img0_configure(const cnfImg0_t *configure);
extern	int32_t	stub_img0_acquisition(void);
extern	int32_t	stub_img0_getImage(volatile void **image);
extern	int32_t	stub_img0_ctrlLeds(uint8_t ledState);
extern	int32_t	stub_img0_getSerialNumber(uint64_t *serialNumber);
extern	int32_t	stub_img0_getRegister(uint8_t registerNb, uint16_t *value);
extern	int32_t	stub_img0_putRegister(uint8_t registerNb, uint16_t value);
extern  int32_t	stub_img0_ctrlStandby(uint8_t mode);
extern  int32_t	stub_img0_setExposure(uint32_t time);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Img0, KIDPERI, KIMG0NUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Reserve the img0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (img0_reserve(KDEVALL) == KIMG0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    img0_xyz();
 *    status = img0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KIMG0NOERR	The manager is reserved
 * \param[out]	KIMG0CHBSY 	The manager is busy
 * \param[out]	KIMG0TIMEO	Timeout error
 *
 */
int32_t	img0_reserve(uint8_t mode) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	INTERRUPTION_OFF;
	if (vReserved) { RETURN_INT_RESTORED(KIMG0CHBSY); }
	vReserved = TRUE;
	RETURN_INT_RESTORED(KIMG0NOERR);
}

/*
 * \brief Release the img0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = img0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0TIMEO	Timeout error
 *
 */
int32_t	img0_release(uint8_t mode) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	INTERRUPTION_OFF;
	vReserved = FALSE;
	RETURN_INT_RESTORED(KIMG0NOERR);
}

/*
 * \brief Configure the img0 manager	
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
 * // Image 60 x 60 pixels; this table concerns the Aptina MT9V032 imager (Fixed exposure)
 *
 * static    const    mt9v032_t    aTab060060F[] = {
 *                                     { 1,   0x0001 }, { 2,   0x0005 }, { 3,   0x003C }, { 4,   0x003D },
 *                                     { 5,   0x026F }, { 6,   0x0032 }, { 7,   0x0388 }, { 8,   0x01E0 },
 *                                     { 9,   0x3FFE }, { 10,  0x0164 }, { 11,  0x001D }, { 16,  0x002D },
 *                                     { 21,  0x8032 }, { 32,  0x03D5 }, { 33,  0x0018 }, { 34,  0x0028 },
 *                                     { 40,  0x0016 }, { 41,  0x001C }, { 43,  0x0003 }, { 44,  0x0006 },
 *                                     { 46,  0x0004 }, { 47,  0x0003 }, { 70,  0x1606 }, { 71,  0x8081 },
 *                                     { 72,  0x007F }, { 104, 0xE5F9 }, { 105, 0x65D7 }, { 106, 0x2950 },
 *                                     { 107, 0xA040 }, { 112, 0x0032 }, { 114, 0x0001 }, { 115, 0x0307 },
 *                                     { 116, 0x0010 }, { 128, 0x00F4 }, { 129, 0x0004 }, { 130, 0x0004 },
 *                                     { 131, 0x0004 }, { 132, 0x0004 }, { 133, 0x0004 }, { 134, 0x0004 },
 *                                     { 135, 0x0004 }, { 136, 0x0004 }, { 137, 0x0004 }, { 138, 0x0004 },
 *                                     { 139, 0x0004 }, { 140, 0x0004 }, { 141, 0x0004 }, { 142, 0x0004 },
 *                                     { 143, 0x0004 }, { 144, 0x0004 }, { 145, 0x0004 }, { 146, 0x0004 },
 *                                     { 147, 0x0004 }, { 148, 0x0004 }, { 149, 0x0004 }, { 150, 0x0004 },
 *                                     { 151, 0x0004 }, { 152, 0x0004 }, { 154, 0x003B }, { 155, 0x02F0 },
 *                                     { 156, 0x02F0 }, { 157, 0x02F0 }, { 160, 0x003B }, { 161, 0x01E0 },
 *                                     { 162, 0x01E0 }, { 163, 0x01E0 }, { 165, 0x0020 }, { 166, 0x0008 },
 *                                     { 168, 0x0001 }, { 169, 0x0008 }, { 171, 0x0001 }, { 175, 0x0000 },
 *                                     { 176, 0x01B0 }, { 189, 0x006C }, { 194, 0x0850 }, { 0,   0x0000 }
 *                                 };
 *
 * int32_t    status;
 * const      cnfImgx_t    configure = {
 *                             .oAcqMode  = KCONT;
 *                             .oPixMode  = PIX8BITS;
 *                             .oAcqPage  = KINTERNAL;
 *                             .oNbRows   = 60;
 *                             .oNbCols   = 60;
 *                             .oKernSync = (1<<BIMSEMA) | (1<<BVSSEMA);
 *                             .oHSync    = 0;
 *                             .oFrame    = 0;
 *                             .oVSync    = 0;
 *                             .oDMAEc    = _myRoutine;
 *                             .oImgCnf   = (void *)aTab060060F;
 *                         };
 *
 *    status = img0_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0TIMEO	Timeout error
 * \param[out]	KIMG0NOMEM	Not enough memory
 *
 */
int32_t	img0_configure(const cnfImg0_t *configure) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_configure(configure));
}

/*
 * \brief Acquisition of an image
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = img0_acquisition();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0TIMEO	Timeout error
 *
 */
int32_t	img0_acquisition(void) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_acquisition());
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
 *    status = img0_getImage(&image);
 * \endcode
 *
 * \param[in]	**image		Ptr on the image
 * \param[out]	KIMG0NOERR	OK
 *
 */
int32_t	img0_getImage(volatile void **image) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_getImage(image));
}

/*
 * \brief Control of the LEDs
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = img0_ctrlLeds(0b00001010);
 * \endcode
 *
 * \param[in]	ledState	State of the LEDs
 * \param[out]	KIMG0NOERR	OK
 *
 */
int32_t	img0_ctrlLeds(uint8_t ledState) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_ctrlLeds(ledState));
}

/*
 * \brief Get the serial number of the imager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint64_t    serialNumber;
 *
 *    status = img0_getSerialNumber(&serailNumber);
 * \endcode
 *
 * \param[in]	*serialNumber	Ptr on the serial number
 * \param[out]	KIMG0NOERR		OK
 * \param[out]	KIMG0TIMEO		Timeout error
 *
 */
int32_t	img0_getSerialNumber(uint64_t *serialNumber) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_getSerialNumber(serialNumber));
}

/*
 * \brief Get the value of an imager register
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    value;
 *
 *    status = img0_getRegister(0x34, &value);
 * \endcode
 *
 * \param[in]	registerNb	The register address
 * \param[in]	*value		Ptr on the value
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0TIMEO	Timeout error
 *
 */
int32_t	img0_getRegister(uint8_t registerNb, uint16_t *value) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_getRegister(registerNb, value));
}

/*
 * \brief Put a value into an imager register
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint16_t    value = 0x1234;
 *
 *    status = img0_putRegister(0x34, value);
 * \endcode
 *
 * \param[in]	registerNb	The register address
 * \param[in]	value		The value
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0TIMEO	Timeout error
 *
 */
int32_t	img0_putRegister(uint8_t registerNb, uint16_t value) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_putRegister(registerNb, value));
}

/*
 * \brief Enables or disables the standby mode of the imager (only in KSNAP mode)
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = img0_ctrlStandby(KSTANDBY);
 * \endcode
 *
 * \param[in]	mode		KOPERATE = standby OFF
 * \param[in]	mode		KSTANDBY = standby ON
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0BDMOD	Bad mode for called function
 *
 */
int32_t	img0_ctrlStandby(uint8_t mode) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_ctrlStandby(mode));
}

/*
 * \brief Set the exposure time
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = img0_setExposure(15000);
 * \endcode
 *
 * \param[in]	time		Integration time (-1 = auto-exposure)
 * \param[out]	KIMG0NOERR	OK
 * \param[out]	KIMG0BDMOD	Bad mode for called function
 *
 */
int32_t	img0_setExposure(uint32_t time) {
	int32_t		status;

	status = _init();
	if (status != KIMG0NOERR) {
		return (status);
	}

	return (stub_img0_setExposure(time));
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
			int32_t	status = KIMG0NOERR;
	static	bool_t	init = FALSE;

	if (!init) {
		init = TRUE;

		status = stub_img0_init();
	}
	return (status);
}

