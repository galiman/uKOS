/*
; stub_vis0_vs.
; =============

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
; Goal:		stub for the connection of the "vis0" manager to the vs device.
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

#define	__VS_SEMA_IM__
#define	__VS_INVERSE__
#define	__VS_SCOTCH__	

#define	cnfVs_t					cnfVis0_t

#define	vs_init					stub_vis0_init			
#define	vs_configure			stub_vis0_configure		
#define	vs_acquisition			stub_vis0_acquisition			
#define	vs_getImage				stub_vis0_getImage			

#define	KVSNOERR				KVIS0NOERR
#define	KVSSYCNA				KVIS0SYCNA
#define	KVSGEERR				KVIS0GEERR
#define	KVSCHBSY				KVIS0CHBSY
#define	KVSTROPR				KVIS0TROPR
#define	KVSCMNSU				KVIS0CMNSU
#define	KVSPGNOE				KVIS0PGNOE

#define	KSEMAPHORE_VSIM			KSEMAPHORE_VIS0IM

#include 	<model_vs.cm>

