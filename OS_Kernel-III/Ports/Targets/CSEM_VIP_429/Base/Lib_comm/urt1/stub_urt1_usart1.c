/*
; stub_urt1_usart1.
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
; Goal:		stub for the connection of the "urt1" manager to the usart1 device.
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

#define	usart1_init				stub_urt1_init			
#define	usart1_configure		stub_urt1_configure		
#define	usart1_write			stub_urt1_write			
#define	usart1_read				stub_urt1_read			
#define	usart1_flush			stub_urt1_flush			

#define	KFREQUENCY_USART1		KFREQUENCY_APB2

#define	KUSART1NOERR			KURT1NOERR
#define	KUSART1SYCNA			KURT1SYCNA
#define	KUSART1GEERR			KURT1GEERR
#define	KUSART1NODEV			KURT1NODEV
#define	KUSART1NOCHA			KURT1NOCHA
#define	KUSART1CHBSY			KURT1CHBSY
#define	KUSART1LNBU0			KURT1LNBU0
#define	KUSART1LNBUB			KURT1LNBUB
#define	KUSART1SBUEM			KURT1SBUEM
#define	KUSART1SBUNE			KURT1SBUNE
#define	KUSART1SBFUL			KURT1SBFUL
#define	KUSART1SEPRO			KURT1SEPRO
#define	KUSART1RBUEM			KURT1RBUEM
#define	KUSART1RBFUL			KURT1RBFUL
#define	KUSART1EROVR			KURT1EROVR
#define	KUSART1ERNOI			KURT1ERNOI
#define	KUSART1ERFRA			KURT1ERFRA
#define	KUSART1ERPAR			KURT1ERPAR
#define	KUSART1NOCNF			KURT1NOCNF
#define	KUSART1NOHAR			KURT1NOHAR
#define	KUSART1TIMEO			KURT1TIMEO
#define	KUSART1SENOE			KURT1SENOE

#define	KSEMAPHORE_USART1RX		KSEMAPHORE_URT1RX
#define	KSEMAPHORE_USART1TX		KSEMAPHORE_URT1TX

#define	KUSART1LNSNDBUF			128
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

