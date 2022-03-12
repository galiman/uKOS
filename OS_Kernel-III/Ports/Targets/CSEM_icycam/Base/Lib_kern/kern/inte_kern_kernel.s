; inte_kern_kernel.
; =================

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
;					+-----------------------+
; Message switch	|        message        | .32
;					+-----------------------+
;
;					+-----------------------+
; Normal switch		|         r7..r0        | 8 x .32
;					+-----------------------+
;					|        px6..px0       | 7 x .32
;					+-----------------------+
;					|          psu1         | .64
;					+-----------------------+
;					|          psu0         | .64
;					+-----------------------+
;					|          iel          | .32
;					+-----------------------+
;					|          sbr          | .32
;					+-----------------------+
;					|          scnt         | .32
;					+-----------------------+
;					|   MSB LSB Hard Stack  | scnt x .64
;					+-----------------------+
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

				.global		_handle_kernel_interruptionFast
				.global		_handle_kernel_switcher
				.global		_handle_kernel_message
				.extern		_kernel_interruptionFast
				.extern		_kernel_switcher
				.extern		_kernel_message
				.extern		_vKern_stackProcess
				.extern		_vKern_message
				.extern		_saveScratchRegisters
				.extern		_restoreScratchRegisters

				.section	.text

; Interruption Fast
; -----------------

_handle_kernel_interruptionFast:
				jsr			_saveScratchRegisters				; Save the scratch registers
				nop												;
				sjsr		_kernel_interruptionFast			; Process the interruption
				nop												;
				jsr			_restoreScratchRegisters			; Restore the scratch registers
				nop												;
				rte												;
				nop

; Save and restore the context
; ----------------------------

_handle_kernel_switcher:
				mmovl		-(px7),r0-r7						; r7  .. r0
				mmovl		-(px7),px0-px6						; px6 .. px0
				pmov		r3:r2,psu1							; PB PA DM PF
				pmov		r1:r0,psu0							; HD EC EX IN
				mmovl		-(px7),r0-r3						;

				movil.h		r0.u,#0								;
				pmov		r0.l,iel							;
				movbp		+(px7,-4),r0						; 0x00iel
				movil.h		r0.l,#0								;
				pmov		iel,r0.l							; ioff
				pmov		r0,sbr								;
				movbp		+(px7,-4),r0						; sbr

				movuip		r0,#0								;
				pmov		r0.l,scnt							;
				addi		r0,#-1								;
				jmp			pa2,1f								;
				nop												;

				loop		r0.l,1f								;
				pop			r3:r2								; 64-bits HS
				movbp		+(px7,-4),r2						; LSB
				movbp		+(px7,-4),r3						; MSB
1:				addil		r0,#1								;
				movbp		+(px7,-4),r0						; scnt

				movil.h		px0.u,#sval.u(_vKern_stackProcess)	;
				movil.h		px0.l,#sval.l(_vKern_stackProcess)	;
				movbl		(px0,0),px7							; Save the pointer on the context

; Call the nano kernel for switching a task

				sjsr		_kernel_switcher					; Call the nano kernel
				nop												;

; Restore the new context

				movil.h		px0.u,#sval.u(_vKern_stackProcess)	;
				movil.h		px0.l,#sval.l(_vKern_stackProcess)	;

				movbl		px7,(px0,0)							; Recover the pointer on the context
				movbp		r0,(px7,4)+							; scnt
				addil		r0,#-1								;
				jmp			pa2,1f								;
				nop   											;

				loop		r0.l,1f								;
				movbp		r3,(px7,4)+							; MSB
				movbp		r2,(px7,4)+							; LSB
				push		r3:r2								; 64-bits HS

1:				movbp		r0,(px7,4)+							; sbr
				pmov		sbr,r0								;
				movbp		r0,(px7,4)+							; 0x00iel
				pmov		iel,r0.l							;

				mmovl		r0-r3,(px7)+						;
				pmov		psu0,r1:r0							; HD EC EX IN
				pmov		psu1,r3:r2							; PB PA DM PF
				mmovl		px0-px6,(px7)+						; px6 ..  px0
				mmovl		r0-r7,(px7)+						; r7  ..  r0
				rte				 					  			;
				nop

; Save and restore the context (for a trap message)
; -------------------------------------------------

				.set 	posMsg,	84

_handle_kernel_message:
				mmovl		-(px7),r0-r7						; r7  .. r0
				mmovl		-(px7),px0-px6						; px6 .. px0
				pmov		r3:r2,psu1							; PB PA DM PF
				pmov		r1:r0,psu0							; HD EC EX IN
				mmovl		-(px7),r0-r3						;

				movil.h		r0.u,#0								;
				pmov		r0.l,iel							;
				movbp		+(px7,-4),r0						; 0x00iel
				movil.h		r0.l,#0								;
				pmov		iel,r0.l							; ioff
				pmov		r0,sbr								;
				movbp		+(px7,-4),r0						; sbr

				movil.h		px0.u,#sval.u(_vKern_message)		;
				movil.h		px0.l,#sval.l(_vKern_message)		;
				movbl 		r7,(px7,posMsg)						; 84 = sum(r7..r0, px6..px0, psu1..psu0, iel, sbr)
				movbl 		(px0,0),r7							; Save the message

				movuip		r0,#0								;
				pmov		r0.l,scnt							;
				addi		r0,#-1								;
				jmp			pa2,1f								;
				nop												;

				loop		r0.l,1f								;
				pop			r3:r2								; 64-bits HS
				movbp		+(px7,-4),r2						; LSB
				movbp		+(px7,-4),r3						; MSB
1:				addil		r0,#1								;
				movbp		+(px7,-4),r0						; scnt

				movil.h		px0.u,#sval.u(_vKern_stackProcess)	;
				movil.h		px0.l,#sval.l(_vKern_stackProcess)	;
				movbl		(px0,0),px7							; Save the pointer on the context

; Call the nano kernel for switching a task and 
; for the message analisys

				sjsr		_kernel_message						; Call the nano kernel
				nop												;

; Restore the new context

				movil.h		px0.u,#sval.u(_vKern_stackProcess)	;
				movil.h		px0.l,#sval.l(_vKern_stackProcess)	;

				movbl		px7,(px0,0)							; Recover the pointer on the context
				movbp		r0,(px7,4)+							; scnt
				addil		r0,#-1								;
				jmp			pa2,1f								;
				nop   											;
				loop		r0.l,1f								;
				movbp		r3,(px7,4)+							; MSB
				movbp		r2,(px7,4)+							; LSB
				push		r3:r2								; 64-bits HS

1:				movbp		r0,(px7,4)+							; sbr
				pmov		sbr,r0								;
				movbp		r0,(px7,4)+							; 0x00iel
				pmov		iel,r0.l							;

				mmovl		r0-r3,(px7)+						;
				pmov		psu0,r1:r0							; HD EC EX IN
				pmov		psu1,r3:r2							; PB PA DM PF
				mmovl		px0-px6,(px7)+						; px6 ..  px0
				mmovl		r0-r7,(px7)+						; r7  ..  r0
				rte				 					  			;
				nop
