; inte_vis0.
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

				.global		_handle_vs_interruptionDma0
				.extern		_vs_interruptionDma0
				.global		_handle_vs_interruptionVSInit
				.extern		_vs_interruptionVSInit
				.global		_handle_vs_interruptionVSExpo
				.extern		_vs_interruptionVSExpo
				.global		_handle_vs_interruptionVSRamp
				.extern		_vs_interruptionVSRamp
				.extern		_saveScratchRegisters
				.extern		_restoreScratchRegisters

				.section	.text

; Interruption dma0
; -----------------

_handle_vs_interruptionDma0:
				jsr			_saveScratchRegisters			; Save the scratch registers
				nop											;
				sjsr		_vs_interruptionDma0			; Process the dma0 interruption
				nop											;
				jsr			_restoreScratchRegisters		; Restore the scratch registers
				nop											;
				rte											;
				nop

; Interruption VSInit
; -------------------

_handle_vs_interruptionVSInit:
				jsr			_saveScratchRegisters			; Save the scratch registers
				nop											;
				sjsr		_vs_interruptionVSInit			; Process the VSInit interruption
				nop											;
				jsr			_restoreScratchRegisters		; Restore the scratch registers
				nop											;
				rte											;
				nop

; Interruption VSExpo
; -------------------

_handle_vs_interruptionVSExpo:
				jsr			_saveScratchRegisters			; Save the scratch registers
				nop											;
				sjsr		_vs_interruptionVSExpo			; Process the VSExpo interruption
				nop											;
				jsr			_restoreScratchRegisters		; Restore the scratch registers
				nop											;
				rte											;
				nop

; Interruption VSRamp
; -------------------

_handle_vs_interruptionVSRamp:
				jsr			_saveScratchRegisters			; Save the scratch registers
				nop											;
				sjsr		_vs_interruptionVSRamp			; Process the VSRamp interruption
				nop											;
				jsr			_restoreScratchRegisters		; Restore the scratch registers
				nop											;
				rte											;
				nop
