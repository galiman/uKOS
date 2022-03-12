/*
; stm32L471_pwr.
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
; Goal:		stm32L471_pwr equates.
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

#ifndef __STM32L471_PWR__
#define __STM32L471_PWR__

// PWR address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	CR1;
	volatile	uint32_t	CR2;
	volatile	uint32_t	CR3;
	volatile	uint32_t	CR4;
	volatile	uint32_t	SR1;
	volatile	uint32_t	SR2;
	volatile	uint32_t	SCR;
	volatile	uint32_t	RESERVED;
	volatile	uint32_t	PUCRA;
	volatile	uint32_t	PDCRA;
	volatile	uint32_t	PUCRB;
	volatile	uint32_t	PDCRB;
	volatile	uint32_t	PUCRC;
	volatile	uint32_t	PDCRC;
	volatile	uint32_t	PUCRD;
	volatile	uint32_t	PDCRD;
	volatile	uint32_t	PUCRE;
	volatile	uint32_t	PDCRE;
	volatile	uint32_t	PUCRF;
	volatile	uint32_t	PDCRF;
	volatile	uint32_t	PUCRG;
	volatile	uint32_t	PDCRG;
	volatile	uint32_t	PUCRH;
	volatile	uint32_t	PDCRH;
} PWR_TypeDef;

#define PWR		((PWR_TypeDef *)PWR_BASE)

//  PWR_CR1 register

#define	PWR_CR1_LPR				0x00004000	//
#define	PWR_CR1_VOS				0x00000600	//
#define	PWR_CR1_VOS_0			0x00000200	//
#define	PWR_CR1_VOS_1			0x00000400	//
#define	PWR_CR1_DBP				0x00000100	//
#define	PWR_CR1_LPMS 			0x00000007	//
#define	PWR_CR1_LPMS_STOP0		0x00000000	//
#define	PWR_CR1_LPMS_STOP1		0x00000001	//
#define	PWR_CR1_LPMS_STOP2		0x00000002	//
#define	PWR_CR1_LPMS_STANDBY	0x00000003	//
#define	PWR_CR1_LPMS_SHUTDOWN	0x00000004	//

// PWR_CR2 register

#define	PWR_CR2_IOSV 			0x00000200	//
#define	PWR_CR2_PVME 			0x000000E0	//
#define	PWR_CR2_PVME4			0x00000080	//
#define	PWR_CR2_PVME3			0x00000040	//
#define	PWR_CR2_PVME2			0x00000020	//
#define	PWR_CR2_PLS				0x0000000E	//
#define	PWR_CR2_PLS_LEV0  		0x00000000	//
#define	PWR_CR2_PLS_LEV1  		0x00000002	//
#define	PWR_CR2_PLS_LEV2  		0x00000004	//
#define	PWR_CR2_PLS_LEV3  		0x00000006	//
#define	PWR_CR2_PLS_LEV4  		0x00000008	//
#define	PWR_CR2_PLS_LEV5  		0x0000000A	//
#define	PWR_CR2_PLS_LEV6  		0x0000000C	//
#define	PWR_CR2_PLS_LEV7  		0x0000000E	//
#define	PWR_CR2_PVDE 			0x00000001	//

// PWR_CR3 register

#define	PWR_CR3_EIWF 			0x00008000	//
#define	PWR_CR3_APC				0x00000400	//
#define	PWR_CR3_RRS				0x00000100	//
#define	PWR_CR3_EWUP5			0x00000010	//
#define	PWR_CR3_EWUP4			0x00000008	//
#define	PWR_CR3_EWUP3			0x00000004	//
#define	PWR_CR3_EWUP2			0x00000002	//
#define	PWR_CR3_EWUP1			0x00000001	//
#define	PWR_CR3_EWUP 			0x0000001F	//

// PWR_CR4 register

#define	PWR_CR4_VBRS 			0x00000200	//
#define	PWR_CR4_VBE				0x00000100	//
#define	PWR_CR4_WP5				0x00000010	//
#define	PWR_CR4_WP4				0x00000008	//
#define	PWR_CR4_WP3				0x00000004	//
#define	PWR_CR4_WP2				0x00000002	//
#define	PWR_CR4_WP1				0x00000001	//

// PWR_SR1 register

#define	PWR_SR1_WUFI 			0x00008000	//
#define	PWR_SR1_SBF				0x00000100	//
#define	PWR_SR1_WUF				0x0000001F	//
#define	PWR_SR1_WUF5 			0x00000010	//
#define	PWR_SR1_WUF4 			0x00000008	//
#define	PWR_SR1_WUF3 			0x00000004	//
#define	PWR_SR1_WUF2 			0x00000002	//
#define	PWR_SR1_WUF1 			0x00000001	//

// PWR_SR2 register

#define	PWR_SR2_PVMO4			0x00008000	//
#define	PWR_SR2_PVMO3			0x00004000	//
#define	PWR_SR2_PVMO2			0x00002000	//
#define	PWR_SR2_PVDO 			0x00000800	//
#define	PWR_SR2_VOSF 			0x00000400	//
#define	PWR_SR2_REGLPF    		0x00000200	//
#define	PWR_SR2_REGLPS    		0x00000100	//

// PWR_SCR register

#define	PWR_SCR_CSBF 			0x00000100	//
#define	PWR_SCR_CWUF 			0x0000001F	//
#define	PWR_SCR_CWUF5			0x00000010	//
#define	PWR_SCR_CWUF4			0x00000008	//
#define	PWR_SCR_CWUF3			0x00000004	//
#define	PWR_SCR_CWUF2			0x00000002	//
#define	PWR_SCR_CWUF1			0x00000001	//

// PWR_PUCRA register

#define	PWR_PUCRA_PA15    		0x00008000	//
#define	PWR_PUCRA_PA13    		0x00002000	//
#define	PWR_PUCRA_PA12    		0x00001000	//
#define	PWR_PUCRA_PA11    		0x00000800	//
#define	PWR_PUCRA_PA10    		0x00000400	//
#define	PWR_PUCRA_PA9			0x00000200	//
#define	PWR_PUCRA_PA8			0x00000100	//
#define	PWR_PUCRA_PA7			0x00000080	//
#define	PWR_PUCRA_PA6			0x00000040	//
#define	PWR_PUCRA_PA5			0x00000020	//
#define	PWR_PUCRA_PA4			0x00000010	//
#define	PWR_PUCRA_PA3			0x00000008	//
#define	PWR_PUCRA_PA2			0x00000004	//
#define	PWR_PUCRA_PA1			0x00000002	//
#define	PWR_PUCRA_PA0			0x00000001	//

// PWR_PDCRA register

#define	PWR_PDCRA_PA14    		0x00004000	//
#define	PWR_PDCRA_PA12    		0x00001000	//
#define	PWR_PDCRA_PA11    		0x00000800	//
#define	PWR_PDCRA_PA10    		0x00000400	//
#define	PWR_PDCRA_PA9			0x00000200	//
#define	PWR_PDCRA_PA8			0x00000100	//
#define	PWR_PDCRA_PA7			0x00000080	//
#define	PWR_PDCRA_PA6			0x00000040	//
#define	PWR_PDCRA_PA5			0x00000020	//
#define	PWR_PDCRA_PA4			0x00000010	//
#define	PWR_PDCRA_PA3			0x00000008	//
#define	PWR_PDCRA_PA2			0x00000004	//
#define	PWR_PDCRA_PA1			0x00000002	//
#define	PWR_PDCRA_PA0			0x00000001	//

// PWR_PUCRB register

#define	PWR_PUCRB_PB15    		0x00008000	//
#define	PWR_PUCRB_PB14    		0x00004000	//
#define	PWR_PUCRB_PB13    		0x00002000	//
#define	PWR_PUCRB_PB12    		0x00001000	//
#define	PWR_PUCRB_PB11    		0x00000800	//
#define	PWR_PUCRB_PB10    		0x00000400	//
#define	PWR_PUCRB_PB9			0x00000200	//
#define	PWR_PUCRB_PB8			0x00000100	//
#define	PWR_PUCRB_PB7			0x00000080	//
#define	PWR_PUCRB_PB6			0x00000040	//
#define	PWR_PUCRB_PB5			0x00000020	//
#define	PWR_PUCRB_PB4			0x00000010	//
#define	PWR_PUCRB_PB3			0x00000008	//
#define	PWR_PUCRB_PB2			0x00000004	//
#define	PWR_PUCRB_PB1			0x00000002	//
#define	PWR_PUCRB_PB0			0x00000001	//

// PWR_PDCRB register

#define	PWR_PDCRB_PB15    		0x00008000	//
#define	PWR_PDCRB_PB14    		0x00004000	//
#define	PWR_PDCRB_PB13    		0x00002000	//
#define	PWR_PDCRB_PB12    		0x00001000	//
#define	PWR_PDCRB_PB11    		0x00000800	//
#define	PWR_PDCRB_PB10    		0x00000400	//
#define	PWR_PDCRB_PB9			0x00000200	//
#define	PWR_PDCRB_PB8			0x00000100	//
#define	PWR_PDCRB_PB7			0x00000080	//
#define	PWR_PDCRB_PB6			0x00000040	//
#define	PWR_PDCRB_PB5			0x00000020	//
#define	PWR_PDCRB_PB3			0x00000008	//
#define	PWR_PDCRB_PB2			0x00000004	//
#define	PWR_PDCRB_PB1			0x00000002	//
#define	PWR_PDCRB_PB0			0x00000001	//

// PWR_PUCRC register

#define	PWR_PUCRC_PC15    		0x00008000	//
#define	PWR_PUCRC_PC14    		0x00004000	//
#define	PWR_PUCRC_PC13    		0x00002000	//
#define	PWR_PUCRC_PC12    		0x00001000	//
#define	PWR_PUCRC_PC11    		0x00000800	//
#define	PWR_PUCRC_PC10    		0x00000400	//
#define	PWR_PUCRC_PC9			0x00000200	//
#define	PWR_PUCRC_PC8			0x00000100	//
#define	PWR_PUCRC_PC7			0x00000080	//
#define	PWR_PUCRC_PC6			0x00000040	//
#define	PWR_PUCRC_PC5			0x00000020	//
#define	PWR_PUCRC_PC4			0x00000010	//
#define	PWR_PUCRC_PC3			0x00000008	//
#define	PWR_PUCRC_PC2			0x00000004	//
#define	PWR_PUCRC_PC1			0x00000002	//
#define	PWR_PUCRC_PC0			0x00000001	//

// PWR_PDCRC register

#define	PWR_PDCRC_PC15    		0x00008000	//
#define	PWR_PDCRC_PC14    		0x00004000	//
#define	PWR_PDCRC_PC13    		0x00002000	//
#define	PWR_PDCRC_PC12    		0x00001000	//
#define	PWR_PDCRC_PC11    		0x00000800	//
#define	PWR_PDCRC_PC10    		0x00000400	//
#define	PWR_PDCRC_PC9			0x00000200	//
#define	PWR_PDCRC_PC8			0x00000100	//
#define	PWR_PDCRC_PC7			0x00000080	//
#define	PWR_PDCRC_PC6			0x00000040	//
#define	PWR_PDCRC_PC5			0x00000020	//
#define	PWR_PDCRC_PC4			0x00000010	//
#define	PWR_PDCRC_PC3			0x00000008	//
#define	PWR_PDCRC_PC2			0x00000004	//
#define	PWR_PDCRC_PC1			0x00000002	//
#define	PWR_PDCRC_PC0			0x00000001	//

// PWR_PUCRD register

#define	PWR_PUCRD_PD15    		0x00008000	//
#define	PWR_PUCRD_PD14    		0x00004000	//
#define	PWR_PUCRD_PD13    		0x00002000	//
#define	PWR_PUCRD_PD12    		0x00001000	//
#define	PWR_PUCRD_PD11    		0x00000800	//
#define	PWR_PUCRD_PD10    		0x00000400	//
#define	PWR_PUCRD_PD9			0x00000200	//
#define	PWR_PUCRD_PD8			0x00000100	//
#define	PWR_PUCRD_PD7			0x00000080	//
#define	PWR_PUCRD_PD6			0x00000040	//
#define	PWR_PUCRD_PD5			0x00000020	//
#define	PWR_PUCRD_PD4			0x00000010	//
#define	PWR_PUCRD_PD3			0x00000008	//
#define	PWR_PUCRD_PD2			0x00000004	//
#define	PWR_PUCRD_PD1			0x00000002	//
#define	PWR_PUCRD_PD0			0x00000001	//

// PWR_PDCRD register

#define	PWR_PDCRD_PD15    		0x00008000	//
#define	PWR_PDCRD_PD14    		0x00004000	//
#define	PWR_PDCRD_PD13    		0x00002000	//
#define	PWR_PDCRD_PD12    		0x00001000	//
#define	PWR_PDCRD_PD11    		0x00000800	//
#define	PWR_PDCRD_PD10    		0x00000400	//
#define	PWR_PDCRD_PD9			0x00000200	//
#define	PWR_PDCRD_PD8			0x00000100	//
#define	PWR_PDCRD_PD7			0x00000080	//
#define	PWR_PDCRD_PD6			0x00000040	//
#define	PWR_PDCRD_PD5			0x00000020	//
#define	PWR_PDCRD_PD4			0x00000010	//
#define	PWR_PDCRD_PD3			0x00000008	//
#define	PWR_PDCRD_PD2			0x00000004	//
#define	PWR_PDCRD_PD1			0x00000002	//
#define	PWR_PDCRD_PD0			0x00000001	//

// PWR_PUCRE register

#define	PWR_PUCRE_PE15    		0x00008000	//
#define	PWR_PUCRE_PE14    		0x00004000	//
#define	PWR_PUCRE_PE13    		0x00002000	//
#define	PWR_PUCRE_PE12    		0x00001000	//
#define	PWR_PUCRE_PE11    		0x00000800	//
#define	PWR_PUCRE_PE10    		0x00000400	//
#define	PWR_PUCRE_PE9			0x00000200	//
#define	PWR_PUCRE_PE8			0x00000100	//
#define	PWR_PUCRE_PE7			0x00000080	//
#define	PWR_PUCRE_PE6			0x00000040	//
#define	PWR_PUCRE_PE5			0x00000020	//
#define	PWR_PUCRE_PE4			0x00000010	//
#define	PWR_PUCRE_PE3			0x00000008	//
#define	PWR_PUCRE_PE2			0x00000004	//
#define	PWR_PUCRE_PE1			0x00000002	//
#define	PWR_PUCRE_PE0			0x00000001	//

// PWR_PDCRE register

#define	PWR_PDCRE_PE15    		0x00008000	//
#define	PWR_PDCRE_PE14    		0x00004000	//
#define	PWR_PDCRE_PE13    		0x00002000	//
#define	PWR_PDCRE_PE12    		0x00001000	//
#define	PWR_PDCRE_PE11    		0x00000800	//
#define	PWR_PDCRE_PE10    		0x00000400	//
#define	PWR_PDCRE_PE9			0x00000200	//
#define	PWR_PDCRE_PE8			0x00000100	//
#define	PWR_PDCRE_PE7			0x00000080	//
#define	PWR_PDCRE_PE6			0x00000040	//
#define	PWR_PDCRE_PE5			0x00000020	//
#define	PWR_PDCRE_PE4			0x00000010	//
#define	PWR_PDCRE_PE3			0x00000008	//
#define	PWR_PDCRE_PE2			0x00000004	//
#define	PWR_PDCRE_PE1			0x00000002	//
#define	PWR_PDCRE_PE0			0x00000001	//

// PWR_PUCRF register

#define	PWR_PUCRF_PF15    		0x00008000	//
#define	PWR_PUCRF_PF14    		0x00004000	//
#define	PWR_PUCRF_PF13    		0x00002000	//
#define	PWR_PUCRF_PF12    		0x00001000	//
#define	PWR_PUCRF_PF11    		0x00000800	//
#define	PWR_PUCRF_PF10    		0x00000400	//
#define	PWR_PUCRF_PF9			0x00000200	//
#define	PWR_PUCRF_PF8			0x00000100	//
#define	PWR_PUCRF_PF7			0x00000080	//
#define	PWR_PUCRF_PF6			0x00000040	//
#define	PWR_PUCRF_PF5			0x00000020	//
#define	PWR_PUCRF_PF4			0x00000010	//
#define	PWR_PUCRF_PF3			0x00000008	//
#define	PWR_PUCRF_PF2			0x00000004	//
#define	PWR_PUCRF_PF1			0x00000002	//
#define	PWR_PUCRF_PF0			0x00000001	//

// PWR_PDCRF register

#define	PWR_PDCRF_PF15    		0x00008000	//
#define	PWR_PDCRF_PF14    		0x00004000	//
#define	PWR_PDCRF_PF13    		0x00002000	//
#define	PWR_PDCRF_PF12    		0x00001000	//
#define	PWR_PDCRF_PF11    		0x00000800	//
#define	PWR_PDCRF_PF10    		0x00000400	//
#define	PWR_PDCRF_PF9			0x00000200	//
#define	PWR_PDCRF_PF8			0x00000100	//
#define	PWR_PDCRF_PF7			0x00000080	//
#define	PWR_PDCRF_PF6			0x00000040	//
#define	PWR_PDCRF_PF5			0x00000020	//
#define	PWR_PDCRF_PF4			0x00000010	//
#define	PWR_PDCRF_PF3			0x00000008	//
#define	PWR_PDCRF_PF2			0x00000004	//
#define	PWR_PDCRF_PF1			0x00000002	//
#define	PWR_PDCRF_PF0			0x00000001	//

// PWR_PUCRG register

#define	PWR_PUCRG_PG15    		0x00008000	//
#define	PWR_PUCRG_PG14    		0x00004000	//
#define	PWR_PUCRG_PG13    		0x00002000	//
#define	PWR_PUCRG_PG12    		0x00001000	//
#define	PWR_PUCRG_PG11    		0x00000800	//
#define	PWR_PUCRG_PG10    		0x00000400	//
#define	PWR_PUCRG_PG9			0x00000200	//
#define	PWR_PUCRG_PG8			0x00000100	//
#define	PWR_PUCRG_PG7			0x00000080	//
#define	PWR_PUCRG_PG6			0x00000040	//
#define	PWR_PUCRG_PG5			0x00000020	//
#define	PWR_PUCRG_PG4			0x00000010	//
#define	PWR_PUCRG_PG3			0x00000008	//
#define	PWR_PUCRG_PG2			0x00000004	//
#define	PWR_PUCRG_PG1			0x00000002	//
#define	PWR_PUCRG_PG0			0x00000001	//

// PWR_PDCRG register

#define	PWR_PDCRG_PG15    		0x00008000	//
#define	PWR_PDCRG_PG14    		0x00004000	//
#define	PWR_PDCRG_PG13    		0x00002000	//
#define	PWR_PDCRG_PG12    		0x00001000	//
#define	PWR_PDCRG_PG11    		0x00000800	//
#define	PWR_PDCRG_PG10    		0x00000400	//
#define	PWR_PDCRG_PG9			0x00000200	//
#define	PWR_PDCRG_PG8			0x00000100	//
#define	PWR_PDCRG_PG7			0x00000080	//
#define	PWR_PDCRG_PG6			0x00000040	//
#define	PWR_PDCRG_PG5			0x00000020	//
#define	PWR_PDCRG_PG4			0x00000010	//
#define	PWR_PDCRG_PG3			0x00000008	//
#define	PWR_PDCRG_PG2			0x00000004	//
#define	PWR_PDCRG_PG1			0x00000002	//
#define	PWR_PDCRG_PG0			0x00000001	//

// PWR_PUCRH register

#define	PWR_PUCRH_PH1			0x00000002	//
#define	PWR_PUCRH_PH0			0x00000001	//

// PWR_PDCRH register

#define	PWR_PDCRH_PH1			0x00000002	//
#define	PWR_PDCRH_PH0			0x00000001	//
#endif
