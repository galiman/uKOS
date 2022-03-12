/*
; link.
; =====

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
; Goal:		Linker description for the uKOS system.
;
;                       EPROM
; _stTEXT        +-----------------+
;                |                 |
;                | .text           |
;                | .eh_frame       |
;                | .rodata         |
;				 |                 |                   DATA
; _enRODATA      +-----------------+ .......... +-----------------+ _stGOT
;                |                 |            |                 |
;                | .got            |            | .got            |
;                | .data           |            | .data           |
;                |                 |            |                 |
;                +-----------------+ .......... +-----------------+ _enDATA
;                                               |                 | _stBSS
;                                               | .bss            |
;                                               | COMMON          |
;                                               |                 |
;                                               +-----------------+ _enBSS
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

ENTRY(_start)

_stEPROM 	= 0x08000000;												/* EPROM, start of the Rom, EPROM, Flash, ...	*/
_lnEPROM	= (1024K);													/* Length of the Rom, EPROM, Flash, ... 		*/

_stSCode 	= 0x08000000;												/* SCode, start of the uKOS code				*/
_lnSCode 	= ((_enRODATA - _stVECTORS) + (__exidx_end - _stDATA));		/* Length of the uKOS code						*/

_stEXRAM 	= 0;														/* EXRAM, start of the external memory			*/
_lnEXRAM	= 0;														/* Length of the external memory	  			*/

_stUMemo	= 0x10000000;												/* UMemo, start of the external user memory 	*/
_lnUMemo	= (32K);													/* Length of the external user memory	  		*/

_stHeapC	= 0;														/* HeapC, start of the core coupled memory heap	*/
_lnHeapC	= 0;														/* Length of the core coupled memory heap  		*/

_stHeapE	= 0;														/* HeapE, start of the external memory heap		*/
_lnHeapE	= 0;														/* Length of the external memory heap  			*/

_stHeapI	= 0x20010000;												/* HeapI, start of the internal memory heap		*/
_lnHeapI	= (31K);													/* Length of the internal memory heap 			*/

_stSData  	= 0x20000000;												/* SData, start of the uKOS data				*/
_lnSData	= (96K - _lsStack);											/* Length of the uKOS data  					*/

_stStMSP	= (0x20018000 - 8);											/* MSP Stack, OS system stack					*/
_stStPSP	= (0x20018000 - _lnStMSP);									/* PSP Stack, initial thread stack				*/
_lsStack	= (_lnStMSP + _lnStPSP);									/* Length of the stack area  					*/
_lnStMSP	= (512);													/* Length of the OS stack  						*/
_lnStPSP	= (512);													/* Length of the "first" thread stack 			*/

MEMORY {
  prgm_code (rx)	: ORIGIN = 0x08000000, LENGTH = 1024K
  prgm_data (rwx)	: ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS {
	.text : {
		_stVECTORS = ABSOLUTE(.);
		KEEP(*(.isr_vector))
		_enVECTORS = ABSOLUTE(.);
		_stTEXT = ABSOLUTE(.);
    	*(.text .text.*)
		_enTEXT = ABSOLUTE(.);
		_stRODATA = ABSOLUTE(.);
    	*(.rodata .rodata.*)
    	. = ALIGN(4);
		_enRODATA = ABSOLUTE(.);
	} > prgm_code

	.data : AT (_enRODATA) {
		_stDATA = ABSOLUTE(.);
    	*(.data .data.*)
    	. = ALIGN(4);
		*(vtable)
		*(.data*)
		_enDATA = ABSOLUTE(.);
	} > prgm_data

    . = ALIGN(4);
    .eh_frame : {
        KEEP (*(.eh_frame))
    } > prgm_data

    . = ALIGN(4);
	.ARM.extab : {
		*(.ARM.extab*)
	} > prgm_data

    . = ALIGN(4);
	__exidx_start = ABSOLUTE(.);
	.ARM.exidx : {
		*(.ARM.exidx*)
	} > prgm_data
	__exidx_end = ABSOLUTE(.);

	.bss (NOLOAD) : {
		_stBSS = ABSOLUTE(.);
    	*(.bss .bss.*)
    	. = ALIGN(4);
		*(COMMON)
    	. = ALIGN(4);
		_enBSS = ABSOLUTE(.);
		_end = ABSOLUTE(.);
		end = ABSOLUTE(.);
	} > prgm_data
}
