/*
; stm32L476_rtc.
; ==============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 117                  $:  Revision of last commit
; $Date:: 2017-04-23 21:52:03#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stm32L476_rtc equates.
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

#ifndef __STM32L476_RTC__
#define	__STM32L476_RTC__

// RTC address definitions
// -----------------------

typedef struct {
	volatile	uint32_t	TR;
	volatile	uint32_t	DR;
	volatile	uint32_t	CR;
	volatile	uint32_t	ISR;
	volatile	uint32_t	PRER;
	volatile	uint32_t	WUTR;
	volatile	uint32_t	RESERVED;
	volatile	uint32_t	ALRMAR;
	volatile	uint32_t	ALRMBR;
	volatile	uint32_t	WPR;
	volatile	uint32_t	SSR;
	volatile	uint32_t	SHIFTR;
	volatile	uint32_t	TSTR;
	volatile	uint32_t	TSDR;
	volatile	uint32_t	TSSSR;
	volatile	uint32_t	CALR;
	volatile	uint32_t	TAMPCR;
	volatile	uint32_t	ALRMASSR;
	volatile	uint32_t	ALRMBSSR;
	volatile	uint32_t	OR;
	volatile	uint32_t	BKP0R;
	volatile	uint32_t	BKP1R;
	volatile	uint32_t	BKP2R;
	volatile	uint32_t	BKP3R;
	volatile	uint32_t	BKP4R;
	volatile	uint32_t	BKP5R;
	volatile	uint32_t	BKP6R;
	volatile	uint32_t	BKP7R;
	volatile	uint32_t	BKP8R;
	volatile	uint32_t	BKP9R;
	volatile	uint32_t	BKP10R;
	volatile	uint32_t	BKP11R;
	volatile	uint32_t	BKP12R;
	volatile	uint32_t	BKP13R;
	volatile	uint32_t	BKP14R;
	volatile	uint32_t	BKP15R;
	volatile	uint32_t	BKP16R;
	volatile	uint32_t	BKP17R;
	volatile	uint32_t	BKP18R;
	volatile	uint32_t	BKP19R;
	volatile	uint32_t	BKP20R;
	volatile	uint32_t	BKP21R;
	volatile	uint32_t	BKP22R;
	volatile	uint32_t	BKP23R;
	volatile	uint32_t	BKP24R;
	volatile	uint32_t	BKP25R;
	volatile	uint32_t	BKP26R;
	volatile	uint32_t	BKP27R;
	volatile	uint32_t	BKP28R;
	volatile	uint32_t	BKP29R;
	volatile	uint32_t	BKP30R;
	volatile	uint32_t	BKP31R;
} RTC_TypeDef;

#define	RTC		((RTC_TypeDef *)RTC_BASE)	//

// RTC_TR register

#define	RTC_TR_PM				0x00400000	//
#define	RTC_TR_HT				0x00300000	//
#define	RTC_TR_HT_0				0x00100000	//
#define	RTC_TR_HT_1				0x00200000	//
#define	RTC_TR_HU				0x000F0000	//
#define	RTC_TR_HU_0				0x00010000	//
#define	RTC_TR_HU_1				0x00020000	//
#define	RTC_TR_HU_2				0x00040000	//
#define	RTC_TR_HU_3				0x00080000	//
#define	RTC_TR_MNT 				0x00007000	//
#define	RTC_TR_MNT_0			0x00001000	//
#define	RTC_TR_MNT_1			0x00002000	//
#define	RTC_TR_MNT_2			0x00004000	//
#define	RTC_TR_MNU 				0x00000F00	//
#define	RTC_TR_MNU_0			0x00000100	//
#define	RTC_TR_MNU_1			0x00000200	//
#define	RTC_TR_MNU_2			0x00000400	//
#define	RTC_TR_MNU_3			0x00000800	//
#define	RTC_TR_ST				0x00000070	//
#define	RTC_TR_ST_0				0x00000010	//
#define	RTC_TR_ST_1				0x00000020	//
#define	RTC_TR_ST_2				0x00000040	//
#define	RTC_TR_SU				0x0000000F	//
#define	RTC_TR_SU_0				0x00000001	//
#define	RTC_TR_SU_1				0x00000002	//
#define	RTC_TR_SU_2				0x00000004	//
#define	RTC_TR_SU_3				0x00000008	//

// RTC_DR register

#define	RTC_DR_YT				0x00F00000	//
#define	RTC_DR_YT_0				0x00100000	//
#define	RTC_DR_YT_1				0x00200000	//
#define	RTC_DR_YT_2				0x00400000	//
#define	RTC_DR_YT_3				0x00800000	//
#define	RTC_DR_YU				0x000F0000	//
#define	RTC_DR_YU_0				0x00010000	//
#define	RTC_DR_YU_1				0x00020000	//
#define	RTC_DR_YU_2				0x00040000	//
#define	RTC_DR_YU_3				0x00080000	//
#define	RTC_DR_WDU 				0x0000E000	//
#define	RTC_DR_WDU_0			0x00002000	//
#define	RTC_DR_WDU_1			0x00004000	//
#define	RTC_DR_WDU_2			0x00008000	//
#define	RTC_DR_MT				0x00001000	//
#define	RTC_DR_MU				0x00000F00	//
#define	RTC_DR_MU_0				0x00000100	//
#define	RTC_DR_MU_1				0x00000200	//
#define	RTC_DR_MU_2				0x00000400	//
#define	RTC_DR_MU_3				0x00000800	//
#define	RTC_DR_DT				0x00000030	//
#define	RTC_DR_DT_0				0x00000010	//
#define	RTC_DR_DT_1				0x00000020	//
#define	RTC_DR_DU				0x0000000F	//
#define	RTC_DR_DU_0				0x00000001	//
#define	RTC_DR_DU_1				0x00000002	//
#define	RTC_DR_DU_2				0x00000004	//
#define	RTC_DR_DU_3				0x00000008	//

// RTC_CR register

#define RTC_CR_ITSE				0x01000000	//
#define RTC_CR_COE				0x00800000	//
#define RTC_CR_OSEL				0x00600000	//
#define RTC_CR_OSEL_0			0x00200000	//
#define RTC_CR_OSEL_1			0x00400000	//
#define RTC_CR_POL				0x00100000	//
#define RTC_CR_COSEL			0x00080000	//
#define RTC_CR_BKP				0x00040000	//
#define RTC_CR_SUB1H			0x00020000	//
#define RTC_CR_ADD1H			0x00010000	//
#define RTC_CR_TSIE				0x00008000	//
#define RTC_CR_WUTIE			0x00004000	//
#define RTC_CR_ALRBIE			0x00002000	//
#define RTC_CR_ALRAIE			0x00001000	//
#define RTC_CR_TSE				0x00000800	//
#define RTC_CR_WUTE				0x00000400	//
#define RTC_CR_ALRBE			0x00000200	//
#define RTC_CR_ALRAE			0x00000100	//
#define RTC_CR_FMT				0x00000040	//
#define RTC_CR_BYPSHAD			0x00000020	//
#define RTC_CR_REFCKON			0x00000010	//
#define RTC_CR_TSEDGE			0x00000008	//
#define RTC_CR_WUCKSEL			0x00000007	//
#define RTC_CR_WUCKSEL_0		0x00000001	//
#define RTC_CR_WUCKSEL_1		0x00000002	//
#define RTC_CR_WUCKSEL_2		0x00000004	//

// RTC_ISR register
	
#define RTC_ISR_ITSF    		0x00020000	//
#define RTC_ISR_RECALPF 		0x00010000	//
#define RTC_ISR_TAMP3F  		0x00008000	//
#define RTC_ISR_TAMP2F  		0x00004000	//
#define RTC_ISR_TAMP1F  		0x00002000	//
#define RTC_ISR_TSOVF   		0x00001000	//
#define RTC_ISR_TSF     		0x00000800	//
#define RTC_ISR_WUTF    		0x00000400	//
#define RTC_ISR_ALRBF   		0x00000200	//
#define RTC_ISR_ALRAF   		0x00000100	//
#define RTC_ISR_INIT    		0x00000080	//
#define RTC_ISR_INITF   		0x00000040	//
#define RTC_ISR_RSF     		0x00000020	//
#define RTC_ISR_INITS   		0x00000010	//
#define RTC_ISR_SHPF    		0x00000008	//
#define RTC_ISR_WUTWF   		0x00000004	//
#define RTC_ISR_ALRBWF  		0x00000002	//
#define RTC_ISR_ALRAWF  		0x00000001	//

// RTC_PRER register

#define	RTC_PRER_PREDIV_A		0x007F0000	//
#define	RTC_PRER_PREDIV_S		0x00007FFF	//

// RTC_WUTR register

#define	RTC_WUTR_WUT			0x0000FFFF	//

// RTC_ALRMAR register

#define	RTC_ALRMAR_MSK4 		0x80000000	//
#define	RTC_ALRMAR_WDSEL		0x40000000	//
#define	RTC_ALRMAR_DT			0x30000000	//
#define	RTC_ALRMAR_DT_0 		0x10000000	//
#define	RTC_ALRMAR_DT_1 		0x20000000	//
#define	RTC_ALRMAR_DU			0x0F000000	//
#define	RTC_ALRMAR_DU_0 		0x01000000	//
#define	RTC_ALRMAR_DU_1 		0x02000000	//
#define	RTC_ALRMAR_DU_2 		0x04000000	//
#define	RTC_ALRMAR_DU_3 		0x08000000	//
#define	RTC_ALRMAR_MSK3 		0x00800000	//
#define	RTC_ALRMAR_PM			0x00400000	//
#define	RTC_ALRMAR_HT			0x00300000	//
#define	RTC_ALRMAR_HT_0 		0x00100000	//
#define	RTC_ALRMAR_HT_1 		0x00200000	//
#define	RTC_ALRMAR_HU			0x000F0000	//
#define	RTC_ALRMAR_HU_0 		0x00010000	//
#define	RTC_ALRMAR_HU_1 		0x00020000	//
#define	RTC_ALRMAR_HU_2 		0x00040000	//
#define	RTC_ALRMAR_HU_3 		0x00080000	//
#define	RTC_ALRMAR_MSK2 		0x00008000	//
#define	RTC_ALRMAR_MNT			0x00007000	//
#define	RTC_ALRMAR_MNT_0		0x00001000	//
#define	RTC_ALRMAR_MNT_1		0x00002000	//
#define	RTC_ALRMAR_MNT_2		0x00004000	//
#define	RTC_ALRMAR_MNU			0x00000F00	//
#define	RTC_ALRMAR_MNU_0		0x00000100	//
#define	RTC_ALRMAR_MNU_1		0x00000200	//
#define	RTC_ALRMAR_MNU_2		0x00000400	//
#define	RTC_ALRMAR_MNU_3		0x00000800	//
#define	RTC_ALRMAR_MSK1 		0x00000080	//
#define	RTC_ALRMAR_ST			0x00000070	//
#define	RTC_ALRMAR_ST_0 		0x00000010	//
#define	RTC_ALRMAR_ST_1 		0x00000020	//
#define	RTC_ALRMAR_ST_2 		0x00000040	//
#define	RTC_ALRMAR_SU			0x0000000F	//
#define	RTC_ALRMAR_SU_0 		0x00000001	//
#define	RTC_ALRMAR_SU_1 		0x00000002	//
#define	RTC_ALRMAR_SU_2 		0x00000004	//
#define	RTC_ALRMAR_SU_3 		0x00000008	//

// RTC_ALRMBR register

#define	RTC_ALRMBR_MSK4 		0x80000000	//
#define	RTC_ALRMBR_WDSEL		0x40000000	//
#define	RTC_ALRMBR_DT			0x30000000	//
#define	RTC_ALRMBR_DT_0 		0x10000000	//
#define	RTC_ALRMBR_DT_1 		0x20000000	//
#define	RTC_ALRMBR_DU			0x0F000000	//
#define	RTC_ALRMBR_DU_0 		0x01000000	//
#define	RTC_ALRMBR_DU_1 		0x02000000	//
#define	RTC_ALRMBR_DU_2 		0x04000000	//
#define	RTC_ALRMBR_DU_3 		0x08000000	//
#define	RTC_ALRMBR_MSK3 		0x00800000	//
#define	RTC_ALRMBR_PM			0x00400000	//
#define	RTC_ALRMBR_HT			0x00300000	//
#define	RTC_ALRMBR_HT_0 		0x00100000	//
#define	RTC_ALRMBR_HT_1 		0x00200000	//
#define	RTC_ALRMBR_HU			0x000F0000	//
#define	RTC_ALRMBR_HU_0 		0x00010000	//
#define	RTC_ALRMBR_HU_1 		0x00020000	//
#define	RTC_ALRMBR_HU_2 		0x00040000	//
#define	RTC_ALRMBR_HU_3 		0x00080000	//
#define	RTC_ALRMBR_MSK2 		0x00008000	//
#define	RTC_ALRMBR_MNT			0x00007000	//
#define	RTC_ALRMBR_MNT_0		0x00001000	//
#define	RTC_ALRMBR_MNT_1		0x00002000	//
#define	RTC_ALRMBR_MNT_2		0x00004000	//
#define	RTC_ALRMBR_MNU			0x00000F00	//
#define	RTC_ALRMBR_MNU_0		0x00000100	//
#define	RTC_ALRMBR_MNU_1		0x00000200	//
#define	RTC_ALRMBR_MNU_2		0x00000400	//
#define	RTC_ALRMBR_MNU_3		0x00000800	//
#define	RTC_ALRMBR_MSK1 		0x00000080	//
#define	RTC_ALRMBR_ST			0x00000070	//
#define	RTC_ALRMBR_ST_0 		0x00000010	//
#define	RTC_ALRMBR_ST_1 		0x00000020	//
#define	RTC_ALRMBR_ST_2 		0x00000040	//
#define	RTC_ALRMBR_SU			0x0000000F	//
#define	RTC_ALRMBR_SU_0 		0x00000001	//
#define	RTC_ALRMBR_SU_1 		0x00000002	//
#define	RTC_ALRMBR_SU_2 		0x00000004	//
#define	RTC_ALRMBR_SU_3 		0x00000008	//

// RTC_WPR register

#define	RTC_WPR_KEY				0x000000FF	//

// RTC_SSR register

#define	RTC_SSR_SS 				0x0000FFFF	//

// RTC_SHIFTR register

#define	RTC_SHIFTR_SUBFS		0x00007FFF	//
#define	RTC_SHIFTR_ADD1S		0x80000000	//

// RTC_TSTR register

#define	RTC_TSTR_PM				0x00400000	//
#define	RTC_TSTR_HT				0x00300000	//
#define	RTC_TSTR_HT_0			0x00100000	//
#define	RTC_TSTR_HT_1			0x00200000	//
#define	RTC_TSTR_HU				0x000F0000	//
#define	RTC_TSTR_HU_0			0x00010000	//
#define	RTC_TSTR_HU_1			0x00020000	//
#define	RTC_TSTR_HU_2			0x00040000	//
#define	RTC_TSTR_HU_3			0x00080000	//
#define	RTC_TSTR_MNT			0x00007000	//
#define	RTC_TSTR_MNT_0			0x00001000	//
#define	RTC_TSTR_MNT_1			0x00002000	//
#define	RTC_TSTR_MNT_2			0x00004000	//
#define	RTC_TSTR_MNU			0x00000F00	//
#define	RTC_TSTR_MNU_0			0x00000100	//
#define	RTC_TSTR_MNU_1			0x00000200	//
#define	RTC_TSTR_MNU_2			0x00000400	//
#define	RTC_TSTR_MNU_3			0x00000800	//
#define	RTC_TSTR_ST				0x00000070	//
#define	RTC_TSTR_ST_0			0x00000010	//
#define	RTC_TSTR_ST_1			0x00000020	//
#define	RTC_TSTR_ST_2			0x00000040	//
#define	RTC_TSTR_SU				0x0000000F	//
#define	RTC_TSTR_SU_0			0x00000001	//
#define	RTC_TSTR_SU_1			0x00000002	//
#define	RTC_TSTR_SU_2			0x00000004	//
#define	RTC_TSTR_SU_3			0x00000008	//

// RTC_TSDR register

#define	RTC_TSDR_WDU			0x0000E000	//
#define	RTC_TSDR_WDU_0			0x00002000	//
#define	RTC_TSDR_WDU_1			0x00004000	//
#define	RTC_TSDR_WDU_2			0x00008000	//
#define	RTC_TSDR_MT				0x00001000	//
#define	RTC_TSDR_MU				0x00000F00	//
#define	RTC_TSDR_MU_0			0x00000100	//
#define	RTC_TSDR_MU_1			0x00000200	//
#define	RTC_TSDR_MU_2			0x00000400	//
#define	RTC_TSDR_MU_3			0x00000800	//
#define	RTC_TSDR_DT				0x00000030	//
#define	RTC_TSDR_DT_0			0x00000010	//
#define	RTC_TSDR_DT_1			0x00000020	//
#define	RTC_TSDR_DU				0x0000000F	//
#define	RTC_TSDR_DU_0			0x00000001	//
#define	RTC_TSDR_DU_1			0x00000002	//
#define	RTC_TSDR_DU_2			0x00000004	//
#define	RTC_TSDR_DU_3			0x00000008	//

// RTC_TSSSR register

#define	RTC_TSSSR_SS			0x0000FFFF	//

// RTC_CAL register

#define	RTC_CALR_CALP			0x00008000	//
#define	RTC_CALR_CALW8			0x00004000	//
#define	RTC_CALR_CALW16 		0x00002000	//
#define	RTC_CALR_CALM			0x000001FF	//
#define	RTC_CALR_CALM_0 		0x00000001	//
#define	RTC_CALR_CALM_1 		0x00000002	//
#define	RTC_CALR_CALM_2 		0x00000004	//
#define	RTC_CALR_CALM_3 		0x00000008	//
#define	RTC_CALR_CALM_4 		0x00000010	//
#define	RTC_CALR_CALM_5 		0x00000020	//
#define	RTC_CALR_CALM_6 		0x00000040	//
#define	RTC_CALR_CALM_7 		0x00000080	//
#define	RTC_CALR_CALM_8 		0x00000100	//

// RTC_TAMPCR register
	
#define RTC_TAMPCR_TAMP3MF     	0x01000000	//
#define RTC_TAMPCR_TAMP3NOERASE	0x00800000	//
#define RTC_TAMPCR_TAMP3IE     	0x00400000	//
#define RTC_TAMPCR_TAMP2MF     	0x00200000	//
#define RTC_TAMPCR_TAMP2NOERASE	0x00100000	//
#define RTC_TAMPCR_TAMP2IE     	0x00080000	//
#define RTC_TAMPCR_TAMP1MF     	0x00040000	//
#define RTC_TAMPCR_TAMP1NOERASE	0x00020000	//
#define RTC_TAMPCR_TAMP1IE     	0x00010000	//
#define RTC_TAMPCR_TAMPPUDIS   	0x00008000	//
#define RTC_TAMPCR_TAMPPRCH    	0x00006000	//
#define RTC_TAMPCR_TAMPPRCH_0  	0x00002000	//
#define RTC_TAMPCR_TAMPPRCH_1  	0x00004000	//
#define RTC_TAMPCR_TAMPFLT     	0x00001800	//
#define RTC_TAMPCR_TAMPFLT_0   	0x00000800	//
#define RTC_TAMPCR_TAMPFLT_1   	0x00001000	//
#define RTC_TAMPCR_TAMPFREQ    	0x00000700	//
#define RTC_TAMPCR_TAMPFREQ_0  	0x00000100	//
#define RTC_TAMPCR_TAMPFREQ_1  	0x00000200	//
#define RTC_TAMPCR_TAMPFREQ_2  	0x00000400	//
#define RTC_TAMPCR_TAMPTS      	0x00000080	//
#define RTC_TAMPCR_TAMP3_TRG   	0x00000040	//
#define RTC_TAMPCR_TAMP3E      	0x00000020	//
#define RTC_TAMPCR_TAMP2_TRG   	0x00000010	//
#define RTC_TAMPCR_TAMP2E      	0x00000008	//
#define RTC_TAMPCR_TAMPIE      	0x00000004	//
#define RTC_TAMPCR_TAMP1_TRG   	0x00000002	//
#define RTC_TAMPCR_TAMP1E      	0x00000001	//

// RTC_ALRMASSR register

#define	RTC_ALRMASSR_MASKSS   	0x0F000000	//
#define	RTC_ALRMASSR_MASKSS_0 	0x01000000	//
#define	RTC_ALRMASSR_MASKSS_1 	0x02000000	//
#define	RTC_ALRMASSR_MASKSS_2 	0x04000000	//
#define	RTC_ALRMASSR_MASKSS_3 	0x08000000	//
#define	RTC_ALRMASSR_SS 		0x00007FFF	//

// RTC_ALRMBSSR register

#define	RTC_ALRMBSSR_MASKSS   	0x0F000000	//
#define	RTC_ALRMBSSR_MASKSS_0 	0x01000000	//
#define	RTC_ALRMBSSR_MASKSS_1 	0x02000000	//
#define	RTC_ALRMBSSR_MASKSS_2 	0x04000000	//
#define	RTC_ALRMBSSR_MASKSS_3 	0x08000000	//
#define	RTC_ALRMBSSR_SS 		0x00007FFF	//

// RTC_OR register

#define RTC_OR_OUT_RMP			0x00000002	//
#define RTC_OR_ALARMOUTTYPE 	0x00000001	//

// RTC_BKPxxR register

#define	RTC_BKP0R				0xFFFFFFFF	//
#define	RTC_BKP1R				0xFFFFFFFF	//
#define	RTC_BKP2R				0xFFFFFFFF	//
#define	RTC_BKP3R				0xFFFFFFFF	//
#define	RTC_BKP4R				0xFFFFFFFF	//
#define	RTC_BKP5R				0xFFFFFFFF	//
#define	RTC_BKP6R				0xFFFFFFFF	//
#define	RTC_BKP7R				0xFFFFFFFF	//
#define	RTC_BKP8R				0xFFFFFFFF	//
#define	RTC_BKP9R				0xFFFFFFFF	//
#define	RTC_BKP10R 				0xFFFFFFFF	//
#define	RTC_BKP11R 				0xFFFFFFFF	//
#define	RTC_BKP12R 				0xFFFFFFFF	//
#define	RTC_BKP13R 				0xFFFFFFFF	//
#define	RTC_BKP14R 				0xFFFFFFFF	//
#define	RTC_BKP15R 				0xFFFFFFFF	//
#define	RTC_BKP16R 				0xFFFFFFFF	//
#define	RTC_BKP17R 				0xFFFFFFFF	//
#define	RTC_BKP18R 				0xFFFFFFFF	//
#define	RTC_BKP19R 				0xFFFFFFFF	//
#define	RTC_BKP20R 				0xFFFFFFFF	//
#define	RTC_BKP21R 				0xFFFFFFFF	//
#define	RTC_BKP22R 				0xFFFFFFFF	//
#define	RTC_BKP23R 				0xFFFFFFFF	//
#define	RTC_BKP24R 				0xFFFFFFFF	//
#define	RTC_BKP25R 				0xFFFFFFFF	//
#define	RTC_BKP26R 				0xFFFFFFFF	//
#define	RTC_BKP27R 				0xFFFFFFFF	//
#define	RTC_BKP28R 				0xFFFFFFFF	//
#define	RTC_BKP29R 				0xFFFFFFFF	//
#define	RTC_BKP30R 				0xFFFFFFFF	//
#define	RTC_BKP31R 				0xFFFFFFFF	//
#endif
