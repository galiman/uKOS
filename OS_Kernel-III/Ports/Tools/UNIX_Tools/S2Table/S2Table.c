/*
; S2Table.
; ========

;------------------------------------------------------------------------
; Author:	Franzi Edo.		The 2006-06-28
; Modifs:	Türetken Engin	The 2016-07-20
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Generate an S2 table.
;
; Usage:	S2Table [input_table] [output_table]
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

/*
 * \brief _generateSFile
 *
 * - Create an S2 table
 * - Format:
 *
 *        Dt           VREF
 *  31 ------- 11 10 ------- 0
 *
 * in: 0x614766
 * out: (0xC28*K)<<11 + 0x766
 *
 * \param[in]	fpin	Ptr on the binary file
 * \param[in]	fpout	Ptr on the checksum file
 * \param[out]	-
 *
 */
static	void	_generateS2Table(FILE *fpin, FILE *fpout) {
	uint32_t	aValue, time = 0, i = 0, j = 0;

	fprintf(fpout, "#define	K			1\n\n");

	fprintf(fpout, "const	uint32_t	aTabLUTDay[1024] = {\n");

	while (fscanf(fpin, "%X,", &aValue) == 1) {
		time += aValue>>11;
		if (i++ == 0)        { fprintf(fpout, "\t\t\t\t\t\t");                                                       }
		if (i < 8)           { fprintf(fpout, "(((0X%04X>>K)<<11) + 0X%04X), ",  aValue>>11, aValue & 0x7FF);        }
		else if (j++ < 1024) { fprintf(fpout, "(((0X%04X>>K)<<11) + 0X%04X),\n", aValue>>11, aValue & 0x7FF); i = 0; }
		     else            { fprintf(fpout, "(((0X%04X>>K)<<11) + 0X%04X)\n",  aValue>>11, aValue & 0x7FF);        }
	}
	fprintf(fpout, "\t\t\t\t\t};");
	fprintf(stdout, "Time = %f\n", time * 20.8);

}

/*
 * \brief _usage
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
static	void	_usage(void) {
    
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage:       S2Table [input_table] [output_table]\n\n");
}

int32_t	main(int32_t argc, char_t *argv[]) {
	char_t		*inputFileName, *outputFileName;
	FILE		*fpin, *fpout;

	fprintf(stdout, __DATE__"  "__TIME__" (c) EFr-2017\n");

// Analyze the command line
// ------------------------
//
// 1. S2Table input_file output_file

	if (argc != 3) {
		fprintf(stderr, "### Error: too few parameters ...\n");
		_usage();
		exit(EXIT_FAILURE);
	}

	inputFileName  = argv[1];
	outputFileName = argv[2];

// Create the new table
    
	fpin  = fopen(inputFileName, "r");
	fpout = fopen(outputFileName, "w");

	if (fpin == NULL) {
		fclose(fpin);
		fprintf(stderr, "### Error: input file not found\n");
		exit(EXIT_FAILURE);
	}

	if (fpout == NULL) {
		fclose(fpout);
		fprintf(stderr, "### Error: output file not found\n");
		exit(EXIT_FAILURE);
	}

	_generateS2Table(fpin, fpout);
	fclose(fpin);
	fclose(fpout);
	fprintf(stdout, "S2 table generated\n");
	exit(EXIT_SUCCESS);
}

