/*
; stub_img0_mt9v03x.
; ==================

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
; Goal:		stub for the connection of the "img0" manager to the imager,
;			MT9V03x device.
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
#include 	<MT9V03x/MT9V03x.h>

// Connect the physical device to the logical manager
// --------------------------------------------------

#define	__IMAGER_SEMA_IM__
#define	__IMAGER_SEMA_VS__

#define	cnfImager_t				cnfImg0_t

#define	imager_init				stub_img0_init
#define	imager_configure		stub_img0_configure
#define	imager_acquisition		stub_img0_acquisition
#define	imager_getImage			stub_img0_getImage
#define	imager_ctrlLeds			stub_img0_ctrlLeds
#define	imager_getSerialNumber	stub_img0_getSerialNumber
#define	imager_getRegister		stub_img0_getRegister
#define	imager_putRegister		stub_img0_putRegister
#define	imager_ctrlStandby		stub_img0_ctrlStandby
#define	imager_setExposure		stub_img0_setExposure

#define	KIMAGERNOERR			KIMG0NOERR
#define	KIMAGERSYCNA			KIMG0SYCNA
#define	KIMAGERCNERR			KIMG0CNERR
#define	KIMAGERGEERR			KIMG0GEERR
#define	KIMAGERTIMEO			KIMG0TIMEO
#define	KIMAGERCHBSY			KIMG0CHBSY
#define	KIMAGERPGNOE			KIMG0PGNOE
#define	KIMAGERBDMOD			KIMG0BDMOD
#define	KIMAGERNOMEM			KIMG0NOMEM

#define	KSEMAPHORE_IMAGERIM		KSEMAPHORE_IMG0IM
#define	KSEMAPHORE_IMAGERVS		KSEMAPHORE_IMG0VS

#define KINIT					0
#define KTRIGIMG				1
#define KLEDSTATE				2
#define KIMGSTBY				3
#define KIMGNORM				4
#define KPIXMODE				5
#define KIMGEXPO				6

// Prototypes
// ==========

static	int32_t	_setAptina(mt9v03x_t *cnfTable);

// Model callbacks
// ---------------

/*
 * \brief cb_control
 *
 * - Control of the imager
 *   - Initialize
 *   - Trigger
 *   - Led state
 *   - Imager in standby mode
 *   - Imager in normal mode
 *   - Pixel mode
 *   - Exposure time
 *
 */
static	int32_t	cb_control(uint8_t mode, uint32_t value) {
	int32_t		status = KIMG0NOERR;
	cnfI2cx_t	configureI2C1;

	switch (mode) {
		case KINIT: {
			GPIOI->ODR |= (1<<BCLK_EN);
			kern_suspendProcess(10);
			GPIOH->ODR &= ~(1<<BSTANDBY);
			GPIOH->ODR &= ~(1<<BEXPOSUR);

// Initialize the i2c1

			configureI2C1.oSpeed    = K100KHZ;
			configureI2C1.oKernSync = 0;
			
			RESERVE(I2C1, KDEVALL);
			status = i2c1_configure(&configureI2C1);
			status = (status == KI2C1NOERR) ? (KIMG0NOERR) : (KIMG0TIMEO);
			RELEASE(I2C1, KDEVALL);
			break;
		}
		case KTRIGIMG: {
			GPIOH->ODR |= (1<<BEXPOSUR);
			kern_waitAtLeast(10);
			GPIOH->ODR &= ~(1<<BEXPOSUR);
			break;
		}
		case KLEDSTATE: {
			break;
		}
		case KIMGSTBY: {
			GPIOH->ODR |= (1<<BSTANDBY);
			kern_suspendProcess(10);
			GPIOI->ODR &= ~(1<<BCLK_EN);
			break;
		}
		case KIMGNORM: {
			GPIOI->ODR |= (1<<BCLK_EN);
			kern_suspendProcess(10);
			GPIOH->ODR &= ~(1<<BSTANDBY);
			break;
		}
		case KPIXMODE: {
			break;
		}
		default: {
			break;
		}
	}
	return (status);
}

/*
 * \brief cb_getRegister
 *
 * - Read an imager register
 *
 */
static	int32_t	cb_getRegister(uint8_t registerNb, uint16_t *value) {
	uint8_t		buffer[2];
	int32_t		status;

	buffer[0] = registerNb;

	RESERVE(I2C1, KDEVALL);
	status = i2c1_read(KAPTINAD, &buffer[0], 2);
	status = (status == KI2C1NOERR) ? (KIMG0NOERR) : (KIMG0TIMEO);
	RELEASE(I2C1, KDEVALL);

	*value = (uint16_t)(buffer[0]<<8 | buffer[1]);
	return (status);
}

/*
 * \brief cb_putRegister
 *
 * - Write an imager register
 *
 */
static	int32_t	cb_putRegister(uint8_t registerNb, uint16_t value) {
	uint8_t		buffer[3];
	int32_t		status;

	buffer[0] = registerNb;
	buffer[1] = value>>8;
	buffer[2] = value;

	RESERVE(I2C1, KDEVALL);
	status = i2c1_write(KAPTINAD, &buffer[0], 3);
	status = (status == KI2C1NOERR) ? (KIMG0NOERR) : (KIMG0TIMEO);
	RELEASE(I2C1, KDEVALL);

	return (status);
}

/*
 * \brief cb_getSerial
 *
 * - Read the imager serial number
 *
 */
static	int32_t	cb_getSerial(uint64_t *serialNumber) {
				uint8_t		i, buffer[2];
				uint16_t	numberW[4];
				int32_t		status;
	const		uint8_t		order[4] = { KAPTINAPN0, KAPTINAPN1, KAPTINAPN2, KAPTINAPN3 };

	RESERVE(I2C1, KDEVALL);
	for (i = 0; i < 4; i++) {
		buffer[0] = order[i];
		status = i2c1_read(KAPTINAD, &buffer[0], 2);
		status = (status == KI2C1NOERR) ? (KIMG0NOERR) : (KIMG0TIMEO);
		if (status != KI2C1NOERR) { RELEASE(I2C1, KDEVALL); return (status); }

		numberW[i] = (uint16_t)(buffer[0]<<8 | buffer[1]);
	}
	RELEASE(I2C1, KDEVALL);

	*serialNumber =                       (uint64_t)(numberW[3]);
	*serialNumber = (*serialNumber<<16) | (uint64_t)(numberW[2]);
	*serialNumber = (*serialNumber<<16) | (uint64_t)(numberW[1]);
	*serialNumber = (*serialNumber<<16) | (uint64_t)(numberW[0]);
	return (status);
}

/*
 * \brief cb_configure
 *
 * - Configure the imager
 *
 */
static	int32_t	cb_configure(const cnfImg0_t *configure) {
	int32_t		status = KIMG0NOERR;

// Configure the sensitivity to the synchro polarity
// Pixel clock -> active H
// Horizontal  -> active L
// Vertical    -> active L

	DCMI->CR |= DCMI_CR_PCKPOL;

// The full Aptina registers configuration including ROI, exposure,
// gain, shutters, interface and control
// Set the vertical blanking to 4 lines
// Set SNAPSHOT + SEQUENTIAL + SLAVE mode

	status = cb_putRegister(KAPTINAVBLANK, 0x0004);    	      if (status != KIMG0NOERR) { return (status); }
	status = cb_putRegister(KAPTINAREG20,  0x03C7);    	      if (status != KIMG0NOERR) { return (status); }
	status = cb_putRegister(KAPTINAREG24,  0x001B);    	      if (status != KIMG0NOERR) { return (status); }
	status = cb_putRegister(KAPTINAREG2B,  0x0003);    	      if (status != KIMG0NOERR) { return (status); }
	status = cb_putRegister(KAPTINAREG2F,  0x0003);    	      if (status != KIMG0NOERR) { return (status); }

	if (configure->oAcqMode == KSNAP) {											
		status = cb_putRegister(KAPTINACTRLREG, 0x0398);      if (status != KIMG0NOERR) { return (status); }			
	}																			
	if (configure->oAcqPage == KEXTERNAL) {										
		status = cb_putRegister(KAPTINAHBLANK,    1020);      if (status != KIMG0NOERR) { return (status); }			
	}
	if (configure->oImgCnf != NULL) {
		status = _setAptina((mt9v03x_t *)configure->oImgCnf); if (status != KIMG0NOERR) { return (status); }			
	}
	return (status);
}

// Local routines
// ==============

/*
 * \brief _setAptina
 *
 */
static	int32_t	_setAptina(mt9v03x_t *cnfTable) {
	uint8_t		buffer[3];
	uint16_t	i = 0;
	int32_t		status;

	RESERVE(I2C1, KDEVALL);
	while (TRUE) {
		buffer[0] = cnfTable[i].oRegNumber;
		buffer[1] = cnfTable[i].oValue>>8;
		buffer[2] = cnfTable[i].oValue;

		if ((buffer[0] == 0) && (i > 0)) { RELEASE(I2C1, KDEVALL); return (KIMG0NOERR); }	
		i++;

		status = i2c1_write(KAPTINAD, &buffer[0], 3);
		status = (status == KI2C1NOERR) ? (KIMG0NOERR) : (KIMG0TIMEO);
		if (status != KIMG0NOERR) { RELEASE(I2C1, KDEVALL); return (status); }			
	}
}

#include 	<model_imager_dcmi_dma2.cm>

