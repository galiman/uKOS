/*
; i2cx.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		i2cx manager.
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

#ifndef	__I2CX__
#define	__I2CX__

// Configuration structure
// -----------------------

typedef	struct	cnfI2cx	cnfI2cx_t;

struct cnfI2cx {
			uint8_t		oSpeed;											// Speed
			#define		K100KHZ		0									// bus at 100-KHz
			#define		K400KHZ		1									// bus at 400-KHz

			uint8_t		oKernSync;										// uKernel synchro
			#define		BRXSEMA		0									// RX semaphore
			#define		BTXSEMA		1									// TX semaphore
};

// Semaphores
// ----------

#define	KSEMARX			0												// Semaphore RX
#define	KSEMATX			1												// Semaphore TX
#endif
