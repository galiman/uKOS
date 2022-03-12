/*
; xferSpi.
; ========

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
; Goal:		Dump a spi serial stream on a file. xferSpi is a simplified version of the dump.
;
; Usage:	xferSpi [driverID]
;			driverID	input	(ie. FT000001)
;
;			1. rm -rf /tmp/file_name
;			2. xferSpi FTWS6VG0D
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

#define KNBCOLS					752										// Number of cols
#define KNBROWS					480										// Number of rows
#define KNBPACKETS 				80										// Number of packs (optimal value)
#define KPACKETSIZE 			((KNBCOLS*KNBROWS)/KNBPACKETS)			// Packet size (for a 480 x 480 buffer)
#define	KMAXIMAGEREAD			(2000*KNBCOLS*KNBROWS)					// Nb of images to read before stopping

enum { KPIPE, KFILE };
enum { KDISPLAY, KNODISPLAY };
enum { KERRNOT, KERRAPP, KERRCLI };

static	uint8_t		vBuffer[KPACKETSIZE];

// Prototypes
// ==========

extern  void		spi_list(void);
extern	FT_HANDLE	spi_open(const char_t *driverID);
extern	void		spi_close(FT_HANDLE handle);
extern	bool_t		spi_read(FT_HANDLE handle, uint8_t *receive, uint32_t size, uint32_t *count);
extern	void		spi_flush(FT_HANDLE handle);

static	void		_swap(uint8_t *buffer, uint32_t count);
static	bool_t		_open(uint32_t type, char_t *fileName, FILE **fpFile, int32_t *fpPipe);
static	void		_close(uint32_t type, char_t *fileName, FILE *fpFile, int32_t fpPipe);
static	void		_write(uint32_t type, FILE *fpFile, int32_t fpPipe, uint8_t *buffer, uint32_t count);
static	int32_t		_app_spi(char_t *driverID, uint32_t type, uint32_t display, char_t *fileName);

/*
 * \brief Main entry point
 *
 */
int32_t	main(int32_t argc, char_t *argv[]) {
	char_t		*fileName, *driver;
	char_t		driverID[50];
	int32_t		error;
	uint32_t	type, display, baudrate;

	fprintf(stdout, "\nXfer data from the selected D2XX I/O port to the /tmp/pipe file using the spi\n");
	fprintf(stdout, __DATE__"  "__TIME__" (c) EFr-2017\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// xferSpi DCWZ1FL8

	error = KERRNOT;
	switch (argc) {
		case 2: {
			type = KFILE;										// Type file
			display = KNODISPLAY;								// No display
			strcpy(driverID, argv[1]);							// Driver ID (FT000001)
			fileName = "/tmp/pipe";								// Filename
			break;
		}
		default: {
			error = KERRCLI;
			break;
		}
	}

	if (error == KERRNOT)
		error = _app_spi(driverID, type, display, fileName);

	switch (error) {
		case KERRNOT: { fprintf(stdout, "Terminated\n");							 return (EXIT_SUCCESS); }
		case KERRAPP: { fprintf(stderr, "### Error: input/output file not found\n"); return (EXIT_FAILURE); }
		case KERRCLI: { fprintf(stderr, "xferSpi [driverID]\n");
						fprintf(stderr, "[driverID]  driverID (ie. FT000001)\n");
						spi_list();
 																					 return (EXIT_FAILURE); }
		default: {
			break;
		}
	}
}

// Local routines
// ==============

/*
 * \brief _app_spi
 *
 * - Open the driver & the file or pipe
 * - Fill the file
 * - Close the driver & the file or pipe
 *
 */
static	int32_t	_app_spi(char_t *driverID, uint32_t type, uint32_t display, char_t *fileName) {
	FT_HANDLE	handle;
	uint32_t	i, nbRead, nbPackets, nbImages, count;
    bool_t		error;
	FILE		*fpFile;
	int32_t		fpPipe;
	uint8_t		tmp;

	handle = spi_open(driverID);
	error  = _open(type, fileName, &fpFile, &fpPipe);
	if ((handle == NULL) || (!error)) {
		spi_close(handle);
		_close(type, fileName, fpFile, fpPipe);
		return (KERRAPP);
	}

// Fill the file

	spi_flush(handle);
	nbRead = 0; nbPackets = 0; nbImages = 0;

	while (TRUE) {
		if (!spi_read(handle, vBuffer, KPACKETSIZE, &count)) {
			break;
		}
		_swap(vBuffer, KPACKETSIZE);
		_write(type, fpFile, fpPipe, vBuffer, count);
		nbRead += count;
		if (display == KDISPLAY) {
			for (i = 0; i < count; i++) {
				fprintf(stdout, "%c", vBuffer[i]);
			}
		}
    	usleep(20);
		nbPackets++;
		if (nbPackets == KNBPACKETS) {
			nbPackets = 0;
			fprintf(stdout, "Image %d\n", ++nbImages);
		}
		if (nbRead > KMAXIMAGEREAD) {
			break;
		}
	}

	spi_close(handle);
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
			break;
		}
		case KPIPE: {
			write(fpPipe, buffer, count);
			break;
		}
	}
}

/*
 * \brief _swap
 *
 * - Swap the buffer (endianness)
 *
 */
static	void	_swap(uint8_t *buffer, uint32_t count) {
	uint8_t		tmp;
	uint32_t	i;

	for (i = 0; i < count/2; i++) {
		tmp = *(buffer + 2*i + 0); 
		*(buffer + 2*i + 0) = *(buffer + 2*i + 1);
		*(buffer + 2*i + 1) = tmp;
	}
}

