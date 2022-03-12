/*
; stm32F429_usb.
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
; Goal:		stm32F429_usb equates.
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

#ifndef __STM32F429_USB__
#define __STM32F429_USB__

#define	USB_OTG_GLOBAL_BASE			0x000			//
#define	USB_OTG_DEVICE_BASE			0x800			//
#define	USB_OTG_IN_ENDPOINT_BASE	0x900			//
#define	USB_OTG_OUT_ENDPOINT_BASE	0xB00			//
#define	USB_OTG_EP_REG_SIZE			0x20			//
#define	USB_OTG_HOST_BASE			0x400			//
#define	USB_OTG_HOST_PORT_BASE		0x440			//
#define	USB_OTG_HOST_CHANNEL_BASE	0x500			//
#define	USB_OTG_HOST_CHANNEL_SIZE	0x20			//
#define	USB_OTG_PCGCCTL_BASE		0xE00			//
#define	USB_OTG_FIFO_BASE			0x1000			//
#define	USB_OTG_FIFO_SIZE			0x1000			// USB OTG base address

// USB_OTG_Core address definitions
// --------------------------------

typedef struct {
	volatile	uint32_t	GOTGCTL;				// USB_OTG Control and Status Register       0x00
	volatile	uint32_t	GOTGINT;				// USB_OTG Interrupt Register                0x04
	volatile	uint32_t	GAHBCFG;				// Core AHB Configuration Register           0x08
	volatile	uint32_t	GUSBCFG;				// Core USB Configuration Register           0x0C
	volatile	uint32_t	GRSTCTL;				// Core Reset Register                       0x10
	volatile	uint32_t	GINTSTS;				// Core Interrupt Register                   0x14
	volatile	uint32_t	GINTMSK;				// Core Interrupt Mask Register              0x18
	volatile	uint32_t	GRXSTSR;				// Receive Sts Q Read Register               0x1C
	volatile	uint32_t	GRXSTSP;				// Receive Sts Q Read & POP Register         0x20
	volatile	int32_t		GRXFSIZ;				// Receive FIFO Size Register                0x24
	volatile	uint32_t	DIEPTXF0_HNPTXFSIZ;		// EP0 / Non Periodic Tx FIFO Size Register  0x28
	volatile	uint32_t	HNPTXSTS;				// Non Periodic Tx FIFO/Queue Sts reg        0x2C
	volatile	uint32_t	Reserved30[2];			// Reserved                                  0x30
	volatile	uint32_t	GCCFG;					// General Purpose IO Register               0x38
	volatile	uint32_t	CID;					// User ID Register                          0x3C
	volatile	uint32_t	Reserved40[48];			// Reserved                                  0x40-0xFF
	volatile	uint32_t	HPTXFSIZ;				// Host Periodic Tx FIFO Size Reg            0x100
	volatile	uint32_t	DIEPTXF[0x0F];			// Dev Periodic Transmit FIFO
} USB_OTG_GlobalTypeDef;

// device_Registers address definitions
// ------------------------------------

typedef struct  {
	volatile	uint32_t	DCFG;					// Dev Configuration Register   			 0x800
	volatile	uint32_t	DCTL;					// Dev Control Register         			 0x804
	volatile	uint32_t	DSTS;					// Dev Status Register (RO)     			 0x808
	volatile	uint32_t	Reserved0C;				// Reserved                     			 0x80C
	volatile	uint32_t	DIEPMSK;				// Dev IN Endpoint Mask         			 0x810
	volatile	uint32_t	DOEPMSK;				// Dev OUT Endpoint Mask        			 0x814
	volatile	uint32_t	DAINT;					// Dev All Endpoints Itr Reg    			 0x818
	volatile	uint32_t	DAINTMSK;				// Dev All Endpoints Itr Mask   			 0x81C
	volatile	uint32_t	Reserved20;				// Reserved                     			 0x820
	volatile	uint32_t	Reserved9;				// Reserved                     			 0x824
	volatile	uint32_t	DVBUSDIS;				// Dev VBUS discharge Register  			 0x828
	volatile	uint32_t	DVBUSPULSE;				// Dev VBUS Pulse Register      			 0x82C
	volatile	uint32_t	DTHRCTL;				// Dev thr                      			 0x830
	volatile	uint32_t	DIEPEMPMSK;				// Dev empty msk             				 0x834
	volatile	uint32_t	DEACHINT;				// Dedicated EP interrupt       			 0x838
	volatile	uint32_t	DEACHMSK;				// Dedicated EP msk             			 0x83C
	volatile	uint32_t	Reserved40;				// Dedicated EP mask           				 0x840
	volatile	uint32_t	DINEP1MSK;				// Dedicated EP mask           				 0x844
	volatile	uint32_t	Reserved44[15];			// Reserved                 				 0x844-0x87C
	volatile	uint32_t	DOUTEP1MSK;				// dedicated EP msk            				 0x884
} USB_OTG_DeviceTypeDef;

// IN_Endpoint-Specific_Register address definitions
// -------------------------------------------------

typedef struct  {
	volatile	uint32_t	DIEPCTL;				// Dev IN Endpoint Control Reg               0x900 + (ep_num * 0x20) + 0x00
	volatile	uint32_t	Reserved04;				// Reserved                                  0x900 + (ep_num * 0x20) + 0x04
	volatile	uint32_t	DIEPINT;				// Dev IN Endpoint Itr Reg                   0x900 + (ep_num * 0x20) + 0x08
	volatile	uint32_t	Reserved0C;				// Reserved                                  0x900 + (ep_num * 0x20) + 0x0C
	volatile	uint32_t	DIEPTSIZ;				// IN Endpoint Txfer Size                    0x900 + (ep_num * 0x20) + 0x10
	volatile	uint32_t	DIEPDMA;				// IN Endpoint DMA Address Reg               0x900 + (ep_num * 0x20) + 0x14
	volatile	uint32_t	DTXFSTS;				// IN Endpoint Tx FIFO Status Reg            0x900 + (ep_num * 0x20) + 0x18
	volatile	uint32_t	Reserved18;				// Reserved 0x900 + (ep_num * 0x20) + 0x1C - 0x900 + (ep_num * 0x20) + 0x1C
} USB_OTG_INEndpointTypeDef;

// OUT_Endpoint-Specific_Registers address definitions
// ---------------------------------------------------

typedef struct  {
	volatile	uint32_t	DOEPCTL;				// Dev OUT Endpoint Control Reg              0xB00 + (ep_num * 0x20) + 0x00
	volatile	uint32_t	Reserved04;				// Reserved                                  0xB00 + (ep_num * 0x20) + 0x04
	volatile	uint32_t	DOEPINT;				// Dev OUT Endpoint Itr Reg                  0xB00 + (ep_num * 0x20) + 0x08
	volatile	uint32_t	Reserved0C;				// Reserved                                  0xB00 + (ep_num * 0x20) + 0x0C
	volatile	uint32_t	DOEPTSIZ;				// Dev OUT Endpoint Txfer Size               0xB00 + (ep_num * 0x20) + 0x10
	volatile	uint32_t	DOEPDMA;				// Dev OUT Endpoint DMA Address              0xB00 + (ep_num * 0x20) + 0x14
	volatile	uint32_t	Reserved18[2];			// Reserved 0xB00 + (ep_num * 0x20) + 0x18 - 0xB00 + (ep_num * 0x20) + 0x1C
} USB_OTG_OUTEndpointTypeDef;

// Host_Mode_Register_Structures address definitions
// -------------------------------------------------

typedef struct  {
	volatile	uint32_t	HCFG;					// Host Configuration Register          	 0x400
	volatile	uint32_t	HFIR;					// Host Frame Interval Register         	 0x404
	volatile	uint32_t	HFNUM;					// Host Frame Nbr/Frame Remaining       	 0x408
	volatile	uint32_t	Reserved40C;			// Reserved                             	 0x40C
	volatile	uint32_t	HPTXSTS;				// Host Periodic Tx FIFO/ Queue Status  	 0x410
	volatile	uint32_t	HAINT;					// Host All Channels Interrupt Register 	 0x414
	volatile	uint32_t	HAINTMSK;				// Host All Channels Interrupt Mask     	 0x418
} USB_OTG_HostTypeDef;

// Host_Channel_Specific_Registers address definitions
// ---------------------------------------------------

typedef struct {
	volatile	uint32_t	HCCHAR;					//
	volatile	uint32_t	HCSPLT;					//
	volatile	uint32_t	HCINT;					//
	volatile	uint32_t	HCINTMSK;				//
	volatile	uint32_t	HCTSIZ;					//
	volatile	uint32_t	HCDMA;					//
	volatile	uint32_t	Reserved[2];			//
} USB_OTG_HostChannelTypeDef;

#define	USB_OTG_FS		((USB_OTG_GlobalTypeDef *)      USB_OTG_FS_PERIPH_BASE)
#define	USB_OTG_HS		((USB_OTG_GlobalTypeDef *)      USB_OTG_HS_PERIPH_BASE)

#define USBx 			USB_OTG_FS_PERIPH_BASE
#define USBx_DEVICE     ((USB_OTG_DeviceTypeDef *)      ((uint32_t)USBx + USB_OTG_DEVICE_BASE)) 
#define USBx_INEP(i)    ((USB_OTG_INEndpointTypeDef *)  ((uint32_t)USBx + USB_OTG_IN_ENDPOINT_BASE  + (i)*USB_OTG_EP_REG_SIZE))        
#define USBx_OUTEP(i)   ((USB_OTG_OUTEndpointTypeDef *) ((uint32_t)USBx + USB_OTG_OUT_ENDPOINT_BASE + (i)*USB_OTG_EP_REG_SIZE))        
#define USBx_DFIFO(i)   *(volatile uint32_t *)          ((uint32_t)USBx + USB_OTG_FIFO_BASE         + (i)*USB_OTG_FIFO_SIZE)
#define USBx_HOST       ((USB_OTG_HostTypeDef *)        ((uint32_t)USBx + USB_OTG_HOST_BASE))  
#define USBx_HC(i)      ((USB_OTG_HostChannelTypeDef *) ((uint32_t)USBx + USB_OTG_HOST_CHANNEL_BASE + (i)*USB_OTG_HOST_CHANNEL_SIZE))
#define USBx_PCGCR		*(volatile uint32_t *)          ((uint32_t)USBx + USB_OTG_PCGCCTL_BASE)

// USB_OTG_GOTGCTL register

#define	USB_OTG_GOTGCTL_SRQSCS					0x00000001		//
#define	USB_OTG_GOTGCTL_SRQ						0x00000002		//
#define	USB_OTG_GOTGCTL_HNGSCS					0x00000100		//
#define	USB_OTG_GOTGCTL_HNPRQ					0x00000200		//
#define	USB_OTG_GOTGCTL_HSHNPEN					0x00000400		//
#define	USB_OTG_GOTGCTL_DHNPEN					0x00000800		//
#define	USB_OTG_GOTGCTL_CIDSTS					0x00010000		//
#define	USB_OTG_GOTGCTL_DBCT					0x00020000		//
#define	USB_OTG_GOTGCTL_ASVLD					0x00040000		//
#define	USB_OTG_GOTGCTL_BSVLD					0x00080000		//

// USB_OTG_HCFG register

#define	USB_OTG_HCFG_FSLSPCS					0x00000003		//
#define	USB_OTG_HCFG_FSLSPCS_0					0x00000001		//
#define	USB_OTG_HCFG_FSLSPCS_1					0x00000002		//
#define	USB_OTG_HCFG_FSLSS 						0x00000004		//
	
// USB_OTG_DCFG register	
	
#define	USB_OTG_DCFG_DSPD  						0x00000003		//
#define	USB_OTG_DCFG_DSPD_0						0x00000001		//
#define	USB_OTG_DCFG_DSPD_1						0x00000002		//
#define	USB_OTG_DCFG_NZLSOHSK 					0x00000004		//
#define	USB_OTG_DCFG_DAD   						0x000007F0		//
#define	USB_OTG_DCFG_DAD_0 						0x00000010		//
#define	USB_OTG_DCFG_DAD_1 						0x00000020		//
#define	USB_OTG_DCFG_DAD_2 						0x00000040		//
#define	USB_OTG_DCFG_DAD_3 						0x00000080		//
#define	USB_OTG_DCFG_DAD_4 						0x00000100		//
#define	USB_OTG_DCFG_DAD_5 						0x00000200		//
#define	USB_OTG_DCFG_DAD_6 						0x00000400		//
#define	USB_OTG_DCFG_PFIVL 						0x00001800		//
#define	USB_OTG_DCFG_PFIVL_0					0x00000800		//
#define	USB_OTG_DCFG_PFIVL_1					0x00001000		//
#define	USB_OTG_DCFG_PERSCHIVL					0x03000000		//
#define	USB_OTG_DCFG_PERSCHIVL_0    			0x01000000		//
#define	USB_OTG_DCFG_PERSCHIVL_1    			0x02000000		//

// USB_OTG_PCGCR register

#define	USB_OTG_PCGCR_STPPCLK					0x00000001		//
#define	USB_OTG_PCGCR_GATEHCLK 					0x00000002		//
#define	USB_OTG_PCGCR_PHYSUSP					0x00000010		//

// USB_OTG_GOTGINT register

#define	USB_OTG_GOTGINT_SEDET 					0x00000004		//
#define	USB_OTG_GOTGINT_SRSSCHG					0x00000100		//
#define	USB_OTG_GOTGINT_HNSSCHG					0x00000200		//
#define	USB_OTG_GOTGINT_HNGDET					0x00020000		//
#define	USB_OTG_GOTGINT_ADTOCHG					0x00040000		//
#define	USB_OTG_GOTGINT_DBCDNE					0x00080000		//

// USB_OTG_DCTL register

#define	USB_OTG_DCTL_RWUSIG						0x00000001		//
#define	USB_OTG_DCTL_SDIS  						0x00000002		//
#define	USB_OTG_DCTL_GINSTS						0x00000004		//
#define	USB_OTG_DCTL_GONSTS						0x00000008		//
#define	USB_OTG_DCTL_TCTL  						0x00000070		//
#define	USB_OTG_DCTL_TCTL_0						0x00000010		//
#define	USB_OTG_DCTL_TCTL_1						0x00000020		//
#define	USB_OTG_DCTL_TCTL_2						0x00000040		//
#define	USB_OTG_DCTL_SGINAK						0x00000080		//
#define	USB_OTG_DCTL_CGINAK						0x00000100		//
#define	USB_OTG_DCTL_SGONAK						0x00000200		//
#define	USB_OTG_DCTL_CGONAK						0x00000400		//
#define	USB_OTG_DCTL_POPRGDNE 					0x00000800		//
	
// USB_OTG_HFIR register	
	
#define	USB_OTG_HFIR_FRIVL 						0x0000FFFF		//

// USB_OTG_HFNUM register

#define	USB_OTG_HFNUM_FRNUM 					0x0000FFFF		//
#define	USB_OTG_HFNUM_FTREM 					0xFFFF0000		//

// USB_OTG_DSTS register

#define	USB_OTG_DSTS_SUSPSTS					0x00000001		//
#define	USB_OTG_DSTS_ENUMSPD					0x00000006		//
#define	USB_OTG_DSTS_ENUMSPD_0					0x00000002		//
#define	USB_OTG_DSTS_ENUMSPD_1					0x00000004		//
#define	USB_OTG_DSTS_EERR  						0x00000008		//
#define	USB_OTG_DSTS_FNSOF 						0x003FFF00		//

// USB_OTG_GAHBCFG register

#define	USB_OTG_GAHBCFG_GINT  					0x00000001		//
#define	USB_OTG_GAHBCFG_HBSTLEN					0x0000001E		//
#define	USB_OTG_GAHBCFG_HBSTLEN_0				0x00000002		//
#define	USB_OTG_GAHBCFG_HBSTLEN_1				0x00000004		//
#define	USB_OTG_GAHBCFG_HBSTLEN_2				0x00000008		//
#define	USB_OTG_GAHBCFG_HBSTLEN_3				0x00000010		//
#define	USB_OTG_GAHBCFG_DMAEN 					0x00000020		//
#define	USB_OTG_GAHBCFG_TXFELVL					0x00000080		//
#define	USB_OTG_GAHBCFG_PTXFELVL 				0x00000100		//

// USB_OTG_GUSBCFG register

#define	USB_OTG_GUSBCFG_TOCAL 					0x00000007		//
#define	USB_OTG_GUSBCFG_TOCAL_0					0x00000001		//
#define	USB_OTG_GUSBCFG_TOCAL_1					0x00000002		//
#define	USB_OTG_GUSBCFG_TOCAL_2					0x00000004		//
#define	USB_OTG_GUSBCFG_PHYSEL					0x00000040		//
#define	USB_OTG_GUSBCFG_SRPCAP					0x00000100		//
#define	USB_OTG_GUSBCFG_HNPCAP					0x00000200		//
#define	USB_OTG_GUSBCFG_TRDT  					0x00003C00		//
#define	USB_OTG_GUSBCFG_TRDT_0					0x00000400		//
#define	USB_OTG_GUSBCFG_TRDT_1					0x00000800		//
#define	USB_OTG_GUSBCFG_TRDT_2					0x00001000		//
#define	USB_OTG_GUSBCFG_TRDT_3					0x00002000		//
#define	USB_OTG_GUSBCFG_PHYLPCS					0x00008000		//
#define	USB_OTG_GUSBCFG_ULPIFSLS 				0x00020000		//
#define	USB_OTG_GUSBCFG_ULPIAR					0x00040000		//
#define	USB_OTG_GUSBCFG_ULPICSM					0x00080000		//
#define	USB_OTG_GUSBCFG_ULPIEVBUSD     			0x00100000		//
#define	USB_OTG_GUSBCFG_ULPIEVBUSI     			0x00200000		//
#define	USB_OTG_GUSBCFG_TSDPS 					0x00400000		//
#define	USB_OTG_GUSBCFG_PCCI  					0x00800000		//
#define	USB_OTG_GUSBCFG_PTCI  					0x01000000		//
#define	USB_OTG_GUSBCFG_ULPIIPD					0x02000000		//
#define	USB_OTG_GUSBCFG_FHMOD 					0x20000000		//
#define	USB_OTG_GUSBCFG_FDMOD 					0x40000000		//
#define	USB_OTG_GUSBCFG_CTXPKT					0x80000000		//

// USB_OTG_GRSTCTL register

#define	USB_OTG_GRSTCTL_CSRST 					0x00000001		//
#define	USB_OTG_GRSTCTL_HSRST 					0x00000002		//
#define	USB_OTG_GRSTCTL_FCRST 					0x00000004		//
#define	USB_OTG_GRSTCTL_RXFFLSH					0x00000010		//
#define	USB_OTG_GRSTCTL_TXFFLSH					0x00000020		//
#define	USB_OTG_GRSTCTL_TXFNUM					0x000007C0		//
#define	USB_OTG_GRSTCTL_TXFNUM_0 				0x00000040		//
#define	USB_OTG_GRSTCTL_TXFNUM_1 				0x00000080		//
#define	USB_OTG_GRSTCTL_TXFNUM_2 				0x00000100		//
#define	USB_OTG_GRSTCTL_TXFNUM_3 				0x00000200		//
#define	USB_OTG_GRSTCTL_TXFNUM_4 				0x00000400		//
#define	USB_OTG_GRSTCTL_DMAREQ					0x40000000		//
#define	USB_OTG_GRSTCTL_AHBIDL					0x80000000		//

// USB_OTG_DIEPMSK register

#define	USB_OTG_DIEPMSK_XFRCM 					0x00000001		//
#define	USB_OTG_DIEPMSK_EPDM  					0x00000002		//
#define	USB_OTG_DIEPMSK_TOM   					0x00000008		//
#define	USB_OTG_DIEPMSK_ITTXFEMSK				0x00000010		//
#define	USB_OTG_DIEPMSK_INEPNMM					0x00000020		//
#define	USB_OTG_DIEPMSK_INEPNEM					0x00000040		//
#define	USB_OTG_DIEPMSK_TXFURM					0x00000100		//
#define	USB_OTG_DIEPMSK_BIM   					0x00000200		//

// USB_OTG_HPTXSTS register

#define	USB_OTG_HPTXSTS_PTXFSAVL 				0x0000FFFF		//
#define	USB_OTG_HPTXSTS_PTXQSAV					0x00FF0000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_0				0x00010000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_1				0x00020000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_2				0x00040000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_3				0x00080000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_4				0x00100000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_5				0x00200000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_6				0x00400000		//
#define	USB_OTG_HPTXSTS_PTXQSAV_7				0x00800000		//
#define	USB_OTG_HPTXSTS_PTXQTOP					0xFF000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_0				0x01000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_1				0x02000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_2				0x04000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_3				0x08000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_4				0x10000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_5				0x20000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_6				0x40000000		//
#define	USB_OTG_HPTXSTS_PTXQTOP_7				0x80000000		//

// USB_OTG_HAINT register

#define	USB_OTG_HAINT_HAINT 					0x0000FFFF		//

// USB_OTG_DOEPMSK register

#define	USB_OTG_DOEPMSK_XFRCM 					0x00000001		//
#define	USB_OTG_DOEPMSK_EPDM  					0x00000002		//
#define	USB_OTG_DOEPMSK_STUPM 					0x00000008		//
#define	USB_OTG_DOEPMSK_OTEPDM					0x00000010		//
#define	USB_OTG_DOEPMSK_B2BSTUP					0x00000040		//
#define	USB_OTG_DOEPMSK_OPEM  					0x00000100		//
#define	USB_OTG_DOEPMSK_BOIM  					0x00000200		//

// USB_OTG_GINTSTS register

#define	USB_OTG_GINTSTS_CMOD  					0x00000001		//
#define	USB_OTG_GINTSTS_MMIS  					0x00000002		//
#define	USB_OTG_GINTSTS_OTGINT					0x00000004		//
#define	USB_OTG_GINTSTS_SOF   					0x00000008		//
#define	USB_OTG_GINTSTS_RXFLVL					0x00000010		//
#define	USB_OTG_GINTSTS_NPTXFE					0x00000020		//
#define	USB_OTG_GINTSTS_GINAKEFF 				0x00000040		//
#define	USB_OTG_GINTSTS_BOUTNAKEFF     			0x00000080		//
#define	USB_OTG_GINTSTS_ESUSP 					0x00000400		//
#define	USB_OTG_GINTSTS_USBSUSP					0x00000800		//
#define	USB_OTG_GINTSTS_USBRST					0x00001000		//
#define	USB_OTG_GINTSTS_ENUMDNE					0x00002000		//
#define	USB_OTG_GINTSTS_ISOODRP					0x00004000		//
#define	USB_OTG_GINTSTS_EOPF  					0x00008000		//
#define	USB_OTG_GINTSTS_IEPINT					0x00040000		//
#define	USB_OTG_GINTSTS_OEPINT					0x00080000		//
#define	USB_OTG_GINTSTS_IISOIXFR				0x00100000		//
#define	USB_OTG_GINTSTS_PXFR_INCOMPISOOUT		0x00200000		//
#define	USB_OTG_GINTSTS_DATAFSUSP				0x00400000		//
#define	USB_OTG_GINTSTS_HPRTINT					0x01000000		//
#define	USB_OTG_GINTSTS_HCINT 					0x02000000		//
#define	USB_OTG_GINTSTS_PTXFE 					0x04000000		//
#define	USB_OTG_GINTSTS_CIDSCHG					0x10000000		//
#define	USB_OTG_GINTSTS_DISCINT					0x20000000		//
#define	USB_OTG_GINTSTS_SRQINT					0x40000000		//
#define	USB_OTG_GINTSTS_WKUINT					0x80000000		//

// USB_OTG_GINTMSK register

#define	USB_OTG_GINTMSK_MMISM 					0x00000002		//
#define	USB_OTG_GINTMSK_OTGINT					0x00000004		//
#define	USB_OTG_GINTMSK_SOFM  					0x00000008		//
#define	USB_OTG_GINTMSK_RXFLVLM					0x00000010		//
#define	USB_OTG_GINTMSK_NPTXFEM					0x00000020		//
#define	USB_OTG_GINTMSK_GINAKEFFM				0x00000040		//
#define	USB_OTG_GINTMSK_GONAKEFFM				0x00000080		//
#define	USB_OTG_GINTMSK_ESUSPM					0x00000400		//
#define	USB_OTG_GINTMSK_USBSUSPM 				0x00000800		//
#define	USB_OTG_GINTMSK_USBRST					0x00001000		//
#define	USB_OTG_GINTMSK_ENUMDNEM 				0x00002000		//
#define	USB_OTG_GINTMSK_ISOODRPM 				0x00004000		//
#define	USB_OTG_GINTMSK_EOPFM 					0x00008000		//
#define	USB_OTG_GINTMSK_EPMISM					0x00020000		//
#define	USB_OTG_GINTMSK_IEPINT					0x00040000		//
#define	USB_OTG_GINTMSK_OEPINT					0x00080000		//
#define	USB_OTG_GINTMSK_IISOIXFRM				0x00100000		//
#define	USB_OTG_GINTMSK_PXFRM_IISOOXFRM			0x00200000		//
#define	USB_OTG_GINTMSK_FSUSPM					0x00400000		//
#define	USB_OTG_GINTMSK_PRTIM 					0x01000000		//
#define	USB_OTG_GINTMSK_HCIM  					0x02000000		//
#define	USB_OTG_GINTMSK_PTXFEM					0x04000000		//
#define	USB_OTG_GINTMSK_CIDSCHGM 				0x10000000		//
#define	USB_OTG_GINTMSK_DISCINT					0x20000000		//
#define	USB_OTG_GINTMSK_SRQIM 					0x40000000		//
#define	USB_OTG_GINTMSK_WUIM  					0x80000000		//

// USB_OTG_DAINT register

#define	USB_OTG_DAINT_IEPINT					0x0000FFFF		//
#define	USB_OTG_DAINT_OEPINT					0xFFFF0000		//

// USB_OTG_HAINTMSK register

#define	USB_OTG_HAINTMSK_HAINTM					0x0000FFFF		//

// USB_OTG_GRXSTSP register

#define	USB_OTG_GRXSTSP_EPNUM  					0x0000000F		//
#define	USB_OTG_GRXSTSP_BCNT   					0x00007FF0		//
#define	USB_OTG_GRXSTSP_DPID   					0x00018000		//
#define	USB_OTG_GRXSTSP_PKTSTS 					0x001E0000		//

// USB_OTG_DAINTMSK register

#define	USB_OTG_DAINTMSK_IEPM  					0x0000FFFF		//
#define	USB_OTG_DAINTMSK_OEPM  					0xFFFF0000		//

// USB_OTG register

#define	USB_OTG_CHNUM 							0x0000000F		//
#define	USB_OTG_CHNUM_0							0x00000001		//
#define	USB_OTG_CHNUM_1							0x00000002		//
#define	USB_OTG_CHNUM_2							0x00000004		//
#define	USB_OTG_CHNUM_3							0x00000008		//
#define	USB_OTG_BCNT  							0x00007FF0		//
#define	USB_OTG_DPID  							0x00018000		//
#define	USB_OTG_DPID_0							0x00008000		//
#define	USB_OTG_DPID_1							0x00010000		//
#define	USB_OTG_PKTSTS							0x001E0000		//
#define	USB_OTG_PKTSTS_0 						0x00020000		//
#define	USB_OTG_PKTSTS_1 						0x00040000		//
#define	USB_OTG_PKTSTS_2 						0x00080000		//
#define	USB_OTG_PKTSTS_3 						0x00100000		//
#define	USB_OTG_EPNUM 							0x0000000F		//
#define	USB_OTG_EPNUM_0							0x00000001		//
#define	USB_OTG_EPNUM_1							0x00000002		//
#define	USB_OTG_EPNUM_2							0x00000004		//
#define	USB_OTG_EPNUM_3							0x00000008		//
#define	USB_OTG_FRMNUM							0x01E00000		//
#define	USB_OTG_FRMNUM_0 						0x00200000		//
#define	USB_OTG_FRMNUM_1 						0x00400000		//
#define	USB_OTG_FRMNUM_2 						0x00800000		//
#define	USB_OTG_FRMNUM_3 						0x01000000		//
#define	USB_OTG_NPTXFSA							0x0000FFFF		//
#define	USB_OTG_NPTXFD							0xFFFF0000		//
#define	USB_OTG_TX0FSA							0x0000FFFF		//
#define	USB_OTG_TX0FD 							0xFFFF0000		//

// USB_OTG_GRXFSIZ register

#define	USB_OTG_GRXFSIZ_RXFD   					0x0000FFFF		//

// USB_OTG_DVBUSDIS register

#define	USB_OTG_DVBUSDIS_VBUSDT					0x0000FFFF		//

// USB_OTG_DVBUSPULSE register

#define	USB_OTG_DVBUSPULSE_DVBUSP				0x00000FFF		//

// USB_OTG_GNPTXSTS register

#define	USB_OTG_GNPTXSTS_NPTXFSAV 				0x0000FFFF		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV 				0x00FF0000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_0     		0x00010000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_1     		0x00020000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_2     		0x00040000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_3     		0x00080000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_4     		0x00100000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_5     		0x00200000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_6     		0x00400000		//
#define	USB_OTG_GNPTXSTS_NPTQXSAV_7     		0x00800000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP 				0x7F000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_0     		0x01000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_1     		0x02000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_2     		0x04000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_3     		0x08000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_4     		0x10000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_5     		0x20000000		//
#define	USB_OTG_GNPTXSTS_NPTXQTOP_6     		0x40000000		//

// USB_OTG_DTHRCTL register

#define	USB_OTG_DTHRCTL_NONISOTHREN    			0x00000001		//
#define	USB_OTG_DTHRCTL_ISOTHREN 				0x00000002		//
#define	USB_OTG_DTHRCTL_TXTHRLEN 				0x000007FC		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_0     			0x00000004		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_1     			0x00000008		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_2     			0x00000010		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_3     			0x00000020		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_4     			0x00000040		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_5     			0x00000080		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_6     			0x00000100		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_7     			0x00000200		//
#define	USB_OTG_DTHRCTL_TXTHRLEN_8     			0x00000400		//
#define	USB_OTG_DTHRCTL_RXTHREN					0x00010000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN 				0x03FE0000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_0     			0x00020000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_1     			0x00040000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_2     			0x00080000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_3     			0x00100000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_4     			0x00200000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_5     			0x00400000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_6     			0x00800000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_7     			0x01000000		//
#define	USB_OTG_DTHRCTL_RXTHRLEN_8     			0x02000000		//
#define	USB_OTG_DTHRCTL_ARPEN 					0x08000000		//

// USB_OTG_DIEPEMPMSK register

#define	USB_OTG_DIEPEMPMSK_INEPTXFEM			0x0000FFFF		//

// USB_OTG_DEACHINT register

#define	USB_OTG_DEACHINT_IEP1INT				0x00000002		//
#define	USB_OTG_DEACHINT_OEP1INT				0x00020000		//

// USB_OTG_GCCFG register

#define	USB_OTG_GCCFG_PWRDWN					0x00010000		//
#define	USB_OTG_GCCFG_I2CPADEN 					0x00020000		//
#define	USB_OTG_GCCFG_VBUSASEN 					0x00040000		//
#define	USB_OTG_GCCFG_VBUSBSEN 					0x00080000		//
#define	USB_OTG_GCCFG_SOFOUTEN 					0x00100000		//
#define	USB_OTG_GCCFG_NOVBUSSENS     			0x00200000		//

// USB_OTG_DEACHINTMSK register

#define	USB_OTG_DEACHINTMSK_IEP1INTM 			0x00000002		//
#define	USB_OTG_DEACHINTMSK_OEP1INTM 			0x00020000		//

// USB_OTG_CID register

#define	USB_OTG_CID_PRODUCT_ID     				0xFFFFFFFF		//

// USB_OTG_DIEPEACHMSK1 register

#define	USB_OTG_DIEPEACHMSK1_XFRCM 				0x00000001		//
#define	USB_OTG_DIEPEACHMSK1_EPDM  				0x00000002		//
#define	USB_OTG_DIEPEACHMSK1_TOM   				0x00000008		//
#define	USB_OTG_DIEPEACHMSK1_ITTXFEMSK			0x00000010		//
#define	USB_OTG_DIEPEACHMSK1_INEPNMM			0x00000020		//
#define	USB_OTG_DIEPEACHMSK1_INEPNEM			0x00000040		//
#define	USB_OTG_DIEPEACHMSK1_TXFURM				0x00000100		//
#define	USB_OTG_DIEPEACHMSK1_BIM   				0x00000200		//
#define	USB_OTG_DIEPEACHMSK1_NAKM  				0x00002000		//

// USB_OTG_HPRT register

#define	USB_OTG_HPRT_PCSTS 						0x00000001		//
#define	USB_OTG_HPRT_PCDET 						0x00000002		//
#define	USB_OTG_HPRT_PENA  						0x00000004		//
#define	USB_OTG_HPRT_PENCHNG					0x00000008		//
#define	USB_OTG_HPRT_POCA  						0x00000010		//
#define	USB_OTG_HPRT_POCCHNG					0x00000020		//
#define	USB_OTG_HPRT_PRES  						0x00000040		//
#define	USB_OTG_HPRT_PSUSP 						0x00000080		//
#define	USB_OTG_HPRT_PRST  						0x00000100		//
#define	USB_OTG_HPRT_PLSTS 						0x00000C00		//
#define	USB_OTG_HPRT_PLSTS_0					0x00000400		//
#define	USB_OTG_HPRT_PLSTS_1					0x00000800		//
#define	USB_OTG_HPRT_PPWR  						0x00001000		//
#define	USB_OTG_HPRT_PTCTL 						0x0001E000		//
#define	USB_OTG_HPRT_PTCTL_0					0x00002000		//
#define	USB_OTG_HPRT_PTCTL_1					0x00004000		//
#define	USB_OTG_HPRT_PTCTL_2					0x00008000		//
#define	USB_OTG_HPRT_PTCTL_3					0x00010000		//
#define	USB_OTG_HPRT_PSPD  						0x00060000		//
#define	USB_OTG_HPRT_PSPD_0						0x00020000		//
#define	USB_OTG_HPRT_PSPD_1						0x00040000		//

// USB_OTG_DOEPEACHMSK1 register

#define	USB_OTG_DOEPEACHMSK1_XFRCM 				0x00000001		//
#define	USB_OTG_DOEPEACHMSK1_EPDM  				0x00000002		//
#define	USB_OTG_DOEPEACHMSK1_TOM   				0x00000008		//
#define	USB_OTG_DOEPEACHMSK1_ITTXFEMSK			0x00000010		//
#define	USB_OTG_DOEPEACHMSK1_INEPNMM			0x00000020		//
#define	USB_OTG_DOEPEACHMSK1_INEPNEM			0x00000040		//
#define	USB_OTG_DOEPEACHMSK1_TXFURM				0x00000100		//
#define	USB_OTG_DOEPEACHMSK1_BIM   				0x00000200		//
#define	USB_OTG_DOEPEACHMSK1_BERRM 				0x00001000		//
#define	USB_OTG_DOEPEACHMSK1_NAKM  				0x00002000		//
#define	USB_OTG_DOEPEACHMSK1_NYETM 				0x00004000		//

// USB_OTG_HPTXFSIZ register

#define	USB_OTG_HPTXFSIZ_PTXSA 					0x0000FFFF		//
#define	USB_OTG_HPTXFSIZ_PTXFD 					0xFFFF0000		//

// USB_OTG_DIEPCTL register

#define	USB_OTG_DIEPCTL_MPSIZ 					0x000007FF		//
#define	USB_OTG_DIEPCTL_USBAEP					0x00008000		//
#define	USB_OTG_DIEPCTL_EONUM_DPID     			0x00010000		//
#define	USB_OTG_DIEPCTL_NAKSTS					0x00020000		//
#define	USB_OTG_DIEPCTL_EPTYP 					0x000C0000		//
#define	USB_OTG_DIEPCTL_EPTYP_0					0x00040000		//
#define	USB_OTG_DIEPCTL_EPTYP_1					0x00080000		//
#define	USB_OTG_DIEPCTL_STALL 					0x00200000		//
#define	USB_OTG_DIEPCTL_TXFNUM					0x03C00000		//
#define	USB_OTG_DIEPCTL_TXFNUM_0 				0x00400000		//
#define	USB_OTG_DIEPCTL_TXFNUM_1 				0x00800000		//
#define	USB_OTG_DIEPCTL_TXFNUM_2 				0x01000000		//
#define	USB_OTG_DIEPCTL_TXFNUM_3 				0x02000000		//
#define	USB_OTG_DIEPCTL_CNAK  					0x04000000		//
#define	USB_OTG_DIEPCTL_SNAK  					0x08000000		//
#define	USB_OTG_DIEPCTL_SD0PID_SEVNFRM 			0x10000000		//
#define	USB_OTG_DIEPCTL_SODDFRM					0x20000000		//
#define	USB_OTG_DIEPCTL_EPDIS 					0x40000000		//
#define	USB_OTG_DIEPCTL_EPENA 					0x80000000		//

// USB_OTG_HCCHAR register

#define	USB_OTG_HCCHAR_MPSIZ 					0x000007FF		//
#define	USB_OTG_HCCHAR_EPNUM 					0x00007800		//
#define	USB_OTG_HCCHAR_EPNUM_0					0x00000800		//
#define	USB_OTG_HCCHAR_EPNUM_1					0x00001000		//
#define	USB_OTG_HCCHAR_EPNUM_2					0x00002000		//
#define	USB_OTG_HCCHAR_EPNUM_3					0x00004000		//
#define	USB_OTG_HCCHAR_EPDIR 					0x00008000		//
#define	USB_OTG_HCCHAR_LSDEV 					0x00020000		//
#define	USB_OTG_HCCHAR_EPTYP 					0x000C0000		//
#define	USB_OTG_HCCHAR_EPTYP_0					0x00040000		//
#define	USB_OTG_HCCHAR_EPTYP_1					0x00080000		//
#define	USB_OTG_HCCHAR_MC    					0x00300000		//
#define	USB_OTG_HCCHAR_MC_0  					0x00100000		//
#define	USB_OTG_HCCHAR_MC_1  					0x00200000		//
#define	USB_OTG_HCCHAR_DAD   					0x1FC00000		//
#define	USB_OTG_HCCHAR_DAD_0 					0x00400000		//
#define	USB_OTG_HCCHAR_DAD_1 					0x00800000		//
#define	USB_OTG_HCCHAR_DAD_2 					0x01000000		//
#define	USB_OTG_HCCHAR_DAD_3 					0x02000000		//
#define	USB_OTG_HCCHAR_DAD_4 					0x04000000		//
#define	USB_OTG_HCCHAR_DAD_5 					0x08000000		//
#define	USB_OTG_HCCHAR_DAD_6 					0x10000000		//
#define	USB_OTG_HCCHAR_ODDFRM					0x20000000		//
#define	USB_OTG_HCCHAR_CHDIS 					0x40000000		//
#define	USB_OTG_HCCHAR_CHENA 					0x80000000		//

// USB_OTG_HCSPLT register

#define	USB_OTG_HCSPLT_PRTADDR					0x0000007F		//
#define	USB_OTG_HCSPLT_PRTADDR_0				0x00000001		//
#define	USB_OTG_HCSPLT_PRTADDR_1				0x00000002		//
#define	USB_OTG_HCSPLT_PRTADDR_2				0x00000004		//
#define	USB_OTG_HCSPLT_PRTADDR_3				0x00000008		//
#define	USB_OTG_HCSPLT_PRTADDR_4				0x00000010		//
#define	USB_OTG_HCSPLT_PRTADDR_5				0x00000020		//
#define	USB_OTG_HCSPLT_PRTADDR_6				0x00000040		//
#define	USB_OTG_HCSPLT_HUBADDR					0x00003F80		//
#define	USB_OTG_HCSPLT_HUBADDR_0				0x00000080		//
#define	USB_OTG_HCSPLT_HUBADDR_1				0x00000100		//
#define	USB_OTG_HCSPLT_HUBADDR_2				0x00000200		//
#define	USB_OTG_HCSPLT_HUBADDR_3				0x00000400		//
#define	USB_OTG_HCSPLT_HUBADDR_4				0x00000800		//
#define	USB_OTG_HCSPLT_HUBADDR_5				0x00001000		//
#define	USB_OTG_HCSPLT_HUBADDR_6				0x00002000		//
#define	USB_OTG_HCSPLT_XACTPOS					0x0000C000		//
#define	USB_OTG_HCSPLT_XACTPOS_0				0x00004000		//
#define	USB_OTG_HCSPLT_XACTPOS_1				0x00008000		//
#define	USB_OTG_HCSPLT_COMPLSPLT				0x00010000		//
#define	USB_OTG_HCSPLT_SPLITEN					0x80000000		//

// USB_OTG_HCINT register

#define	USB_OTG_HCINT_XFRC  					0x00000001		//
#define	USB_OTG_HCINT_CHH   					0x00000002		//
#define	USB_OTG_HCINT_AHBERR					0x00000004		//
#define	USB_OTG_HCINT_STALL 					0x00000008		//
#define	USB_OTG_HCINT_NAK   					0x00000010		//
#define	USB_OTG_HCINT_ACK   					0x00000020		//
#define	USB_OTG_HCINT_NYET  					0x00000040		//
#define	USB_OTG_HCINT_TXERR 					0x00000080		//
#define	USB_OTG_HCINT_BBERR 					0x00000100		//
#define	USB_OTG_HCINT_FRMOR 					0x00000200		//
#define	USB_OTG_HCINT_DTERR 					0x00000400		//

// USB_OTG_DIEPINT register

#define	USB_OTG_DIEPINT_XFRC  					0x00000001		//
#define	USB_OTG_DIEPINT_EPDISD					0x00000002		//
#define	USB_OTG_DIEPINT_TOC   					0x00000008		//
#define	USB_OTG_DIEPINT_ITTXFE					0x00000010		//
#define	USB_OTG_DIEPINT_INEPNE					0x00000040		//
#define	USB_OTG_DIEPINT_TXFE  					0x00000080		//
#define	USB_OTG_DIEPINT_TXFIFOUDRN     			0x00000100		//
#define	USB_OTG_DIEPINT_BNA   					0x00000200		//
#define	USB_OTG_DIEPINT_PKTDRPSTS				0x00000800		//
#define	USB_OTG_DIEPINT_BERR  					0x00001000		//
#define	USB_OTG_DIEPINT_NAK   					0x00002000		//

// USB_OTG_HCINTMSK register

#define	USB_OTG_HCINTMSK_XFRCM 					0x00000001		//
#define	USB_OTG_HCINTMSK_CHHM  					0x00000002		//
#define	USB_OTG_HCINTMSK_AHBERR					0x00000004		//
#define	USB_OTG_HCINTMSK_STALLM					0x00000008		//
#define	USB_OTG_HCINTMSK_NAKM  					0x00000010		//
#define	USB_OTG_HCINTMSK_ACKM  					0x00000020		//
#define	USB_OTG_HCINTMSK_NYET  					0x00000040		//
#define	USB_OTG_HCINTMSK_TXERRM					0x00000080		//
#define	USB_OTG_HCINTMSK_BBERRM					0x00000100		//
#define	USB_OTG_HCINTMSK_FRMORM					0x00000200		//
#define	USB_OTG_HCINTMSK_DTERRM					0x00000400		//

// USB_OTG_DIEPTSIZ register

#define	USB_OTG_DIEPTSIZ_XFRSIZ					0x0007FFFF		//
#define	USB_OTG_DIEPTSIZ_PKTCNT					0x1FF80000		//
#define	USB_OTG_DIEPTSIZ_MULCNT					0x60000000		//

// USB_OTG_HCTSIZ register

#define	USB_OTG_HCTSIZ_XFRSIZ  					0x0007FFFF		//
#define	USB_OTG_HCTSIZ_PKTCNT  					0x1FF80000		//
#define	USB_OTG_HCTSIZ_DOPING  					0x80000000		//
#define	USB_OTG_HCTSIZ_DPID    					0x60000000		//
#define	USB_OTG_HCTSIZ_DPID_0  					0x20000000		//
#define	USB_OTG_HCTSIZ_DPID_1  					0x40000000		//

// USB_OTG_DIEPDMA register

#define	USB_OTG_DIEPDMA_DMAADDR					0xFFFFFFFF		//

// USB_OTG_HCDMA register

#define	USB_OTG_HCDMA_DMAADDR  					0xFFFFFFFF		//

// USB_OTG_DTXFSTS register

#define	USB_OTG_DTXFSTS_INEPTFSAV 				0x0000FFFF		//

// USB_OTG_DIEPTXF register

#define	USB_OTG_DIEPTXF_INEPTXSA				0x0000FFFF		//
#define	USB_OTG_DIEPTXF_INEPTXFD				0xFFFF0000		//

// USB_OTG_DOEPCTL register

#define	USB_OTG_DOEPCTL_MPSIZ   				0x000007FF		//
#define	USB_OTG_DOEPCTL_USBAEP  				0x00008000		//
#define	USB_OTG_DOEPCTL_NAKSTS  				0x00020000		//
#define	USB_OTG_DOEPCTL_SD0PID_SEVNFRM   		0x10000000		//
#define	USB_OTG_DOEPCTL_SODDFRM 				0x20000000		//
#define	USB_OTG_DOEPCTL_EPTYP   				0x000C0000		//
#define	USB_OTG_DOEPCTL_EPTYP_0 				0x00040000		//
#define	USB_OTG_DOEPCTL_EPTYP_1 				0x00080000		//
#define	USB_OTG_DOEPCTL_SNPM    				0x00100000		//
#define	USB_OTG_DOEPCTL_STALL   				0x00200000		//
#define	USB_OTG_DOEPCTL_CNAK    				0x04000000		//
#define	USB_OTG_DOEPCTL_SNAK    				0x08000000		//
#define	USB_OTG_DOEPCTL_EPDIS   				0x40000000		//
#define	USB_OTG_DOEPCTL_EPENA   				0x80000000		//

// USB_OTG_DOEPINT register
#define	USB_OTG_DOEPINT_XFRC  					0x00000001		//
#define	USB_OTG_DOEPINT_EPDISD					0x00000002		//
#define	USB_OTG_DOEPINT_STUP  					0x00000008		//
#define	USB_OTG_DOEPINT_OTEPDIS					0x00000010		//
#define	USB_OTG_DOEPINT_B2BSTUP					0x00000040		//
#define	USB_OTG_DOEPINT_NYET  					0x00004000		//

// USB_OTG_DOEPTSIZ register

#define	USB_OTG_DOEPTSIZ_XFRSIZ					0x0007FFFF		//
#define	USB_OTG_DOEPTSIZ_PKTCNT					0x1FF80000		//
#define	USB_OTG_DOEPTSIZ_STUPCNT				0x60000000		//
#define	USB_OTG_DOEPTSIZ_STUPCNT_0				0x20000000		//
#define	USB_OTG_DOEPTSIZ_STUPCNT_1				0x40000000		//

// USB_OTG_PCGCCTL register

#define	USB_OTG_PCGCCTL_STOPCLK					0x00000001		//
#define	USB_OTG_PCGCCTL_GATECLK					0x00000002		//
#define	USB_OTG_PCGCCTL_PHYSUSP					0x00000010		//
#endif
