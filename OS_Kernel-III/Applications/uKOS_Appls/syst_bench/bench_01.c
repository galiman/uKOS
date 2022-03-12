/*
; bench_01.
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
; Goal:		Bench 01: 	Compute the X-Y projection and the histogram of
;						a small square array (1000 x 1000 uint8_t) elements.
;						The data is located in the CPU external memory
;
;			Target used: Baphomet 429 @ 180-MHz
;			Target used: Baphomet 746 @ 216-MHz
;
;			Bench results in [us]:
;
;						Ext Ram Execution				Int Flash Execution
;						cpu 429 (M4)	cpu 746 (M7)	cpu 429 (M4)	cpu 746 (M7)
;			_fill		236202			9339			38980			9322
;			_projX		617702			133772			297959			136009
;			_projY		582815			80257			297878			79202
;			_histo		490940			189230			98489			86059
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

#define	KNBX	1000
#define	KNBY	1000

// Prototypes
// ==========

static	void	_fill(uint8_t *array, uint32_t *time);
static	void	_prjX(uint8_t *array, uint32_t *time, uint32_t *x);
static	void	_prjY(uint8_t *array, uint32_t *time, uint32_t *y);
static	void	_hist(uint8_t *array, uint32_t *time, uint32_t *h);

/*
 * \brief bench_01
 *
 * - X-Y projection and histogram of a big array
 *
 */
bool_t		bench_01(void) {
	uint8_t		*array;
	uint32_t	*x, *y, *h, time[4];

	array = (uint8_t  *)syos_malloc(KEXTERNAL, (KNBX*KNBY));             if (array == NULL) {                                 				return (FALSE); }
	x     = (uint32_t *)syos_malloc(KEXTERNAL, (KNBX*sizeof(uint32_t))); if (x == NULL)     { syos_free(array);               				return (FALSE); }
	y     = (uint32_t *)syos_malloc(KEXTERNAL, (KNBY*sizeof(uint32_t))); if (y == NULL)     { syos_free(array); syos_free(x); 				return (FALSE); }
	h     = (uint32_t *)syos_malloc(KEXTERNAL, (256*sizeof(uint32_t)));  if (h == NULL)     { syos_free(array); syos_free(x); syos_free(y); return (FALSE); }

	_fill(array, &time[0]);
	_prjX(array, &time[1], x);
	_prjY(array, &time[2], y);
	_hist(array, &time[3], h);

	syos_free(h);
	syos_free(y);
	syos_free(x);
	syos_free(array);

// Results
//                      Bench xy: Descriptions, comments, ....                 Execution time
// ---------------------|---------|--------------------------------------------|-----------------|
//
	iotx_printf(KSYST, "Bench 01: Fill a small 2D array (1000 x 1000) elements\n"
					   "          in the internal memory. Then, compute the\n"
					   "          X-Y projections and the histogram.\n"
					   "          Fill the array                               t = %d-us\n"
					   "          X projection                                 t = %d-us\n"
					   "          Y projection                                 t = %d-us\n"
					   "          Histogram                                    t = %d-us\n\n", time[0], time[1], time[2], time[3]);
	return (TRUE);
}

/*
 * \brief _fill
 *
 * - Fill the array with a random number
 *
 */
static	void	_fill(uint8_t *array, uint32_t *time) {
	uint32_t	i, j;
	uint64_t	tStamp[2];

	kern_getTiccount(&tStamp[0]);
	for (i = 0; i < KNBX; i++) {
		for (j = 0; j < KNBY; j++) {
			*(array + i*KNBY + j) = (uint8_t)(j);
		}
	}
	kern_getTiccount(&tStamp[1]);

	*time = ((uint32_t)(tStamp[1] - tStamp[0]));
}

/*
 * \brief _prjX
 *
 * - Compute the X projections
 *
 */
static	void	_prjX(uint8_t *array, uint32_t *time, uint32_t *x) {
				uint32_t	i, j;
				uint64_t	tStamp[2];
	register	uint32_t	*p;

	kern_getTiccount(&tStamp[0]);
	for (i = 0; i < KNBX; i++) {
		*(x + i) = 0;
		for (j = 0; j < KNBY; j++) {
			p = (x + i);
			*p += (uint32_t)*(array + j*KNBX + i);
		}
	}
	kern_getTiccount(&tStamp[1]);

	*time = ((uint32_t)(tStamp[1] - tStamp[0]));
}

/*
 * \brief _prjY
 *
 * - Compute the Y projections
 *
 */
static	void	_prjY(uint8_t *array, uint32_t *time, uint32_t *y) {
				uint32_t	i, j;
				uint64_t	tStamp[2];
	register	uint32_t	*p, k;

	kern_getTiccount(&tStamp[0]);
	for (j = 0; j < KNBY; j++) {
		*(y + j) = 0;
		k = j*KNBX;
		for (i = 0; i < KNBX; i++) {
			p = (y + j);
			*p += (uint32_t)*(array + k + i);
		}
	}
	kern_getTiccount(&tStamp[1]);

	*time = ((uint32_t)(tStamp[1] - tStamp[0]));
}

/*
 * \brief _hist
 *
 * - Compute the histogram
 *
 */
static	void	_hist(uint8_t *array, uint32_t *time, uint32_t *h) {
				uint32_t	i, histo[256];
				uint64_t	tStamp[2];
	register	uint32_t	*p;

	kern_getTiccount(&tStamp[0]);
	p = histo;
	for (i = 0; i < 256; i++) {
		*p++ = 0;
	}

	for (i = 0; i < (KNBX*KNBY); i++) {
		p = (uint32_t *)((uint32_t)&histo[0] + (uint32_t)*(array + i));
		*p += 1;
	}
	kern_getTiccount(&tStamp[1]);

	*time = ((uint32_t)(tStamp[1] - tStamp[0]));
}

