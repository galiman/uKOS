/*
; bin2xml.
; ========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:	Franzi Edo.	The 2011-01-03	Support of the ferroelectric memories.
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Generate an xml file. This is useful to create an xml file
;			for programming SPI serial EEPROMs/ferromagnetic memories
;			with the Aardvak probe.
;
; Usage:	bin2xml [behavior] [header] [address] [memory] [binary_file] [xml_file]
;			behavior	-erase			full erase the memory
;			behavior	-append			append without erasing the memory
;			header		-icycamheader	append the icycam header for the bootstrap
;			header		-noheader		without any header
;			address		 0xYXYXYXYX		relative address of the memory
;			memory		-E256K			256-Kbit EEPROM memory
;			memory		-E512K			512-Kbit EEPROM memory
;			memory		-E1024K			1024-Kbit EEPROM memory
;			memory		-F2048K			2048-Kbit ferroelectric memories
;			memory		-E8192K			8192-Kbit EEPROM memory
;			binary_file	input			any .bin files
;			xml_file	output			the generated .xml file
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
#include	<types_unix.h>

// Supported SPI EEPROM/ferroelectric memories
// -------------------------------------------

// EEPROM 25AA64

#define	KSZPAGE256				64										// Page of 64-Bytes
#define	KSIZE256				0x08000									// Size in Bytes
#define	KNBPAGES256				(KSIZE256/KSZPAGE256)					// Number of pages

// EEPROM 25AA512

#define	KSZPAGE512				128										// Page of 128-Bytes
#define	KSIZE512				0x10000									// Size in Bytes
#define	KNBPAGES512				(KSIZE512/KSZPAGE512)					// Number of pages

// EEPROM 25AA1024

#define	KSZPAGE1024				256										// Page of 256-Bytes
#define	KSIZE1024				0x20000									// Size in Bytes
#define	KNBPAGES1024			(KSIZE1024/KSZPAGE1024)					// Number of pages

// EEPROM 25AA8192

#define	KSZPAGE8192				256										// Page of 256-Bytes
#define	KSIZE8192				0x100000								// Size in Bytes
#define	KNBPAGES8192			(KSIZE8192/KSZPAGE8192)					// Number of pages

// ferroelectric memories 25H20

#define	KSZPAGE2048				256										// Page of 256-Bytes
#define	KSIZE2048				0x20000									// Size in Bytes
#define	KNBPAGES2048			(KSIZE2048/KSZPAGE2048)					// Number of pages

// icycam header

#define	KSZMHEADER				17										// Size of the header
#define	KSZBUFFER				(KSZMHEADER+KSIZE8192)					// Size of the buffer

// Soft switches
// -------------

enum	{ KERASE, KAPPEND };
enum	{ KICYCAMHEADER, KNOHEADER };
enum	{ KE256, KE512, KE1024, KE8192, KF2048 };

/*
 * \brief _printXML_header
 *
 * - Print the aardwark header
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[out]	-
 *
 */
void	_printXML_header(FILE *fpout) {

	fprintf(fpout, "<aardvark>\n");
	fprintf(fpout, "<configure i2c=\"1\" spi=\"1\" gpio=\"0\" tpower=\"0\" pullups=\"0\"/>\n");
	fprintf(fpout, "<spi_configure polarity=\"rising/falling\" phase=\"sample/setup\" bitorder=\"msb\" ss=\"active_low\"/>\n");
	fprintf(fpout, "<spi_bitrate khz=\"1000\"/>\n");
}

/*
 * \brief _printXML_footer
 *
 * - Print the aardwark footer
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[out]	-
 *
 */
void	_printXML_footer(FILE *fpout) {

	fprintf(fpout, "</aardvark>\n");
}

/*
 * \brief _printXML_E256
 *
 * - Print the aardwark file for the EEPROM 256K
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[in]	erase	If true erase the EEPROM
 * \param[in]	address	Relative address of the memory
 * \param[in]	nbBytes	Number of bytes to program
 * \param[in]	*buffer	Ptr on the programming buffer
 * \param[in]	header	Header mode {KICYCAMHEADER, KNOHEADER}
 * \param[out]	-
 *
 */
void	_printXML_E256(FILE *fpout, bool_t erase, uint32_t address, uint32_t nbBytes, char_t *buffer) {
			uint32_t	nLines, i = 0, j = 0;
			char_t		*prgmBuffer;
	const	char_t 		valToStr[16] = {
							'0', '1', '2', '3',
							'4', '5', '6', '7',
							'8', '9', 'A', 'B',
							'C', 'D', 'E', 'F'
						};

	nLines = (nbBytes+KSZPAGE256-1 ) / KSZPAGE256;
	if (nLines > KNBPAGES256) {
		nLines = KNBPAGES256;
	}

	prgmBuffer = buffer;
	for (i = 0; i < nLines; i++) {
		if (erase) {
			fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		}

		fprintf(fpout, "<spi_write count=\"67\" radix=\"16\">02 %2.2X %2.2X", (address>>8) & 0x000000FF, (address & 0x000000FF));
		for (j = 0; j < KSZPAGE256; j++) {
			fprintf(fpout, " ");
			fprintf(fpout, "%c%c", valToStr[(prgmBuffer[i*KSZPAGE256+j]>>4) & 0x0F], valToStr[prgmBuffer[i*KSZPAGE256+j] & 0x0F]);
		}
		address += KSZPAGE256;
		fprintf(fpout, "</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"15\"/>\n");
	}
}

/*
 * \brief _printXML_E512
 *
 * - Print the aardwark file for the EEPROM 512
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[in]	erase	If true erase the EEPROM
 * \param[in]	address	Relative address of the memory
 * \param[in]	nbBytes	Number of bytes to program
 * \param[in]	*buffer	Ptr on the programming buffer
 * \param[in]	header	Header mode {KICYCAMHEADER, KNOHEADER}
 * \param[out]	-
 *
 */
void	_printXML_E512(FILE *fpout, bool_t erase, uint32_t address, uint32_t nbBytes, char_t *buffer) {
			uint32_t	nLines, i = 0, j = 0;
			char_t		*prgmBuffer;
	const	char_t 		valToStr[16] = {
							'0', '1', '2', '3',
							'4', '5', '6', '7',
							'8', '9', 'A', 'B',
							'C', 'D', 'E', 'F'
						};

	nLines = (nbBytes+KSZPAGE512-1 ) / KSZPAGE512;
	if (nLines > KNBPAGES512) {
		nLines = KNBPAGES512;
	}

	prgmBuffer = buffer;
	if (erase) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">C7</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"20\"/>\n");
	}
	for (i = 0; i < nLines; i++) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"131\" radix=\"16\">02 %2.2X %2.2X", (address>>8) & 0x000000FF, (address & 0x000000FF));
		for (j = 0; j < KSZPAGE512; j++) {
			fprintf(fpout, " ");
			fprintf(fpout, "%c%c", valToStr[(prgmBuffer[i*KSZPAGE512+j]>>4) & 0x0F], valToStr[prgmBuffer[i*KSZPAGE512+j] & 0x0F]);
		}
		address += KSZPAGE512;
		fprintf(fpout, "</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"15\"/>\n");
	}
}

/*
 * \brief _printXML_E1024
 *
 * - Print the aardwark file for the EEPROM 1024
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[in]	erase	If true erase the EEPROM
 * \param[in]	address	Relative address of the memory
 * \param[in]	nbBytes	Number of bytes to program
 * \param[in]	*buffer	Ptr on the programming buffer
 * \param[in]	header	Header mode {KICYCAMHEADER, KNOHEADER}
 * \param[out]	-
 *
 */
void	_printXML_E1024(FILE *fpout, bool_t erase, uint32_t address, uint32_t nbBytes, char_t *buffer) {
			uint32_t	nLines, i = 0, j = 0;
			char_t		*prgmBuffer;
	const	char_t 		valToStr[16] = {
							'0', '1', '2', '3',
							'4', '5', '6', '7',
							'8', '9', 'A', 'B',
							'C', 'D', 'E', 'F'
						};

	nLines = (nbBytes+KSZPAGE1024-1 ) / KSZPAGE1024;
	if (nLines > KNBPAGES1024) {
		nLines = KNBPAGES1024;
	}
	prgmBuffer = buffer;
	if (erase) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">C7</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"20\"/>\n");
	}
	for (i = 0; i < nLines; i++) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"260\" radix=\"16\">02 %2.2X %2.2X %2.2X", (address>>16) & 0x000000FF, (address>>8) & 0x000000FF, (address & 0x000000FF));
		for (j = 0; j < KSZPAGE1024; j++) {
			fprintf(fpout, " ");
			fprintf(fpout, "%c%c", valToStr[(prgmBuffer[i*KSZPAGE1024+j]>>4) & 0x0F], valToStr[prgmBuffer[i*KSZPAGE1024+j] & 0x0F]);
		}
		address += KSZPAGE1024;
		fprintf(fpout, "</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"15\"/>\n");
	}
}

/*
 * \brief _printXML_E8192
 *
 * - Print the aardwark file for the EEPROM 8192
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[in]	erase	If true erase the EEPROM
 * \param[in]	address	Relative address of the memory
 * \param[in]	nbBytes	Number of bytes to program
 * \param[in]	*buffer	Ptr on the programming buffer
 * \param[in]	header	Header mode {KICYCAMHEADER, KNOHEADER}
 * \param[out]	-
 *
 */
void	_printXML_E8192(FILE *fpout, bool_t erase, uint32_t address, uint32_t nbBytes, char_t *buffer) {
			uint32_t	nLines, i = 0, j = 0;
			char_t		*prgmBuffer;
	const	char_t 		valToStr[16] = {
							'0', '1', '2', '3',
							'4', '5', '6', '7',
							'8', '9', 'A', 'B',
							'C', 'D', 'E', 'F'
						};

	nLines = (nbBytes+KSZPAGE8192-1 ) / KSZPAGE8192;
	if (nLines > KNBPAGES8192) {
		nLines = KNBPAGES8192;
	}
	prgmBuffer = buffer;
	if (erase) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">C7</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"20000\"/>\n");
	}
	for (i = 0; i < nLines; i++) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"260\" radix=\"16\">02 %2.2X %2.2X %2.2X", (address>>16) & 0x000000FF, (address>>8) & 0x000000FF, (address & 0x000000FF));
		for (j = 0; j < KSZPAGE8192; j++) {
			fprintf(fpout, " ");
			fprintf(fpout, "%c%c", valToStr[(prgmBuffer[i*KSZPAGE8192+j]>>4) & 0x0F], valToStr[prgmBuffer[i*KSZPAGE8192+j] & 0x0F]);
		}
		address += KSZPAGE8192;
		fprintf(fpout, "</spi_write>\n");
		fprintf(fpout, "<sleep ms=\"15\"/>\n");
	}
}

/*
 * \brief _printXML_F2048
 *
 * - Print the aardwark file for the ferroelectric memories 2048
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[in]	erase	If true erase the EEPROM
 * \param[in]	address	Relative address of the memory
 * \param[in]	nbBytes	Number of bytes to program
 * \param[in]	*buffer	Ptr on the programming buffer
 * \param[in]	header	Header mode {KICYCAMHEADER, KNOHEADER}
 * \param[out]	-
 *
 */
void	_printXML_F2048(FILE *fpout, bool_t erase, uint32_t address, uint32_t nbBytes, char_t *buffer) {
			uint32_t	nLines, i = 0, j = 0;
			char_t		*prgmBuffer;
	const	char_t 		valToStr[16] = {
							'0', '1', '2', '3',
							'4', '5', '6', '7',
							'8', '9', 'A', 'B',
							'C', 'D', 'E', 'F'
						};

	nLines = (nbBytes+KSZPAGE2048-1 ) / KSZPAGE2048;
	if (nLines > KNBPAGES2048) {
		nLines = KNBPAGES2048;
	}
	prgmBuffer = buffer;
	for (i = 0; i < nLines; i++) {
		fprintf(fpout, "<spi_write count=\"1\" radix=\"16\">06</spi_write>\n");
		fprintf(fpout, "<spi_write count=\"260\" radix=\"16\">02 %2.2X %2.2X %2.2X", (address>>16) & 0x000000FF, (address>>8) & 0x000000FF, (address & 0x000000FF));
		for (j = 0; j < KSZPAGE2048; j++) {
			fprintf(fpout, " ");
			fprintf(fpout, "%c%c", valToStr[(prgmBuffer[i*KSZPAGE2048+j]>>4) & 0x0F], valToStr[prgmBuffer[i*KSZPAGE2048+j] & 0x0F]);
		}
		address += KSZPAGE2048;
		fprintf(fpout, "</spi_write>\n");
	}
}

/*
 * \brief _generateXmlFile
 *
 * - Convert the binary to the xml file for some SPI EEPROMs
 *
 *   - behavior	-erase			Full erase the memory
 *   - behavior	-append			Append without erasing the memory
 *   - header	-icycamheader	Append the icycam header for the bootstrap
 *   - header	-noheader		Without any header
 *   - address	-0xYXYXYXYX		Relative address of the memory
 *   - memory	-E256K			256-Kbit EEPROM memory
 *   - memory	-E512K			512-Kbit EEPROM memory
 *   - memory	-E1024K			1024-Kbit EEPROM memory
 *   - memory	-F2048K			2048-Kbit ferroelectric memories
 *   - memory	-E8192K			8192-Kbit EEPROM memory
 *
 * \param[in]	behavior		Kind of behavior
 * \param[in]	header			icycam header for the bootstrap
 * \param[in]	address			Relative address of the memory
 * \param[in]	memory			EEPROM memory
 * \param[in]	*fpin			Ptr on the binary files
 * \param[in]	*fpout			Ptr on the xml file
 * \param[out]	-
 *
 */
void	_generateXmlFile(uint32_t behavior, uint32_t header, uint32_t address, uint32_t memory, FILE *fpin, FILE *fpout) {
	uint32_t	nbBytes = 0, nbWords = 0, i = 0;
    char_t		aByte, msbb, lsbb, msbw, lsbw, *buffer, *prgmBuffer;
    bool_t		erase = FALSE;

	buffer = (char_t *)malloc(KSZBUFFER);
	if (buffer == NULL) {
		exit(EXIT_FAILURE);
	}

// Read the file and prepare the icycam header for the DMA transfer
// ----------------------------------------------------------------

	for (i = 0; i < KSZBUFFER; i++) {
		buffer[i] = (char_t)0xFF;
	}

	i = 0;
	nbBytes = 0;
	while (fscanf(fpin, "%c", &aByte) == 1) {
		if (nbBytes < (KSZBUFFER - KSZMHEADER)) {
			buffer[KSZMHEADER + nbBytes++] = aByte;
		}
	}

	nbWords = (nbBytes>>2);
	msbb = (char_t)((nbBytes>>8) & 0x000000FF); lsbb = (char_t)(nbBytes & 0x000000FF);
	msbw = (char_t)((nbWords>>8) & 0x000000FF); lsbw = (char_t)(nbWords & 0x000000FF);

// The icycam header

	buffer[i++] = 0x00;									// Dummy (for 16-bit address devices)
	buffer[i++] = 0x00; buffer[i++] = 0x05;				// 0x0005 descriptor
	buffer[i++] = 0x02; buffer[i++] = 0x01;				// 0x0201 (8-bits EEPROM, 32-bits internal RAM)
	buffer[i++] = 0x00; buffer[i++] = 0x00;				//
	buffer[i++] = 0x00; buffer[i++] = 0x00;				// 0x00000000 Destination address
	buffer[i++] = msbb; buffer[i++] = lsbb;				// MSB - LSB (limited to 16-bits)
	buffer[i++] = msbw; buffer[i++] = lsbw;				// MSB - LSB (limited to 16-bits)
	buffer[i++] = 0x00; buffer[i++] = 0x00;				//
	buffer[i++] = 0x00; buffer[i++] = 0x04;				// Incremental factor = 1

// Generate the xml
// ----------------

	switch (memory) {

// For the EEPROM 256K

		case KE256: {
			switch (header) {
				case KICYCAMHEADER: {
					prgmBuffer = buffer;									// Consider the Dummy byte
					nbBytes = nbBytes + KSZMHEADER;							// FILE + Header icycam
					break;
				}
				case KNOHEADER:
					prgmBuffer = buffer + KSZMHEADER;						// Skip the Header icycam
					break;
			}

			if (behavior == KERASE) {
				erase = TRUE;												//
			}
			_printXML_header(fpout);										//
			_printXML_E256(fpout, erase, address, nbBytes, prgmBuffer);		//
			_printXML_footer(fpout);										// Generate for the 256K
			break;
		}

// For the EEPROM 512K

		case KE512: {
			switch (header) {
				case KICYCAMHEADER: {
					prgmBuffer = buffer;									// Consider the Dummy byte
					nbBytes = nbBytes + KSZMHEADER;							// FILE + Header icycam
					break;
				}
				case KNOHEADER:
					prgmBuffer = buffer + KSZMHEADER;						// Skip the Header icycam
					break;
			}

			if (behavior == KERASE) {
				erase = TRUE;												//
			}
			_printXML_header(fpout);										//
			_printXML_E512(fpout, erase, address, nbBytes, prgmBuffer);		//
			_printXML_footer(fpout);										// Generate for the 512K
			break;
		}

// For the EEPROM 1024K

		case KE1024: {
			switch (header) {
				case KICYCAMHEADER: {
					prgmBuffer = buffer + 1;								// Skip the Dummy byte (24-bits address device)
					nbBytes = nbBytes + KSZMHEADER - 1;						// FILE + Header icycam
					break;
				}
				case KNOHEADER:
					prgmBuffer = buffer + KSZMHEADER;						// Skip the Header icycam
					break;
			}

			if (behavior == KERASE) {
				erase = TRUE;												//
			}
			_printXML_header(fpout);										//
			_printXML_E1024(fpout, erase, address, nbBytes, prgmBuffer);	//
			_printXML_footer(fpout);										// Generate for the 1024K
			break;
		}

// For the EEPROM 8192K

		case KE8192: {
			switch (header) {
				case KICYCAMHEADER: {
					prgmBuffer = buffer + 1;								// Skip the Dummy byte (24-bits address device)
					nbBytes = nbBytes + KSZMHEADER - 1;						// FILE + Header icycam
					break;
				}
				case KNOHEADER:
					prgmBuffer = buffer + KSZMHEADER;						// Skip the Header icycam
					break;
			}

			if (behavior == KERASE) {
				erase = TRUE;												//
			}
			_printXML_header(fpout);										//
			_printXML_E8192(fpout, erase, address, nbBytes, prgmBuffer);	//
			_printXML_footer(fpout);										// Generate for the 8192K
			break;
		}

// For the ferroelectric memories 2048K

		case KF2048: {
			switch (header) {
				case KICYCAMHEADER: {
					prgmBuffer = buffer + 1;								// Skip the Dummy byte (24-bits address device)
					nbBytes = nbBytes + KSZMHEADER - 1;						// FILE + Header icycam
					break;
				}
				case KNOHEADER:
					prgmBuffer = buffer + KSZMHEADER;						// Skip the Header icycam
					break;
			}

			if (behavior == KERASE) {
				erase = TRUE;												//
			}
			_printXML_header(fpout);										//
			_printXML_F2048(fpout, erase, address, nbBytes, prgmBuffer);	//
			_printXML_footer(fpout);										// Generate for the 2048K
			break;
		}
		default: {
			break;
		}
	}

	free(buffer);
}

int32_t	main(int32_t argc, char_t *argv[]) {
	char_t		*inputFileName, *outputFileName;
	FILE		*fpin, *fpout;
	uint32_t	behavior, header, address, memory;
	bool_t		error = FALSE;

	fprintf(stdout, __DATE__"  "__TIME__" (c) EFr-2017\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// bin2xml -erase -icycamheader -address -8192K input.bin output.xml

	switch (argc) {
		case 7: {
			if      (strcmp(argv[1], "-erase")        == 0) behavior = KERASE;
			else if (strcmp(argv[1], "-append")       == 0) behavior = KAPPEND;
			else error = TRUE;

			if      (strcmp(argv[2], "-icycamheader") == 0) header    = KICYCAMHEADER;
			else if (strcmp(argv[2], "-noheader")     == 0) header    = KNOHEADER;
			else error = TRUE;

			if      (strcmp(argv[4], "-E256K") 		  == 0) memory    = KE256;
			else if (strcmp(argv[4], "-E512K") 		  == 0) memory    = KE512;
			else if (strcmp(argv[4], "-E1024K") 	  == 0) memory    = KE1024;
			else if (strcmp(argv[4], "-E8192K") 	  == 0) memory    = KE8192;
			else if (strcmp(argv[4], "-F2048K") 	  == 0) memory    = KF2048;
			else error = TRUE;

			address = (uint32_t)strtol(argv[3], NULL, 16);

			inputFileName  = argv[5];
			outputFileName = argv[6];
			break;
		}
		default:
			error = TRUE;
			break;
	}

// Error (data entry not correct)

	if (error) {
		fprintf(stderr, "%s %s %s %s %s %s\n\n", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
		fprintf(stderr, "\nUSAGE: bin2xml [behavior] [header] [memory] [binary_file] [xml_file]\n");
		fprintf(stderr, "[behavior]  -erase, -append\n");
		fprintf(stderr, "[header]    -icycamheader, -noheader\n");
		fprintf(stderr, "[address]    0xyyyyxxxx\n");
		fprintf(stderr, "[memory]    -E256K, -E512K, -E1024K, -E8192K, -F2048K\n");
		exit(EXIT_FAILURE);
	}

// Open the binary & the xml files
// -------------------------------

	fpin  = fopen(inputFileName,  "r");
	fpout = fopen(outputFileName, "w");

	if ((fpin == NULL) || (fpout == NULL)) {
		fclose(fpin); fclose(fpout);
		fprintf(stderr, "### Error: input/output file not found\n");
		exit(EXIT_FAILURE);
	}

// Generate the xml file for the selected memory
// ---------------------------------------------

	_generateXmlFile(behavior, header, address, memory, fpin, fpout);
	fclose(fpin);
	fclose(fpout);
	fprintf(stdout, "xml file generated\n");
	return (EXIT_SUCCESS);
}

