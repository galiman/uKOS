; first.
; ======

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
; Goal:		Vectors for the uKOS system (first).
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

				.global		_saveScratchRegisters
				.global		_restoreScratchRegisters
				.extern		_exce_display
				.extern		_start

; Exception vectors
; -----------------

				.section	.text

reset:			jmp			_resetHandle
				nop

illopExeption:	jmp			_exce_display_1
				nop

hwcntExeption:	jmp			_exce_display_2
				nop

buerrExeption:	jmp			_exce_display_3
				nop

hwstkExeption:	jmp			_exce_display_4
				nop

debugExeption:	jmp			_exce_display_5
				nop

priviExeption:	jmp			_exce_display_6
				nop

trap1Exeption:	jmp			_exce_display_7
				nop

hdu1Exeption:	jmp			_exce_display_8
				nop

dpu3Exeption:	jmp			_exce_display_9
				nop

dpu2Exeption:	jmp			_exce_display_10
				nop

dmu1Exeption:	jmp			_exce_display_11
				nop

dpu1Exeption:	jmp			_exce_display_12
				nop

dmu0Exeption:	jmp			_exce_display_13
				nop

dpu0Exeption:	jmp			_exce_display_14
				nop

hdu0Exeption:	jmp			_exce_display_15
				nop

irq0Exeption:	jmp			_exce_display_16
				nop

irq1Exeption:	jmp			_exce_display_17
				nop

irq2Exeption:	jmp			_exce_display_18
				nop

irq3Exeption:	jmp			_exce_display_19
				nop

irq4Exeption:	jmp			_exce_display_20
				nop

irq5Exeption:	jmp			_exce_display_21
				nop

irq6Exeption:	jmp			_exce_display_22
				nop

irq7Exeption:	jmp			_exce_display_23
				nop

trap0Exeption:	jmp			_exce_display_24
				nop

; Some initializations
; --------------------

_resetHandle:
				movil.h		px7.u,#sval.u(_stStack)					; Upper the supervisor stack pointer
				movil.h 	px7.l,#sval.l(_stStack)					; Lower the supervisor stack pointer
				pxmov		px6,px7									; U-L frame pointer
				nop

				jmp			_start
				nop

_exce_display_1:													;
				movsip.h	r7.l, #1								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_2:													;
				movsip.h	r7.l, #2								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_3:													;
				movsip.h	r7.l, #3								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_4:													;
				movsip.h	r7.l, #4								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_5:													;
				movsip.h	r7.l, #5								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_6:													;
				movsip.h	r7.l, #6								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_7:													;
				movsip.h	r7.l, #7								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_8:													;
				movsip.h	r7.l, #8								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_9:													;
				movsip.h	r7.l, #9								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_10:													;
				movsip.h	r7.l, #10								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_11:													;
				movsip.h	r7.l, #11								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_12:													;
				movsip.h	r7.l, #12								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_13:													;
				movsip.h	r7.l, #13								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_14:													;
				movsip.h	r7.l, #14								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_15:													;
				movsip.h	r7.l, #15								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_16:													;
				movsip.h	r7.l, #16								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_17:													;
				movsip.h	r7.l, #17								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_18:													;
				movsip.h	r7.l, #18								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_19:													;
				movsip.h	r7.l, #19								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_20:													;
				movsip.h	r7.l, #20								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_21:													;
				movsip.h	r7.l, #21								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_22:													;
				movsip.h	r7.l, #22								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_23:													;
				movsip.h	r7.l, #23								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

_exce_display_24:													;
				movsip.h	r7.l, #24								; Exception number
				sjsr		_exce_display							; !!! Display it and never return !!!
				nop

; Save and restore the scratch registers
; --------------------------------------

_saveScratchRegisters:
				mmovl		-(px7),r4-r7		; Save r4, r5, r6, r7
				pmov		r5:r4,psu1 			;
				mmovl		-(px7),r4-r5		; Save the flags
				mmovl		-(px7),px0-px2		; Save px0, px1, px2
				movsbr		-(px7),sbr			; Save the sbr
				rts
				nop

_restoreScratchRegisters:
				movsbr		sbr,(px7)+			; Restore the sbr
				mmovl		px0-px2,(px7)+		; Restore px0, px1, px2
				mmovl		r4-r5,(px7)+		;
				pmov		psu1,r5:r4			; Restore the flags
				mmovl		r4-r7,(px7)+		; Restore r4, r5, r6, r7
				rts
				nop
