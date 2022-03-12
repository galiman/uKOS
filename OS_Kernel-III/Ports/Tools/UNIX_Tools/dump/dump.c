/*
; dump.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2014-01-12
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Dump a serial stream on a file.
;
; Usage:	dump [driverID]
;			or
; 			dump [driver] [driverID] [baudrate] [display] [type] [file_name]
;			driver		input	-vcp | -d2x
;			driverID 	input	(ie. usbserial-FT000001) for the vcp driver\n");
;			driverID    input	(ie. FT000001)           for the d2x driver\n");
;			baudrate	input	serial baudrate
;			display		input	-display | -nodisplay
;			type		input	-pipe | -file
;			file_name	output	the file_name
;
;			1. rm -rf /tmp/file_name
;			2. dump -vcp usbserial-FT000001 460800 -display -file /tmp/file_name
;			   dump -d2x FTWS6VG0D 460800 -nodisplay -pipe /tmp/file_name
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

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include 	<unistd.h>
#include 	<fcntl.h>
#include 	<sys/stat.h>
#include	<types_unix.h>
#include	<ftd2xx.h>

#define	KMAXNBREAD				100000000								// 100'000'000 bytes before stopping

enum { KVCP, KD2X };
enum { KPIPE, KFILE };
enum { KDISPLAY, KNODISPLAY };
enum { KERRNOT, KERRAPP, KERRCLI };

static	uint8_t		vBuffer[100000];

// Prototypes
// ==========

extern  void		vcp_list(void);
extern	int32_t		vcp_open(const char_t *driverID, int32_t baudrate);
extern	void		vcp_close(int32_t handle);
extern	bool_t		vcp_readAByte(int32_t handle, uint8_t *receive);
extern	bool_t		vcp_read(int32_t handle, uint8_t *receive, uint32_t *count);
extern	void		vcp_flush(int32_t handle);

extern  void		d2x_list(void);
extern	FT_HANDLE	d2x_open(const char_t *driverID, int32_t baudrate);
extern	void		d2x_close(FT_HANDLE handle);
extern	bool_t		d2x_readAByte(FT_HANDLE handle, uint8_t *receive);
extern	bool_t		d2x_read(FT_HANDLE handle, uint8_t *receive, uint32_t *count);
extern	void		d2x_flush(FT_HANDLE handle);

static	bool_t		_open(uint32_t type, char_t *fileName, FILE **fpFile, int32_t *fpPipe);
static	void		_close(uint32_t type, char_t *fileName, FILE *fpFile, int32_t fpPipe);
static	void		_write(uint32_t type, FILE *fpFile, int32_t fpPipe, uint8_t *buffer, uint32_t count);
static	int32_t		_app_vcp(char_t *driverID, uint32_t type, uint32_t display, char_t *fileName, uint32_t baudrate);
static	int32_t		_app_d2x(char_t *driverID, uint32_t type, uint32_t display, char_t *fileName, uint32_t baudrate);

/*
 * \brief Main entry point
 *
 */
int32_t	main(int32_t argc, char_t *argv[]) {
	char_t		*fileName, *driver;
	char_t		driverID[50];
	int32_t		error;
	uint32_t	mode, type, display, baudrate;

	fprintf(stdout, "\nXfer data from the selected D2XX/vcp I/O port to the /tmp/pipe file/pipe\n");
	fprintf(stdout, __DATE__"  "__TIME__" (c) EFr-2017\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// dump DCWZ1FL8
// dump -vcp DCWZ1FL8 115200 -display -file /tmp/file_name
// dump -d2x DCWZ1FL8 460800 -nodisplay -pipe /tmp/file_name

	error = KERRNOT;
	switch (argc) {
		case 2: {
			mode = KD2X;															// D2XX driver
			strcpy(driverID, argv[1]);												// Driver ID (FT000001)
			baudrate = 460800;														// Baudrate (460800)
			display = KDISPLAY;														// Display
			type = KFILE;															// Type file
			fileName = "/tmp/pipe";													// Filename
			break;
		}
		case 7: {
			if      (strcmp(argv[1], "-vcp")       == 0) { mode = KVCP;          }	// VCP driver
			else if (strcmp(argv[1], "-d2x")       == 0) { mode = KD2X;          }	// D2XX driver
			else                                         { error = KERRCLI; 	 }

			if      (strcmp(argv[5], "-pipe")      == 0) { type = KPIPE;         }	// Type pipe
			else if (strcmp(argv[5], "-file")      == 0) { type = KFILE;         }	// Type file
			else                                         { error = KERRCLI; 	 }

			if      (strcmp(argv[4], "-display")   == 0) { display = KDISPLAY;	 }	// Display
			else if (strcmp(argv[4], "-nodisplay") == 0) { display = KNODISPLAY; }	// No display
			else                                         { error = KERRCLI; 	 }

			strcpy(driverID, argv[2]);												// Driver ID (FT000001)
			baudrate = (uint32_t)strtol(argv[3], NULL, 10);							// Baudrate (460800)
			fileName = argv[6];														// Filename
			break;
		}
		default: {
			error = KERRCLI;
			break;
		}
	}

	if (error == KERRNOT) {
		switch (mode) {
			case KVCP: { error = _app_vcp(driverID, type, display, fileName, baudrate); break; }
			case KD2X: { error = _app_d2x(driverID, type, display, fileName, baudrate); break; }
			default: {
				break;
			}
		}
	}

	switch (error) {
		case KERRNOT: { fprintf(stdout, "Terminated\n");															return (EXIT_SUCCESS); }
		case KERRAPP: { fprintf(stderr, "### Error: input/output file not found\n");								return (EXIT_FAILURE); }
		case KERRCLI: { fprintf(stderr, "dump [driverID]\n");
						fprintf(stderr, "or\n");
						fprintf(stderr, "dump [driver] [driverID] [baudrate] [display] [type] [file_name]\n");
						fprintf(stderr, "[driver]        -vcp\n");
						fprintf(stderr, "[driver]        -d2x\n");
						fprintf(stderr, "[driverID]      driverID (ie. usbserial-FT000001) for the vcp driver\n");
						fprintf(stderr, "[driverID]      driverID (ie. FT000001)           for the d2x driver\n");
						fprintf(stderr, "[baudrate]      serial baudrate\n");
						fprintf(stderr, "[display]       -display\n");
						fprintf(stderr, "[display]       -nodisplay\n");
						fprintf(stderr, "[type]          -pipe\n");
						fprintf(stderr, "[type]          -file\n");
						fprintf(stderr, "[file_name]     the file_name file\n\n");
						d2x_list();
 																													return (EXIT_FAILURE); }
		default: {
			break;
		}
	}
}

// Local routines
// ==============

/*
 * \brief _app_vcp
 *
 * - Open the driver & the file or pipe
 * - Fill the file
 * - Close the driver & the file or pipe
 *
 */
static	int32_t	_app_vcp(char_t *driverID, uint32_t type, uint32_t display, char_t *fileName, uint32_t baudrate) {
	int32_t		handle;
	uint32_t	i, nbRead, count;
    bool_t		error;
	FILE		*fpFile;
	int32_t		fpPipe;

	handle = vcp_open(driverID, baudrate);
	error  = _open(type, fileName, &fpFile, &fpPipe);
	if ((handle == 0) || (!error)) {
		vcp_close(handle);
		_close(type, fileName, fpFile, fpPipe);
		return (KERRAPP);
	}

// Fill the file

	vcp_flush(handle);
	nbRead = 0;

	while (TRUE) {
		if (!vcp_read(handle, vBuffer, &count)) {
			break;
		}
		_write(type, fpFile, fpPipe, vBuffer, count);
		nbRead += count;
		if (display == KDISPLAY) {
			for (i = 0; i < count; i++) {
				fprintf(stdout, "%c", vBuffer[i]);
			}
		}
		if (nbRead > KMAXNBREAD) {
			break;
		}
	}

	vcp_flush(handle);
	vcp_close(handle);
	_close(type, fileName, fpFile, fpPipe);
	return (KERRNOT);
}

/*
 * \brief _app_d2x
 *
 * - Open the driver & the file or pipe
 * - Fill the file
 * - Close the driver & the file or pipe
 *
 */
static	int32_t	_app_d2x(char_t *driverID, uint32_t type, uint32_t display, char_t *fileName, uint32_t baudrate) {
	FT_HANDLE	handle;
	uint32_t	i, nbRead, count;
    bool_t		error;
	FILE		*fpFile;
	int32_t		fpPipe;

	handle = d2x_open(driverID, baudrate);
	error  = _open(type, fileName, &fpFile, &fpPipe);
	if ((handle == NULL) || (!error)) {
		d2x_close(handle);
		_close(type, fileName, fpFile, fpPipe);
		return (KERRAPP);
	}

// Fill the file

	d2x_flush(handle);
	nbRead = 0;

	while (TRUE) {
		if (!d2x_read(handle, vBuffer, &count)) {
			break;
		}
		_write(type, fpFile, fpPipe, vBuffer, count);
		nbRead += count;
		if (display == KDISPLAY) {
			for (i = 0; i < count; i++) {
				fprintf(stdout, "%c", vBuffer[i]);
			}
		}
		if (nbRead > KMAXNBREAD) {
			break;
		}
	}

	d2x_flush(handle);
	d2x_close(handle);
	_close(type, fileName, fpFile, fpPipe);
	return (KERRNOT);
}

/*
 * \brief _open
 *
 * - Open a file or a pipe
 *
 */
static	bool_t	_open(uint32_t type, char_t *fileName, FILE **fpFile, int32_t *fpPipe) {

	switch (type) {
		default:
		case KFILE: {
			*fpFile = fopen(fileName, "w");
			return (*fpFile == 0) ? (FALSE) : (TRUE);
		}
		case KPIPE: {
			mkfifo(fileName, 0666);
			*fpPipe = open(fileName, O_WRONLY);
			return (*fpPipe == 0) ? (FALSE) : (TRUE);
		}
	}
}

/*
 * \brief _close
 *
 * - Close a file or a pipe
 *
 */
static	void	_close(uint32_t type, char_t *fileName, FILE *fpFile, int32_t fpPipe) {

	switch (type) {
		default:
		case KFILE: {
			fclose(fpFile);
			break;
		}
		case KPIPE: {
			unlink(fileName);
			close(fpPipe);
			break;
		}
	}
}

/*
 * \brief _write
 *
 * - Write a file or a pipe
 *
 */
static	void	_write(uint32_t type, FILE *fpFile, int32_t fpPipe, uint8_t *buffer, uint32_t count) {

	switch (type) {
		default:
		case KFILE: {
			fwrite(buffer, 1, (size_t)count, fpFile);
			fflush(fpFile);
			break;
		}
		case KPIPE: {
			write(fpPipe, buffer, count);
			break;
		}
	}
}

