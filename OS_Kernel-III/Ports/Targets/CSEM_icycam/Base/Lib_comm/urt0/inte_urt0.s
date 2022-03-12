; inte_urt0.
; ==========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 161                  $:  Revision of last commit
; $Date:: 2017-06-15 15:01:25#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Interruption management for the module.
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

				.global		_handle_uart_interruptionTX
				.global		_handle_uart_interruptionRX
				.extern		_uart_interruptionTX
				.extern		_uart_interruptionRX
				.extern		_saveScratchRegisters
				.extern		_restoreScratchRegisters

				.section	.text

; Interruption urt0
; -----------------

_handle_uart_interruptionTX:
				jsr			_saveScratchRegisters			; Save the scratch registers
				nop											;
				sjsr		_uart_interruptionTX			; Process the urt0 TX interruption
				nop											;
				jsr			_restoreScratchRegisters		; Restore the scratch registers
				nop											;
				rte											;
				nop

_handle_uart_interruptionRX:
				jsr			_saveScratchRegisters			; Save the scratch registers
				nop											;
				sjsr		_uart_interruptionRX			; Process the urt0 RX interruption
				nop											;
				jsr			_restoreScratchRegisters		; Restore the scratch registers
				nop											;
				rte											;
				nop
