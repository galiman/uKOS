/*
; test_jpeg.
; ==========

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
; Goal:		Demo of a C application.
;			Test of the hardware jpeg encoder/decoder.
;			(From the Türetken Engin initial investigations)
;
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Decompress a jpeg coded image
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

#define KNBCOLS			8		// Number of cols
#define KNBROWS			8		// Number of rows

// Raw image of 8 x8

static	const	uint8_t	aRefImage[] = {
							144,120, 94,143,150,146,181,129,
							119,101, 88,115,120,111,113, 84,
							146,139,147,147,159,151,116,125,
							106,108,134,116,136,136,85, 130,
							131,135,158,134,147,148,108,161,
							121,122,131,115,108,100,88, 120,
							121,123,121,131,108,91, 117,116,
							119,125,121,157,130,111,165,140
						};

// The two last elements are added manually to make sure that the array is word-aligned.
// This is a constraint that has to be met!

static	const	uint8_t	aJpegImage[] = {
							255, 216, 255, 224, 0, 16, 74, 70, 73, 70, 0, 1, 1, 0, 0, 1,
							0, 1, 0, 0, 255, 219, 0, 67, 0, 16, 11, 12, 14, 12, 10, 16,
							14, 13, 14, 18, 17, 16, 19, 24, 40, 26, 24, 22, 22, 24, 49, 35,
							37, 29, 40, 58, 51, 61, 60, 57, 51, 56, 55, 64, 72, 92, 78, 64,
							68, 87, 69, 55, 56, 80, 109, 81, 87, 95, 98, 103, 104, 103, 62, 77,
							113, 121, 112, 100, 120, 92, 101, 103, 99, 255, 192, 0, 11, 8, 0, 8,
							0, 8, 1, 1, 17, 0, 255, 196, 0, 31, 0, 0, 1, 5, 1, 1,
							1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
							5, 6, 7, 8, 9, 10, 11, 255, 196, 0, 181, 16, 0, 2, 1, 3,
							3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125, 1, 2, 3, 0,
							4, 17, 5, 18, 33, 49, 65, 6, 19, 81, 97, 7, 34, 113, 20, 50,
							129, 145, 161, 8, 35, 66, 177, 193, 21, 82, 209, 240, 36, 51, 98, 114,
							130, 9, 10, 22, 23, 24, 25, 26, 37, 38, 39, 40, 41, 42, 52, 53,
							54, 55, 56, 57, 58, 67, 68, 69, 70, 71, 72, 73, 74, 83, 84, 85,
							86, 87, 88, 89, 90, 99, 100, 101, 102, 103, 104, 105, 106, 115, 116, 117,
							118, 119, 120, 121, 122, 131, 132, 133, 134, 135, 136, 137, 138, 146, 147, 148,
							149, 150, 151, 152, 153, 154, 162, 163, 164, 165, 166, 167, 168, 169, 170, 178,
							179, 180, 181, 182, 183, 184, 185, 186, 194, 195, 196, 197, 198, 199, 200, 201,
							202, 210, 211, 212, 213, 214, 215, 216, 217, 218, 225, 226, 227, 228, 229, 230,
							231, 232, 233, 234, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 255, 218,
							0, 8, 1, 1, 0, 0, 63, 0, 76, 225, 112, 161, 242, 231, 146, 185,
							252, 123, 250, 87, 255, 217,
							0, 0
						};

extern						void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
static			volatile	bool_t		vTerminate;
static						uint8_t		*vImage;
static	const	volatile	uint8_t		*vJpegImage;
static			volatile	uint32_t	vSzJpegImage;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"test_jpeg    Test of the hardware jpag compressor.     (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  test_jpeg\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	void	_initJPEG(void);
static	void	jpeg_decode(const uint8_t *jpegImage, uint32_t szJpegImage, uint8_t *image);
static	void	jpeg_verify(const uint8_t *refImage, const uint8_t *decImage);
static	void	aJPEG_IRQHandler(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);
		misc_toggleLed(0);
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Every 1000-ms
 *  		- Decode a jpeg image
 *  		- Verify the decoded image with a reference
 *
 */
static	void	aProcess_1(const void *argument) {
	uint32_t	sizeJpeg, cpt = 0;
	uint8_t 	*image;

	_initJPEG();

	sizeJpeg = sizeof(aJpegImage) / sizeof(uint8_t);
	image = (uint8_t *)syos_malloc(KINTERNAL, 2000);
	if (image == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	while (TRUE) {
		kern_suspendProcess(1000);
		iotx_printf(KSYST, "\n\nDecompression %d\n", cpt++);

		jpeg_decode(aJpegImage, sizeJpeg, image);
		jpeg_verify(aRefImage, image);
		exit(EXIT_SUCCESS_OS);
	}

}

/*
 * \brief jpeg_decode
 *
 * - Decode a jpeg image
 *
 * \param[in]	*jpegImage		Ptr on the jpeg image
 * \param[in]	szJpegImage		Size of the jpeg image
 * \param[in]	*image			Ptr on the raw image
 *
 */
static	void	jpeg_decode(const uint8_t *jpegImage, uint32_t szJpegImage, uint8_t *image) {
	uint64_t	time[2];
	uint32_t	delta = 0;

	vJpegImage   = jpegImage;
	vImage       = image;
	vSzJpegImage = szJpegImage;

// Flush the FIFO in case they are not empty, and reset the jpeg registers.
// Enable the in/out/eoc interruptions.
// Allow setting the registers by enabling the jpeg core.

	JPEG->CR = JPEG_CR_IFF
			 | JPEG_CR_OFF
			 | JPEG_CR_IFNFIE
			 | JPEG_CR_OFNEIE
			 | JPEG_CR_EOCIE
			 | JPEG_CR_JCEN;

// Clear the header parsing and end of conversion flags
// Prepare the jpeg module for decoding.
// Start the jpeg decoder. The start bit has to be set every time a new image is to be processed.

	JPEG->CFR = JPEG_CFR_CEOCF
			  | JPEG_CFR_CHPDF;

	JPEG->CONFR1 = JPEG_CONFR1_HDR
				 | JPEG_CONFR1_DE;

	kern_getTiccount(&time[0]);
	JPEG->CONFR0 = JPEG_CONFR0_START;

// Continue until the jpeg conversion is done

	vTerminate = FALSE;
	while (!vTerminate) { kern_switchFast(); }

	kern_getTiccount(&time[1]);
	delta = (uint32_t)(time[1] - time[0]);
	iotx_printf(KSYST, "Decoding time %d [us]\n", delta);

	JPEG->CONFR0 = 0;
}

/*
 * \brief jpeg_verify
 *
 * - Verify the original image with the decoded one
 *
 * \param[in]	*refImage	Ptr on the raw image (reference)
 * \param[in]	*decImage	Ptr on the raw image (decoded)
 *
 */
static	void	jpeg_verify(const uint8_t *refImage, const uint8_t *decImage) {
	uint32_t	i;

	iotx_printf(KSYST, "\nX - %d, Y - %d\n\n", (JPEG->CONFR3>>16), (JPEG->CONFR1>>16));

	for (i = 0; i < (KNBCOLS*KNBROWS); i++) {
		if (refImage[i] != decImage[i]) {
			iotx_printf(KSYST, "Incoherence at pixel (%d,%d): expected %d, got %d\n", i % KNBCOLS, i / KNBCOLS,
																					  refImage[i],
																					  decImage[i]);
		}
	}
}

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {
	volatile	proc_t 	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

// Local routines
// ==============

/*
 * \brief _initJPEG
 *
 * - Initialize the hardware jpeg codec
 *
 */
static	void	_initJPEG(void) {

    RCC->AHB2ENR |= RCC_AHB2ENR_JPEGEN;

	vExce_indIntVectors[JPEG_IRQn] 	= aJPEG_IRQHandler;
	NVIC->IP[JPEG_IRQn]				= (KINT_LEVEL_PERIPH<<4);
	NVIC->ISER[3]				   |= (1<<(JPEG_IRQn & 0x1F));
}

/*
 * \brief aJPEG_IRQHandler
 *
 * - JPEG interruption
 *
 */
static	void	aJPEG_IRQHandler(void) {

// End of decode

	if ((JPEG->SR & JPEG_SR_EOCF) != 0) {
		JPEG->CR &= ~JPEG_CR_EOCIE;
		JPEG->CR &= ~JPEG_CR_IFNFIE;
		vTerminate = TRUE;
	}

// Fill the FIFO

	if ((JPEG->SR & JPEG_SR_IFNFF) != 0) {
		if (vSzJpegImage > 0) {
			JPEG->DIR = *vJpegImage++;
			vSzJpegImage -= 4;
		}
		else {
			JPEG->CR &= ~JPEG_CR_IFNFIE;
		}
	}

// Read the FIFO

	if ((JPEG->SR & JPEG_SR_OFNEF) != 0) {
		*vImage++ = JPEG->DOR;
	}
}

