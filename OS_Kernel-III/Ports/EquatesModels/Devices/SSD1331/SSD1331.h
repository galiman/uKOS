/*
; SSD1331.
; ========

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
; Goal:		SSD1331 oled display equates.
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

#ifndef	__SSD1331__
#define	__SSD1331__

// Max display size

#define SSD1331_HEIGHT 					64								// Max 64 rows
#define SSD1331_WIDTH 					96								// Max 96 cols

// Font selections

#define SSD1331_FONT_3x5				0								//
#define SSD1331_FONT_5x7				1								//

// Timing Delays

#define SSD1331_DELAYS_HWFILL			3								// 3-mS
#define SSD1331_DELAYS_HWLINE			1								// 1-mS

// SSD1331 Commands

#define SSD1331_CMD_DRAWLINE 			0x21							//
#define SSD1331_CMD_DRAWRECT 			0x22							//
#define SSD1331_CMD_COPYRECT 			0x23							//
#define SSD1331_CMD_FILL 				0x26							//
#define SSD1331_CMD_SETCOLUMN 			0x15							//
#define SSD1331_CMD_SETROW    			0x75							//
#define SSD1331_CMD_CONTRASTA 			0x81							//
#define SSD1331_CMD_CONTRASTB 			0x82							//
#define SSD1331_CMD_CONTRASTC			0x83							//
#define SSD1331_CMD_MASTERCURRENT 		0x87							//
#define SSD1331_CMD_SETREMAP 			0xA0							//
#define SSD1331_CMD_STARTLINE 			0xA1							//
#define SSD1331_CMD_DISPLAYOFFSET 		0xA2							//
#define SSD1331_CMD_NORMALDISPLAY 		0xA4							//
#define SSD1331_CMD_DISPLAYALLON  		0xA5							//
#define SSD1331_CMD_DISPLAYALLOFF 		0xA6							//
#define SSD1331_CMD_INVERTDISPLAY 		0xA7							//
#define SSD1331_CMD_SETMULTIPLEX  		0xA8							//
#define SSD1331_CMD_SETMASTER 			0xAD							//
#define SSD1331_CMD_DISPLAYOFF 			0xAE							//
#define SSD1331_CMD_DISPLAYON     		0xAF							//
#define SSD1331_CMD_POWERMODE 			0xB0							//
#define SSD1331_CMD_PRECHARGE 			0xB1							//
#define SSD1331_CMD_CLOCKDIV 			0xB3							//
#define SSD1331_CMD_PRECHARGEA 			0x8A							//
#define SSD1331_CMD_PRECHARGEB 			0x8B							//
#define SSD1331_CMD_PRECHARGEC 			0x8C							//
#define SSD1331_CMD_PRECHARGELEVEL 		0xBB							//
#define SSD1331_CMD_VCOMH 				0xBE							//

// Char table constants from ASCII 32 to 126

#define MAKE_BINARY(a,b,c,d,e,f,g,h) (((a)<<7) | ((b)<<6) | ((c)<<5) | ((d)<4) | ((e)<<3) | ((f)<<2) | ((g)<<1) | ((h)<<0))

const	uint8_t OLEDFont3x5[] = {
						MAKE_BINARY(0,0,0,0,0,0,0,0), // [space]
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // !
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // "
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // #
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1), // $
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // %
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // &
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // '
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // (
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // )
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // *
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // +
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // ,
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // -
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // .
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // /
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 0
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // 1
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 2
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 3
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // 4
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 5
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 6
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 7
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 8
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // 9
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // :
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // ;
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // <
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // =
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // >
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // ?
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // @
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // A
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // B
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1), // C
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // D
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // E
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // F
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1), // G
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // H
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // I
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // J
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // K
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // L
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // M
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // N
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // O
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // P
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // Q
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // R
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1), // S
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // T
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // U
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // V
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // W
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // X
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1), // Y
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // Z
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // [
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // BACKSLASH
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // ]
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // ^
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // _
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // `
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // a
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // b
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // c
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // d
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // e
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // f
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // g
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // h
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // i
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // j
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // k
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // l
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // m
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // n
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // o
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // p
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // q
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // r
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // s
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // t
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // u
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // v
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // w
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // x
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // y
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // z
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1), // {
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // |
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // }
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,1), // ~
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // [plain]
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,1)
				};
	
// Char table constants from ASCII 32 to 126

const	uint8_t	OLEDFont5x7[] = {
						MAKE_BINARY(0,0,0,0,0,0,0,0), // [space]
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // !
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0), // "
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0), // #
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // $
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,1,0,0,0), // %
						MAKE_BINARY(0,0,0,1,1,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,1),
						MAKE_BINARY(0,0,0,0,1,1,0,0), // &
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,1),
						MAKE_BINARY(0,0,0,0,1,1,0,0), // '
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // (
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0), // )
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // *
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // +
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // ,
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // -
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // .
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // /
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // 0
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,1,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,1,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // 1
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // 2
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // 3
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // 4
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // 5
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // 6
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // 7
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // 8
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // 9
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // :
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // ;
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // <
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // =
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0), // >
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // ?
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // @
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // A
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0), // B
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // C
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,1,1,0,0), // D
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,1,1,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // E
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // F
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // G
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // H
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // I
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,1,1), // J
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // K
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,1,0,0),
						MAKE_BINARY(0,0,0,1,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,1,0,0),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0), // L
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // M
						MAKE_BINARY(0,0,0,1,1,0,1,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // N
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,0,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,0,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // O
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0), // P
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // Q
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0), // R
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,1,0,0),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1), // S
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // T
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // U
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // V
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // W
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // X
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1), // Y
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // Z
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // [
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // BACKSLASH
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0), // ]
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // ^
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // _
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // `
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // a
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0), // b
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // c
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // d
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // e
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // f
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // g
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0), // h
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // i
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0), // j
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0), // k
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,1,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,0,0), // l
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // m
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,0,1,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // n
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,1,1,0),
						MAKE_BINARY(0,0,0,1,1,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // o
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // p
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // q
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // r
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,1,1,0),
						MAKE_BINARY(0,0,0,1,1,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // s
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,1),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,1,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // t
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // u
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // v
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // w
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,1,0,1,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // x
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // y
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,1),
						MAKE_BINARY(0,0,0,0,1,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0), // z
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,0,0,1,1,0), // {
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,1,0,0,0),
						MAKE_BINARY(0,0,0,0,0,1,1,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0), // |
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,0,1,0,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0), // }
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,0,0,1,0),
						MAKE_BINARY(0,0,0,0,1,1,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,1), // ~
						MAKE_BINARY(0,0,0,0,1,1,1,0),
						MAKE_BINARY(0,0,0,1,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,0,0,0,0,0),
						MAKE_BINARY(0,0,0,1,1,1,1,1), // [plain]
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1),
						MAKE_BINARY(0,0,0,1,1,1,1,1)
				};

#endif
