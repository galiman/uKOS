/*
; stm32F429_fmc.
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
; Goal:		stm32F429_fmc equates.
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

#ifndef __STM32F429_FMC__
#define __STM32F429_FMC__

// FMC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	BCR1;   
	volatile	uint32_t	BTR1;   
	volatile	uint32_t	BCR2;   
	volatile	uint32_t	BTR2;   
	volatile	uint32_t	BCR3;   
	volatile	uint32_t	BTR3;   
	volatile	uint32_t	BCR4;   
	volatile	uint32_t	BTR4;   
} FMC_Bank1_TypeDef; 

typedef struct {
	volatile	uint32_t	BWTR1;
	volatile	uint32_t	RESERVED1;   
	volatile	uint32_t	BWTR2;
	volatile	uint32_t	RESERVED2;   
	volatile	uint32_t	BWTR3;
	volatile	uint32_t	RESERVED3;   
	volatile	uint32_t	BWTR4;
	volatile	uint32_t	RESERVED4;   
} FMC_Bank1E_TypeDef;

typedef struct {
	volatile	uint32_t	PCR2;
	volatile	uint32_t	SR2;
	volatile	uint32_t	PMEM2;
	volatile	uint32_t	PATT2;
	volatile	uint32_t	RESERVED0;   
	volatile	uint32_t	ECCR2; 
} FMC_Bank2_TypeDef;  

typedef struct {
	volatile	uint32_t	PCR3;
	volatile	uint32_t	SR3;
	volatile	uint32_t	PMEM3;
	volatile	uint32_t	PATT3;
	volatile	uint32_t	RESERVED0;   
	volatile	uint32_t	ECCR3; 
} FMC_Bank3_TypeDef; 

typedef struct {
	volatile	uint32_t	PCR4;
	volatile	uint32_t	SR4;
	volatile	uint32_t	PMEM4;
	volatile	uint32_t	PATT4;
	volatile	uint32_t	PIO4; 
} FMC_Bank4_TypeDef; 

typedef struct {
	volatile	uint32_t	SDCR[2];
	volatile	uint32_t	SDTR[2];
	volatile	uint32_t	SDCMR;
	volatile	uint32_t	SDRTR;
	volatile	uint32_t	SDSR; 
} FMC_Bank5_6_TypeDef; 

#define FMC_Bank1	((FMC_Bank1_TypeDef   *)FMC_Bank1_R_BASE)
#define FMC_Bank2	((FMC_Bank2_TypeDef   *)FMC_Bank2_R_BASE)
#define FMC_Bank3	((FMC_Bank3_TypeDef   *)FMC_Bank3_R_BASE)
#define FMC_Bank4	((FMC_Bank4_TypeDef   *)FMC_Bank4_R_BASE)
#define FMC_Bank1E	((FMC_Bank1E_TypeDef  *)FMC_Bank1E_R_BASE)
#define FMC_Bank5_6	((FMC_Bank5_6_TypeDef *)FMC_Bank5_6_R_BASE)

#define	FMC_Bank1_NORSRAM1			0x00000000
#define FMC_Bank1_NORSRAM2			0x00000002
#define FMC_Bank1_NORSRAM3			0x00000004
#define FMC_Bank1_NORSRAM4			0x00000006
#define FMC_Bank2_NAND				0x00000010
#define FMC_Bank3_NAND				0x00000100
#define FMC_Bank4_PCCARD			0x00001000

// FMC_BCR1..4 register

#define	FMC_BCR_MBKEN				0x00000001	//
#define	FMC_BCR_MUXEN				0x00000002	//
#define	FMC_BCR_MTYP 				0x0000000C	//
#define	FMC_BCR_MTYP_0				0x00000004	//
#define	FMC_BCR_MTYP_1				0x00000008	//
#define	FMC_BCR_MWID 				0x00000030	//
#define	FMC_BCR_MWID_0				0x00000010	//
#define	FMC_BCR_MWID_1				0x00000020	//
#define	FMC_BCR_BURSTEN				0x00000100	//
#define	FMC_BCR_WAITPOL				0x00000200	//
#define	FMC_BCR_WRAPMOD				0x00000400	//
#define	FMC_BCR_WAITCFG				0x00000800	//
#define	FMC_BCR_WREN 				0x00001000	//
#define	FMC_BCR_WAITEN				0x00002000	//
#define	FMC_BCR_EXTMOD				0x00004000	//
#define	FMC_BCR_CBURSTRW			0x00080000	//
#define	FMC_BCR_CCLKEN				0x00100000	//

// FMC_BTR1..4 register

#define	FMC_BTR_ADDSET				0x0000000F	//
#define	FMC_BTR_ADDSET_0			0x00000001	//
#define	FMC_BTR_ADDSET_1			0x00000002	//
#define	FMC_BTR_ADDSET_2			0x00000004	//
#define	FMC_BTR_ADDSET_3			0x00000008	//

#define	FMC_BTR_ADDHLD				0x000000F0	//
#define	FMC_BTR_ADDHLD_0			0x00000010	//
#define	FMC_BTR_ADDHLD_1			0x00000020	//
#define	FMC_BTR_ADDHLD_2			0x00000040	//
#define	FMC_BTR_ADDHLD_3			0x00000080	//

#define	FMC_BTR_DATAST				0x0000FF00	//
#define	FMC_BTR_DATAST_0			0x00000100	//
#define	FMC_BTR_DATAST_1			0x00000200	//
#define	FMC_BTR_DATAST_2			0x00000400	//
#define	FMC_BTR_DATAST_3			0x00000800	//
#define	FMC_BTR_DATAST_4			0x00001000	//
#define	FMC_BTR_DATAST_5			0x00002000	//
#define	FMC_BTR_DATAST_6			0x00004000	//
#define	FMC_BTR_DATAST_7			0x00008000	//

#define	FMC_BTR_BUSTURN				0x000F0000	//
#define	FMC_BTR_BUSTURN_0			0x00010000	//
#define	FMC_BTR_BUSTURN_1			0x00020000	//
#define	FMC_BTR_BUSTURN_2			0x00040000	//
#define	FMC_BTR_BUSTURN_3			0x00080000	//

#define	FMC_BTR_CLKDIV				0x00F00000	//
#define	FMC_BTR_CLKDIV_0			0x00100000	//
#define	FMC_BTR_CLKDIV_1			0x00200000	//
#define	FMC_BTR_CLKDIV_2			0x00400000	//
#define	FMC_BTR_CLKDIV_3			0x00800000	//

#define	FMC_BTR_DATLAT				0x0F000000	//
#define	FMC_BTR_DATLAT_0			0x01000000	//
#define	FMC_BTR_DATLAT_1			0x02000000	//
#define	FMC_BTR_DATLAT_2			0x04000000	//
#define	FMC_BTR_DATLAT_3			0x08000000	//

#define	FMC_BTR_ACCMOD				0x30000000	//
#define	FMC_BTR_ACCMOD_0			0x10000000	//
#define	FMC_BTR_ACCMOD_1			0x20000000	//

// FMC_BWTR1..4 register

#define	FMC_BWTR_ADDSET				0x0000000F	//
#define	FMC_BWTR_ADDSET_0			0x00000001	//
#define	FMC_BWTR_ADDSET_1			0x00000002	//
#define	FMC_BWTR_ADDSET_2			0x00000004	//
#define	FMC_BWTR_ADDSET_3			0x00000008	//

#define	FMC_BWTR_ADDHLD				0x000000F0	//
#define	FMC_BWTR_ADDHLD_0			0x00000010	//
#define	FMC_BWTR_ADDHLD_1			0x00000020	//
#define	FMC_BWTR_ADDHLD_2			0x00000040	//
#define	FMC_BWTR_ADDHLD_3			0x00000080	//

#define	FMC_BWTR_DATAST				0x0000FF00	//
#define	FMC_BWTR_DATAST_0			0x00000100	//
#define	FMC_BWTR_DATAST_1			0x00000200	//
#define	FMC_BWTR_DATAST_2			0x00000400	//
#define	FMC_BWTR_DATAST_3			0x00000800	//
#define	FMC_BWTR_DATAST_4			0x00001000	//
#define	FMC_BWTR_DATAST_5			0x00002000	//
#define	FMC_BWTR_DATAST_6			0x00004000	//
#define	FMC_BWTR_DATAST_7			0x00008000	//

#define	FMC_BWTR_CLKDIV				0x00F00000	//
#define	FMC_BWTR_CLKDIV_0			0x00100000	//
#define	FMC_BWTR_CLKDIV_1			0x00200000	//
#define	FMC_BWTR_CLKDIV_2			0x00400000	//
#define	FMC_BWTR_CLKDIV_3			0x00800000	//

#define	FMC_BWTR_DATLAT				0x0F000000	//
#define	FMC_BWTR_DATLAT_0			0x01000000	//
#define	FMC_BWTR_DATLAT_1			0x02000000	//
#define	FMC_BWTR_DATLAT_2			0x04000000	//
#define	FMC_BWTR_DATLAT_3			0x08000000	//

#define	FMC_BWTR_ACCMOD				0x30000000	//
#define	FMC_BWTR_ACCMOD_0			0x10000000	//
#define	FMC_BWTR_ACCMOD_1			0x20000000	//

// FMC_PCR2..4 register

#define	FMC_PCR_PWAITEN				0x00000002	//
#define	FMC_PCR_PBKEN				0x00000004	//
#define	FMC_PCR_PTYP 				0x00000008	//
#define	FMC_PCR_PWID 				0x00000030	//
#define	FMC_PCR_PWID_0				0x00000010	//
#define	FMC_PCR_PWID_1				0x00000020	//
#define	FMC_PCR_ECCEN				0x00000040	//
#define	FMC_PCR_TCLR 				0x00001E00	//
#define	FMC_PCR_TCLR_0				0x00000200	//
#define	FMC_PCR_TCLR_1				0x00000400	//
#define	FMC_PCR_TCLR_2				0x00000800	//
#define	FMC_PCR_TCLR_3				0x00001000	//
#define	FMC_PCR_TAR  				0x0001E000	//
#define	FMC_PCR_TAR_0				0x00002000	//
#define	FMC_PCR_TAR_1				0x00004000	//
#define	FMC_PCR_TAR_2				0x00008000	//
#define	FMC_PCR_TAR_3				0x00010000	//
#define	FMC_PCR_ECCPS				0x000E0000	//
#define	FMC_PCR_ECCPS_0				0x00020000	//
#define	FMC_PCR_ECCPS_1				0x00040000	//
#define	FMC_PCR_ECCPS_2				0x00080000	//

// FMC_SR2..4 register

#define	FMC_SR_IRS					0x01		//
#define	FMC_SR_ILS					0x02		//
#define	FMC_SR_IFS					0x04		//
#define	FMC_SR_IREN					0x08		//
#define	FMC_SR_ILEN					0x10		//
#define	FMC_SR_IFEN					0x20		//
#define	FMC_SR_FEMPT				0x40		//

// FMC_PMEM2..4 register

#define	FMC_PMEM_MEMSET2			0x000000FF	//
#define	FMC_PMEM_MEMSET2_0			0x00000001	//
#define	FMC_PMEM_MEMSET2_1			0x00000002	//
#define	FMC_PMEM_MEMSET2_2			0x00000004	//
#define	FMC_PMEM_MEMSET2_3			0x00000008	//
#define	FMC_PMEM_MEMSET2_4			0x00000010	//
#define	FMC_PMEM_MEMSET2_5			0x00000020	//
#define	FMC_PMEM_MEMSET2_6			0x00000040	//
#define	FMC_PMEM_MEMSET2_7			0x00000080	//

#define	FMC_PMEM_MEMWAIT2			0x0000FF00	//
#define	FMC_PMEM_MEMWAIT2_0			0x00000100	//
#define	FMC_PMEM_MEMWAIT2_1			0x00000200	//
#define	FMC_PMEM_MEMWAIT2_2			0x00000400	//
#define	FMC_PMEM_MEMWAIT2_3			0x00000800	//
#define	FMC_PMEM_MEMWAIT2_4			0x00001000	//
#define	FMC_PMEM_MEMWAIT2_5			0x00002000	//
#define	FMC_PMEM_MEMWAIT2_6			0x00004000	//
#define	FMC_PMEM_MEMWAIT2_7			0x00008000	//

#define	FMC_PMEM_MEMHOLD2			0x00FF0000	//
#define	FMC_PMEM_MEMHOLD2_0			0x00010000	//
#define	FMC_PMEM_MEMHOLD2_1			0x00020000	//
#define	FMC_PMEM_MEMHOLD2_2			0x00040000	//
#define	FMC_PMEM_MEMHOLD2_3			0x00080000	//
#define	FMC_PMEM_MEMHOLD2_4			0x00100000	//
#define	FMC_PMEM_MEMHOLD2_5			0x00200000	//
#define	FMC_PMEM_MEMHOLD2_6			0x00400000	//
#define	FMC_PMEM_MEMHOLD2_7			0x00800000	//

#define	FMC_PMEM_MEMHIZ2			0xFF000000	//
#define	FMC_PMEM_MEMHIZ2_0			0x01000000	//
#define	FMC_PMEM_MEMHIZ2_1			0x02000000	//
#define	FMC_PMEM_MEMHIZ2_2			0x04000000	//
#define	FMC_PMEM_MEMHIZ2_3			0x08000000	//
#define	FMC_PMEM_MEMHIZ2_4			0x10000000	//
#define	FMC_PMEM_MEMHIZ2_5			0x20000000	//
#define	FMC_PMEM_MEMHIZ2_6			0x40000000	//
#define	FMC_PMEM_MEMHIZ2_7			0x80000000	//

// FMC_PATT2..4 register

#define	FMC_PATT_ATTSET2			0x000000FF	//
#define	FMC_PATT_ATTSET2_0			0x00000001	//
#define	FMC_PATT_ATTSET2_1			0x00000002	//
#define	FMC_PATT_ATTSET2_2			0x00000004	//
#define	FMC_PATT_ATTSET2_3			0x00000008	//
#define	FMC_PATT_ATTSET2_4			0x00000010	//
#define	FMC_PATT_ATTSET2_5			0x00000020	//
#define	FMC_PATT_ATTSET2_6			0x00000040	//
#define	FMC_PATT_ATTSET2_7			0x00000080	//

#define	FMC_PATT_ATTWAIT2			0x0000FF00	//
#define	FMC_PATT_ATTWAIT2_0			0x00000100	//
#define	FMC_PATT_ATTWAIT2_1			0x00000200	//
#define	FMC_PATT_ATTWAIT2_2			0x00000400	//
#define	FMC_PATT_ATTWAIT2_3			0x00000800	//
#define	FMC_PATT_ATTWAIT2_4			0x00001000	//
#define	FMC_PATT_ATTWAIT2_5			0x00002000	//
#define	FMC_PATT_ATTWAIT2_6			0x00004000	//
#define	FMC_PATT_ATTWAIT2_7			0x00008000	//

#define	FMC_PATT_ATTHOLD2			0x00FF0000	//
#define	FMC_PATT_ATTHOLD2_0			0x00010000	//
#define	FMC_PATT_ATTHOLD2_1			0x00020000	//
#define	FMC_PATT_ATTHOLD2_2			0x00040000	//
#define	FMC_PATT_ATTHOLD2_3			0x00080000	//
#define	FMC_PATT_ATTHOLD2_4			0x00100000	//
#define	FMC_PATT_ATTHOLD2_5			0x00200000	//
#define	FMC_PATT_ATTHOLD2_6			0x00400000	//
#define	FMC_PATT_ATTHOLD2_7			0x00800000	//

#define	FMC_PATT_ATTHIZ2			0xFF000000	//
#define	FMC_PATT_ATTHIZ2_0			0x01000000	//
#define	FMC_PATT_ATTHIZ2_1			0x02000000	//
#define	FMC_PATT_ATTHIZ2_2			0x04000000	//
#define	FMC_PATT_ATTHIZ2_3			0x08000000	//
#define	FMC_PATT_ATTHIZ2_4			0x10000000	//
#define	FMC_PATT_ATTHIZ2_5			0x20000000	//
#define	FMC_PATT_ATTHIZ2_6			0x40000000	//
#define	FMC_PATT_ATTHIZ2_7			0x80000000	//

// FMC_PIO4 register

#define	FMC_PIO_IOSET4				0x000000FF	//
#define	FMC_PIO_IOSET4_0			0x00000001	//
#define	FMC_PIO_IOSET4_1			0x00000002	//
#define	FMC_PIO_IOSET4_2			0x00000004	//
#define	FMC_PIO_IOSET4_3			0x00000008	//
#define	FMC_PIO_IOSET4_4			0x00000010	//
#define	FMC_PIO_IOSET4_5			0x00000020	//
#define	FMC_PIO_IOSET4_6			0x00000040	//
#define	FMC_PIO_IOSET4_7			0x00000080	//

#define	FMC_PIO_IOWAIT4				0x0000FF00	//
#define	FMC_PIO_IOWAIT4_0			0x00000100	//
#define	FMC_PIO_IOWAIT4_1			0x00000200	//
#define	FMC_PIO_IOWAIT4_2			0x00000400	//
#define	FMC_PIO_IOWAIT4_3			0x00000800	//
#define	FMC_PIO_IOWAIT4_4			0x00001000	//
#define	FMC_PIO_IOWAIT4_5			0x00002000	//
#define	FMC_PIO_IOWAIT4_6			0x00004000	//
#define	FMC_PIO_IOWAIT4_7			0x00008000	//

#define	FMC_PIO_IOHOLD4				0x00FF0000	//
#define	FMC_PIO_IOHOLD4_0			0x00010000	//
#define	FMC_PIO_IOHOLD4_1			0x00020000	//
#define	FMC_PIO_IOHOLD4_2			0x00040000	//
#define	FMC_PIO_IOHOLD4_3			0x00080000	//
#define	FMC_PIO_IOHOLD4_4			0x00100000	//
#define	FMC_PIO_IOHOLD4_5			0x00200000	//
#define	FMC_PIO_IOHOLD4_6			0x00400000	//
#define	FMC_PIO_IOHOLD4_7			0x00800000	//

#define	FMC_PIO_IOHIZ4				0xFF000000	//
#define	FMC_PIO_IOHIZ4_0			0x01000000	//
#define	FMC_PIO_IOHIZ4_1			0x02000000	//
#define	FMC_PIO_IOHIZ4_2			0x04000000	//
#define	FMC_PIO_IOHIZ4_3			0x08000000	//
#define	FMC_PIO_IOHIZ4_4			0x10000000	//
#define	FMC_PIO_IOHIZ4_5			0x20000000	//
#define	FMC_PIO_IOHIZ4_6			0x40000000	//
#define	FMC_PIO_IOHIZ4_7			0x80000000	//

// FMC_ECCR2..3 register

#define	FMC_ECCR					0xFFFFFFFF	//

// FMC_SDCR1..2 register

#define	FMC_SDCR_NC					0x00000003	//
#define	FMC_SDCR_NC_0				0x00000001	//
#define	FMC_SDCR_NC_1				0x00000002	//
#define	FMC_SDCR_NR					0x0000000C	//
#define	FMC_SDCR_NR_0				0x00000004	//
#define	FMC_SDCR_NR_1				0x00000008	//
#define	FMC_SDCR_MWID				0x00000030	//
#define	FMC_SDCR_MWID_0				0x00000010	//
#define	FMC_SDCR_MWID_1				0x00000020	//
#define	FMC_SDCR_NB					0x00000040	//
#define	FMC_SDCR_CAS 				0x00000180	//
#define	FMC_SDCR_CAS_0 				0x00000080	//
#define	FMC_SDCR_CAS_1 				0x00000100	//
#define	FMC_SDCR_WP					0x00000200	//
#define	FMC_SDCR_SDCLK 				0x00000C00	//
#define	FMC_SDCR_SDCLK_0			0x00000400	//
#define	FMC_SDCR_SDCLK_1			0x00000800	//
#define	FMC_SDCR_RBURST				0x00001000	//
#define	FMC_SDCR_RPIPE 				0x00006000	//
#define	FMC_SDCR_RPIPE_0			0x00002000	//
#define	FMC_SDCR_RPIPE_1			0x00004000	//

// FMC_SDTR1..2 register

#define	FMC_SDTR_TMRD				0x0000000F	//
#define	FMC_SDTR_TMRD_0				0x00000001	//
#define	FMC_SDTR_TMRD_1				0x00000002	//
#define	FMC_SDTR_TMRD_2				0x00000004	//
#define	FMC_SDTR_TMRD_3				0x00000008	//
#define	FMC_SDTR_TXSR				0x000000F0	//
#define	FMC_SDTR_TXSR_0				0x00000010	//
#define	FMC_SDTR_TXSR_1				0x00000020	//
#define	FMC_SDTR_TXSR_2				0x00000040	//
#define	FMC_SDTR_TXSR_3				0x00000080	//
#define	FMC_SDTR_TRAS				0x00000F00	//
#define	FMC_SDTR_TRAS_0				0x00000100	//
#define	FMC_SDTR_TRAS_1				0x00000200	//
#define	FMC_SDTR_TRAS_2				0x00000400	//
#define	FMC_SDTR_TRAS_3				0x00000800	//
#define	FMC_SDTR_TRC 				0x0000F000	//
#define	FMC_SDTR_TRC_0 				0x00001000	//
#define	FMC_SDTR_TRC_1 				0x00002000	//
#define	FMC_SDTR_TRC_2 				0x00004000	//
#define	FMC_SDTR_TWR 				0x000F0000	//
#define	FMC_SDTR_TWR_0 				0x00010000	//
#define	FMC_SDTR_TWR_1 				0x00020000	//
#define	FMC_SDTR_TWR_2 				0x00040000	//
#define	FMC_SDTR_TRP 				0x00F00000	//
#define	FMC_SDTR_TRP_0 				0x00100000	//
#define	FMC_SDTR_TRP_1 				0x00200000	//
#define	FMC_SDTR_TRP_2 				0x00400000	//
#define	FMC_SDTR_TRCD				0x0F000000	//
#define	FMC_SDTR_TRCD_0				0x01000000	//
#define	FMC_SDTR_TRCD_1				0x02000000	//
#define	FMC_SDTR_TRCD_2				0x04000000	//

// FMC_SDCMR register

#define	FMC_SDCMR_MODE				0x00000007	//
#define	FMC_SDCMR_MODE_0			0x00000001	//
#define	FMC_SDCMR_MODE_1			0x00000002	//
#define	FMC_SDCMR_MODE_2			0x00000004	//
#define	FMC_SDCMR_CTB2				0x00000008	//
#define	FMC_SDCMR_CTB1				0x00000010	//
#define	FMC_SDCMR_NRFS				0x000001E0	//
#define	FMC_SDCMR_NRFS_0			0x00000020	//
#define	FMC_SDCMR_NRFS_1			0x00000040	//
#define	FMC_SDCMR_NRFS_2			0x00000080	//
#define	FMC_SDCMR_NRFS_3			0x00000100	//
#define	FMC_SDCMR_MRD 				0x003FFE00	//

// FMC_SDRTR register

#define	FMC_SDRTR_CRE 				0x00000001	//
#define	FMC_SDRTR_COUNT 			0x00003FFE	//
#define	FMC_SDRTR_REIE				0x00004000	//

// FMC_SDSR register

#define	FMC_SDSR_RE   				0x00000001	//
#define	FMC_SDSR_MODES1 			0x00000006	//
#define	FMC_SDSR_MODES1_0			0x00000002	//
#define	FMC_SDSR_MODES1_1			0x00000004	//
#define	FMC_SDSR_MODES2 			0x00000018	//
#define	FMC_SDSR_MODES2_0			0x00000008	//
#define	FMC_SDSR_MODES2_1			0x00000010	//
#define	FMC_SDSR_BUSY 				0x00000020	//
#endif
