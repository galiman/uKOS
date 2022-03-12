/*
; bin2cksum.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2012-02-11
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Generate an file containing the checksum information of a bin file.
;
; Usage:	bin2cksum [binary_file] [cksum_file]
;			binary_file	input			any .bin files
;			cksum_file	output			the generated .ck file
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
#include	<cpu_reg.h>

#define	KLNTEXTBLKICY			0x8000									// The checksum for the icyflex cpu (only RAM system) is 
																		// arbitrary limited to 32-K (see syos manager)

/*
 * \brief _computeCkSumm
 *
 * - Compute the checksum of the binary (uKOS signature)
 *
 * \param[in]	fpin	Ptr on the binary file
 * \param[in]	fpout	Ptr on the checksum file
 * \param[in]	skip	Skip the icyflex1 exceptions
 * \param[out]	-
 *
 */
void	_computeCkSumm(FILE *fpin, FILE *fpout, bool_t skip) {
	uint32_t	length = 0, cksumm = 0;
    uint8_t		aByte;

	while (fscanf(fpin, "%c", &aByte) == 1) {
		if ((skip && (length < KSZEXCEPTIONS)) || (skip && (length >= KLNTEXTBLKICY))) {
			length++;
		}
		else {
			cksumm += (uint32_t)aByte;
			length++;
		}
	}

// The header
// ----------

	fprintf(fpout, "/*\n");
	fprintf(fpout, "; EPROM.ck.\n");
	fprintf(fpout, "; =========\n\n");

	fprintf(fpout, ";------------------------------------------------------------------------\n");
	fprintf(fpout, "; Author:	Franzi Edo.	The 2013-02-11\n");
	fprintf(fpout, "; Modifs:\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, "; SVN:\n");
	fprintf(fpout, "; $Author:: efr               $:  Author of last commit\n");
	fprintf(fpout, "; $Rev:: 166                  $:  Revision of last commit\n");
	fprintf(fpout, "; $Date:: 2017-07-02 17:51:46#$:  Date of last commit\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, "; Project:	uKOS\n");
	fprintf(fpout, "; Goal:		Cksumm of the uKOS OS (EPROM.bin)\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, ";   (c) 1992-2017, Franzi Edo.\n");
	fprintf(fpout, ";   --------------------------\n");
	fprintf(fpout, ";                                              __ ______  _____\n");
	fprintf(fpout, ";   Franzi Edo.                         __  __/ //_/ __ \\/ ___/\n");
	fprintf(fpout, ";   5-Route de Cheseaux                / / / / ,< / / / /\\__ \\\n");
	fprintf(fpout, ";   CH 1400 Cheseaux-Noreaz           / /_/ / /| / /_/ /___/ /\n");
	fprintf(fpout, ";                                     \\__,_/_/ |_\\____//____/\n");
	fprintf(fpout, ";   edo.franzi@ukos.ch\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, ";   This program is free software: you can redistribute it and/or modify\n");
	fprintf(fpout, ";   it under the terms of the GNU Affero General Public License as published by\n");
	fprintf(fpout, ";   the Free Software Foundation, either version 3 of the License, or\n");
	fprintf(fpout, ";   (at your option) any later version.\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, ";   This program is distributed in the hope that it will be useful,\n");
	fprintf(fpout, ";   but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	fprintf(fpout, ";   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n");
	fprintf(fpout, ";   GNU Affero General Public License for more details.\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, ";   You should have received a copy of the GNU Affero General Public License\n");
	fprintf(fpout, ";   along with this program. If not, see <http://www.gnu.org/licenses/>.\n");
	fprintf(fpout, ";\n");
	fprintf(fpout, ";------------------------------------------------------------------------\n");
	fprintf(fpout, "*/\n\n");

	fprintf(fpout, "#ifndef	__OS_CKSUM__\n");
	fprintf(fpout, "#define	__OS_CKSUM__\n\n");

	fprintf(fpout, "#define	KOSCKSUMM		%d				// uKOS OS checksum\n", cksumm);
	fprintf(fpout, "#define	KOSLENGTH		%d				// uKOS OS length\n", length);
	fprintf(fpout, "#endif\n");
}

int32_t	main(int32_t argc, char_t *argv[]) {
	char_t		*inputFileName, *outputFileName;
	FILE		*fpin, *fpout;
	bool_t		error = FALSE, skip = FALSE;

	fprintf(stdout, __DATE__"  "__TIME__" (c) EFr-2017\n");

// Analyze the command line
// ------------------------
//
// Example:
//
// computeCkSumm input.bin output.h
// computeCkSumm -icyflex1 input.bin output.h

	switch (argc) {
		case 3: {
			inputFileName  = argv[1];
			outputFileName = argv[2];
			break;
		}
		case 4: {
			inputFileName  = argv[2];
			outputFileName = argv[3];
			(strcmp(argv[1], "-icyflex1") == 0) ? (skip = TRUE) : (error = TRUE);
			break;
		}
		default: {
			error = TRUE;
			break;
		}
	}

// Error (data entry not correct)

	if (error) {
		fprintf(stderr, "\nUSAGE: bin2cksum {-icyflex1} [binary_file] [cksum_file]\n");
		exit(EXIT_FAILURE);
	}

// Open the binary & the h files
// -----------------------------

	fpin  = fopen(inputFileName,  "r");
	fpout = fopen(outputFileName, "w");

	if ((fpin == NULL) || (fpout == NULL)) {
		fclose(fpin); fclose(fpout);
		fprintf(stderr, "### Error: input/output file not found\n");
		exit(EXIT_FAILURE);
	}

// Compute the checksum
// --------------------

	_computeCkSumm(fpin, fpout, skip);
	fclose(fpin);
	fclose(fpout);
	fprintf(stdout, "h file generated\n");
	return (EXIT_SUCCESS);
}

