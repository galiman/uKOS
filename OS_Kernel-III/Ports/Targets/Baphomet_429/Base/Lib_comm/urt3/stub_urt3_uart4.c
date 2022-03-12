/*
; stub_urt3_uart4.
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
; Goal:		stub for the connection of the "urt3" manager to the uart4 device.
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

#define	uart4_init				stub_urt3_init			
#define	uart4_configure			stub_urt3_configure		
#define	uart4_write				stub_urt3_write			
#define	uart4_read				stub_urt3_read			
#define	uart4_flush				stub_urt3_flush			

#define	KFREQUENCY_UART4		KFREQUENCY_APB1

#define	KUART4NOERR				KURT3NOERR
#define	KUART4SYCNA				KURT3SYCNA
#define	KUART4GEERR				KURT3GEERR
#define	KUART4NODEV				KURT3NODEV
#define	KUART4NOCHA				KURT3NOCHA
#define	KUART4CHBSY				KURT3CHBSY
#define	KUART4LNBU0				KURT3LNBU0
#define	KUART4LNBUB				KURT3LNBUB
#define	KUART4SBUEM				KURT3SBUEM
#define	KUART4SBUNE				KURT3SBUNE
#define	KUART4SBFUL				KURT3SBFUL
#define	KUART4SEPRO				KURT3SEPRO
#define	KUART4RBUEM				KURT3RBUEM
#define	KUART4RBFUL				KURT3RBFUL
#define	KUART4EROVR				KURT3EROVR
#define	KUART4ERNOI				KURT3ERNOI
#define	KUART4ERFRA				KURT3ERFRA
#define	KUART4ERPAR				KURT3ERPAR
#define	KUART4NOCNF				KURT3NOCNF
#define	KUART4NOHAR				KURT3NOHAR
#define	KUART4TIMEO				KURT3TIMEO
#define	KUART4SENOE				KURT3SENOE

#define	KSEMAPHORE_UART4RX		KSEMAPHORE_URT3RX
#define	KSEMAPHORE_UART4TX		KSEMAPHORE_URT3TX

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

