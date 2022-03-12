/*
; stub_urt0_usart2.
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
; Goal:		stub for the connection of the "urt0" manager to the usart2 device.
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

#define	__USART2_SEMA_TX__
#define	__USART2_SEMA_RX__
#define	__USART2_DMA_TX__

#define	cnfUsart2_t				cnfUrtx_t

#define	usart2_init				stub_urt0_init			
#define	usart2_configure		stub_urt0_configure		
#define	usart2_write			stub_urt0_write			
#define	usart2_read				stub_urt0_read			
#define	usart2_flush			stub_urt0_flush			

#define	KFREQUENCY_USART2		KFREQUENCY_APB1

#define	KUSART2NOERR			KURT0NOERR
#define	KUSART2SYCNA			KURT0SYCNA
#define	KUSART2GEERR			KURT0GEERR
#define	KUSART2NODEV			KURT0NODEV
#define	KUSART2NOCHA			KURT0NOCHA
#define	KUSART2CHBSY			KURT0CHBSY
#define	KUSART2LNBU0			KURT0LNBU0
#define	KUSART2LNBUB			KURT0LNBUB
#define	KUSART2SBUEM			KURT0SBUEM
#define	KUSART2SBUNE			KURT0SBUNE
#define	KUSART2SBFUL			KURT0SBFUL
#define	KUSART2SEPRO			KURT0SEPRO
#define	KUSART2RBUEM			KURT0RBUEM
#define	KUSART2RBFUL			KURT0RBFUL
#define	KUSART2EROVR			KURT0EROVR
#define	KUSART2ERNOI			KURT0ERNOI
#define	KUSART2ERFRA			KURT0ERFRA
#define	KUSART2ERPAR			KURT0ERPAR
#define	KUSART2NOCNF			KURT0NOCNF
#define	KUSART2NOHAR			KURT0NOHAR
#define	KUSART2TIMEO			KURT0TIMEO
#define	KUSART2SENOE			KURT0SENOE

#define	KSEMAPHORE_USART2RX		KSEMAPHORE_URT0RX
#define	KSEMAPHORE_USART2TX		KSEMAPHORE_URT0TX

#define	KUSART2LNSNDBUF			64
#define	KUSART2LNRECBUF			128

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

#include 	<model_usart2.cm>

