			list b=4

; test_a.
; =======

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
; Goal:		Simple test for displaying a pattern on the board LEDs.
;			The program was written by Jean-Daniel Nicoud in CALM
;			assembler. This code is the Microchip assembler version.
;			The target can be requested at:
;
;			http://www.didel.com
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

#include	<p16F630.inc>

errorlevel  -302    			; Suppress message 302 from list file
errorlevel  -224    			; Suppress message 224 from list file

			__CONFIG	0x3F94

KPATTERN	equ		0x55		; Visual pattern

			org		0x0020

vCx1		res		1			; For the number of calibrated loops
vCx2		res		1			; For the number of loops

			org		0x0000

; Set-up the cpu and the I/Os
; ---------------------------

			bsf		STATUS,RP0	; Set the bank 1
			call	0x3FF		;
			movwf	OSCCAL		; Calibrate
			bcf		STATUS,RP0	; Set the bank 0

			movlw	0x00		;
			tris	PORTA		;
			movlw	0x00		;
			tris	PORTC		; Directions -> outs

; The main loop
; -------------

			movlw	KPATTERN	; 
Loop:		xorlw	-1			; Inverse the pattern
			movwf	PORTA		;
			movwf	PORTC		; Display it

l0:			nop					; For a small delay
			decfsz	vCx1,1		;
			goto	l0			; 256 times this loop

			decfsz	vCx2,1		;
			goto	l0			; 256 the previous 
			goto	Loop		; Forever

			org		0x0018
			de		"Pict630 "

			end

