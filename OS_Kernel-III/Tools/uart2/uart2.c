/*
; uart2.
; ======

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
; Goal:		Set the baud-rate of the uart2 manager.
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

#include	<uKOS.h>

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"uart2        Set the baud-rate of the urt2 manager.    (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Set the baud-rate of the urt2 manager\n"
									"=====================================\n\n"

									"This tool sets the baud-rate of the serial\n"
									"manager urt2.\n\n"

									"Input format:  uart2 baud-rate\n"
									"Output format: [result]\n\n"

									"Possible baud-rates:\n"
									"   2400 4800 9600 19200 38400\n"
									"   57600 115200 230400 460800 500000\n"
									"   921600 1000000 1500000 1843200\n"
									"   2000000 2500000 3000000-bits/s.\n\n";

// Prototypes
// ==========

static	int32_t	prgm(uint32_t argc, const char_t *argv[]);
static	void	_setBaudRate(const char_t *strBaudRate, uint32_t baudRate);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Uart2, KIDTOOL, KUART2NUM, prgm, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief Main entry point
 *
 */
static	int32_t	prgm(uint32_t argc, const char_t *argv[]) {
	int32_t		status, binary;
	bool_t		error = FALSE;

	iotx_printf(KSYST, "Uart configuration.\n");
	iotx_printf(KSYST, __DATE__"  "__TIME__" (c) EFr-2017\n\n");

	if (argc != 2) {
		error = TRUE;
	}
	else {
		strg_cnvtDecAsciiToBufInt32(&binary, argv[1], 1);

		switch (binary) {
			case 2400:		{ _setBaudRate("2400",    KBD2400);    break; }
			case 4800:		{ _setBaudRate("4800",    KBD4800);    break; }
			case 9600:		{ _setBaudRate("9600",    KBD9600);    break; }
			case 19200:		{ _setBaudRate("19200",   KBD19200);   break; }
			case 38400:		{ _setBaudRate("38400",   KBD38400);   break; }
			case 57600:		{ _setBaudRate("57600",   KBD57600);   break; }
			case 115200:	{ _setBaudRate("115200",  KBD115200);  break; }
			case 230400:	{ _setBaudRate("230400",  KBD230400);  break; }
			case 460800:	{ _setBaudRate("460800",  KBD460800);  break; }
			case 500000:	{ _setBaudRate("500000",  KBD500000);  break; }
			case 921600:	{ _setBaudRate("921600",  KBD921600);  break; }
			case 1000000:	{ _setBaudRate("1000000", KBD1000000); break; }
			case 1500000:	{ _setBaudRate("1500000", KBD1500000); break; }
			case 1843200:	{ _setBaudRate("1843200", KBD1843200); break; }
			case 2000000:	{ _setBaudRate("2000000", KBD2000000); break; }
			case 2500000:	{ _setBaudRate("2500000", KBD2500000); break; }
			case 3000000:	{ _setBaudRate("3000000", KBD3000000); break; }
			default:		{ _setBaudRate("460800",  KBD460800);  break; }
		}
	}

	if (!error) {										 	 status = EXIT_SUCCESS_OS; }
	else        { iotx_printf(KSYST, "Protocol error.\n\n"); status = EXIT_FAILURE;    }
	return (status);
}

// Local routines
// ==============

/*
 * \brief _setBaudRate
 *
 * - Set a new Baud-Rate
 *
 */
static	void	_setBaudRate(const char_t *strBaudRate, uint32_t baudRate) {
	cnfUrtx_t	configureURTx;

	configureURTx.oNBits    = K8BIT;
	configureURTx.oStopBits = K1STBIT;
	configureURTx.oParity   = KNONE;
	configureURTx.oBaudRate = baudRate;
	configureURTx.oKernSync = (1<<BRXSEMA);

	iotx_printf(KSYST, "Change the terminal to %s-bits/s.\n\n", strBaudRate);
	kern_suspendProcess(500);
	urt2_configure(&configureURTx);
}

