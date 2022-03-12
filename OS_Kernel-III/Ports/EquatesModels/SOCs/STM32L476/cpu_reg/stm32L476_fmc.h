/*
; stm32L476_fmc.
; ==============

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
; Goal:		stm32L476_fmc equates.
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

#ifndef __STM32L476_FMC__
#define __STM32L476_FMC__

// FMC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	BTCR[8];
} FMC_Bank1_TypeDef;

typedef struct {
	volatile	uint32_t	BWTR[7];
} FMC_Bank1E_TypeDef;

typedef struct {
	volatile	uint32_t	PCR;
	volatile	uint32_t	SR;
	volatile	uint32_t	PMEM;
	volatile	uint32_t	PATT;
	volatile	uint32_t	RESERVED0;
	volatile	uint32_t	ECCR;
} FMC_Bank3_TypeDef;

#define	FMC_Bank1_R		((FMC_Bank1_TypeDef *)FMC_Bank1_R_BASE)
#define	FMC_Bank1E_R	((FMC_Bank1E_TypeDef *)FMC_Bank1E_R_BASE)
#define	FMC_Bank3_R		((FMC_Bank3_TypeDef *)FMC_Bank3_R_BASE)

// FMC_BCR1 register

#define	FMC_BCR1_CCLKEN				0x00100000	//

// FMC_BCRx registers (x=1..4)

#define	FMC_BCRx_MBKEN				0x00000001	//
#define	FMC_BCRx_MUXEN				0x00000002	//
#define	FMC_BCRx_MTYP				0x0000000C	//
#define	FMC_BCRx_MTYP_0				0x00000004	//
#define	FMC_BCRx_MTYP_1				0x00000008	//
#define	FMC_BCRx_MWID 				0x00000030	//
#define	FMC_BCRx_MWID_0				0x00000010	//
#define	FMC_BCRx_MWID_1				0x00000020	//
#define	FMC_BCRx_FACCEN				0x00000040	//
#define	FMC_BCRx_BURSTEN			0x00000100	//
#define	FMC_BCRx_WAITPOL			0x00000200	//
#define	FMC_BCRx_WAITCFG			0x00000800	//
#define	FMC_BCRx_WREN 				0x00001000	//
#define	FMC_BCRx_WAITEN				0x00002000	//
#define	FMC_BCRx_EXTMOD				0x00004000	//
#define	FMC_BCRx_ASYNCWAIT			0x00008000	//
#define	FMC_BCRx_CPSIZE				0x00070000	//
#define	FMC_BCRx_CPSIZE_0			0x00010000	//
#define	FMC_BCRx_CPSIZE_1			0x00020000	//
#define	FMC_BCRx_CPSIZE_2			0x00040000	//
#define	FMC_BCRx_CBURSTRW			0x00080000	//

// FMC_BTRx registers (x=1..4)

#define	FMC_BTRx_ADDSET				0x0000000F	//
#define	FMC_BTRx_ADDSET_0			0x00000001	//
#define	FMC_BTRx_ADDSET_1			0x00000002	//
#define	FMC_BTRx_ADDSET_2			0x00000004	//
#define	FMC_BTRx_ADDSET_3			0x00000008	//
#define	FMC_BTRx_ADDHLD				0x000000F0	//
#define	FMC_BTRx_ADDHLD_0			0x00000010	//
#define	FMC_BTRx_ADDHLD_1			0x00000020	//
#define	FMC_BTRx_ADDHLD_2			0x00000040	//
#define	FMC_BTRx_ADDHLD_3			0x00000080	//
#define	FMC_BTRx_DATAST				0x0000FF00	//
#define	FMC_BTRx_DATAST_0			0x00000100	//
#define	FMC_BTRx_DATAST_1			0x00000200	//
#define	FMC_BTRx_DATAST_2			0x00000400	//
#define	FMC_BTRx_DATAST_3			0x00000800	//
#define	FMC_BTRx_DATAST_4			0x00001000	//
#define	FMC_BTRx_DATAST_5			0x00002000	//
#define	FMC_BTRx_DATAST_6			0x00004000	//
#define	FMC_BTRx_DATAST_7			0x00008000	//
#define	FMC_BTRx_BUSTURN			0x000F0000	//
#define	FMC_BTRx_BUSTURN_0			0x00010000	//
#define	FMC_BTRx_BUSTURN_1			0x00020000	//
#define	FMC_BTRx_BUSTURN_2			0x00040000	//
#define	FMC_BTRx_BUSTURN_3			0x00080000	//
#define	FMC_BTRx_CLKDIV				0x00F00000	//
#define	FMC_BTRx_CLKDIV_0			0x00100000	//
#define	FMC_BTRx_CLKDIV_1			0x00200000	//
#define	FMC_BTRx_CLKDIV_2			0x00400000	//
#define	FMC_BTRx_CLKDIV_3			0x00800000	//
#define	FMC_BTRx_DATLAT				0x0F000000	//
#define	FMC_BTRx_DATLAT_0			0x01000000	//
#define	FMC_BTRx_DATLAT_1			0x02000000	//
#define	FMC_BTRx_DATLAT_2			0x04000000	//
#define	FMC_BTRx_DATLAT_3			0x08000000	//
#define	FMC_BTRx_ACCMOD				0x30000000	//
#define	FMC_BTRx_ACCMOD_0			0x10000000	//
#define	FMC_BTRx_ACCMOD_1			0x20000000	//

// FMC_BWTRx registers (x=1..4)

#define	FMC_BWTRx_ADDSET			0x0000000F	//
#define	FMC_BWTRx_ADDSET_0			0x00000001	//
#define	FMC_BWTRx_ADDSET_1			0x00000002	//
#define	FMC_BWTRx_ADDSET_2			0x00000004	//
#define	FMC_BWTRx_ADDSET_3			0x00000008	//
#define	FMC_BWTRx_ADDHLD			0x000000F0	//
#define	FMC_BWTRx_ADDHLD_0			0x00000010	//
#define	FMC_BWTRx_ADDHLD_1			0x00000020	//
#define	FMC_BWTRx_ADDHLD_2			0x00000040	//
#define	FMC_BWTRx_ADDHLD_3			0x00000080	//
#define	FMC_BWTRx_DATAST			0x0000FF00	//
#define	FMC_BWTRx_DATAST_0			0x00000100	//
#define	FMC_BWTRx_DATAST_1			0x00000200	//
#define	FMC_BWTRx_DATAST_2			0x00000400	//
#define	FMC_BWTRx_DATAST_3			0x00000800	//
#define	FMC_BWTRx_DATAST_4			0x00001000	//
#define	FMC_BWTRx_DATAST_5			0x00002000	//
#define	FMC_BWTRx_DATAST_6			0x00004000	//
#define	FMC_BWTRx_DATAST_7			0x00008000	//
#define	FMC_BWTRx_BUSTURN			0x000F0000	//
#define	FMC_BWTRx_BUSTURN_0			0x00010000	//
#define	FMC_BWTRx_BUSTURN_1			0x00020000	//
#define	FMC_BWTRx_BUSTURN_2			0x00040000	//
#define	FMC_BWTRx_BUSTURN_3			0x00080000	//
#define	FMC_BWTRx_ACCMOD			0x30000000	//
#define	FMC_BWTRx_ACCMOD_0			0x10000000	//
#define	FMC_BWTRx_ACCMOD_1			0x20000000	//

// FMC_PCR register

#define	FMC_PCR_PWAITEN				0x00000002	//
#define	FMC_PCR_PBKEN				0x00000004	//
#define	FMC_PCR_PTYP				0x00000008	//
#define	FMC_PCR_PWID				0x00000030	//
#define	FMC_PCR_PWID_0				0x00000010	//
#define	FMC_PCR_PWID_1				0x00000020	//
#define	FMC_PCR_ECCEN 				0x00000040	//
#define	FMC_PCR_TCLR				0x00001E00	//
#define	FMC_PCR_TCLR_0				0x00000200	//
#define	FMC_PCR_TCLR_1				0x00000400	//
#define	FMC_PCR_TCLR_2				0x00000800	//
#define	FMC_PCR_TCLR_3				0x00001000	//
#define	FMC_PCR_TAR					0x0001E000	//
#define	FMC_PCR_TAR_0				0x00002000	//
#define	FMC_PCR_TAR_1				0x00004000	//
#define	FMC_PCR_TAR_2				0x00008000	//
#define	FMC_PCR_TAR_3				0x00010000	//
#define	FMC_PCR_ECCPS				0x000E0000	//
#define	FMC_PCR_ECCPS_0				0x00020000	//
#define	FMC_PCR_ECCPS_1				0x00040000	//
#define	FMC_PCR_ECCPS_2				0x00080000	//

// FMC_SR register

#define	FMC_SR_IRS					0x00000001	//
#define	FMC_SR_ILS					0x00000002	//
#define	FMC_SR_IFS					0x00000004	//
#define	FMC_SR_IREN					0x00000008	//
#define	FMC_SR_ILEN					0x00000010	//
#define	FMC_SR_IFEN					0x00000020	//
#define	FMC_SR_FEMPT				0x00000040	//

// FMC_PMEM register

#define	FMC_PMEM_MEMSET				0x000000FF	//
#define	FMC_PMEM_MEMSET_0			0x00000001	//
#define	FMC_PMEM_MEMSET_1			0x00000002	//
#define	FMC_PMEM_MEMSET_2			0x00000004	//
#define	FMC_PMEM_MEMSET_3			0x00000008	//
#define	FMC_PMEM_MEMSET_4			0x00000010	//
#define	FMC_PMEM_MEMSET_5			0x00000020	//
#define	FMC_PMEM_MEMSET_6			0x00000040	//
#define	FMC_PMEM_MEMSET_7			0x00000080	//
#define	FMC_PMEM_MEMWAIT			0x0000FF00	//
#define	FMC_PMEM_MEMWAIT_0			0x00000100	//
#define	FMC_PMEM_MEMWAIT_1			0x00000200	//
#define	FMC_PMEM_MEMWAIT_2			0x00000400	//
#define	FMC_PMEM_MEMWAIT_3			0x00000800	//
#define	FMC_PMEM_MEMWAIT_4			0x00001000	//
#define	FMC_PMEM_MEMWAIT_5			0x00002000	//
#define	FMC_PMEM_MEMWAIT_6			0x00004000	//
#define	FMC_PMEM_MEMWAIT_7			0x00008000	//
#define	FMC_PMEM_MEMHOLD			0x00FF0000	//
#define	FMC_PMEM_MEMHOLD_0			0x00010000	//
#define	FMC_PMEM_MEMHOLD_1			0x00020000	//
#define	FMC_PMEM_MEMHOLD_2			0x00040000	//
#define	FMC_PMEM_MEMHOLD_3			0x00080000	//
#define	FMC_PMEM_MEMHOLD_4			0x00100000	//
#define	FMC_PMEM_MEMHOLD_5			0x00200000	//
#define	FMC_PMEM_MEMHOLD_6			0x00400000	//
#define	FMC_PMEM_MEMHOLD_7			0x00800000	//
#define	FMC_PMEM_MEMHIZ				0xFF000000	//
#define	FMC_PMEM_MEMHIZ_0			0x01000000	//
#define	FMC_PMEM_MEMHIZ_1			0x02000000	//
#define	FMC_PMEM_MEMHIZ_2			0x04000000	//
#define	FMC_PMEM_MEMHIZ_3			0x08000000	//
#define	FMC_PMEM_MEMHIZ_4			0x10000000	//
#define	FMC_PMEM_MEMHIZ_5			0x20000000	//
#define	FMC_PMEM_MEMHIZ_6			0x40000000	//
#define	FMC_PMEM_MEMHIZ_7			0x80000000	//

// FMC_PATT register

#define	FMC_PATT_ATTSET				0x000000FF	//
#define	FMC_PATT_ATTSET_0			0x00000001	//
#define	FMC_PATT_ATTSET_1			0x00000002	//
#define	FMC_PATT_ATTSET_2			0x00000004	//
#define	FMC_PATT_ATTSET_3			0x00000008	//
#define	FMC_PATT_ATTSET_4			0x00000010	//
#define	FMC_PATT_ATTSET_5			0x00000020	//
#define	FMC_PATT_ATTSET_6			0x00000040	//
#define	FMC_PATT_ATTSET_7			0x00000080	//
#define	FMC_PATT_ATTWAIT			0x0000FF00	//
#define	FMC_PATT_ATTWAIT_0			0x00000100	//
#define	FMC_PATT_ATTWAIT_1			0x00000200	//
#define	FMC_PATT_ATTWAIT_2			0x00000400	//
#define	FMC_PATT_ATTWAIT_3			0x00000800	//
#define	FMC_PATT_ATTWAIT_4			0x00001000	//
#define	FMC_PATT_ATTWAIT_5			0x00002000	//
#define	FMC_PATT_ATTWAIT_6			0x00004000	//
#define	FMC_PATT_ATTWAIT_7			0x00008000	//
#define	FMC_PATT_ATTHOLD			0x00FF0000	//
#define	FMC_PATT_ATTHOLD_0			0x00010000	//
#define	FMC_PATT_ATTHOLD_1			0x00020000	//
#define	FMC_PATT_ATTHOLD_2			0x00040000	//
#define	FMC_PATT_ATTHOLD_3			0x00080000	//
#define	FMC_PATT_ATTHOLD_4			0x00100000	//
#define	FMC_PATT_ATTHOLD_5			0x00200000	//
#define	FMC_PATT_ATTHOLD_6			0x00400000	//
#define	FMC_PATT_ATTHOLD_7			0x00800000	//
#define	FMC_PATT_ATTHIZ				0xFF000000	//
#define	FMC_PATT_ATTHIZ_0			0x01000000	//
#define	FMC_PATT_ATTHIZ_1			0x02000000	//
#define	FMC_PATT_ATTHIZ_2			0x04000000	//
#define	FMC_PATT_ATTHIZ_3			0x08000000	//
#define	FMC_PATT_ATTHIZ_4			0x10000000	//
#define	FMC_PATT_ATTHIZ_5			0x20000000	//
#define	FMC_PATT_ATTHIZ_6			0x40000000	//
#define	FMC_PATT_ATTHIZ_7			0x80000000	//

// FMC_ECCR register

#define	FMC_ECCR_ECC  				0xFFFFFFFF	//
#endif
