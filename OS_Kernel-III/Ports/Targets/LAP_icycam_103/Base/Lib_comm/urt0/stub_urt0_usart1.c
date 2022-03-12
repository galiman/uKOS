/*
; stub_urt0_usart1.
; =================

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
; Goal:		stub for the connection of the "urt0" manager to the usart1 device.
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

#define	__USART1_SEMA_TX__
#define	__USART1_SEMA_RX__

#define	cnfUsart1_t				cnfUrtx_t

#define	usart1_init				stub_urt0_init			
#define	usart1_configure		stub_urt0_configure		
#define	usart1_write			stub_urt0_write			
#define	usart1_read				stub_urt0_read			
#define	usart1_flush			stub_urt0_flush			

#define	KFREQUENCY_USART1		KFREQUENCY_AHB

#define	KUSART1NOERR			KURT0NOERR
#define	KUSART1SYCNA			KURT0SYCNA
#define	KUSART1GEERR			KURT0GEERR
#define	KUSART1NODEV			KURT0NODEV
#define	KUSART1NOCHA			KURT0NOCHA
#define	KUSART1CHBSY			KURT0CHBSY
#define	KUSART1LNBU0			KURT0LNBU0
#define	KUSART1LNBUB			KURT0LNBUB
#define	KUSART1SBUEM			KURT0SBUEM
#define	KUSART1SBUNE			KURT0SBUNE
#define	KUSART1SBFUL			KURT0SBFUL
#define	KUSART1SEPRO			KURT0SEPRO
#define	KUSART1RBUEM			KURT0RBUEM
#define	KUSART1RBFUL			KURT0RBFUL
#define	KUSART1EROVR			KURT0EROVR
#define	KUSART1ERNOI			KURT0ERNOI
#define	KUSART1ERFRA			KURT0ERFRA
#define	KUSART1ERPAR			KURT0ERPAR
#define	KUSART1NOCNF			KURT0NOCNF
#define	KUSART1NOHAR			KURT0NOHAR
#define	KUSART1TIMEO			KURT0TIMEO
#define	KUSART1SENOE			KURT0SENOE

#define	KSEMAPHORE_USART1RX		KSEMAPHORE_URT0RX
#define	KSEMAPHORE_USART1TX		KSEMAPHORE_URT0TX

#define	KUSART1LNSNDBUF			64
#define	KUSART1LNRECBUF			128

// Model callbacks
// ---------------

/*
 * \brief cb_CTSCheck
 *
 * - Verify the CTS state
 *   If CTS =  1, then disable the uart1 TX interruptions
 *
 */
static	bool_t	cb_CTSCheck(void) {

	return (TRUE);
}

/*
 * \brief cb_init
 *                                   __
 * - Initialize the CTS interruption   \__
 *
 */
static	void	cb_init(void) {

}

#include 	<model_usart1.cm>

