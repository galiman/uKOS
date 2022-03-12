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

#ifndef	__IMS2__
#define	__IMS2__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KIMS2MAN	(KIMS2NUM<<8)
#define	KIMS2ERR	((KIDPERI<<24) | KIMS2MAN)

#define	KOPERATE	0													// Imager in a normal mode (run)
#define	KSTANDBY	1													// Imager in standby

// Configuration structure
// -----------------------

typedef	struct	cnfIms2	cnfIms2_t;

struct cnfIms2 {
			uint8_t		oAcqMode;										// Acquisition mode
			#define		KCONT			0								// KCONT = continuous mode
			#define		KSNAP			1								// KSNAP = snapshot mode
			#define		KSYNC			2								// KSYNC = sync mode

			uint8_t		oPixMode;										// Pixel mode
			#define		KLUMINANCE8		0								// KLUMINANCE 8-bits  = readout a luminance image
			#define		KLUMINANCE10	1								// KLUMINANCE 10-bits = readout a luminance image
			#define		KCONTRAST8		2								// KCONTRAST 8-bits   = readout a contrast image
			#define		KCONTRAST10		3								// KCONTRAST 10-bits  = readout a contrast image
			#define		KORICON8		4								// KORICON 8-bits     = readout an oricon image
			#define		KORICON10		5								// KORICON 10-bits    = readout an oricon image

			uint8_t		oAcqPage;										// Acquisition storage page or device
			#define		KINTERNAL		0								// Acquisition result on the CPU internal memory (only 320 x 240 x 8bits)
			#define		KEXTERNAL		1								// Acquisition result on the external memory

			uint16_t	oStRows;										// Start of rows
			uint16_t	oNbRows;										// Number of rows
			#define		KNBROWSQVGA		240								// Number of rows (QVGA)

			uint16_t	oStCols;										// Start of cols
			uint16_t	oNbCols;										// Number of cols
			#define		KNBCOLSQVGA		320								// Number of columns (QVGA)

			uint8_t		oKernSync;										// uKernel synchro
			#define		BIMSEMA			0								// IM semaphore
			#define		BVSSEMA			1								// Vertical synchro semaphore

			void		*oImgCnf;										// Imager configuration table
			void		(*oHSync)(void);								// Ptr on the callback HSync routine
			void		(*oFrame)(void);								// Ptr on the callback Frame routine
			void		(*oVSync)(void);								// Ptr on the callback VSync routine
			void		(*oDMAEc)(void);								// Ptr on the callback DMAEc routine
};

// S2 order structure
// ------------------

typedef	struct	S2Order	S2Order_t;

struct	S2Order {
			uint8_t		oSPICmd2;										// SPI CMD P2
			uint32_t	oRegister;										// Register address
			uint8_t		oSPICmd4;										// SPI CMD P4
			bool_t		oRdDummy;										// Dummy read
			uint32_t	oNbParam;										// Number of parameters
			uint32_t	oParam;											// Parameters
};									

// Semaphores
// ----------

#define	KSEMAPHORE_IMS2VS	"ims2 - V synchro"
#define	KSEMAPHORE_IMS2IM	"ims2 - img OK"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	IMS2_reserve	ims2_reserve
#define	IMS2_release	ims2_release

/*!
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
extern	int32_t	ims2_reserve(uint8_t mode);

/*!
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
extern	int32_t	ims2_release(uint8_t mode);

/*!
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
extern	int32_t	ims2_configure(const cnfIms2_t *configure);

/*!
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
extern	int32_t	ims2_acquisition(void);

/*!
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
extern	int32_t	ims2_getImage(volatile void **image);

/*!
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
extern	int32_t	ims2_ctrlLeds(uint8_t ledState);

/*!
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
extern	int32_t	ims2_getRegister(S2Order_t *order);

/*!
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
extern	int32_t	ims2_putRegister(const S2Order_t *order);

#ifdef	__cplusplus
}
#endif

// img0 manager errors
// -------------------

#define	KIMS2NOERR	0				// No error
#define	KIMS2SYCNA	-(KIMS2ERR+1)	// System call not available
#define	KIMS2CNERR	-(KIMS2ERR+2)	// Configuration error
#define	KIMS2GEERR	-(KIMS2ERR+3)	// General error
#define	KIMS2TIMEO	-(KIMS2ERR+4)	// Timeout error
#define	KIMS2CHBSY	-(KIMS2ERR+5)	// The manager is busy
#define	KIMS2PGNOE	-(KIMS2ERR+6)	// The image page does not exist
#define	KIMS2BDMOD	-(KIMS2ERR+7)	// Bad mode for called function
#define	KIMS2NOMEM	-(KIMS2ERR+8)	// Not enough memory

/*@}*/

#endif
