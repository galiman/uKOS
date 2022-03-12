/*
; macros_generic.
; ===============

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
; Goal:		Important generic macros.
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

#ifndef	__MACROS_GENERIC__
#define	__MACROS_GENERIC__

// System & compiler
// -----------------

#define	QUOTE(name)				#name
#define	STR(macro)				QUOTE(macro)

#define	uKOS_COMPILER			"gcc-"STR(__GNUC__)"."STR(__GNUC_MINOR__)"."STR(__GNUC_PATCHLEVEL__)
#define uKOS_GCC_VERSION 		((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + (__GNUC_PATCHLEVEL__))
#define	uKOS_KCPU				KCPU
#define	uKOS_KCONTROLLER		KCONTROLLER
#define	uKOS_KBOARD				KBOARD
#define	uKOS_KTARGET			KTARGET

// Trace
// -----

#if (defined(__RECTRACE__))
#define	TRACE(text)				vDebug->oTextTrace_3 = vDebug->oTextTrace_2; \
								vDebug->oTextTrace_2 = vDebug->oTextTrace_1; \
								vDebug->oTextTrace_1 = vDebug->oTextTrace_0; \
								vDebug->oTextTrace_0 = (text);

#else
#define	TRACE(text);
#endif

// Device reserve/release macros
// -----------------------------
//
// Reserve/release the device in read mode
// Reserve/release the device in write mode
// Reserve/release the device in all modes

#define	KDEVREAD				0
#define	KDEVWRITE				1
#define	KDEVALL					2

#define	RESERVE(device, mode)				while (device##_reserve(mode) == K##device##CHBSY)   { kern_switchFast(); }
#define	RELEASE(device, mode)				device##_release(mode);

#define	RESERVE_COMM(commManager, mode)		while (comm_reserve(commManager, mode) == KCOMMCHBSY) { kern_switchFast(); }
#define	RELEASE_COMM(commManager, mode)		comm_release(commManager, mode);

// Get a 32-bit value from a string pointer
// ----------------------------------------

#define	GET_PTR_32(ptr) 		((uint32_t)*((ptr) + 0)<<24) + \
								((uint32_t)*((ptr) + 1)<<16) + \
								((uint32_t)*((ptr) + 2)<<8)  + \
								((uint32_t)*((ptr) + 3));

// Print float value with iotx_printf
// ----------------------------------
//
// Only for values of x > 0
// If x <= 0, then, multiply x . n in order to have x > 0
//
// iotx_printf(KSYST, "%d.01%d", FLOAT_1(value);
// iotx_printf(KSYST, "%d.03%d", FLOAT_3(value);
// iotx_printf(KSYST, "%d.09%d", FLOAT_9(value);

#define	ABS(x)					((x) < 0) ? -(x) : (x)
#define	FLOAT_1(x)				(((int32_t)((x) * 10) / 10)), (ABS((int32_t)((x) * 10) % 10))
#define	FLOAT_2(x)				(((int32_t)((x) * 100) / 100)), (ABS((int32_t)((x) * 100) % 100))
#define	FLOAT_3(x)				(((int32_t)((x) * 1000) / 1000)), (ABS((int32_t)((x) * 1000) % 1000))
#define	FLOAT_4(x)				(((int32_t)((x) * 10000) / 10000)), (ABS((int32_t)((x) * 10000) % 10000))
#define	FLOAT_5(x)				(((int32_t)((x) * 100000) / 100000)), (ABS((int32_t)((x) * 100000) % 100000))
#define	FLOAT_6(x)				(((int32_t)((x) * 1000000) / 1000000)), (ABS((int32_t)((x) * 1000000) % 1000000))
#define	FLOAT_7(x)				(((int32_t)((x) * 10000000) / 10000000)), (ABS((int32_t)((x) * 10000000) % 10000000))
#define	FLOAT_8(x)				(((int32_t)((x) * 100000000) / 100000000)), (ABS((int32_t)((x) * 100000000) % 100000000))
#define	FLOAT_9(x)				(((int32_t)((x) * 1000000000) / 1000000000)), (ABS((int32_t)((x) * 1000000000) % 1000000000))

// Tic / Tac
// ---------

#define	TIC_EXCEPTION_TIME		uint64_t	tic, tac; \
								kern_getTiccount(&tic);

#define	TAC_EXCEPTION_TIME 		kern_getTiccount(&tac); \
								vTimeException += (uint16_t)(tac - tic);

// Trace for impulse
// -----------------

#define	TAG_LOG(commManager, id, s0,s1,s2,s3) { \
								volatile	proc_t		*localProcess[1]; \
											uint8_t		log[64] = { \
															'u','K','O','S',	/* + 0		32	Header								*/ \
															id,					/* + 4		8	Process ID							*/ \
															0,					/* + 5		8	Process priority					*/ \
															0,0,				/* + 6		16	Reserve (alignment)					*/ \
															0,0,0,0,			/* + 8		32	Process Stack						*/ \
															s0,s1,s2,s3,		/* + 12		32	Process State						*/ \
															0,0,0,0,0,0,0,0,	/* + 16		64	Temporal tag us						*/ \
															0,0,0,0,0,0,0,0,	/* + 24		64	Nb of executions of the process		*/ \
															0,0,0,0,0,0,0,0,	/* + 32		64	Cumulated time used by the process	*/ \
														}; \
								kern_getProcessRun(&localProcess[0]); \
								kern_getTiccount((uint64_t *)(log + 16)); \
								*((uint8_t  *)(log + 5))  = localProcess[0]->oSpecification.oPriority; \
								*((uint32_t *)(log + 8))  = (uint32_t)localProcess[0]->oSpecification.oStkSup; \
								*((uint64_t *)(log + 24)) = localProcess[0]->oStatistic.oNbExecutions; \
								*((uint64_t *)(log + 32)) = localProcess[0]->oStatistic.oTimePCum; \
								comm_write(commManager, (uint8_t *)&log[0], 40); \
								};
#endif
