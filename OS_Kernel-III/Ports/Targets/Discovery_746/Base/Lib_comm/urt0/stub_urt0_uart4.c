/*
; stub_urt0_uart4.
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
; Goal:		stub for the connection of the "urt0" manager to the uart4 device.
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

#define	__UART4_SEMA_TX__
#define	__UART4_SEMA_RX__

#define	cnfUart4_t				cnfUrtx_t

#define	uart4_init				stub_urt0_init			
#define	uart4_configure			stub_urt0_configure		
#define	uart4_write				stub_urt0_write			
#define	uart4_read				stub_urt0_read			
#define	uart4_flush				stub_urt0_flush			

#define	KFREQUENCY_UART4		KFREQUENCY_APB1

#define	KUART4NOERR				KURT0NOERR
#define	KUART4SYCNA				KURT0SYCNA
#define	KUART4GEERR				KURT0GEERR
#define	KUART4NODEV				KURT0NODEV
#define	KUART4NOCHA				KURT0NOCHA
#define	KUART4CHBSY				KURT0CHBSY
#define	KUART4LNBU0				KURT0LNBU0
#define	KUART4LNBUB				KURT0LNBUB
#define	KUART4SBUEM				KURT0SBUEM
#define	KUART4SBUNE				KURT0SBUNE
#define	KUART4SBFUL				KURT0SBFUL
#define	KUART4SEPRO				KURT0SEPRO
#define	KUART4RBUEM				KURT0RBUEM
#define	KUART4RBFUL				KURT0RBFUL
#define	KUART4EROVR				KURT0EROVR
#define	KUART4ERNOI				KURT0ERNOI
#define	KUART4ERFRA				KURT0ERFRA
#define	KUART4ERPAR				KURT0ERPAR
#define	KUART4NOCNF				KURT0NOCNF
#define	KUART4NOHAR				KURT0NOHAR
#define	KUART4TIMEO				KURT0TIMEO
#define	KUART4SENOE				KURT0SENOE

#define	KSEMAPHORE_UART4RX		KSEMAPHORE_URT0RX
#define	KSEMAPHORE_UART4TX		KSEMAPHORE_URT0TX

#define	KUART4LNSNDBUF			64
#define	KUART4LNRECBUF			128

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

#include 	<model_uart4.cm>

