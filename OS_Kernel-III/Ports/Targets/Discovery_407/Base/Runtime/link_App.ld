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

_lnApplication	= ((_enRODATA - _stTEXT) + (__exidx_end - _stDATA));	/* Length of the application 	*/

MEMORY {
  prgm_code (rx)	: ORIGIN = 0x20000000, LENGTH = 52K
  prgm_data (rwx)	: ORIGIN = 0x2000D000, LENGTH = 12K
}

SECTIONS {
	.text : {
		_stTEXT = ABSOLUTE(.);
    	*(.text .text.*)
		_enTEXT = ABSOLUTE(.);
		_stRODATA = ABSOLUTE(.);
    	*(.rodata .rodata.*)
    	. = ALIGN(4);
		_enRODATA = ABSOLUTE(.);

		_stCTOR = ABSOLUTE(.);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		_enCTOR = ABSOLUTE(.);

		_stDTOR = ABSOLUTE(.);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		_enDTOR = ABSOLUTE(.);

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
