/*
; EV76C541.
; =========

;------------------------------------------------------------------------
; Author:	Nussbaum Pascal	The 2016-09-02
; Modifs:	Franzi Edo.		The 2016-11-13
;
; SVN:
; $Author:: efr              $:  Author of last commit
; $Rev:: 162                 $:  Revision of last commit
; $Date:: 2017-06-27 1#$:  Date of last commit
;
; Project:	uKOS
; Goal:		E2V EV76C541 imager equates.
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

#ifndef	__EV76C541__
#define	__EV76C541__

// E2V initialization table

typedef	struct	ev76c541	ev76c541_t;

struct ev76c541 {
			uint8_t		oRegNumber;										// Register number
			uint16_t	oValue;											// Value
};

// Max image size

#define	KNBROWS					480										// Max 480 rows
#define	KNBCOLS					752										// Max 752 cols

// Imager 8-bit registers

#define	KE2V_REGISTER_0			0x00									//
#define	KE2V_SOFT_RESET			0x01									//
#define	KE2V_CALIB_MBX			0x02									//
#define	KE2V_ABORT_MBX			0x03									//

// Imager 16-bit registers

#define	KE2V_LINE_CFG			0x04									//
#define	KE2V_FLASH_DELAY		0x05									//
#define	KE2V_MISCEL1			0x06									//
#define	KE2V_MISCEL2			0x07									//
#define	KE2V_CLOCK_CFG			0x08									//
#define	KE2V_PLL_CFG			0x09									//
#define	KE2V_CHAIN_CFG			0x0A									//
#define	KE2V_CONTROL_CFG		0x0B									//
#define	KE2V_FRAME_PERIOD		0x0C									//
#define	KE2V_WAIT				0x0D									//

#define	KE2V_ROI1_INT_LL		0x0E									//
#define	KE2V_ROI1_REP_NB		0x0F									//
#define	KE2V_ROI1_WAIT			0x10									//
#define	KE2V_ROI1_GAIN			0x11									//
#define	KE2V_ROI1_0L1			0x12									//
#define	KE2V_ROI1_H1  			0x13									//
#define	KE2V_ROI1_0C1  			0x14									//
#define	KE2V_ROI1_W1			0x15									//
#define	KE2V_ROI1_0L2			0x16									//
#define	KE2V_ROI1_H2			0x17									//
#define	KE2V_ROI1_0C2			0x18									//
#define	KE2V_ROI1_W2			0x19									//
#define	KE2V_ROI1_SUBS			0x1A									//

#define	KE2V_ROI2_INT_LL		0x1B									//
#define	KE2V_ROI2_REP_NB		0x1C									//
#define	KE2V_ROI2_WAIT			0x1D									//
#define	KE2V_ROI2_GAIN			0x1E									//
#define	KE2V_ROI2_0L1			0x1F									//
#define	KE2V_ROI2_H1  			0x20									//
#define	KE2V_ROI2_0C1  			0x21									//
#define	KE2V_ROI2_W1			0x22									//
#define	KE2V_ROI2_SUBS			0x23									//

#define	KE2V_ROI3_INT_LL		0x24									//
#define	KE2V_ROI3_REP_NB		0x25									//
#define	KE2V_ROI3_WAIT			0x26									//
#define	KE2V_ROI3_GAIN			0x27									//
#define	KE2V_ROI3_0L1			0x28									//
#define	KE2V_ROI3_H1  			0x29									//
#define	KE2V_ROI3_0C1  			0x2A									//
#define	KE2V_ROI3_W1			0x2B									//
#define	KE2V_ROI3_SUBS			0x2C									//

#define	KE2V_ROI4_INT_LL		0x2D									//
#define	KE2V_ROI4_REP_NB		0x2E									//
#define	KE2V_ROI4_WAIT			0x2F									//
#define	KE2V_ROI4_GAIN			0x30									//
#define	KE2V_ROI4_0L1			0x31									//
#define	KE2V_ROI4_H1  			0x32									//
#define	KE2V_ROI4_0C1  			0x33									//
#define	KE2V_ROI4_W1			0x34									//
#define	KE2V_ROI4_SUBS			0x35									//

#define	KE2V_DIG_GAIN_GB_GR		0x36									//
#define	KE2V_DIG_GAIN_B_R		0x37									//
#define	KE2V_CLAMP_OFFSET  		0x38									//
#define	KE2V_CLAMP_CFG			0x39									//
#define	KE2V_PRG_OSC			0x3A									//
#define	KE2V_CAL_CNT_REF		0x3B									//
#define	KE2V_CAL_OSC_CNT		0x3C									//
#define	KE2V_CLAMP				0x3D									//
#define	KE2V_STATUS				0x3E									//
#define	KE2V_CLK_OUT_LOWPR		0x44									//
#define	KE2V_PXTIM_RD_WIDTH		0x49									//
#define	KE2V_CHIP_ID  			0x7F									//

// Imager registers bitfield options

#define	KE2V_CONTROL_CFG_LOCK_DYN_REG	(1<<10)							//
#define	KE2V_CONTROL_CFG_TRIG_PAD_INV	(1<<9)							//
#define	KE2V_CONTROL_CFG_TRIG_PAD_EN	(1<<8)							//
#define	KE2V_CONTROL_CFG_ROI_FLASH_ON	(1<<6)							//
#define	KE2V_CONTROL_CFG_ROI_VIDEO_EN	(1<<3)							//
#define	KE2V_CONTROL_CFG_ROI_OVERLAP_EN	(1<<2)							//
#define	KE2V_CONTROL_CFG_SPI_TRIG_EN	(1<<1)							//
#define	KE2V_CONTROL_CFG_STDBY_RQST	 	(1<<0)							//
#endif
