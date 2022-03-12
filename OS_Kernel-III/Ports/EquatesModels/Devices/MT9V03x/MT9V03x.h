/*
; MT9V03x.
; ========

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
; Goal:		Aptina MT9V03x imager equates.
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

#ifndef	__MT9V03x__
#define	__MT9V03x__

// Aptina initialization table

typedef	struct	mt9v03x	mt9v03x_t;

struct mt9v03x {
			uint8_t		oRegNumber;										// Register number
			uint16_t	oValue;											// Value
};

// Max image size

#define	KNBROWS					480										// Max 480 rows
#define	KNBCOLS					752										// Max 752 cols

// Constant for the temperature
//
// T = (Ta + KOFFSETTMP) x KGAINTMP

#define	KOFFSETTMP				(-26.9)									// offset
#define	KGAINTMP				(10.88)									// Gain

// I2c interface

#define	KAPTINAD				0x48									// I2C Aptina address

// Some imager registers

#define KAPTINACTRLREG  		0x07									// Aptina Control Register Address
#define KAPTINACOLSTART 		0x01									// ROI horizontal offset
#define KAPTINAROWSTART 		0x02									// ROI vertical offset
#define KAPTINAHEIGHT   		0x03									// ROI height
#define KAPTINAWIDTH    		0x04									// ROI width
#define KAPTINAHBLANK   		0x05 									// Horizontal blank duration in number of pixels
#define KAPTINAVBLANK   		0x06 									// Vertical blank duration in number of lines
#define KAPTINAEXPTIME  		0x0B 									// Exposure Time
#define KAPTINABINNING			0x0D 									// Binning control
#define KAPTINAANAGAIN  		0x35 									// Analog Gain
#define KAPTINAVBLCTRL  		0x47 									// Black level correction
#define KAPTINAVBLLEVEL 		0x48 									// VBlack value for correction
#define KAPTINANOISECA  		0x70									// Noise correction activation
#define KAPTINANOISECO  		0x71									// Constant for noise correction
#define KAPTINAAECAEG   		0xAF 									// Automatic exposure + gain control
#define	KAPTINATEMP				0xC1									// I2C Aptina temperature register
#define KAPTINAFINEEXP  		0xD5 									// Fine exposure tuning

// Reserved register (Aptina datasheet page 21)

#define	KAPTINAREG20			0x20									// Reserved
#define	KAPTINAREG24			0x24									// Reserved
#define	KAPTINAREG2B			0x2B									// Reserved
#define	KAPTINAREG2F			0x2F									// Reserved

// Reserved register (not documented)

#define KAPTINAPN0				0x68									// Production number 0
#define KAPTINAPN1				0x69									// Production number 1
#define KAPTINAPN2				0x6A									// Production number 2
#define KAPTINAPN3				0x6B									// Production number 3
#endif
