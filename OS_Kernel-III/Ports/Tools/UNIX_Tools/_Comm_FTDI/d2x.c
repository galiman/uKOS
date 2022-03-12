/*
; d2X.
; ====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2014-01-12
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 166                  $:  Revision of last commit
; $Date:: 2017-07-02 17:51:46#$:  Date of last commit
;
; Project:	uKOS
; Goal:		D2XX uart I/O management.
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

#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include 	<time.h>
#include	<unistd.h>
#include	<ftd2xx.h>
#include	<types_unix.h>

#define	KINITIOREC				3000000									// Initial timeout reception of 30s
#define	KNORTIOREC				200000									// Normal timeout reception of 2s (for stopping the process)

typedef	struct timespec	timespec_t;

/*
 * \brief d2x_list
 *
 * - List of the devices
 * 
 */
void	d2x_list(void) {
	FT_STATUS	status;
	DWORD		nbDevices;
	uint32_t	device;
	char_t		buffer[64];

	status = FT_ListDevices(&nbDevices, NULL, FT_LIST_NUMBER_ONLY);
	if (status == FT_OK) { fprintf(stdout, "Number of device: %0d\n", nbDevices);           }
	else				 { fprintf(stdout, "Error: FT_ListDevices status = %0d\n", status); }

	for (device = 0; device < nbDevices; device++) {
		status = FT_ListDevices((PVOID)((intptr_t)device), buffer, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
		if (status == FT_OK) { fprintf(stdout, "Driver ID: %s\n", buffer);                      }
		else				 { fprintf(stdout, "Error: FT_ListDevices status = %0d\n", status); }
	}
}

/*
 * \brief d2x_open
 *
 * - Open the device
 *
 * \param[in]	*driverID	Ptr on the driver ID "FTXG86D7D"
 * \param[out]	handle		Driver handle ID
 *
 */
FT_HANDLE	d2x_open(const char_t *driverID, int32_t baudrate) {
	FT_STATUS	status;
	FT_HANDLE	handle;

	#if (defined(__APPLE__))
	system("sudo kextunload /Library/Extensions/FTDIUSBSerialDriver.kext/");
	status = FT_OpenEx((PVOID)driverID, FT_OPEN_BY_SERIAL_NUMBER, &handle);
	status = FT_SetBaudRate(handle, baudrate);
	system("sudo kextload /Library/Extensions/FTDIUSBSerialDriver.kext/");

	#elif (defined(__linux__))
	status = FT_OpenEx((PVOID)driverID, FT_OPEN_BY_SERIAL_NUMBER, &handle);
	status = FT_SetBaudRate(handle, baudrate);

	#else     
	#error Platform not supported
	#endif

	return (handle);
}

/*
 * \brief d2x_close
 *
 * - Close the device
 *
 * \param[in]	handle	Driver handle ID
 * \param[out]	-
 *
 */
void	d2x_close(FT_HANDLE handle) {

	FT_Close(handle);
}

/*
 * \brief d2x_readAByte
 *
 * - Read a byte on the device
 *
 * \param[in]	handle		Driver handle ID
 * \param[in]	*receive	Ptr on the receive buffer
 * \param[out]	status		TRUE -> OK
 *							FALSE -> timeout error
 *
 */
bool_t	d2x_readAByte(FT_HANDLE handle, uint8_t *receive) {
			FT_STATUS	status;
			DWORD		eventDWord;
			DWORD		txBytes;
			DWORD		rxBytes;
			DWORD		bytesReceived;
			uint32_t	timeout = 0;
			timespec_t	time;
	static	uint32_t	vTimeoutLimit = KINITIOREC;

	while (TRUE) {
		FT_GetStatus(handle, &rxBytes, &txBytes, &eventDWord);
		if (rxBytes > 0) {
			vTimeoutLimit = KNORTIOREC;
			break;
		}
		time.tv_sec  = 0;
		time.tv_nsec = 10000;
		nanosleep(&time, NULL);
		if (timeout++ > vTimeoutLimit) {
			return (FALSE);
		}
	}
    status = FT_Read(handle, receive, 1, &bytesReceived);
    return (TRUE);
}

/*
 * \brief d2x_read
 *
 * - Read bytes on the device
 *
 * \param[in]	handle		Driver handle ID
 * \param[in]	*receive	Ptr on the receive buffer
 * \param[in]	*count		Ptr on the count
 * \param[out]	status		TRUE -> OK
 *							FALSE -> timeout error
 *
 */
bool_t	d2x_read(FT_HANDLE handle, uint8_t *receive, uint32_t *count) {
			FT_STATUS	status;
			DWORD		eventDWord;
			DWORD		txBytes;
			DWORD		rxBytes;
			DWORD		bytesReceived;
			uint32_t	timeout = 0;
			timespec_t	time;
	static	uint32_t	vTimeoutLimit = KINITIOREC;

	while (TRUE) {
		FT_GetStatus(handle, &rxBytes, &txBytes, &eventDWord);
		if (rxBytes > 0) {
			vTimeoutLimit = KNORTIOREC;
			break;
		}
		time.tv_sec  = 0;
		time.tv_nsec = 10000;
		nanosleep(&time, NULL);
		if (timeout++ > vTimeoutLimit) {
			return (FALSE);
		}
	}
    status = FT_Read(handle, receive, rxBytes, &bytesReceived);
    *count = (uint32_t)rxBytes;
    return (TRUE);
}

/*
 * \brief d2x_flush
 *
 * - Flush the device
 *
 * \param[in]	handle	Driver handle ID
 * \param[out]	-
 *
 */
void	d2x_flush(FT_HANDLE handle) {
	uint8_t		byte;
	DWORD		eventDWord;
	DWORD		txBytes;
	DWORD		rxBytes;
	DWORD		bytesReceived;

	while (TRUE) {
		FT_GetStatus(handle, &rxBytes, &txBytes, &eventDWord);
		if (rxBytes > 0) {
			FT_Read(handle, &byte, 1, &bytesReceived);
		}
		else {
			return;
		}
	}
}

