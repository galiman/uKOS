/*
; prt0.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		prt0 SLAVE manager.
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

#ifndef	__PRT0__
#define	__PRT0__

/*!
 * \addtogroup Lib_peri
 */
/*@{*/

#include 	<modules.h>

#define	KPRT0MAN	(KPRT0NUM<<8)
#define	KPRT0ERR	((KIDPERI<<24) | KPRT0MAN)

// Configuration structure
// -----------------------

typedef	struct	cnfPrt0	cnfPrt0_t;

struct cnfPrt0 {
			uint8_t		*oBuffer;										// Buffer data
			uint32_t	oNbBytes;										// Number of Bytes to transfer
			uint32_t	oCommand;										// Command
			#define		KRDHEADER	0									// read the header
			#define		KRDPAYLOAD	1									// read the pay-load
			#define		KWRPAYLOAD	2									// write the pay-load
			#define		KWTHEADER	3									// waiting for header reception
			#define		KWTPAYLOAD	4									// waiting for pay-load reception
};

// Semaphores
// ----------

#define	KSEMAPHORE_PRT0	"prt0 - Header/Pay-load OK"

// Prototypes
// ----------

#ifdef	__cplusplus
extern	"C" {
#endif

#define	PRT0_reserve	prt0_reserve
#define	PRT0_release	prt0_release

/*!
 * \brief Reserve the prt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (prt0_reserve(KDEVALL) == KPRT0CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    prt0_xyz();
 *    status = prt0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KPRT0NOERR	The manager is reserved
 * \param[out]	KPRT0CHBSY 	The manager is busy
 *
 */
extern	int32_t	prt0_reserve(uint8_t mode);

/*!
 * \brief Release the prt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = prt0_release(KDEVALL);
 * \endcode
 *
 * \param[in]	mode		Any mode
 * \param[out]	KPRT0NOERR	OK
 *
 */
extern	int32_t	prt0_release(uint8_t mode);

/*!
 * \brief Write a command to the prt0 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t      status;
 * uint8_t      buffer[256];
 * cnfPrt0_t    command = {
 *                  &buffer[0],
 *                  256,
 *                  KWTPAYLOAD
 *              };
 *
 *    status = prt0_writeCommand(&command);
 * \endcode
 *
 * \param[in]	*command	Ptr on the prt0 command
 * \param[out]	KPRT0NOERR	OK
 * \param[out]	KPRT0LNBUW	The buffer length is wrong
 * \param[out]	KPRT0LNBUB	The buffer length is too big
 * \param[out]	KPRT0LNBU0	The buffer length is = 0
 * \param[out]	KPRT0PALPH	The pay-load phase is on progress
 * \param[out]	KPRT0CMNSU	Command not supported
 *
 */
extern	int32_t	prt0_writeCommand(cnfPrt0_t *command);

#ifdef	__cplusplus
}
#endif

// prt0 manager errors
// -------------------

#define	KPRT0NOERR	0				// No error
#define	KPRT0SYCNA	-(KPRT0ERR+1)	// System call not available
#define	KPRT0GEERR	-(KPRT0ERR+2)	// General error
#define	KPRT0NOHEA	-(KPRT0ERR+3)	// No header
#define	KPRT0LNBUW	-(KPRT0ERR+4)	// The buffer length is wrong
#define	KPRT0LNBU0	-(KPRT0ERR+5)	// The buffer length is = 0
#define	KPRT0LNBUB	-(KPRT0ERR+6)	// The buffer length is too big
#define	KPRT0PALPH	-(KPRT0ERR+7)	// The pay-load phase is on progress
#define	KPRT0CHBSY	-(KPRT0ERR+8)	// The manager is busy
#define	KPRT0NOACK	-(KPRT0ERR+9)	// No ack bit
#define	KPRT0TROPR	-(KPRT0ERR+10)	// Transfer on progress
#define	KPRT0CMNSU	-(KPRT0ERR+11)	// Command not supported

/*@}*/

#endif
