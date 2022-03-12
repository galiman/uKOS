/*
; bench_04.
; =========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 184                  $:  Revision of last commit
; $Date:: 2017-07-17 10:50:19#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Bench 04: 	Compute the atan2(x, y) by a cordic in a fixed point.
;						The data is located in the external memory
;
;			Target used: Baphomet 429 @ 180-MHz
;			Target used: Baphomet 746 @ 216-MHz
;
;			Bench results in [us]:
;
;						Ext Ram Execution				Int Flash Execution
;						cpu 429 (M4)	cpu 746 (M7)	cpu 429 (M4)	cpu 746 (M7)
;			_atan2		483				46				70				53
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

#define	KMAXITER	32

// We want to represent those angles: -180'000 .. +180'000
// x 1000 just to represent the fractionnary part
// 180'000 needs 17-bits
// So, the fixed point representations will be: s iiiiiiiiiiiiiiiii.ffffffffffffff
//                                              1 17               . 14
#define	KUNIT		(1<<14)

#define KPI			((3.14159265358979)*KUNIT)
#define K180		((180.0)*KUNIT)
#define KRADTODEG	(K180/KPI)

// Prototypes
// ==========

static	void	 _atan2(uint32_t *time, int32_t *angle, int32_t x, int32_t y);

/*
 * \brief bench_04
 *
 * - aten2(y, x)
 *
 */
bool_t		bench_04(void) {
	uint32_t 	time[1];
	int32_t		x, y, angle;

// atan2(0.234, -10.345) = 178.7042-deg

	x = (-10.345)*KUNIT; y = (0.234)*KUNIT;
	
	_atan2(&time[0], &angle, y, x);

// Angle x 1000 in degrees

	angle = ((angle*1000)*KRADTODEG)/KUNIT;

// Results
//                      Bench xy: Descriptions, comments, ....                 Execution time
// ---------------------|---------|--------------------------------------------|-----------------|
//
	iotx_printf(KSYST, "Bench 04: Compute the integer atan2 using the CORDIC\n"
					   "          algorithm. The code is called 1000 times.\n"
					   "          1000 x atan2(y, x)                           t = %d-us\n\n", time[0]);
	return (TRUE);
}

/*
 * \brief _atan2
 *
 * - Compute the _atan2
 *
 */
static	void _atan2(uint32_t *time, int32_t *angle, int32_t y, int32_t x) {
				uint64_t	tStamp[2];
	volatile	uint32_t	j;
	register	int32_t		yi, i;
	register	int32_t		lx, ly;
	const		int32_t		aTabAtan[] = {
								(0.785398163397448000000000000000*KUNIT), (0.463647609000806000000000000000*KUNIT),
								(0.244978663126864000000000000000*KUNIT), (0.124354994546761000000000000000*KUNIT),
								(0.062418809995957400000000000000*KUNIT), (0.031239833430268300000000000000*KUNIT),
								(0.015623728620476800000000000000*KUNIT), (0.007812341060101110000000000000*KUNIT),
								(0.003906230131966970000000000000*KUNIT), (0.001953122516478820000000000000*KUNIT),
								(0.000976562189559319000000000000*KUNIT), (0.000488281211194898000000000000*KUNIT),
								(0.000244140620149362000000000000*KUNIT), (0.000122070311893670000000000000*KUNIT),
								(0.000061035156174208800000000000*KUNIT), (0.000030517578115526100000000000*KUNIT),
								(0.000015258789061315800000000000*KUNIT), (0.000007629394531101970000000000*KUNIT),
								(0.000003814697265606500000000000*KUNIT), (0.000001907348632810190000000000*KUNIT),
								(0.000000953674316405961000000000*KUNIT), (0.000000476837158203089000000000*KUNIT),
								(0.000000238418579101558000000000*KUNIT), (0.000000119209289550781000000000*KUNIT),
								(0.000000059604644775390600000000*KUNIT), (0.000000029802322387695300000000*KUNIT),
								(0.000000014901161193847700000000*KUNIT), (0.000000007450580596923830000000*KUNIT),
								(0.000000003725290298461910000000*KUNIT), (0.000000001862645149230960000000*KUNIT),
								(0.000000000931322574615479000000*KUNIT), (0.000000000465661287307739000000*KUNIT)
							};

	kern_getTiccount(&tStamp[0]);
	for (j = 0; j < 1000; j++) {
		*angle = (0.0)*KUNIT;

// Special case: vectors = 0

		if ((x == 0) && (y == 0)) {
			*angle = 0;
			kern_getTiccount(&tStamp[1]);
			*time = ((uint32_t)(tStamp[1] - tStamp[0]));
			return;
		}

		lx = x; ly = y;

// Verify the right plane

		if (lx < 0) { lx = -lx; ly = -ly; *angle = KPI;       }
    	if (ly > 0) {                     *angle = -(*angle); }
    
// Rotate the vector and compute the angle

		for (i = 0; i < KMAXITER; i++) {
			if (ly < 0) {
				yi = ly + (lx >> i);
				lx = lx - (ly >> i);
				ly = yi;
				*angle -= aTabAtan[i];
			}
			else {
				yi = ly - (lx >> i);
				lx = lx + (ly >> i);
				ly = yi;
				*angle += aTabAtan[i];
			}
   	 	}
   	 }
	kern_getTiccount(&tStamp[1]);

	*time = ((uint32_t)(tStamp[1] - tStamp[0]));
}

