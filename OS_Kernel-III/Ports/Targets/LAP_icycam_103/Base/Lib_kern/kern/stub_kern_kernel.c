/*
; stub_kern_kernel.
; =================

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2009-01-08
; Modifs:
; Version:	5.2
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the connection of the "kern" manager to the kernel device.
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

// Connect the physical device to the logical manager
// --------------------------------------------------

#define	kernel_init					stub_kern_init
#define	kernel_runKernel			stub_kern_runKernel
#define	kernel_loadOverlay			stub_kern_loadOverlay
#define	kernel_setLowPower			stub_kern_setLowPower
#define	kernel_runPrecise			stub_kern_runPrecise
#define	kernel_initStackFrame		stub_kern_initStackFrame
#define	kernel_setUnixTime			stub_kern_setUnixTime
#define	kernel_getUnixTime			stub_kern_getUnixTime
#define	kernel_timeStmp				stub_kern_timeStmp
#define	kernel_timeNext				stub_kern_timeNext
#define	kernel_getPC				stub_kern_getPC
#define	kernel_getFunctionName		stub_kern_getFunctionName

#define	KFREQUENCY_T1				KFREQUENCY_AHB
#define	KFREQUENCY_T2				KFREQUENCY_AHB
#define	KFREQUENCY_T3				KFREQUENCY_AHB
#define	KFREQUENCY_T4				KFREQUENCY_AHB

// Timers for 1-ms, 20-ms and free running

#define KTTIMEFAST					(KTIMEUNIT)
#define KTTIMESHARING				(KTIMEPROC)
#define KTTIMESTATISTIC				(-1)

#include 	<model_kernel_tim_1_2_3_4_ex0_ex2.cm>

