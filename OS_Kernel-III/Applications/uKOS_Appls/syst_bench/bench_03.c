/*
; bench_03.
; =========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Bench 03: 	Searching for the min / max of a big square array (1000000 uint32_t) elements.
;						The data is located in the external memory
;
;			Target used: Baphomet 429 @ 180-MHz
;			Target used: Baphomet 746 @ 216-MHz
;
;			Bench results in [us]:
;
;						Ext Ram Execution				Int Flash Execution
;						cpu 429 (M4)	cpu 746 (M7)	cpu 429 (M4)	cpu 746 (M7)
;			_minMax		545358			113770			110666			96498
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

#define	KNBELEMENTS		500000

// Prototypes
// ==========

static	void	_minMax(uint32_t *array, uint32_t *time, uint32_t *min, uint32_t *max);

/*
 * \brief bench_03
 *
 * - min / max of an small array
 *
 */
bool_t		bench_03(void) {
	uint32_t	i, *array, value, time[1], min, max;

	array = (uint32_t *)syos_malloc(KEXTERNAL, (KNBELEMENTS*sizeof(uint32_t))); if (array == NULL) { return (FALSE); }

// Initialize the array with a random value

	for (i = 0; i < KNBELEMENTS; i++) {
		glob_getRND(&value);
		*(array + i) = value;
	}

	_minMax(array, &time[0], &min, &max);
	syos_free(array);

// Results
//                      Bench xy: Descriptions, comments, ....                 Execution time
// ---------------------|---------|--------------------------------------------|-----------------|
//
	iotx_printf(KSYST, "Bench 03: Fill a big 1D array (500000) elements in\n"
					   "          the external memory with a random pattern.\n"
					   "          Then, compute the min / max values.\n"
					   "          Min / Max                                    t = %d-us\n\n", time[0]);
	return (TRUE);
}

/*
 * \brief _minMax
 *
 * - Compute the min / max of an array
 *
 */
static	void	_minMax(uint32_t *array, uint32_t *time, uint32_t *min, uint32_t *max) {
	volatile	uint32_t	i;
				uint64_t	tStamp[2];

	kern_getTiccount(&tStamp[0]);
	*min = 0xFFFFFFFF; *max = 0x00000000;
	for (i = 0; i < KNBELEMENTS; i++) {
		if (*(array + i) < *min) *min = *(array + i);
		if (*(array + i) > *max) *max = *(array + i);
	}
	kern_getTiccount(&tStamp[1]);

	*time = ((uint32_t)(tStamp[1] - tStamp[0]));
}

