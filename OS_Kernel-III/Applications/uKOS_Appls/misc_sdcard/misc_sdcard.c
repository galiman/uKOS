/*
; misc_sdcard.
; ============

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
; Goal:		Demo of a C application.
;			This application shows how to operate with the uKOS uKernel.
;
;			Launch 2 processes:
;
;			- P0: Every 1000-ms
;				  Toggle LED 0
;
;			- P1: Operations on the SDCard
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

#undef	__TEST_1__				// Read the sector 0 to ...
#undef	__TEST_2__				// Read all the sectors of the SDCard
#undef	__TEST_3__				// Write/Read 2 consecutive sectors
#undef	__TEST_4__				// Read the SDCard parameters
#define	__TEST_5__				// Write/Read 100 consecutive sectors (for measurements)
#undef	__TEST_6__				// Write/Read 2 float64 numbers

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"misc_sdcard  Example of how to use the SDCard.         (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "This is a romable C application\n"
									"===============================\n\n"

									"This user function module is a C written application.\n\n"

									"Input format:  misc_sdcard\n"
									"Output format: [result]\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(UserAppl, KIDAPPLICATION, KAPPLICATIONNUM, _start, "1.0", ((1<<BSHOW) | (1<<BEXECONSOLE)));

/*
 * \brief aProcess 0
 *
 * - P0: - Every 1000-ms
 * 			- Toggle LED 0
 *
 */
static	void	aProcess_0(const void *argument) {

	while (TRUE) {
		kern_suspendProcess(1000);
		misc_toggleLed(0);
	}
}

#if (defined(__TEST_1__))
/*
 * \brief aProcess 1
 *
 * - P1: - Test of the SPI SDCard
 * 			- Read the sector 0 to ...
 *
 */
static	void	aProcess_1(const void *argument) {
	#define		KINITSECTOR	0
	#define		KNBSECTORS	3
	volatile int32_t		status;
	uint32_t	i, j, n;
	prmUsdc_t	sdCard;
	uint8_t		*buffer;

	buffer = (uint8_t *)syos_malloc(KINTERNAL, KSZFATSECT);
	if (buffer == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	RESERVE(USDC, KDEVALL);
	status = usdc_initialize(&sdCard);
	if (status != KUSDCNOERR) {
		iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
		exit(EXIT_FAILURE);
	}

	for (n = 0; n < KNBSECTORS; n++) {
		misc_toggleLed(1);

// Read the sector 0 to ...

		kern_suspendProcess(1000);

		status = usdc_read(&sdCard, buffer, KINITSECTOR+n, 1);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
			exit(EXIT_FAILURE);
		}

		iotx_printf(KSYST, "Sector = %d\n", KINITSECTOR + n);

		for (j = 0; j < KSZFATSECT; j += 16) {
			for (i = 0; i < 16; i++) {
				iotx_printf(KSYST, "0x%02X ", *(buffer + i+j));
			}
			iotx_printf(KSYST, "\n");
		}
		iotx_printf(KSYST, "\n\n");
	}
	RELEASE(USDC, KDEVALL);
	exit(EXIT_SUCCESS);
}
#endif

#if (defined(__TEST_2__))
/*
 * \brief aProcess 1
 *
 * - P1: - Test of the SPI SDCard
 *  		- Read all the sectors (about. 20h)
 *
 */
static	void	aProcess_1(const void *argument) {
	#define		KNBSECTORS	10
	int32_t		status;
	uint32_t	sector = 0;
	prmUsdc_t	sdCard;
	uint8_t		*buffer;

	buffer = (uint8_t *)syos_malloc(KINTERNAL, KNBSECTORS*KSZFATSECT);
	if (buffer == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	RESERVE(USDC, KDEVALL);
	status = usdc_initialize(&sdCard);
	if (status != KUSDCNOERR) {
		iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
		exit(EXIT_FAILURE);
	}
	
	while (TRUE) {
		misc_toggleLed(1);

// Read all the sectors (about. 20h)

		if (!(sector % 1000)) {
			iotx_printf(KSYST, "sector %d\n", sector);
		}

		status = usdc_read(&sdCard, buffer, sector, KNBSECTORS);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "Error %02X sector %d\n", (-status & 0xFF), sector);
			exit(EXIT_FAILURE);
		}
		sector += 10;
	}
}
#endif

#if (defined(__TEST_3__))
/*
 * \brief aProcess 1
 *
 * - P1: - Test of the SPI SDCard
 * 			- Write/Read 2 consecutive sectors
 *
 */
static	void	aProcess_1(const void *argument) {
	#define		KNBSECTORS	2
	int32_t		status;
	uint32_t	i, j, s, sector = 0;
	prmUsdc_t	sdCard;
	uint8_t		*bufferW;
	uint8_t		*bufferR;

	bufferW = (uint8_t *)syos_malloc(KINTERNAL, KNBSECTORS*KSZFATSECT);
	if (bufferW == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	bufferR = (uint8_t *)syos_malloc(KINTERNAL, KNBSECTORS*KSZFATSECT);
	if (bufferR == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	RESERVE(USDC, KDEVALL);
	status = usdc_initialize(&sdCard);
	if (status != KUSDCNOERR) {
		iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
		exit(EXIT_FAILURE);
	}

// Initialize the buffer

	for (i = 0; i < KSZFATSECT; i++) {
		*(bufferW + i) 			    = 0x55;
		*(bufferW + KSZFATSECT + i) = 0xAA;
	}
	
	while (TRUE) {
		misc_toggleLed(1);

// Write/read 2 consecutive sectors

		kern_suspendProcess(1000);

		iotx_printf(KSYST, "Sector = %d\n\n", sector);

		status = usdc_write(&sdCard, bufferW, sector, KNBSECTORS);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "Error %02X sector %d\n", (-status & 0xFF), sector);
			exit(EXIT_FAILURE);
		}

		status = usdc_read(&sdCard, bufferR, sector, KNBSECTORS);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "Error %02X sector %d\n", (-status & 0xFF), sector);
			exit(EXIT_FAILURE);
		}

		for (s = 0; s < KNBSECTORS; s++) {
			iotx_printf(KSYST, "s = %d\n", s);
			for (j = 0; j < KSZFATSECT; j += 16) {
				for (i = 0; i < 16; i++) {
					iotx_printf(KSYST, "0x%02X ", *(bufferR + KSZFATSECT*s + i+j));
				}
				iotx_printf(KSYST, "\n");
			}
		}
		iotx_printf(KSYST, "\n\n");
		sector += 1;
	}
}
#endif

#if (defined(__TEST_4__))
/*
 * \brief aProcess 1
 *
 * - P1: - Test of the SPI SDCard
 *  		- Read the SDCard parameters
 *
 */
static	void	aProcess_1(const void *argument) {
	char_t		*card;
	int32_t		status;
	prmUsdc_t	sdCard;

	RESERVE(USDC, KDEVALL);
	status = usdc_initialize(&sdCard);
	if (status != KUSDCNOERR) {
		iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
		exit(EXIT_FAILURE);
	}

	while (TRUE) {
		misc_toggleLed(1);

// Read the SDCard sdCard

		kern_suspendProcess(2000);

		if (sdCard.oCCS == KCT_SD1) { card = "SDCard V.1"; }
		else						{ card = "SDCard V.2"; }

		iotx_printf(KSYST, "SDCard type           %s\n",   card);
		iotx_printf(KSYST, "Manufacturer ID       %02X\n", sdCard.oMID);
		iotx_printf(KSYST, "Product Name          %s\n",   sdCard.oPNM);
		iotx_printf(KSYST, "Product Serial Number %04X\n", sdCard.oPSN);
		iotx_printf(KSYST, "Device size [Bytes]   %ld\n",  sdCard.oC_SIZE);
		iotx_printf(KSYST, "SDCard voltage        %04X\n", sdCard.oVoltage);

// Print the supported voltages

		if (sdCard.oVoltage & 0x001) { iotx_printf(KSYST, " - Supported voltage  2.7 to 2.8V\n");   }
		if (sdCard.oVoltage & 0x002) { iotx_printf(KSYST, " - Supported voltage  2.8 to 2.9V\n");   }
		if (sdCard.oVoltage & 0x004) { iotx_printf(KSYST, " - Supported voltage  2.9 to 3.0V\n");   }
		if (sdCard.oVoltage & 0x008) { iotx_printf(KSYST, " - Supported voltage  3.0 to 3.1V\n");   }
		if (sdCard.oVoltage & 0x010) { iotx_printf(KSYST, " - Supported voltage  3.1 to 3.2V\n");   }
		if (sdCard.oVoltage & 0x020) { iotx_printf(KSYST, " - Supported voltage  3.2 to 3.3V\n");   }
		if (sdCard.oVoltage & 0x040) { iotx_printf(KSYST, " - Supported voltage  3.3 to 3.4V\n");   }
		if (sdCard.oVoltage & 0x080) { iotx_printf(KSYST, " - Supported voltage  3.4 to 3.5V\n");   }
		if (sdCard.oVoltage & 0x100) { iotx_printf(KSYST, " - Supported voltage  3.5 to 3.6V\n\n"); }
	}
}
#endif

#if (defined(__TEST_5__))
/*
 * \brief aProcess 1
 *
 * - P1: - Test of the SPI SDCard
 * 			- Write/Read 100 consecutive sectors and measure the Write/Read access time
 *
 */
static	void	aProcess_1(const void *argument) {
	#define		KNBSECTORS	100
	int32_t		status;
	uint32_t	i, j, k, sector = 0;
	uint64_t	tStamp[2];
	prmUsdc_t	sdCard;
	uint8_t		*bufferW;
	uint8_t		*bufferR;

	bufferW = (uint8_t *)syos_malloc(KINTERNAL, KNBSECTORS*KSZFATSECT);
	if (bufferW == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	bufferR = (uint8_t *)syos_malloc(KINTERNAL, KNBSECTORS*KSZFATSECT);
	if (bufferR == NULL) {
		iotx_printf(KSYST, "Not enough memory.\n");
		exit(EXIT_FAILURE);
	}

	RESERVE(USDC, KDEVALL);
	status = usdc_initialize(&sdCard);
	if (status != KUSDCNOERR) {
		iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF)); 
		exit(EXIT_FAILURE);
	}

	k = 0;
	while (TRUE) {
		misc_toggleLed(1);

// Initialize the buffer

		for (i = 0; i < KNBSECTORS; i++) {
			for (j = 0; j < KSZFATSECT; j++) {
				*(bufferW + i*KSZFATSECT + j) = k++;
			}
		}

// Write/read 100 consecutive sectors (3 consecutive tries before stopping)

		kern_getTiccount(&tStamp[0]);
		status = usdc_write(&sdCard, bufferW, sector, KNBSECTORS);
		kern_getTiccount(&tStamp[1]);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "W Error %02X sector %d\n", (-status & 0xFF), sector);
			exit(EXIT_FAILURE);
		}

		iotx_printf(KSYST, "Write time for 100 sectors:  %d-us, sector nb %d\n", (uint32_t)(tStamp[1] - tStamp[0]), sector);

		kern_getTiccount(&tStamp[0]);
		status = usdc_read(&sdCard, bufferR, sector, KNBSECTORS);
		kern_getTiccount(&tStamp[1]);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "R Error %02X sector %d\n", (-status & 0xFF), sector);
			exit(EXIT_FAILURE);
		}

		iotx_printf(KSYST, "Read time for 100 sectors:   %d-us, sector nb %d\n", (uint32_t)(tStamp[1] - tStamp[0]), sector);

		sector += KNBSECTORS;

// Test if the buffers are equals

		for (i = 0; i < KNBSECTORS; i++) {
			for (j = 0; j < KSZFATSECT; j++) {
				if (*(bufferW + i*KSZFATSECT + j) != *(bufferR + i*KSZFATSECT + j)) {
					iotx_printf(KSYST, "Read error\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}
#endif

#if (defined(__TEST_6__))
/*
 * \brief aProcess 1
 *
 * - P1: - Test of the SPI SDCard
 * 			- Write/read float64 numbers
 *
 */
static	void	aProcess_1(const void *argument) {
			int32_t		status;
			prmUsdc_t	sdCard;
			float64_t	*ref, *res;
	static	uint8_t		vBufRef[KSZFATSECT], vBufRes[KSZFATSECT];

	ref = (float64_t *)&vBufRef[0];
	res = (float64_t *)&vBufRes[0];

	*(ref + 0) = 3.141516;
	*(ref + 1) = 1.843256;

	RESERVE(USDC, KDEVALL);
	status = usdc_initialize(&sdCard);
	if (status != KUSDCNOERR) {
		iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
		exit(EXIT_FAILURE);
	}
	
	while (TRUE) {
		misc_toggleLed(1);
		kern_suspendProcess(1000);

// Write 2 float64 numbers on the sector 0

		status = usdc_write(&sdCard, vBufRef, 0, 1);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
			exit(EXIT_FAILURE);
		}

// Read 2 float64 numbers from the sector 0

		status = usdc_read(&sdCard, vBufRes, 0, 1);
		if (status != KUSDCNOERR) {
			iotx_printf(KSYST, "Error %02X\n", (-status & 0xFF));
			exit(EXIT_FAILURE);
		}

		iotx_printf(KSYST, "%d.%02d %%d.%02d %%d.%02d %d.%02d\n", FLOAT_2(*(ref + 0)), FLOAT_2(*(ref + 1)), FLOAT_2(*(res + 0)), FLOAT_2(*(res + 1)));
	}
}
#endif

/*
 * \brief main
 *
 * - Initialize the used libraries
 * - Launch all the processes
 * - Kill the "main". At this moment only the launched processes are executed
 *
 */
int		main(void) {
	volatile	proc_t 	*process_0, *process_1;

// ------------------------------------I-----------------------------------------I--------------I

	LOC_CONST_STRG(aStrIden_0[]) =    "Process_User_0";
	LOC_CONST_STRG(aStrIden_1[]) =    "Process_User_1";
	LOC_CONST_STRG(aStrText_0[]) =    "Process user 0.                           (c) EFr-2017";
	LOC_CONST_STRG(aStrText_1[]) =    "Process user 1.                           (c) EFr-2017";

// Specifications for the processes

	PROC_SUPV(0, vSpecification_0, aStrText_0, KSZSTACKNLB, aProcess_0, aStrIden_0, KDEF0, (KLOWPRIORITY+7),  0, KPROCNORMAL);
	PROC_SUPV(1, vSpecification_1, aStrText_1, KSZSTACKNLB, aProcess_1, aStrIden_1, KDEF0, (KHIGHPRIORITY+1), 0, KPROCNORMAL);

	if (kern_createProcess(&vSpecification_0, NULL, &process_0) != KKERNNOERR) { exit(EXIT_FAILURE); }
	if (kern_createProcess(&vSpecification_1, NULL, &process_1) != KKERNNOERR) { exit(EXIT_FAILURE); }
	return (EXIT_SUCCESS);
}

