/*
; S2.
; ===

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2013-03-08
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		SNAP S2 imager equates.
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

#ifndef	__S2__
#define	__S2__
							
// Max image sizeof							

#define	KNBROWS					240											// Max 240 rows
#define	KNBCOLS					320											// Max 320 cols

// Pixel mode (DAU_PIXEL_MODE)

#define KLUM8					((0x0<<7) | (0x0<<5) | (0x0<<3) | (0x3<<0))	// Luminance 8 bits
#define KLUM10					((0x0<<7) | (0x0<<5) | (0x0<<3) | (0x0<<0))	// Luminance 10 bits
#define KCON8					((0x0<<7) | (0x0<<5) | (0x0<<3) | (0x5<<0))	// Contrast 8 bits
#define KCON10					((0x0<<7) | (0x0<<5) | (0x0<<3) | (0x4<<0))	// Contrast 10 bits
#define KEDG8					((0x0<<7) | (0x0<<5) | (0x0<<3) | (0x7<<0))	// Edge 8 bits
#define KEDG10					((0x0<<7) | (0x0<<5) | (0x0<<3) | (0x6<<0))	// Edge 10 bits

// Commands

#define	SPI_CMD_GET_S2_SPI_STATUS			0x01							//
#define	SPI_CMD_WRITE_1_BYTE				0x02							//
#define	SPI_CMD_WRITE_2_BYTE				0x03							//
#define	SPI_CMD_WRITE_4_BYTE				0x04							//
#define	SPI_CMD_READ_1_BYTE_P2				0x05							//
#define	SPI_CMD_READ_2_BYTE_P2				0x06							//
#define	SPI_CMD_READ_4_BYTE_P2				0x07							//
#define	SPI_CMD_READ_1_BYTE_P4				0x85							//
#define	SPI_CMD_READ_2_BYTE_P4				0x86							//
#define	SPI_CMD_READ_4_BYTE_P4				0x87							//
#define	SPI_CMD_TRIGGER						0x08							//
#define	SPI_CMD_FILL_LUT_LOG				0x09							//
#define	SPI_CMD_FILL_LUT_LIN				0x0A							//
#define	SPI_CMD_WRITE_INT_MEM_P2			0x0E							//
#define	SPI_CMD_WRITE_INT_MEM_P4			0x8E							//
#define	SPI_CMD_READ_INT_MEM_P4				0x8F							//

// Soft registers

#define	REG_S2_NIRQ_O_MASK					0x000A0000						//
#define	REG_S2_NIRQ_O_STATUS				0x000A0004						//
#define	REG_S2_IRQ_INTERNAL_STATUS			0x000A0008						//
#define	REG_S2_CFG							0x000A000C						//
#define	REG_S2_DAU_BL_STRONG_WEAK_DELAY		0x000A0010						//
#define	REG_S2_NLED_O_THR					0x000A0014						//
#define	REG_S2_NLED_O_DEL					0x000A0018						//
#define	REG_S2_FIRST_XFER_CK_DELAY			0x000A001C						//
#define	REG_S2_HCI_LOG_DELAY_EXTENSION		0x000A0020						//
#define	REG_S2_HCI_LOG_RAMP					0x000A0024						//
#define	REG_S2_HCI_VREF_STEP				0x000A0028						//
#define	REG_S2_HCI_N_CODES					0x000A002C						//
#define	REG_S2_HCI_SHUT_DT					0x000A0030						//
#define	REG_S2_HCI_ADDR						0x000A0034						//
#define	REG_S2_HCI_NB_DATA					0x000A0038						//
#define	REG_S2_HCI_EXPOSURE_TIME			0x000A003C						//
#define	REG_S2_SPI_XFER_STATE				0x000A0044						//
#define	REG_S2_SYS_STATUS					0x000A0048						//
#define	REG_S2_SYS_STATE					0x000A004C						//
#define	REG_S2_ID							0x000A0050						//
							
// Data Acquisition Unit registers							
							
#define REG_DAU_CTRL						0x00080400						//
#define REG_DAU_START_SEQ					0x00080402						//
#define REG_DAU_IRQ							0x00080403						//
#define REG_DAU_STATUS						0x00080404						//
#define REG_DAU_TEST						0x00080406						//
#define REG_DAU_ROW_TEST					0x00080407						//
#define REG_DAU_PIX_CTRL					0x00080408						//
#define REG_DAU_ROW							0x00080409						//
#define REG_DAU_COL_GRP						0x0008040A						//
#define REG_DAU_LATCH_CTRL					0x0008040B						//
#define REG_DAU_DIN							0x0008040C						//
#define REG_DAU_CONFIG						0x0008040E						//
#define REG_DAU_DAC0						0x00080410						//
#define REG_DAU_DAC1						0x00080411						//
#define REG_DAU_DAC2						0x00080412						//
#define REG_DAU_DAC3						0x00080413						//
#define REG_DAU_DAC4						0x00080414						//
#define REG_DAU_DAC5						0x00080415						//
#define REG_DAU_DAC6						0x00080416						//
#define REG_DAU_DAC7						0x00080417						//
#define REG_DAU_DAC8						0x00080418						//
#define REG_DAU_DAC9						0x00080419						//
#define REG_DAU_DAC10						0x0008041A						//
#define REG_DAU_DAC11						0x0008041B						//
#define REG_DAU_DAC12						0x0008041C						//
#define REG_DAU_ROW_START					0x00080420						//
#define REG_DAU_ROW_END						0x00080421						//
#define REG_DAU_COL_START					0x00080422						//
#define REG_DAU_COL_END						0x00080423						//
#define REG_DAU_PIXEL_MODE					0x00080424						//
#define REG_DAU_SYNC_MODE					0x00080425						//
#define REG_DAU_START_BLANK					0x00080426						//
#define REG_DAU_END_BLANK					0x00080428						//
#define REG_DAU_HORIZ_BLANK					0x0008042A						//
#define REG_DAU_FRAME_COUNT					0x0008042C						//
#define REG_DAU_INIT_WAIT					0x00080430						//
#define REG_DAU_EXPO_WAIT					0x00080432						//
#define REG_DAU_RST_RAMP_CFG				0x00080434						//
#define REG_DAU_VDROP						0x00080436						//
#define REG_DAU_PULSE_VREF					0x00080437						//
#define REG_DAU_VREF						0x00080438						//
#define REG_DAU_TESTBUS_CFG					0x0008043A						//
#define REG_DAU_BANDGAP						0x0008043C						//
#define REG_DAU_PIX_TEST					0x0008043E						//
#define REG_DAU_XTAL						0x00080440						//
#define REG_DAU_TEST_MODE_EN				0x00080441						//
#define REG_DAU_DET_LIGHT_EXT_CFG			0x00080442						//
#define REG_DAU_DET_LIGHT_THERM				0x00080444						//
#define REG_DAU_DET_LIGHT_PTAT				0x00080446						//
#define REG_DAU_NLED_OUT					0x00080448						//
#define REG_DAU_NIRQ_OUT					0x00080449						//
#define REG_DAU_FIFO_CTRL					0x00080450						//
#define REG_DAU_FIFO_STATUS					0x00080451						//
#define REG_DAU_FIFO_DATA					0x00080454						//
#endif
