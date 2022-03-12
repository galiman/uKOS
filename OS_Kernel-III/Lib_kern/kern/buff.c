/*
; buff.
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
; Goal:		Kern - String management.
;
;			This module is responsible for manipulating the strings of the uKernel.
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

#include 	<uKOS.h>

/*
 * \brief Compare 2 strings (source -> destination)
 *
 * \param[in]	*source			Ptr on the string source
 * \param[in]	*destination	Ptr on the string destination
 * \param[out]	TRUE			Source == Destination
 * \param[out]	FALSE			Source != Destination
 *
 */
bool_t	buff_cmpStrings(const char_t *source, const char_t *destination) {
	uint8_t		i;
	bool_t		status, terminate = FALSE;

	for (i = 0; i < (KLNSTRID-1); i++) {
		if ((*source == '\0') || (*destination == '\0')) {
			terminate = TRUE;
		}
		if (terminate) {
			status = (*source++ == *destination++) ? (TRUE) : (FALSE);
			return (status);
		}

		else if (*source++ != *destination++) {
			return (FALSE);
		}

	}
	return (TRUE);
}
