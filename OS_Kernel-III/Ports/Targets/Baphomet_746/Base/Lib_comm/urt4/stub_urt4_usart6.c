/*
; stub_urt4_usart6.
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
; Goal:		stub for the connection of the "urt4" manager to the usart6 device.
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

#define	__USART6_SEMA_TX__
#define	__USART6_SEMA_RX__

#define	cnfUsart6_t				cnfUrtx_t

#define	usart6_init				stub_urt4_init			
#define	usart6_configure		stub_urt4_configure		
#define	usart6_write			stub_urt4_write			
#define	usart6_read				stub_urt4_read			
#define	usart6_flush			stub_urt4_flush			

#define	KFREQUENCY_USART6		KFREQUENCY_APB2

#define	KUSART6NOERR			KURT4NOERR
#define	KUSART6SYCNA			KURT4SYCNA
#define	KUSART6GEERR			KURT4GEERR
#define	KUSART6NODEV			KURT4NODEV
#define	KUSART6NOCHA			KURT4NOCHA
#define	KUSART6CHBSY			KURT4CHBSY
#define	KUSART6LNBU0			KURT4LNBU0
#define	KUSART6LNBUB			KURT4LNBUB
#define	KUSART6SBUEM			KURT4SBUEM
#define	KUSART6SBUNE			KURT4SBUNE
#define	KUSART6SBFUL			KURT4SBFUL
#define	KUSART6SEPRO			KURT4SEPRO
#define	KUSART6RBUEM			KURT4RBUEM
#define	KUSART6RBFUL			KURT4RBFUL
#define	KUSART6EROVR			KURT4EROVR
#define	KUSART6ERNOI			KURT4ERNOI
#define	KUSART6ERFRA			KURT4ERFRA
#define	KUSART6ERPAR			KURT4ERPAR
#define	KUSART6NOCNF			KURT4NOCNF
#define	KUSART6NOHAR			KURT4NOHAR
#define	KUSART6TIMEO			KURT4TIMEO
#define	KUSART6SENOE			KURT4SENOE

#define	KSEMAPHORE_USART6RX		KSEMAPHORE_URT4RX
#define	KSEMAPHORE_USART6TX		KSEMAPHORE_URT4TX

#define	KUSART6LNSNDBUF			64
#define	KUSART6LNRECBUF			128

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

#include 	<model_usart6.cm>

