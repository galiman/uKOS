/*
; viewer.
; =======

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 184                  $:  Revision of last commit
; $Date:: 2017-07-17 10:50:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Image viewer.
;			This tool allows to send an image via the xfer manager.
;			Usage:
;			- From the uKOS console launch the tool "viewer"
;			- IF Mac
;				1. Disconnect the RS232 terminal
;				2. In the UNIX terminal type:
;					sudo kextunload /Library/Extensions/FTDIUSBSerialDriver.kext/
;				3. On the host execute the application vipViewer.
;				4. In the UNIX terminal type:
;					sudo kextload /Library/Extensions/FTDIUSBSerialDriver.kext/
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

#if (defined(__MAN_IMG0__))
#define KNBCOLSMAX				752										// Number of cols max
#define KNBCOLSMIN				80										// Number of cols min
#define KNBROWSMAX				480										// Number of rows max
#define KNBROWSMIN				80										// Number of rows min
#define KNBCOLS					752										// Number of cols
#define KNBROWS					480										// Number of rows

#define KNBPACKETS 				80										// Number of packs (optimal value)
#define	GETIMAGE				img0_getImage							//
#define	ACQUISITION				img0_acquisition						//
#endif

#if (defined(__MAN_IMS2__))
#define KNBCOLSMAX				320										// Number of cols max
#define KNBCOLSMIN				80										// Number of cols min
#define KNBROWSMAX				240										// Number of rows max
#define KNBROWSMIN				80										// Number of rows min
#define KNBCOLS					320										// Number of cols
#define KNBROWS					240										// Number of rows
#define KNBPACKETS 				80										// Number of packs (optimal value)
#define	GETIMAGE				ims2_getImage							//
#define	ACQUISITION				ims2_acquisition						//

static	volatile	uint8_t		vMode = KLUMINANCE8;
#endif

static				uint8_t		vSize   = 1;
static				uint32_t 	vNbCols = KNBCOLSMAX;
static				uint32_t 	vNbRows = KNBROWSMAX;
static	volatile	bool_t 		vImageReady;
static	volatile	bool_t		vLock = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"viewer       Send images via the xfer manager.         (c) EFr-2017";

#if (defined(__MAN_IMG0__))
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This tool allows to display a img0 luminance image of 8-bits.\n"
									"viewer: no parameter  -> 752 x 480\n"
									"viewer:  2 parameters -> [W H]\n\n"

									"Input format:  viewer [W H]\n"
									"Output format: [result]\n\n";
#endif

#if (defined(__MAN_IMS2__))
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This tool allows to display a ims2 image with these formats:\n"
									"viewer: no parameter  -> 320 x 240, luminance 8-bits\n"
									"viewer:  1 parameter  -> 320 x 240, [-luminance -contrast -oricon], 8-bits\n"
									"viewer:  4 parameters -> [W H] [-luminance | -contrast | -oricon] [8 | 10]\n\n"

									"Input format:  viewer [W H] [-luminance | -contrast | -oricon] [8 | 10]\n"
									"Output format: [result]\n\n";
#endif

LOC_CONST_STRG(aStrIden_0[])      = "Process_Viewer";
LOC_CONST_STRG(aStrText_0[])      = "Process viewer.                           (c) EFr-2017";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_transferSPI(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Viewer, KIDTOOL, KVIEWERNUM, prgm, "1.1", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief _process
 *
 * - P1: - As fast as possible
 *  		- Transfer a memory block (image)
 *
 */
static	void	_process(const void *argument) {
	uint8_t 	*image;
	uint32_t	packetSize;
	int32_t		status;
	cnfXfer_t	configureXFER;

// Mode snap
// 8-Bits/pixels
// Use the external memory
// No configuration (the imager is in a default mode)

	#if (defined(__MAN_IMG0__))
	cnfImg0_t	configureIMG0;

	configureIMG0.oAcqMode	= KSNAP;
	configureIMG0.oImgCnf   = NULL;
	configureIMG0.oPixMode	= PIX8BITS;
	configureIMG0.oStRows	= (uint16_t)((KNBROWSMAX/2) - (vNbRows/2));
	configureIMG0.oStCols	= (uint16_t)((KNBCOLSMAX/2) - (vNbCols/2));
	configureIMG0.oNbRows	= (uint16_t)vNbRows;
	configureIMG0.oNbCols	= (uint16_t)vNbCols;
	configureIMG0.oAcqPage	= KEXTERNAL;
	configureIMG0.oKernSync = 0;
	configureIMG0.oHSync 	= NULL;
	configureIMG0.oFrame 	= NULL;
	configureIMG0.oVSync 	= NULL;
	configureIMG0.oDMAEc 	= NULL;

	status = img0_configure(&configureIMG0);
	if (status != KIMG0NOERR) {
		iotx_printf(KSYST, "img0 manager problem\n");
	}
	#endif

	#if (defined(__MAN_IMS2__))
	cnfIms2_t	configureIMS2;

	configureIMS2.oAcqMode	= KSNAP;
	configureIMS2.oPixMode	= vMode;
	configureIMS2.oStRows	= (uint16_t)((KNBROWSMAX/2) - (vNbRows/2));
	configureIMS2.oStCols	= (uint16_t)((KNBCOLSMAX/2) - (vNbCols/2));
	configureIMS2.oNbRows	= (uint16_t)vNbRows;
	configureIMS2.oNbCols	= (uint16_t)vNbCols;
	configureIMS2.oAcqPage	= KEXTERNAL;
	configureIMS2.oKernSync = 0;
	configureIMS2.oHSync 	= NULL;
	configureIMS2.oFrame 	= NULL;
	configureIMS2.oVSync 	= NULL;
	configureIMS2.oDMAEc 	= NULL;

	status = ims2_configure(&configureIMS2);
	if (status != KIMS2NOERR) {
		iotx_printf(KSYST, "ims2 manager problem\n");
	}
	#endif

// Transfer buffer = image buffer
// Size of the transfer
// Callback after the transfer of all the packets

	packetSize = ((vNbCols*vNbRows)/KNBPACKETS);

	GETIMAGE((void *)&image);
	configureXFER.oXferBuffer = image;
	configureXFER.oPacketSize = packetSize*vSize;
	configureXFER.oNbPackets  = KNBPACKETS;
	configureXFER.oEndXfer	  = _transferSPI;

	status = xfer_configure(&configureXFER);
	if (status != KXFERNOERR) {
		iotx_printf(KSYST, "xfer manager problem\n");
	}

// At the power-on the imager starts to send images.
// Just after the SNAP initialization it is necessary waiting for the end of the
// current transfer (~ 40-ms) before starting.

	kern_suspendProcess(40);
	vImageReady = FALSE;
	ACQUISITION();
	xfer_startXfer();

	while (TRUE) {
		while (!vImageReady) { kern_suspendProcess(1); }

		vImageReady = FALSE;
		kern_suspendProcess(10);
		ACQUISITION();
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
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
				int32_t		status;
				enum 		{ KERRNOT, KERRINA, KERRLOK } error = KERRNOT;
	volatile	proc_t 		*process_0;

	#if (defined(__MAN_IMG0__))

// Analyze the command line
// ------------------------
//
// Examples:
//
// Mode img0
//  viewer: no parameter  -> 752 x 480
//	viewer: 2  parameters -> [W H]

	switch (argc) {

// viewer: no parameter -> 752 x 480

		case 1: {
			break;
		}

// viewer: 2 parameters -> [W H]

		case 3: {
			strg_cnvtDecAsciiToBufInt32((int32_t *)&vNbCols, argv[1], 1);
			strg_cnvtDecAsciiToBufInt32((int32_t *)&vNbRows, argv[2], 1);
			break;
		}
		default: {
			error = KERRINA;
			break;
		}
	}
	#endif

	#if (defined(__MAN_IMS2__))
	bool_t	equals;

// Analyze the command line
// ------------------------
//
// Examples:
//
// Mode ims2
//	viewer: no parameter  -> 320 x 240, luminance 8-bits
//	viewer:  1 parameter  -> 320 x 240, [-luminance -contrast -oricon], 8-bits
//	viewer:  4 parameters -> [W H] [-luminance -contrast -oricon] [8 10]

	switch (argc) {

// viewer: no parameter -> 320 x 240, luminance 8-bits

		case 1: {
			break;
		}

// viewer: 1 parameter -> 320 x 240, [-luminance -contrast -oricon], 8-bits

		case 2: {
			text_checkAsciiBuffer(argv[1], "-luminance",  &equals); if (equals) { vMode = KLUMINANCE8;             break; }
			text_checkAsciiBuffer(argv[1], "-contrast",   &equals); if (equals) { vMode = KCONTRAST8;              break; }
			text_checkAsciiBuffer(argv[1], "-oricon",     &equals); if (equals) { vMode = KORICON8;                break; }
			error = KERRINA;
			break;
		}

// viewer: 4 parameters -> [W H] [-luminance -contrast -oricon] [8 10]

		case 5: {
			strg_cnvtDecAsciiToBufInt32((int32_t *)&vNbCols, argv[1], 1);
			strg_cnvtDecAsciiToBufInt32((int32_t *)&vNbRows, argv[2], 1);

			text_checkAsciiBuffer(argv[3], "-luminance",  &equals);
			if (equals) {
				text_checkAsciiBuffer(argv[4],  "8",      &equals); if (equals) { vMode = KLUMINANCE8;  vSize = 1; break; }
				text_checkAsciiBuffer(argv[4], "10",      &equals); if (equals) { vMode = KLUMINANCE10; vSize = 2; break; }
																	else        { error = KERRINA;                 break; }
			}
			text_checkAsciiBuffer(argv[3], "-contrast",   &equals);
			if (equals) {
				text_checkAsciiBuffer(argv[4],  "8",      &equals); if (equals) { vMode = KCONTRAST8;   vSize = 1; break; }
				text_checkAsciiBuffer(argv[4], "10",      &equals); if (equals) { vMode = KCONTRAST10;  vSize = 2; break; }
																	else        { error = KERRINA;                 break; }
			}
			text_checkAsciiBuffer(argv[3], "-oricon",     &equals);
			if (equals) {
				text_checkAsciiBuffer(argv[4],  "8",      &equals); if (equals) { vMode = KORICON8;     vSize = 1; break; }
				text_checkAsciiBuffer(argv[4], "10",      &equals); if (equals) { vMode = KORICON10;    vSize = 2; break; }
																	else        { error = KERRINA;                 break; }
			}
			error = KERRINA;
			break;
		}
		default: {
			error = KERRINA;
			break;
		}
	}
	#endif

	if (vLock) {
		error = KERRLOK;
	}

// Test the image size limits

	if (vNbCols < KNBCOLSMIN) { vNbCols = KNBCOLSMIN; }
	if (vNbCols > KNBCOLSMAX) { vNbCols = KNBCOLSMAX; }
	if (vNbRows < KNBROWSMIN) { vNbRows = KNBROWSMIN; }
	if (vNbRows > KNBROWSMAX) { vNbRows = KNBROWSMAX; }
	
// The image has to be modulo 8

	vNbCols = (vNbCols + 7) & 0xFFFFFFF8;
	vNbRows = (vNbRows + 7) & 0xFFFFFFF8;

	switch (error) {
		case KERRNOT: {
			vLock = TRUE;

// Launch the viewer

			iotx_printf(KSYST, "Viewer process launched.\n");
			iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");
			
			#if (defined(__MAN_IMG0__))
			iotx_printf(KSYST, "Image size sets @ W=%d x H=%d\n\n", vNbCols, vNbRows);
			#endif

			#if (defined(__MAN_IMS2__))
			if (vMode == KLUMINANCE8)  { iotx_printf(KSYST, "Mode luminance 8-bits, image size sets @ W=%d x H=%d\n\n",  vNbCols, vNbRows); }
			if (vMode == KLUMINANCE10) { iotx_printf(KSYST, "Mode luminance 10-bits, image size sets @ W=%d x H=%d\n\n", vNbCols, vNbRows); }
			if (vMode == KCONTRAST8)   { iotx_printf(KSYST, "Mode contrast 8-bits, image size sets @ W=%d x H=%d\n\n",   vNbCols, vNbRows); }
			if (vMode == KCONTRAST10)  { iotx_printf(KSYST, "Mode contrast 10-bits, image size sets @ W=%d x H=%d\n\n",  vNbCols, vNbRows); }
			if (vMode == KORICON8)     { iotx_printf(KSYST, "Mode oricon 8-bits, image size sets @ W=%d x H=%d\n\n",     vNbCols, vNbRows); }
			if (vMode == KORICON10)    { iotx_printf(KSYST, "Mode oricon 10-bits, image size sets @ W=%d x H=%d\n\n",    vNbCols, vNbRows); }
			#endif

			PROC_SUPV_STACKMALLOC(0, KINTERNAL, vSpecification_0, aStrText_0, KSZSTACKMIN, _process, aStrIden_0, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

			if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { status = EXIT_FAILURE;    break; }
			else      {															         status = EXIT_SUCCESS_OS; break; }
		}
		case KERRINA: { iotx_printf(KSYST, "Incorrect arguments.\n\n"); 		         status = EXIT_FAILURE;    break; }
		case KERRLOK: { iotx_printf(KSYST, "Viewer is running.\n\n");   		         status = EXIT_FAILURE;    break; }
		default:	  {												    		         status = EXIT_FAILURE;    break; }
	}
	return (status);
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

	GETIMAGE((void *)&image);
	xfer_setXferBuffer(image);

	vImageReady = TRUE;
}

