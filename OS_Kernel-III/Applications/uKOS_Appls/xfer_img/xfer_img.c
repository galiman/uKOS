/*
; xfer_img.
; =========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Demo of a C application.
;			Idea, concept and original implementation by Nussbaum Pascal.
;
;			Transferring an image via a slave SPI by DMA.
;			Limitations:
;			- size of the image should be less than 480 x 480 pixels
;
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;				  Display the frame-rate
;
;			- P1: As fast as possible (under the synchro of the end of transfer)
;				  Acquire an image
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

#define KNBCOLS			752								// Number of cols
#define KNBROWS			480								// Number of rows
#define KNBPACKETS 		80								// Number of packs (optimal value)
#define KPACKETSIZE 	((KNBCOLS*KNBROWS)/KNBPACKETS)	// Packet size (for a 720 x 480 buffer)

static	volatile	uint8_t 	vCptImg = 0;
static	volatile	uint32_t 	vCptBlk = 0;
static	volatile	bool_t 		vImageReady = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"xfer_img     Test of a slave SPI transfer by DMA.      (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  xfer_img\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	void	_transferIMG(void);
static	void	_transferSPI(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - As fast as possible
 *  		- Transfer a memory block
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);
		
		iotx_printf(KSYST, "Images/s %d Nb of blocks %d\n", vCptImg, vCptBlk);
		vCptImg = 0;
	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - As fast as possible
 *  		- Transfer a memory block
 *
 */
static	void	aProcess_1(const void *argument) {
	int32_t		status;
	cnfImg0_t	configureIMG0;
	cnfXfer_t	configureXFER;
	uint8_t 	*image;
 
// Mode snap
// 8-Bits/pixels
// Use the external memory
// No configuration (the imager is in a default mode)
// Callback at the end of frame

	configureIMG0.oAcqMode	= KSNAP;
	configureIMG0.oImgCnf   = NULL;
	configureIMG0.oPixMode	= PIX8BITS;
	configureIMG0.oStRows	= 0;
	configureIMG0.oStCols	= 0;
	configureIMG0.oNbRows	= KNBROWS;
	configureIMG0.oNbCols	= KNBCOLS;
	configureIMG0.oAcqPage	= KEXTERNAL;
	configureIMG0.oKernSync = 0;
	configureIMG0.oFrame 	= NULL;
	configureIMG0.oVSync 	= NULL;
	configureIMG0.oHSync 	= NULL;
	configureIMG0.oDMAEc 	= _transferIMG;

	status = img0_configure(&configureIMG0);
	if (status != KIMG0NOERR) {
		iotx_printf(KSYST, "img0 manager problem %02X\n", (-status & 0xFF));
	}

// Transfer buffer = image buffer
// Size of the transfer
// Callback after the transfer of all the packets

	img0_getImage((void *)&image);

	configureXFER.oXferBuffer = image;
	configureXFER.oPacketSize = KPACKETSIZE;
	configureXFER.oNbPackets  = KNBPACKETS;
	configureXFER.oEndXfer	  = _transferSPI;

	status = xfer_configure(&configureXFER);
	if (status != KXFERNOERR) {
		iotx_printf(KSYST, "xfer manager problem %02X\n", (-status & 0xFF));
	}

// Start the first acquisition

	img0_acquisition();
	xfer_startXfer();

	while (TRUE) {
		while (!vImageReady) { kern_suspendProcess(1); }
		vImageReady = FALSE;

		img0_acquisition();
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

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KMEDIUMPRIORITY+2), 0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1),   0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS_OS);
}

// Local routines
// ==============

/*
 * \brief _transferIMG
 *
 * - Indicate that an image is ready
 *
 * - !!! This is an interrupt call-back function.
 *       Not all the system calls are allowed inside this portion of code.
 *
 */
static	void	_transferIMG(void) {

	vCptImg++;
}

/*
 * \brief _transferSPI
 *
 * - Indicate that all the packets were transfered
 * - Load the next image page
 *
 * - !!! This is an interrupt call-back function.
 *       Not all the system calls are allowed inside this portion of code.
 *
 */
static	void	_transferSPI(void) {
	uint8_t 	*image;

	img0_getImage((void *)&image);
	xfer_setXferBuffer(image);

	vCptBlk++;
	vImageReady = TRUE;
}

