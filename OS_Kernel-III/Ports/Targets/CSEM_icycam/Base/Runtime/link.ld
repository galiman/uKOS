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
;                       CODE
; _stTEXT        +-----------------+
;                |                 |
;                | .text           |
; _enTEXT        |                 |
; _stRODATA      +-----------------+ 
;                |                 |
;                | .rodata         |
; _enRODATA 	 |                 |
; _stDATA        +-----------------+ 
;                |                 |
;                | .data           |
; _enDATA	     |                 | 
; _stBSS         +-----------------+ 
;                |                 |
;                | .bss            |
; _enBSS	     |                 |
; _end           +-----------------+
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

/* SoC level mapping (basic fragments)	*/

_stSCode	= 0x00000000;												/* Start of the System code						*/
__stSCode	= _stSCode;													/*												*/
_lnSCode 	= (_enDATA - _stTEXT);										/* Length of the System code					*/
__lnSCode	= _lnSCode;													/*												*/
											
_stUMemo	= 0x00015000;												/* Start of the User data/code					*/
__stUMemo	= _stUMemo;													/*												*/
_enUCode	= 0x0001BFFF;												/* End of the User code 						*/
__enUCode	= _enUCode;													/*												*/
_lnUMemo	= (36K);													/* Length of the external user memory			*/
__lnUMemo	= _lnUMemo;													/* Length of the external user memory			*/

_stHeapC	= 0;														/* HeapC, start of the core coupled memory heap	*/
__stHeapC	= _stHeapC;													/* 												*/
_lnHeapC	= 0;														/* Length of the core coupled memory heap  		*/
__lnHeapC	= _lnHeapC;													/* 										  		*/

_stHeapE	= 0;														/* HeapE, start of the external memory heap		*/
__stHeapE	= _stHeapE;													/* 												*/
_lnHeapE	= 0;														/* Length of the external memory heap  			*/
__lnHeapE	= _lnHeapE;													/* 										  		*/

_stHeapI	= 0x0001E000;												/* HeapI, start of the internal memory heap		*/
__stHeapI	= _stHeapI;													/*												*/
_lnHeapI	= (8K - _lsStack);											/* Length of the internal memory heap 			*/
__lnHeapI	= _lnHeapI;													/*												*/
		
_stStack 	= (0x00020000 - 4);											/*												*/
__stStack 	= _stStack;													/* Last stack position							*/
_lsStack	= (512);													/* 							 					*/
__lsStack	= _lsStack;													/* Length of the stack area  					*/

MEMORY {
  prgm_code	: ORIGIN = 0x00000000, LENGTH = 64K
  prgm_data	: ORIGIN = 0x00010000, LENGTH = 20K
}

SECTIONS {
	.text : {
		_stTEXT = ABSOLUTE(.);
		__stTEXT = ABSOLUTE(.);
    	*(.text)
    	. = ALIGN(4);
    	*(.text.* )
		_enTEXT = ABSOLUTE(.);
		__enTEXT = ABSOLUTE(.);
	} > prgm_code

	.rodata : {
		_stRODATA = ABSOLUTE(.);
		__stRODATA = ABSOLUTE(.);
    	*(.rodata*)
    	. = ALIGN(4);
		_enRODATA = ABSOLUTE(.);
		__enRODATA = ABSOLUTE(.);
	} > prgm_code

	.data : {
		_stDATA = ABSOLUTE(.);
		__stDATA = ABSOLUTE(.);
    	*(.data)
    	. = ALIGN(4);
		_enDATA = ABSOLUTE(.);
		__enDATA = ABSOLUTE(.);
	} > prgm_code

	.bss : {
		_stBSS = ABSOLUTE(.);
		__stBSS = ABSOLUTE(.);
    	*(.bss)
    	. = ALIGN(4);
		*(COMMON)
    	. = ALIGN(4);
		_enBSS = ABSOLUTE(.);
		__enBSS = ABSOLUTE(.);
    	_end = ABSOLUTE(.);
    	__end = ABSOLUTE(.);
    	_heapI = ABSOLUTE(.);
    	__heapI = ABSOLUTE(.);
	} > prgm_data
}
