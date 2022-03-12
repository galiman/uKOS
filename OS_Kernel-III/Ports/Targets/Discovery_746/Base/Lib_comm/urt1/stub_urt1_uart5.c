/*
; stub_urt5_uart5.
; ================

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
; Goal:		stub for the connection of the "urt1" manager to the uart5 device.
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

#define	__UART5_SEMA_TX__
#define	__UART5_SEMA_RX__

#define	cnfUart5_t				cnfUrtx_t

#define	uart5_init				stub_urt1_init			
#define	uart5_configure			stub_urt1_configure		
#define	uart5_write				stub_urt1_write			
#define	uart5_read				stub_urt1_read			
#define	uart5_flush				stub_urt1_flush			

#define	KFREQUENCY_UART5		KFREQUENCY_APB1

#define	KUART5NOERR				KURT1NOERR
#define	KUART5SYCNA				KURT1SYCNA
#define	KUART5GEERR				KURT1GEERR
#define	KUART5NODEV				KURT1NODEV
#define	KUART5NOCHA				KURT1NOCHA
#define	KUART5CHBSY				KURT1CHBSY
#define	KUART5LNBU0				KURT1LNBU0
#define	KUART5LNBUB				KURT1LNBUB
#define	KUART5SBUEM				KURT1SBUEM
#define	KUART5SBUNE				KURT1SBUNE
#define	KUART5SBFUL				KURT1SBFUL
#define	KUART5SEPRO				KURT1SEPRO
#define	KUART5RBUEM				KURT1RBUEM
#define	KUART5RBFUL				KURT1RBFUL
#define	KUART5EROVR				KURT1EROVR
#define	KUART5ERNOI				KURT1ERNOI
#define	KUART5ERFRA				KURT1ERFRA
#define	KUART5ERPAR				KURT1ERPAR
#define	KUART5NOCNF				KURT1NOCNF
#define	KUART5NOHAR				KURT1NOHAR
#define	KUART5TIMEO				KURT1TIMEO
#define	KUART5SENOE				KURT1SENOE

#define	KSEMAPHORE_UART5RX		KSEMAPHORE_URT1RX
#define	KSEMAPHORE_UART5TX		KSEMAPHORE_URT1TX

#define	KUART5LNSNDBUF			64
#define	KUART5LNRECBUF			128

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

#include 	<model_uart5.cm>

