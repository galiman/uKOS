/*
; M25P16.
; =======

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		EEPROM M25P16 equates.
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

#ifndef	__M25P16__
#define	__M25P16__

// Memory structure

#define	KNBPAGE					8192									// Number of pages
#define	KSZPAGE					256										// Page size
#define	KNBSECTOR				32										// Number of sectors
#define	KSZSECTOR				65536									// Sector size
#define	KSZEEPROM				(KNBSECTOR*KSZSECTOR)					// EEPROM size

// Commands for the chip M25P16

#define	KCMDWRSTATUS			0x01									// EEPROM write status command
#define	KCMDWRITE				0x02									// EEPROM write command (pages of 256 bytes)
#define	KCMDREAD				0x03									// EEPROM read command
#define	KCMDWRDIENABLE			0x04									// EEPROM write disable command
#define	KCMDRDSTATUS			0x05									// EEPROM read status command
#define	KCMDWRENABLE			0x06									// EEPROM write enable command
#define	KCMDERASESECTOR			0xD8									// EEPROM erase sector command
#define	KCMDBULKERASE			0xC7									// EEPROM bulk erase command

// Status bits for the chip M25P16

#define	BBUSY					0										// Erase or write in progress
#define	BWEL					1										// Write enable latch
#define	BBP0					2										// Block protect block 0
#define	BBP1					3										// Block protect block 1
#define	BBP2					4										// Block protect block 2
#define	BSRWD					7										// Status register protect
#endif
