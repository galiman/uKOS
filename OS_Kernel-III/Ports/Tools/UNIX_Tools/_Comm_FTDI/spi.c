/*
; spi.
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
; Goal:		D2XX spi I/O management.
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
#include	<time.h>
#include	<unistd.h>
#include	<ftd2xx.h>
#include	<ftdi_common.h>
#include	<types_unix.h>

typedef	struct timespec	timespec_t;

/*
 * \brief spi_list
 *
 * - List of the devices
 * 
 */
void	spi_list(void) {
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
 * \brief spi_open
 *
 * - Open the device
 *
 * \param[in]	*device	Ptr on the device ID "FTXG86D7D"
 * \param[out]	handle	Driver handle ID
 *
 */
FT_HANDLE	spi_open(const char_t *driverID) {
	FT_STATUS	status;
	FT_HANDLE	handle;
    DWORD		nbTransferred;
	timespec_t	time;
	uint8_t		param_0[1] = { 0x8A };
	uint8_t		param_1[1] = { 0x97 };
	uint8_t		param_2[3] = { 0x86, ((30000000/20000000)-1), (((30000000/20000000)-1)>>8) };
	uint8_t		param_3[3] = { MPSSE_CMD_SET_DATA_BITS_LOWBYTE, 0xC9, 0xFB };

	#if (defined(__APPLE__))
	system("sudo kextunload /Library/Extensions/FTDIUSBSerialDriver.kext/");
	status = FT_OpenEx((PVOID)driverID, FT_OPEN_BY_SERIAL_NUMBER, &handle);
	system("sudo kextload /Library/Extensions/FTDIUSBSerialDriver.kext/");

	#elif (defined(__linux__))
	status = FT_OpenEx((PVOID)driverID, FT_OPEN_BY_SERIAL_NUMBER, &handle);

	#else     
	#error Platform not supported
	#endif

// Send a reset command to the device
// Purge receive and transmit buffers in the device
// Set the USB request transfer size
// Set the special characters for the device
// set the read and write timeouts for the device
// Set the latency timer value
// Set the chip mode to reset
// Set the chip mode to MPSSE

    status = FT_ResetDevice(handle);
    status = FT_Purge(handle, FT_PURGE_RX | FT_PURGE_TX);
    status = FT_SetUSBParameters(handle, USB_INPUT_BUFFER_SIZE, USB_OUTPUT_BUFFER_SIZE);
    status = FT_SetChars(handle, FALSE, DISABLE_EVENT, FALSE, DISABLE_CHAR);
    status = FT_SetTimeouts(handle, 5000, 5000);
    status = FT_SetLatencyTimer(handle, 255);
    status = FT_SetBitMode(handle, 0x00, 0x00);
    status = FT_SetBitMode(handle, 0x00, 0x02);
	time.tv_sec  = 0;
	time.tv_nsec = (50*1000000);
	nanosleep(&time, NULL);

// Use 60-MHz master clock (disable divide by 5)
// Turn off adaptive clocking (may be needed for ARM)
// Command to set clock divisor

    status = FT_Write(handle, param_0, sizeof(param_0), &nbTransferred);
    status = FT_Write(handle, param_1, sizeof(param_1), &nbTransferred);
    status = FT_Write(handle, param_2, sizeof(param_2), &nbTransferred);
	time.tv_sec  = 0;
	time.tv_nsec = (20*1000000);
	nanosleep(&time, NULL);

// Set initial states of the MPSSE interface
// - low byte, both pin directions and output values
//
//					Dir		Cnf (0xFB)		Init	Cnf (0xC9)
// ADBUS0	SCK		O		1				H		1
// ADBUS1	MOSI	O		1				L		0
// ADBUS2	MISO	I		0				-		0
// ADBUS3	CS		O		1				H		1
// ADBUS4	GPIOL0	O		1				L		0
// ADBUS5	GPIOL1	O		1				L		0
// ADBUS6	GPIOL2	O		1				H		1
// ADBUS7	GPIOL3	O		1				H		1

    status = FT_Write(handle, param_3, sizeof(param_3), &nbTransferred);
	return (handle);
}

/*
 * \brief spi_close
 *
 * - Close the device
 *
 * \param[in]	handle	Driver handle ID
 * \param[out]	-
 *
 */
void	spi_close(FT_HANDLE handle) {

	FT_Close(handle);
}

/*
 * \brief spi_read
 *
 * - Read bytes on the device
 *
 * \param[in]	handle		Driver handle ID
 * \param[in]	*receive	Ptr on the receive buffer
 * \param[in]	size		Nb of byte to transfer
 * \param[in]	*count		Ptr on the count
 * \param[out]	status		TRUE -> OK
 *							FALSE -> timeout error
 *
 */
bool_t	spi_read(FT_HANDLE handle, uint8_t *receive, uint32_t size, uint32_t *count) {
				FT_STATUS	status;
				DWORD		sent;
				uint8_t		param_0[3] = { MPSSE_CMD_SET_DATA_BITS_LOWBYTE,  0xC0, 0xFB };
				uint8_t		param_1[3] = { MPSSE_CMD_SET_DATA_BITS_LOWBYTE,  0xC8, 0xFB };
				uint8_t		param_2[4] = { MPSSE_CMD_DATA_IN_BYTES_NEG_EDGE, 0x00, 0x00, MPSSE_CMD_SEND_IMMEDIATE };
				uint32_t	i;
	static		uint32_t	vCptTimeout = 100000;

// Enable the CS
// Write the transfer size

    status = FT_Write(handle, param_0, sizeof(param_0), &sent);

	param_2[1] = (uint8_t)( (size-1) & 0x000000FF);
	param_2[2] = (uint8_t)(((size-1) & 0x0000FF00)>>8);
    status = FT_Write(handle, param_2, sizeof(param_2), &sent);

// Read the data
// Disable the CS

    status = FT_Read(handle, receive, size, count);

    status = FT_Write(handle, param_1, sizeof(param_1), &sent);

// Verify if we can generate a pseudo timeout
// - If the target is stopped (reset), then we should continue to receive 0xFF data
//   This because the SPI of the target if off.
// - The timeout is effective if we read 10 consecutive blocs of 0xFF

// Test of the timeout

	for (i = 0; i < size; i++) {
		if (*(receive + i) != 0xFF)	{
			vCptTimeout = 100000;
			return (TRUE);
		}
		else {
			if (--vCptTimeout == 0) {
				vCptTimeout = 100000;
				return (FALSE);
			}
			return (TRUE);
		}
	}
	return (TRUE);
}

/*
 * \brief spi_flush
 *
 * - Flush the device
 *
 * \param[in]	handle	Driver handle ID
 * \param[out]	-
 *
 */
void	spi_flush(FT_HANDLE handle) {
	FT_STATUS	status;
	DWORD		rxBytes;
	DWORD		bytesReceived;
	uint8_t		byte;

	while (TRUE) {
	    status = FT_GetQueueStatus(handle, &rxBytes);
		if (rxBytes > 0) {
			FT_Read(handle, &byte, 1, &bytesReceived);
		}
		else {
			return;
		}
	}
}

