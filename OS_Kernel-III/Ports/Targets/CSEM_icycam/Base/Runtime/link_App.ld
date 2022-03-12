/*
; link_App.
; =========

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
; Goal:		Linker description for uKOS applications.
;
;                       CODE
; _stRODATA      +-----------------+
;                |                 |
;                | .rodata         |
; _enRODATA      |                 |
; _stTEXT        +-----------------+ 
;                |                 |
;                | .text           |
; _enTEXT 	     |                 |
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
__start = _start;

_lnApplication	= (_enDATA - _stRODATA);	/* Length of the application 	*/

MEMORY {
  prgm_code	: ORIGIN = 0x00015000, LENGTH = 20K
  prgm_data	: ORIGIN = 0x0001A000, LENGTH = 16K
}

SECTIONS {
	.rodata : {
		_stRODATA = ABSOLUTE(.);
		__stRODATA = ABSOLUTE(.);
		*(.rodata*)
		. = ALIGN(4);
		_enRODATA = ABSOLUTE(.);
		__enRODATA = ABSOLUTE(.);
	} > prgm_code

	.text : {
		_stTEXT = ABSOLUTE(.);
		__stTEXT = ABSOLUTE(.);
		*(.text)
		. = ALIGN(4);
    	*(.text.* )
		_enTEXT = ABSOLUTE(.);
		__enTEXT = ABSOLUTE(.);
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
	} > prgm_data
}
