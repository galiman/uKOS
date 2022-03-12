/*
; stub_xfer_send.
; ===============

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the "xfer" manager module.
;			Idea, concept and original implementation by Nussbaum Pascal.
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

#include 	<uKOS.h>

// Connect the physical device to the logical manager
// --------------------------------------------------

#define	cnfSend_t			cnfXfer_t

#define	send_init			stub_xfer_init			
#define	send_configure		stub_xfer_configure			
#define	send_setXferBuffer	stub_xfer_setXferBuffer			
#define	send_startXfer		stub_xfer_startXfer			
#define	send_stopXfer		stub_xfer_stopXfer			

#define	KSENDNOERR			KXFERNOERR
#define	KSENDSYCNA			KXFERSYCNA
#define	KSENDGEERR			KXFERGEERR
#define	KSENDCHBSY			KXFERCHBSY

#include 	<model_send_spi4_dma2.cm>

