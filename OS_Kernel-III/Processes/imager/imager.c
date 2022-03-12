/*
; imager.
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
; Goal:		imager process; continuous acquisition of an image.
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
#include 	<MT9V03x/MT9V03x.h>

#define	KTIMEACQ	100

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"imager       imager process: image acquisition.        (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "imager process\n"
									"==============\n\n"

									"Acquisition of an image\n\n";

LOC_CONST_STRG(aStrIden[])        = "Process_synchro";
LOC_CONST_STRG(aStrText[])        = "Process synchro: acquisition.             (c) EFr-2017";

// Image 328 x 248
// Fixed exposure
//
static	const	mt9v03x_t	aTab328248F[] = {
								{ 1,	0x00D9 }, { 2,   0x007D }, { 3,	  0x00F8 }, { 4,   0x0149 }, { 5,   0x016B }, { 6,   0x0032 }, { 7,	  0x0388 }, { 8,   0x01E0 },
								{ 9,	0x01E0 }, { 10,  0x0164 }, { 11,  0x0007 }, { 16,  0x002D }, { 21,  0x8032 }, { 32,	 0x03D5 }, { 33,  0x0012 }, { 34,  0x0028 },
								{ 40,	0x0016 }, { 41,  0x0018 }, { 43,  0x0003 }, { 44,  0x0006 }, { 46,  0x0004 }, { 47,	 0x0003 }, { 70,  0x1606 }, { 71,  0x8081 },
								{ 72,	0x007F }, { 104, 0xE5F9 }, { 105, 0x65D7 }, { 106, 0x2950 }, { 107, 0xA040 }, { 112, 0x0032 }, { 114, 0x0001 }, { 115, 0x0307 },
								{ 116,	0x0000 }, { 128, 0x00F4 }, { 129, 0x0004 }, { 130, 0x0004 }, { 131,	0x0004 }, { 132, 0x0004 }, { 133, 0x0004 }, { 134, 0x0004 },
								{ 135,	0x0004 }, { 136, 0x0004 }, { 137, 0x0004 }, { 138, 0x0004 }, { 139,	0x0004 }, { 140, 0x0004 }, { 141, 0x0004 }, { 142, 0x0004 },
								{ 143,	0x0004 }, { 144, 0x0004 }, { 145, 0x0004 }, { 146, 0x0004 }, { 147,	0x0004 }, { 148, 0x0004 }, { 149, 0x0004 }, { 150, 0x0004 },
								{ 151,	0x0004 }, { 152, 0x0004 }, { 154, 0x013F }, { 155, 0x02F0 }, { 156,	0x02F0 }, { 157, 0x02F0 }, { 160, 0x00EF }, { 161, 0x01E0 },
								{ 162,	0x01E0 }, { 163, 0x01E0 }, { 165, 0x0020 }, { 166, 0x0008 }, { 168,	0x0001 }, { 169, 0x0008 }, { 171, 0x0001 }, { 175, 0x0001 },
								{ 176,	0x2400 }, { 189, 0x0120 }, { 194, 0x0850 }, { 0,   0x0000 }
							};

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_process(const void *argument);
static	void	_transfer(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Imager, KIDPROCESS, KIMAGERNUM, prgm, "1.0", (1<<BSHOW));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	volatile	proc_t 	*process;

	PROC_SUPV(0, vSpecification, aStrText, KSZSTACKMIN, _process, aStrIden, KDEF0, (KMEDIUMPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification, NULL, &process) != KKERNNOERR) { exit(EXIT_PANIC); }
	return (EXIT_SUCCESS_OS);
}

/*
 * \brief _process
 *
 * - Image acquisitions (10-Hz)
 *
 */
static	void	_process(const void *argument) {
				int32_t		status;
				cnfImg0_t	configureIMG0;
	volatile	void		*image;

// Waiting for 20s before starting

	kern_suspendProcess(20000);

	configureIMG0.oAcqMode	= KSNAP;
	configureIMG0.oPixMode 	= PIX8BITS;
	configureIMG0.oStRows	= 0;
	configureIMG0.oStCols	= 0;
	configureIMG0.oNbRows	= KNBROWSQVGA;
	configureIMG0.oNbCols	= KNBCOLSQVGA;
	configureIMG0.oAcqPage	= KINTERNAL;
	configureIMG0.oKernSync = 0;
	configureIMG0.oImgCnf   = aTab328248F;
	configureIMG0.oHSync 	= NULL;
	configureIMG0.oFrame 	= NULL;
	configureIMG0.oVSync	= _transfer;
	configureIMG0.oDMAEc 	= NULL;

	status = img0_configure(&configureIMG0);
	if (status != KIMG0NOERR) {
		exit(EXIT_PANIC);
	}

// Turn on the LED and start with the acquisitions

	img0_getImage(&image);
	img0_ctrlLeds(0b00000001);
	img0_acquisition();

	while (TRUE) {
		kern_suspendProcess(KTIMEACQ);
		usb0_write((uint8_t *)(image), KNBROWSQVGA*KNBCOLSQVGA);
		img0_acquisition();
	}	
}

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

		misc_toggleLed(3);
}

