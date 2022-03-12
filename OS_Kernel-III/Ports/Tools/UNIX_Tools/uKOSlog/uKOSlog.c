/*
; uKOSlog.
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
; Goal:		Generate a log file. This is useful to supervise the uKernl
;			activity.
;
; Usage:	uKOSlog [driver] [driverID] [baudrate] [log_file]
;			driver		input   -vcp or -d2x
;			driver id	input	serial input device "FT000001"
;			baudrate	input	serial baudrate
;			log_file	output	the xyz.log file
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

enum { KVCP, KD2X };
enum { KERRNOT, KERRAPP, KERRCLI };

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

static	int32_t		_app_vcp(char_t *driverID, char_t *fileName, uint32_t baudrate);
static	int32_t		_app_d2x(char_t *driverID, char_t *fileName, uint32_t baudrate);
static	void		_log_vcp(int32_t handle, FILE *fpout);
static	void		_log_d2x(FT_HANDLE handle, FILE *fpout);

int32_t	main(int32_t argc, char_t *argv[]) {
	char_t		*fileName, *driver;
	char_t		driverID[50];
	int32_t		error;
	uint32_t	mode, baudrate;

	fprintf(stdout, __DATE__"  "__TIME__" (c) EFr-2017\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// uKOSlog -vcp DCWZ1FL8 115200 output.log
// uKOSlog -d2x DCWZ1FL8 460800 output.log

	error = KERRNOT;
	switch (argc) {
		case 5: {
			if      (strcmp(argv[1], "-vcp") == 0) { mode = KVCP;     }	// VCP driver
			else if (strcmp(argv[1], "-d2x") == 0) { mode = KD2X;     }	// d2xX driver
			else 								   { error = KERRCLI; }

			strcpy(driverID, argv[2]);									// Driver ID (FT000001)
			baudrate = (uint32_t)strtol(argv[3], NULL, 10);				// Baudrate (460800)
			fileName = argv[4];											// Filename
			break;
		}
		default: {
			error = KERRCLI;
			break;
		}
	}
	if (error == KERRNOT) {
		switch (mode) {
			case KVCP: {
				error = _app_vcp(driverID, fileName, baudrate);
				break;
			}
			case KD2X: {
				error = _app_d2x(driverID, fileName, baudrate);
				break;
			}
			default: {
				break;
			}
		}
	}

	switch (error) {
		case KERRNOT: { fprintf(stdout, "Terminated\n");													return (EXIT_SUCCESS); }
		case KERRAPP: { fprintf(stderr, "### Error: input/output file not found\n");						return (EXIT_FAILURE); }
		case KERRCLI: { fprintf(stderr, "%s %s %s %s %s\n\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
						fprintf(stderr, "uKOSlog [driver] [driverID] [baudrate] [file_name]\n");
						fprintf(stderr, "[driver]        -vcp\n");
						fprintf(stderr, "[driver]        -d2x\n");
						fprintf(stderr, "[driverID]      driverID (ie. FT000001)\n");
						fprintf(stderr, "[baudrate]      serial baudrate\n");
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
 * - Open the driver & the file
 * - Fill the file
 * - Close the driver & the file
 *
 */
static	int32_t	_app_vcp(char_t *driverID, char_t *fileName, uint32_t baudrate) {
	int32_t		handle;
	FILE		*fpout;

	handle = vcp_open(driverID, baudrate);
	fpout  = fopen(fileName, "w");
	if ((handle == 0) || (fpout == NULL)) {
		vcp_close(handle);
		fclose(fpout);
		return (KERRAPP);
	}

// Fill the file

	vcp_flush(handle);
	_log_vcp(handle, fpout);
	vcp_close(handle); fclose(fpout);
	return (KERRNOT);
}

/*
 * \brief _app_d2x
 *
 * - Open the driver & the file
 * - Fill the file
 * - Close the driver & the file
 *
 */
static	int32_t	_app_d2x(char_t *driverID, char_t *fileName, uint32_t baudrate) {
	FT_HANDLE	handle;
	FILE		*fpout;

	handle = d2x_open(driverID, baudrate);
	fpout  = fopen(fileName, "w");
	if ((handle == 0) || (fpout == NULL)) {
		d2x_close(handle);
		fclose(fpout);
		return (KERRAPP);
	}

// Fill the file

	d2x_flush(handle);
	_log_d2x(handle, fpout);
	d2x_close(handle);
	fclose(fpout);
	return (KERRNOT);
}

/*
 * \brief _log_vcp
 *
 * - Generate the log file
 *
 */
static	void	_log_vcp(int32_t handle, FILE *fpout) {
    uint8_t		byte;
	
	while (TRUE) {

//	32	Header

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	8	Process ID

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	8	Process priority

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	16	Reserve (Alignment)

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	32	Process Stack

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	32	Process State

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	64	Temporal tag us

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	64	Nb of executions of the process

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	64	Cumulated time used by the process

		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!vcp_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
	}
}

/*
 * \brief _log_d2x
 *
 * - Generate the log file
 *
 */
static	void	_log_d2x(FT_HANDLE handle, FILE *fpout) {
    uint8_t		byte;
	
	while (TRUE) {

//	32	Header

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	8	Process ID

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	8	Process priority

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	16	Reserve (Alignment)

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	32	Process Stack

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	32	Process State

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	64	Temporal tag us

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	64	Nb of executions of the process

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);

//	64	Cumulated time used by the proces

		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
		if (!d2x_readAByte(handle, &byte)) break;
		fwrite(&byte, 1, 1, fpout);
	}
}

