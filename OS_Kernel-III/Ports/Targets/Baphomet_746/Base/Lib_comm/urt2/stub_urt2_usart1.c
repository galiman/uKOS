/*
; stub_urt2_usart1.
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
; Goal:		stub for the connection of the "urt2" manager to the usart1 device.
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

#define	usart1_init				stub_urt2_init			
#define	usart1_configure		stub_urt2_configure		
#define	usart1_write			stub_urt2_write			
#define	usart1_read				stub_urt2_read			
#define	usart1_flush			stub_urt2_flush			

#define	KFREQUENCY_USART1		KFREQUENCY_APB2

#define	KUSART1NOERR			KURT2NOERR
#define	KUSART1SYCNA			KURT2SYCNA
#define	KUSART1GEERR			KURT2GEERR
#define	KUSART1NODEV			KURT2NODEV
#define	KUSART1NOCHA			KURT2NOCHA
#define	KUSART1CHBSY			KURT2CHBSY
#define	KUSART1LNBU0			KURT2LNBU0
#define	KUSART1LNBUB			KURT2LNBUB
#define	KUSART1SBUEM			KURT2SBUEM
#define	KUSART1SBUNE			KURT2SBUNE
#define	KUSART1SBFUL			KURT2SBFUL
#define	KUSART1SEPRO			KURT2SEPRO
#define	KUSART1RBUEM			KURT2RBUEM
#define	KUSART1RBFUL			KURT2RBFUL
#define	KUSART1EROVR			KURT2EROVR
#define	KUSART1ERNOI			KURT2ERNOI
#define	KUSART1ERFRA			KURT2ERFRA
#define	KUSART1ERPAR			KURT2ERPAR
#define	KUSART1NOCNF			KURT2NOCNF
#define	KUSART1NOHAR			KURT2NOHAR
#define	KUSART1TIMEO			KURT2TIMEO
#define	KUSART1SENOE			KURT2SENOE

#define	KSEMAPHORE_USART1RX		KSEMAPHORE_URT2RX
#define	KSEMAPHORE_USART1TX		KSEMAPHORE_URT2TX

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

