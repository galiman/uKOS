/*
; list.
; =====

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
; Goal:		List the system modules.
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

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"list         List the system modules.                  (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "List the system modules\n"
									"=======================\n\n"

									"This tool displays the modules of the system.\n"
									"All the family modules will be displayed.\n\n"

									"Input format:  list {IDfamily}\n"
									"Output format: IDfamily    name    comment    revision \n\n"

									"IDfamily:\n"
									"   'F' for Firm family\n"
									"   'A' for Application family\n"
									"   'P' for Protocol family\n"
									"   'X' for Tool family\n"
									"   'S' for start-up family\n"
									"   'T' for Process family\n"
									"   'O' for Overlay process family\n"
									"   'K' for Kern manager family\n"
									"   'B' for TBox manager family\n"
									"   'L' for Gene manager family\n"
									"   'I' for Peri manager family\n"
									"   'C' for Comm manager family\n"
									"   'E' for Engi manager family\n"
									"   'N' for Nlib manager family\n"
									"   'H' for Shar manager family\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	bool_t	_listModule(uint32_t mms, uint8_t idFamily);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(List, KIDTOOL, KLISTNUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
			uint8_t		idModule;
			uint16_t	i;
			int32_t		status;
			bool_t		error = FALSE;
	const	char_t		*str;
	const	uint8_t		families[] = {
							KIDSTARTUP, KIDPROTOCOL, KIDPROCESS,
							KIDOVERLAY, KIDTOOL, KIDKERN,
							KIDTBOX, KIDGENE, KIDPERI,
							KIDCOMM, KIDENGI, KIDCOMPANY, KIDNLIB, KIDSHAR
						};

	iotx_printf(KSYST, "uKOS module list.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc == 2) {

// List of the selected family module

		str = argv[1];
		idModule = (uint8_t)(str[0]);
		if (!_listModule(KMEM0, idModule)) {
			error = TRUE;
		}
	}

// List of all the module families

	else {
		for (i = 0; i < sizeof(families)/sizeof(uint8_t); i++) {
			_listModule(KMEM0, families[i]);
		}
		_listModule(KMEM1, KIDAPPLICATION);
	}

	if (!error) {  				 										 status = EXIT_SUCCESS_OS; }
	else        { iotx_printf(KSYST, "This family does not exist.\n\n"); status = EXIT_FAILURE;    }
	return (status);
}

// Local routines
// ==============

/*
 * \brief _listModule
 *
 * - List a family modules
 *
 */
 static	bool_t	_listModule(uint32_t mms, uint8_t idFamily) {
			uint16_t	i, index = 0;
			uint32_t	idModule;
			bool_t		found = FALSE;
			char_t		*idMMS, bufMMS[] = "xyzt";
			char_t		*idIdentifier, bufIdent[] = "xyzt";
	const	module_t	*module;

// List inside MEMx

	while (syos_getFamilyModule(mms, idFamily, &idModule, &index, &module) == KSYOSNOERR) {
		if (!(module->oFlag & (1<<BCONFIDENTIAL))) {
			found = TRUE;

// Find a family code; display its Id
//   The format is:
//   MMSx___nn___XYZT__rev__name_________Comment

     		idMMS        = (char_t *)&mms;
 			idIdentifier = (char_t *)&idModule;
 
   			#if (defined(__LITTLE_ENDIAN__))
   			for (i = 0; i < 4; i++) { bufMMS[i]   = *(idMMS + 3 - i);        }
    		for (i = 0; i < 4; i++) { bufIdent[i] = *(idIdentifier + 3 - i); }

			#else
    		for (i = 0; i < 4; i++) { bufMMS[i]   = *(idMMS + i);            }
    		for (i = 0; i < 4; i++) { bufIdent[i] = *(idIdentifier + i);     }
   			#endif
   
   			iotx_printf(KSYST, "%s %2d  %s  v%s   %s\n", bufMMS, index, bufIdent, module->oStrRevision, module->oStrApplication);
		}
		index++;
	}

	if (found) { iotx_printf(KSYST, "\n"); }
	return (found);
}

