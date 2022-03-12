/*
; vcp.
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
; Goal:		VCP I/O management.
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
#include 	<fcntl.h>
#include 	<termios.h>
#include 	<time.h>
#include	<unistd.h>
#include	<ftd2xx.h>
#include	<types_unix.h>

// Baudrates not supported

#ifndef B460800
#define B460800 				460800
#endif

#ifndef B500000
#define B500000 				500000
#endif

#ifndef B921600
#define B921600 				921600
#endif

#ifndef B1000000
#define B1000000 				1000000
#endif

#ifndef B1843200
#define B1843200 				1843200
#endif

#ifndef B2000000
#define B2000000 				2000000
#endif

#ifndef B2500000
#define B2500000 				2500000
#endif

#ifndef B3000000
#define B3000000 				3000000
#endif

#define	KINITIOREC				3000000									// Initial timeout reception of 30s
#define	KNORTIOREC				200000									// Normal timeout reception of 2s (for stopping the process)

typedef	struct termios	termios_t;
typedef	struct timespec	timespec_t;

static	termios_t	vTTYAttr;

/*
 * \brief vcp_list
 *
 * - List of the devices
 * 
 */
void	vcp_list(void) {
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
 * \brief vcp_open
 *
 * - Open the device
 *
 * \param[in]	*driverID	Ptr on the driver ID "FTXG86D7D"
 * \param[in]	baudrate	Device baudrate
 * \param[out]	handle		Driver handle ID
 *
 */
int32_t	vcp_open(const char_t *driverID, int32_t baudrate) {
	int32_t		handle;
	speed_t		bd;
	termios_t	options;
	char_t		driver[50] = "/dev/tty.usbserial-";

	strcat(driver, driverID);
	switch (baudrate) {
		case 9600:    { bd = B9600;    break; }
		case 19200:   { bd = B19200;   break; }
		case 38400:   { bd = B38400;   break; }
		case 115200:  { bd = B115200;  break; }
		case 230400:  { bd = B230400;  break; }
		case 460800:  { bd = B460800;  break; }
		case 500000:  { bd = B500000;  break; }
		case 921600:  { bd = B921600;  break; }
		case 1000000: { bd = B1000000; break; }
		case 1843200: { bd = B1843200; break; }
		case 2000000: { bd = B2000000; break; }
		case 2500000: { bd = B2500000; break; }
		case 3000000: { bd = B3000000; break; }
		default:      { bd = B460800;  break; }
	}

    handle = open(driver, O_RDONLY | O_NOCTTY | O_NDELAY);
    if (handle == -1) {
    	return (EXIT_FAILURE);
    }

    while (TRUE) {
        if (fcntl(handle, F_SETFL, 0)            == -1) break;
        if (tcgetattr(handle, &vTTYAttr)         == -1) break;

        options 			=  vTTYAttr;
        options.c_cflag 	|= (CLOCAL | CREAD);
        options.c_lflag 	&= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag 	&= ~OPOST;
        options.c_cc[VMIN]	=  0;
        options.c_cc[VTIME]	=  5;
 		options.c_cflag		&= ~(CSIZE | PARENB);
        options.c_cflag		|= (CS8 | CSTOPB);         

		if (cfsetispeed(&options, bd)            == -1) break;
 		if (cfsetospeed(&options, bd)            == -1) break;
	    if (tcsetattr(handle, TCSANOW, &options) == -1) break;

        return (handle);
    }

    close(handle);
    return (EXIT_FAILURE);
}

/*
 * \brief vcp_close
 *
 * - Close the device
 *
 * \param[in]	handle	Driver handle ID
 * \param[out]	-
 *
 */
void	vcp_close(int32_t handle) {

	tcsetattr(handle, TCSADRAIN, &vTTYAttr);
    close(handle);
}

/*
 * \brief vcp_readAByte
 *
 * - Read a byte on the device
 *
 * \param[in]	handle		Driver handle ID
 * \param[in]	*receive	Ptr on the receive buffer
 * \param[out]	status		TRUE -> OK
 *							FALSE -> timeout error
 *
 */
bool_t	vcp_readAByte(int32_t handle, uint8_t *receive) {
			int32_t		nbRead;
			uint32_t	timeout = 0;
			timespec_t	time;
	static	uint32_t	vTimeoutLimit = KINITIOREC;

	while (TRUE) {
    	nbRead = read(handle, receive, 1);
   		if (nbRead > 0) {
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
    return (TRUE);
}

/*
 * \brief vcp_read
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
bool_t	vcp_read(int32_t handle, uint8_t *receive, uint32_t *count) {
			int32_t		nbRead;
			uint32_t	timeout = 0;
			timespec_t	time;
	static	uint32_t	vTimeoutLimit = KINITIOREC;

	while (TRUE) {
    	nbRead = read(handle, receive, 100);
  		if (nbRead > 0) {
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
	*count = nbRead;
    return (TRUE);
}

/*
 * \brief vcp_flush
 *
 * - Flush the device
 *
 * \param[in]	handle	Driver handle ID
 * \param[out]	-
 *
 */
void	vcp_flush(int32_t handle) {
	int32_t		nbRead;
	uint8_t		byte;

	do {
    	nbRead = read(handle, &byte, 1);
	} while (nbRead == 1);
}

