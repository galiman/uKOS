/*
; icycam.
; =======

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2008-09-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 113                  $:  Revision of last commit
; $Date:: 2017-04-16 16:55:48#$:  Date of last commit
;
; Project:	uKOS
; Goal:		icycam equates.
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

#ifndef __CPU_REG__
#define __CPU_REG__

// SoC bloc mapping
// ----------------

#define	SRAM				0x0000000		// Static RAM
#define	BUSCTRL				0x0030000		// Bus controller
#define	RQCTRL				0x0030400		// Int - Dma request controller
#define	DMA0				0x0030800		//
#define	DMA1				0x0030840		//
#define	DMA2				0x0030880		//
#define	DMA3				0x00308C0		// DMAs
#define	SPI1				0x0030C00		//
#define	SPI2				0x0031000		// SPIs
#define	PPI					0x0031400		// PPI
#define	UART				0x0031800		// UART
#define	GPIO				0x0031C00		//
#define	GPIOSWITCH			0x0032000		// GPIOs
#define	CKGEN				0x0032400		// CK controller
#define	TIMER0				0x0032800		//
#define	TIMER1				0x0032840		//
#define	TIMER2				0x0032880		//
#define	TIMER3				0x00328C0		//
#define	WDTIMER				0x0032C00		// Timers + Watchdog
#define	BOOT				0x0033000		// BOOT
#define	SDRAM				0x0033400		//
#define	SDRAM_BASE_ADDRESS	0x4000000		// SDRAM controller
#define	VSLOG				0x0033800		//
#define	VSLOG_BASE_ADDRESS	0x0F00000		// Log Vision Sensor
#define	GPU					0x0033C00		// GPU

#include 	"vectors.h"
#include 	"icycam_busctrl.h"
#include 	"icycam_rqctrl.h"
#include 	"icycam_dma.h"
#include 	"icycam_spi.h"
#include 	"icycam_ppi.h"
#include 	"icycam_uart.h"
#include 	"icycam_gpio.h"
#include 	"icycam_ckgen.h"
#include 	"icycam_timer.h"
#include 	"icycam_boot.h"
#include 	"icycam_sdram.h"
#include 	"icycam_vslog.h"
#include 	"icycam_gpu.h"
#endif
