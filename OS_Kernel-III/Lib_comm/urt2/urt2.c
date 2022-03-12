/*
; urt2.
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
; Goal:		urt2 manager.
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

static	bool_t	vReadReserved  = FALSE;
static	bool_t	vWriteReserved = FALSE;

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"urt2         urt2 manager.                             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "urt2 manager\n"
									"============\n\n"

									"This manager ...\n\n";

// Prototypes
// ==========

static	void	_init(void);
extern	void	stub_urt2_init(void);
extern	int32_t	stub_urt2_configure(const cnfUrtx_t *configure);
extern	int32_t	stub_urt2_write(const uint8_t *buffer, uint32_t size);
extern	int32_t	stub_urt2_read(uint8_t *buffer, uint32_t *size);
extern	int32_t	stub_urt2_flush(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Urt2, KIDCOMM, KURT2NUM, NULL, "2.0", (1<<BSHOW));

/*
 * \brief Reserve the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    while (urt2_reserve(KDEVWRITE) == KURT2CHBSY) {
 *        kern_switchFast();
 *    }
 *
 *    urt2_xyz();
 *    status = urt2_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT2NOERR	The manager is reserved
 * \param[out]  KURT2CHBSY 	The manager is busy
 *
 */
int32_t	urt2_reserve(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	switch (mode) {
		case KDEVREAD: {
			if (vReadReserved)  			     {												RETURN_INT_RESTORED(KURT2CHBSY); }
			else 		       				     { vReadReserved = TRUE;						RETURN_INT_RESTORED(KURT2NOERR); }
			break;
		}
		case KDEVWRITE: {
			if (vWriteReserved) 				 { 											    RETURN_INT_RESTORED(KURT2CHBSY); }
			else 		       				     { vWriteReserved = TRUE;						RETURN_INT_RESTORED(KURT2NOERR); }
			break;
		}
		case KDEVALL: {
			if (vReadReserved || vWriteReserved) {                                              RETURN_INT_RESTORED(KURT2CHBSY); }
			else 		       				     { vReadReserved = TRUE; vWriteReserved = TRUE;	RETURN_INT_RESTORED(KURT2NOERR); }
			break;
		}
		default: {
			break;
		}
	}
	RETURN_INT_RESTORED(KURT2NOERR);
}

/*
 * \brief Release the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt2_release(KDEVWRITE);
 * \endcode
 *
 * \param[in]	mode		KDEVREAD, KDEVWRITE, KDEVALL
 * \param[out]	KURT2NOERR	OK
 *
 */
int32_t	urt2_release(uint8_t mode) {

	_init();

	INTERRUPTION_OFF;
	switch (mode) {
		case KDEVREAD:  { vReadReserved = FALSE;                         RETURN_INT_RESTORED(KURT2NOERR); break; }
		case KDEVWRITE: {                        vWriteReserved = FALSE; RETURN_INT_RESTORED(KURT2NOERR); break; }
		case KDEVALL:   { vReadReserved = FALSE; vWriteReserved = FALSE; RETURN_INT_RESTORED(KURT2NOERR); break; }
		default: {
			break;
		}
	}
	RETURN_INT_RESTORED(KURT2NOERR);
}

/*
 * \brief Configure the urt2 manager
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
 *    status = urt2_configure(&configure);
 * \endcode
 *
 * \param[in]	*configure	Ptr on the configuration buffer
 * \param[out]	KURT2NOERR	OK
 * \param[out]	KURT2NOCNF	The configuration does not exist
 *
 */
int32_t	urt2_configure(const cnfUrtx_t *configure) {

	_init();
	return (stub_urt2_configure(configure));
}

/*
 * \brief Write a buffer to the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * #define    KSIZE    10
 *
 * uint8_t    buffer[KSIZE] = { 0,1,2,3,4,5,6,7,8,9 };
 * int32_t    status;
 *
 *    status = urt2_write(buffer, KSIZE);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	size		Size of the buffer
 * \param[out]	KURT2NOERR	OK
 * \param[out]	KURT2SEPRO	The sender is busy
 * \param[out]	KURT2LNBUB	The buffer length is too big
 * \param[out]	KURT2LNBU0	The buffer length is = 0
 *
 */
int32_t	urt2_write(const uint8_t *buffer, uint32_t size) {

	_init();
	return (stub_urt2_write(buffer, size));
}

/*
 * \brief Read a buffer from the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * uint8_t     buffer[1];
 * uint32_t    size;
 * int32_t     status;
 *
 *    size = 1;
 *    status = urt2_read(buffer, &size);
 * \endcode
 *
 * \param[in]	*buffer		Ptr on the buffer
 * \param[in]	*size		Ptr on the size
 * \param[out]	KURT2NOERR	OK
 * \param[out]	KURT2RBUEM	The receiver buffer is empty
 * \param[out]	KURT2RBFUL	The receiver buffer is full
 * \param[out]	KURT2EROVR	Overrun error
 * \param[out]	KURT2ERNOI	Noise error
 * \param[out]	KURT2ERFRA	Framing error
 * \param[out]	KURT2ERPAR	Parity error
 *
 */
int32_t	urt2_read(uint8_t *buffer, uint32_t *size) {

	_init();
	return (stub_urt2_read(buffer, size));
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
 *    status = urt2_getIdSemaphore(KSEMARX, &identifier[0];
 *    status = urt2_getIdSemaphore(KSEMATX, &identifier[1];
 *
 *    iotx_printf(KSYST, "Semaphore ids: %s, ...%s\n", identifier[0], identifier[1]);
 * \endcode
 *
 * \param[in]	semaphore		RX or TX semaphore
 * \param[in]	**identifier	Ptr on the semaphore identifier
 * \param[out]	KURT2NOERR		OK
 * \param[out]	KURT2SENOE		The semaphore does not exist
 *
 */
int32_t	urt2_getIdSemaphore(uint8_t semaphore, char_t **identifier) {

	_init();
	if (semaphore == KSEMARX) { *identifier = KSEMAPHORE_URT2RX; return (KURT2NOERR); }
	if (semaphore == KSEMATX) { *identifier = KSEMAPHORE_URT2TX; return (KURT2NOERR); }
	return (KURT2SENOE);
}

/*
 * \brief Flush the urt2 manager
 *
 * Call example in C:
 *
 * \code{.c}
 * int32_t    status;
 *
 *    status = urt2_flush();
 * \endcode
 *
 * \param[in]	-
 * \param[out]	KURT2NOERR	OK
 *
 */
int32_t	urt2_flush(void) {

	_init();
	return (stub_urt2_flush());
}

// Local routines
// ==============

/*
 * \brief _init
 *
 * - This function initializes the manager and
 *   has to be called at least once.
 *
 */
static	void	_init(void) {
	static	bool_t	init = FALSE;

	if (!init) {
		init = TRUE;

		stub_urt2_init();
	}
}

