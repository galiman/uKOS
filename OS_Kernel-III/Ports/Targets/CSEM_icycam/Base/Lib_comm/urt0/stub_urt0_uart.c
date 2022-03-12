/*
; stub_urt0_uart.
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
; Goal:		stub for the connection of the "urt0" manager to the uart device.
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

#define	__UART_SEMA_TX__
#define	__UART_SEMA_RX__

#define	cnfUart_t				cnfUrtx_t

#define	uart_init				stub_urt0_init			
#define	uart_configure			stub_urt0_configure		
#define	uart_write				stub_urt0_write			
#define	uart_read				stub_urt0_read			
#define	uart_flush				stub_urt0_flush			

#define	KUARTNOERR				KURT0NOERR
#define	KUARTSYCNA				KURT0SYCNA
#define	KUARTGEERR				KURT0GEERR
#define	KUARTNODEV				KURT0NODEV
#define	KUARTNOCHA				KURT0NOCHA
#define	KUARTCHBSY				KURT0CHBSY
#define	KUARTLNBU0				KURT0LNBU0
#define	KUARTLNBUB				KURT0LNBUB
#define	KUARTSBUEM				KURT0SBUEM
#define	KUARTSBUNE				KURT0SBUNE
#define	KUARTSBFUL				KURT0SBFUL
#define	KUARTSEPRO				KURT0SEPRO
#define	KUARTRBUEM				KURT0RBUEM
#define	KUARTRBFUL				KURT0RBFUL
#define	KUARTEROVR				KURT0EROVR
#define	KUARTERNOI				KURT0ERNOI
#define	KUARTERFRA				KURT0ERFRA
#define	KUARTERPAR				KURT0ERPAR
#define	KUARTNOCNF				KURT0NOCNF
#define	KUARTNOHAR				KURT0NOHAR
#define	KUARTTIMEO				KURT0TIMEO
#define	KUARTSENOE				KURT0SENOE

#define	KSEMAPHORE_UARTRX		KSEMAPHORE_URT0RX
#define	KSEMAPHORE_UARTTX		KSEMAPHORE_URT0TX

#define	KUARTLNSNDBUF			64
#define	KUARTLNRECBUF			128

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

#include 	<model_uart.cm>

