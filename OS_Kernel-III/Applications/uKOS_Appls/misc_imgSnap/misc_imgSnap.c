/*
; misc_imgSnap.
; =============

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
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;	              Display the imager serial number & the temperature 
;				  Toggle LED 0
;
;			- P1: Acquired & send the image
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

#if (defined(__Baphomet_407__))
#define	CHAN_WRITE	usb0_write

#define	KMYNBCOLS	320					// 320
#define	KMYNBROWS	240					// 240
#endif

#if (defined(__Baphomet_429__) || defined(__Baphomet_746__))
#define	CHAN_WRITE	urt2_write

#define	KMYNBCOLS	752					// 752
#define	KMYNBROWS	480					// 480
#endif

#if (defined(__CSEM_VIP_429__) || defined(__CSEM_VIP_746__))
#define	CHAN_WRITE	urt1_write

#define	KMYNBCOLS	752					// 752
#define	KMYNBROWS	480					// 480
#endif

static	volatile	bool_t	vImageReady;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_imgSnap Example of how to use an imager.          (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_imgSnap\n"
									"Output format: [result]\n\n";

// Prototypes
// ==========

static	void	_transfer(void);
static	void	_acquisition(void);
static	void	_getImage(volatile void **image);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 *  		- Display the imager serial number & the temperature 
 *  		- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {
	float64_t	temperature;
	uint64_t	serialNumber;

	while (TRUE) {
		kern_suspendProcess(1000);

		RESERVE(IMG0, KDEVALL);
		img0_getSerialNumber(&serialNumber);
		tmp0_getTemperature(&temperature);
		RELEASE(IMG0, KDEVALL);

		iotx_printf(KSYST,"Sn = %lX, Temperature = %d.%02d\n", serialNumber, FLOAT_2(temperature));

		#if (defined(__Baphomet_407__) || defined(__Baphomet_429__) || defined(__Baphomet_746__))
		misc_toggleLed(0);
		#endif

	}
}

/*
 * \brief aProcess 1
 *
 * - P1: - Acquire an image and send it
 *
 */
static	void	aProcess_1(const void *argument) {
	int32_t		status;
	cnfImg0_t	configureIMG0;
	void		*image;

// Configurations the communications channels

	#if (defined(__CSEM_VIP_429__) || defined(__CSEM_VIP_746__))
	cnfUrtx_t	configureURTx;

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBD3000000;
	configureURTx.oKernSync = (1<<BRXSEMA);
	urt1_configure(&configureURTx);
	#endif

	#if (defined(__Baphomet_429__) || defined(__Baphomet_746__))
	cnfUrtx_t	configureURTx;

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = KBD3000000;
	configureURTx.oKernSync = (1<<BRXSEMA);
	urt2_configure(&configureURTx);
	#endif

// Configurations for an imager APTINA 

	configureIMG0.oAcqMode	= KSNAP;
	configureIMG0.oImgCnf   = NULL;
	configureIMG0.oPixMode	= PIX8BITS;
	configureIMG0.oStRows	= 0;
	configureIMG0.oStCols	= 0;
	configureIMG0.oNbRows	= KMYNBROWS;
	configureIMG0.oNbCols	= KMYNBCOLS;
	configureIMG0.oAcqPage	= KEXTERNAL;
	configureIMG0.oKernSync = 0;
	configureIMG0.oHSync 	= NULL;
	configureIMG0.oFrame 	= NULL;
	configureIMG0.oVSync 	= _transfer;
	configureIMG0.oDMAEc 	= NULL;

	status = img0_configure(&configureIMG0);
	if (status != KIMG0NOERR) {
		iotx_printf(KSYST, "img0 manager problem %02X\n", (-status & 0xFF));
	}

// Turn on the LED

	img0_ctrlLeds(0x01);

// At the power-on the imager starts to send images.
// Just after the SNAP initialization it is necessary waiting for the end of the
// current transfer (~ 40-ms) before starting.

	kern_suspendProcess(40);
	vImageReady = FALSE;
	_acquisition();

	while (TRUE) {
		_getImage((void *)&image);

		CHAN_WRITE((const uint8_t *)image, KMYNBROWS*KMYNBCOLS);
		_acquisition();
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
	volatile	proc_t	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKMIN, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),    0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKMIN, aProcess_1, aStrIden_1, KDEF0, (KNORMALPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS);
}

// Local routines
// ==============

/*
 * \brief _transfer
 *
 * - Indicate that an image is ready
 *
 * - !!! This is an interrupt call-back function.
 *       Not all the system calls are allowed inside this portion of code.
 *
 */
static	void	_transfer(void) {

	#if (defined(__Baphomet_407__) || defined(__Baphomet_429__) || defined(__Baphomet_746__))
	misc_toggleLed(3);
	#endif

	vImageReady = TRUE;
}

/*
 * \brief _getImage
 *
 * - Get an image under timeout
 *
 */
static	void	_getImage(volatile void **image) {

	while (!vImageReady) { kern_suspendProcess(1); }
	vImageReady = FALSE;

	RESERVE(IMG0, KDEVALL);
	img0_getImage(image);
	RELEASE(IMG0, KDEVALL);
}

/*
 * \brief _acquisition
 *
 * - Acquisition of an image
 *
 */
static	void	_acquisition(void) {

	RESERVE(IMG0, KDEVALL);
	img0_acquisition();
	RELEASE(IMG0, KDEVALL);
}

