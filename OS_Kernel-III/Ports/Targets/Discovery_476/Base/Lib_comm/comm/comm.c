/*
; comm.
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
; Goal:		comm manager.
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

static	uint32_t	vDefCommDev;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"comm         comm manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "comm manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_getDevice(uint32_t commManager, uint32_t *manager);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Comm, KIDCOMM, KCOMMNUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Reserve the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (comm_reserve(KDEF0, KDEVWRITE) == KCOMMCHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    comm_xyz();
 *    status = comm_release(KDEF0, KDEVWRITE);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
int32_t	comm_reserve(uint32_t commManager, uint8_t mode) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_reserve(mode)); }
		default:    { return (KCOMMNODEV);         }
	}
}

/*
 * \brief Release the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = comm_release(KDEF0, KDEVWRITE);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
int32_t	comm_release(uint32_t commManager, uint8_t mode) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_release(mode)); }
		default:    { return (KCOMMNODEV);         }
	}
}

/*
 * \brief Configure the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * const      cnfUrtx_t    configure = {
 *                             .oBaudRate = KBD57600,
 *                             .oKernSync = (1<<BRXSEMA),
 *                             .oNBits    = K8BIT,
 *                             .oStopBits = K1STBIT,
 *                             .oParity   = KNONE
 *                         };
 *
 *    status = comm_configure(KURT0, (void *)&configure);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
int32_t	comm_configure(uint32_t commManager, const void *configure) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_configure((const cnfUrtx_t *)configure)); }
		default:    { return (KCOMMNODEV);                                   }
	}
}

/*
 * \brief Write a buffer to the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = comm_write(KDEF0, buffer, KSIZE);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
int32_t	comm_write(uint32_t commManager, const uint8_t *buffer, uint32_t size) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_write(buffer, size)); }
		default:    { return (KCOMMNODEV);               }
	}
}

/*
 * \brief Read a buffer from the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = comm_read(KDEF0, buffer, &size);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[in]	*data		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	-			Depends on the "xxxx" communication manager
 *
 */
int32_t	comm_read(uint32_t commManager, uint8_t *buffer, uint32_t *size) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_read(buffer, size)); }
		default:    { return (KCOMMNODEV);              }
	}
}

/*
 * \brief Get the semaphore id
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 * char_t     *identifier[2];
 *
 *    status = comm_getIdSemaphore(KURT0, KSEMARX, &identifier[0];
 *    status = comm_getIdSemaphore(KURT0, KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	commManager		Communication manager
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore Id
 * \param[out]	-				Depends on the "xxxx" communication manager
 *
 */
int32_t	comm_getIdSemaphore(uint32_t commManager, uint8_t semaphore, char_t **identifier) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_getIdSemaphore(semaphore, identifier)); }
		default:    { return (KCOMMNODEV);                                 }
	}
}

/*
 * \brief Flush the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = comm_flush(KDEF0);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[out]	KCOMMNOERR	OK
 * \param[out]	KCOMMNODEV	The commManager does not exist
 *
 */
int32_t	comm_flush(uint32_t commManager) {
	uint32_t 	manager;

	_getDevice(commManager, &manager);

	switch (manager) {
		case KURT0: { return (urt0_flush()); }
		default:    { return (KCOMMNODEV);   }
	}
}

/*
 * \brief Set the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = comm_setCommDev(KURT0);
 * \endcode
 *
 * \param[in]	commManager	Communication manager
 * \param[out]	KCOMMNOERR	OK
 *
 */
int32_t	comm_setCommDev(uint32_t commManager) {

	vDefCommDev = commManager;
	return (KCOMMNOERR);
}

/*
 * \brief Get the communication manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t     status;
 * uint32_t    commManager;
 *
 *    status = comm_getCommDev(&commManager);
 * \endcode
 *
 * \param[in]	*commManager	Ptr on the communication manager
 * \param[out]	KCOMMNOERR		OK
 *
 */
int32_t	comm_getCommDev(uint32_t *commManager) {

	*commManager = vDefCommDev;
	return (KCOMMNOERR);
}

// Local routines
// ==============

/*
 * \brief _getDevice
 *
 *	- if (commManager == KSYST) ---> use the communication manager coming from the process
 *	- if (commManager == KDEF0) ---> use the default communication manager
 *	- if (commManager == KXYZT) ---> use the KXYZT communication manager
 *
 */
static	void	_getDevice(uint32_t commManager, uint32_t *manager) {
	volatile	proc_t 	*process;

	*manager = commManager;
	if (commManager == KSYST) {
		kern_getProcessRun(&process);
		*manager = process->oSpecification.oCommManager;
	}
	if ((commManager == KDEF0) || (*manager == KDEF0)) {
		*manager = vDefCommDev;
	}
}
