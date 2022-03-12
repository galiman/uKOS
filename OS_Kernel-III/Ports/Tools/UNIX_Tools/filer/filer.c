/*
; filer.
; ======

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2016-01-16
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Generate the file "filer.h" with the table of the module used by uKOS system.
;
;			If the module sqe is in the list, then, include the library sqee
;
; Usage:	filer [module_list_file]
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
 * \brief _print_header
 *
 * - Print the uKOS header
 *
 * \param[in]	fpout	Ptr on the output file
 * \param[out]	-
 *
 */
void	_print_header(FILE *fpout) {

	fprintf(fpout, "/*\n");
	fprintf(fpout, "; filer.h.\n");
	fprintf(fpout, "; ========\n\n");

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
	fprintf(fpout, "; Goal:		Table containing the list of the uKOS module\n");
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

	fprintf(fpout, "#ifndef	__FILER__\n");
	fprintf(fpout, "#define	__FILER__\n");

}

/*
 * \brief _print_declaration
 *
 * - Print the declarations
 *   - extern	const	module_t	aXyzt_Specifications;
 *
 * \param[in]	fpin	Ptr on the input file
 * \param[in]	fpout	Ptr on the output file
 * \param[out]	-
 *
 */
void	_print_declaration(FILE *fpin, FILE *fpout) {
    char_t	aWord[255], name[255];
    bool_t	skip, sqee;

	fprintf(fpout, "\n");
	fprintf(fpout, "// The module directory\n");
	fprintf(fpout, "// ====================\n\n");

 	sqee = FALSE;
 	while (fscanf(fpin, "%s", name) == 1) {

// Add the sqee modules (confidential module not in the makefile list)

		if (strcmp(name, "sqe") == 0) {
			sqee = TRUE;
		}
		if (!skip) {
			strcpy(aWord, "a");												// a
			strcat(aWord, name);											// axytt
			aWord[1] &= ~0x20;												// aXytt
			strcat(aWord, "_Specifications;");								// aXytt_Specifications;
			fprintf(fpout, "extern    const    module_t    %s\n", aWord);	// extern    const    module_t    aXytt_Specifications;
		}
	}

// If the sqe tool module is on the list, then, add the sqee manager

	if (sqee) {
		strcpy(aWord, "a");													// a
		strcat(aWord, "sqee");												// asqee
		aWord[1] &= ~0x20;													// aSqee
		strcat(aWord, "_Specifications;");									// aSqee_Specifications;
		fprintf(fpout, "extern    const    module_t    %s\n", aWord);		// extern    const    module_t    aSqee_Specifications;
	}
}

/*
 * \brief _print_table
 *
 * - Print the table
 *   - const	directory_t	aDirectory[] = {
 *   -              { KBUILDIN, &aSqee_Specifications },
 *   -              { KBUILDIN, &aSqee_Specifications },
 *   -              ...
 *   -              { KNOMODULE, 0                    }
 *   -          };
 *
 * \param[in]	fpin	Ptr on the input file
 * \param[in]	fpout	Ptr on the output file
 * \param[out]	-
 *
 */
void	_print_table(FILE *fpin, FILE *fpout) {
    char_t	aWord[255], name[255];
    bool_t	skip, sqee;

	fprintf(fpout, "\n");
	fprintf(fpout, "// Directory table\n");
	fprintf(fpout, "// ===============\n\n");

	fprintf(fpout, "const	directory_t	aDirectory[] = {\n");

 	sqee = FALSE;
 	while (fscanf(fpin, "%s", name) == 1) {

// Add the sqee modules (confidential module not in the makefile list)

		if (strcmp(name, "sqe")  == 0) {
			sqee = TRUE;
		}
		if (!skip) {
			strcpy(aWord, "a");												// a
			strcat(aWord, name);											// axytt
			aWord[1] &= ~0x20;												// aXytt
			strcat(aWord, "_Specifications");								// aXytt_Specifications;
			fprintf(fpout, "        { KBUILDIN, &%s },\n", aWord);			// { KBUILDIN, &aXytt_Specifications },
		}
	}

// If the sqe tool module is on the list, then, add the sqee manager

	if (sqee) {
		strcpy(aWord, "a");													// a
		strcat(aWord, "sqee");												// asqee
		aWord[1] &= ~0x20;													// aSqee
		strcat(aWord, "_Specifications");									// aSqee_Specifications;
		fprintf(fpout, "        { KBUILDIN, &%s },\n", aWord);				// { KBUILDIN, &aSqee_Specifications },
	}
	fprintf(fpout, "        { KNOMODULE, 0 }\n");							// { KNOMODULE, 0 }
	fprintf(fpout, "    };\n");												//
	fprintf(fpout, "#endif\n");												//
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
// filer input.bin

	switch (argc) {
		case 2: {
			inputFileName  = argv[1];
			outputFileName = "filer.h";
			break;
		}
		default: {
			error = TRUE;
			break;
		}
	}

// Error (data entry not correct)

	if (error) {
		fprintf(stderr, "%s %s\n\n", argv[0], argv[1]);
		fprintf(stderr, "\nUSAGE: filer [binary_file]\n");
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

	_print_header(fpout);
	_print_declaration(fpin, fpout);
	rewind(fpin);
	_print_table(fpin, fpout);
	fclose(fpin);
	fclose(fpout);
	fprintf(stdout, "filer.h generated\n");
	return (EXIT_SUCCESS);
}

