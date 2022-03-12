/*
; icyamr_gpu.
; ===========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 113                  $:  Revision of last commit
; $Date:: 2017-04-16 16:55:48#$:  Date of last commit
;
; Project:	uKOS
; Goal:		icyamr_gpu equates.
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

#ifndef __ICYAMR_GPU__
#define __ICYAMR_GPU__

// GPU address definitions
// -----------------------

#define	GPU_ENABLE			(volatile uint8_t  *)(GPU + 0x00)
#define	GPU_8N16			(volatile uint8_t  *)(GPU + 0x01)
#define	GPU_ALU_OP			(volatile uint8_t  *)(GPU + 0x02)
#define	GPU_SUM				(volatile uint8_t  *)(GPU + 0x03)
#define	GPU_ACCU_MODE		(volatile uint8_t  *)(GPU + 0x04)
#define	GPU_ACCU_CLEAR		(volatile uint8_t  *)(GPU + 0x05)
#define	GPU_ORICON_THR1		(volatile uint8_t  *)(GPU + 0x06)
#define	GPU_ORICON_THR2		(volatile uint8_t  *)(GPU + 0x07)
#define	GPU_MODE_ORICON		(volatile uint8_t  *)(GPU + 0x08)
#endif
