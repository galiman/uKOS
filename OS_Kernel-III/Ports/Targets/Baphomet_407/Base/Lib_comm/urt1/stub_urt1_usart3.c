/*
; stub_urt1_usart3.
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
; Goal:		stub for the connection of the "urt1" manager to the usart3 device.
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

#define	__USART3_SEMA_TX__
#define	__USART3_SEMA_RX__

#define	cnfUsart3_t				cnfUrtx_t

#define	usart3_init				stub_urt1_init			
#define	usart3_configure		stub_urt1_configure		
#define	usart3_write			stub_urt1_write			
#define	usart3_read				stub_urt1_read			
#define	usart3_flush			stub_urt1_flush			

#define	KFREQUENCY_USART3		KFREQUENCY_APB1

#define	KUSART3NOERR			KURT1NOERR
#define	KUSART3SYCNA			KURT1SYCNA
#define	KUSART3GEERR			KURT1GEERR
#define	KUSART3NODEV			KURT1NODEV
#define	KUSART3NOCHA			KURT1NOCHA
#define	KUSART3CHBSY			KURT1CHBSY
#define	KUSART3LNBU0			KURT1LNBU0
#define	KUSART3LNBUB			KURT1LNBUB
#define	KUSART3SBUEM			KURT1SBUEM
#define	KUSART3SBUNE			KURT1SBUNE
#define	KUSART3SBFUL			KURT1SBFUL
#define	KUSART3SEPRO			KURT1SEPRO
#define	KUSART3RBUEM			KURT1RBUEM
#define	KUSART3RBFUL			KURT1RBFUL
#define	KUSART3EROVR			KURT1EROVR
#define	KUSART3ERNOI			KURT1ERNOI
#define	KUSART3ERFRA			KURT1ERFRA
#define	KUSART3ERPAR			KURT1ERPAR
#define	KUSART3NOCNF			KURT1NOCNF
#define	KUSART3NOHAR			KURT1NOHAR
#define	KUSART3TIMEO			KURT1TIMEO
#define	KUSART3SENOE			KURT1SENOE

#define	KSEMAPHORE_USART3RX		KSEMAPHORE_URT1RX
#define	KSEMAPHORE_USART3TX		KSEMAPHORE_URT1TX

#define	KUSART3LNSNDBUF			64
#define	KUSART3LNRECBUF			128

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

#include 	<model_usart3.cm>

