/*
; uKOS.
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
; Goal:		Universal h file for uKOS systems.
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

#ifndef	__uKOS__
#define	__uKOS__

#include	<types.h>
#include	<conf_system.h>
#include	<board.h>
#include	<ip.h>
#include	<macros.h>
#include	<macros_generic.h>
#include 	<modules.h>
#include	<crt0.h>
#include	<cpu_reg.h>
#include	<lib_gene.h>
#include	<lib_comm.h>
#include	<lib_tbox.h>
#include	<lib_kern.h>
#include	<lib_peri.h>
#include	<lib_nlib.h>
#include	<lib_engi.h>

// uKOS main constants
// -------------------

#define	uKOS_VERSION			"4.8.0"
#define	uKOS_VERSION_MAJOR		4
#define	uKOS_VERSION_MINOR		8
#define	uKOS_VERSION_REVISION	0

// Supported machines
// ------------------

enum	{
		Discovery_407,			// Target = Discovery_407			CPU = ARM Cortex M4
		Discovery_429,			// Target = Discovery_429			CPU = ARM Cortex M4
		Discovery_476,			// Target = Discovery_476			CPU = ARM Cortex M4
		Discovery_746,			// Target = Discovery_746			CPU = ARM Cortex M7
		Baphomet_407,			// Target = Baphomet_407			CPU = ARM Cortex M4
		Baphomet_429,			// Target = Baphomet_429			CPU = ARM Cortex M4
		Baphomet_746,			// Target = Baphomet_746			CPU = ARM Cortex M7
		CSEM_VIP_407,			// Target = VIP_407					CPU = ARM Cortex M4
		CSEM_VIP_429,			// Target = VIP_429					CPU = ARM Cortex M4
		CSEM_VIP_746,			// Target = VIP_746					CPU = ARM Cortex M7
		CSEM_VIP_777,			// Target = VIP_777					CPU = ARM Cortex M7
		LAP_icycam_103,			// Target = LAP_icycam_103			CPU = ARM Cortex M3
		CSEM_icycam, 			// Target = icycam,					CPU = icyflex 1
		CSEM_icyamr, 			// Target = icyamr,					CPU = icyflex 1
		CSEM_silicon, 			// Target = silicon,				CPU = icyflex 1
		CSEM_Suntracker_407,	// Target = Suntracker_407			CPU = ARM Cortex M4
		CSEM_Posbone_WL_407,	// Target = Posbone_WL_407			CPU = ARM Cortex M4
		CSEM_Bio_479			// Target = Bio_479					CPU = ARM Cortex M4
		};
#endif
