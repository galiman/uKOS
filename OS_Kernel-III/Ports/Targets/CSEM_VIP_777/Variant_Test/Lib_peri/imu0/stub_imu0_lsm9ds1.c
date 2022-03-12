/*
; stub_imu0_lsm9ds1.
; ==================

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2015-08-20
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		stub for the "imu0" manager module.
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
#include 	<LSM9DS1/LSM9DS1.h>
#include 	<spi5/spi5.h>

// Accelerometer, gyroscope, magnetometer sensitivities

#define	KACCSEN2G		(0.000061)	// Sensitivity for 2-G: 0.061-mg/bit
#define	KACCSEN4G		(0.000122)	// Sensitivity for 4-G: 0.122-mg/bit
#define	KACCSEN8G		(0.000244)	// Sensitivity for 8-G: 0.244-mg/bit
#define	KACCSEN16G		(0.000732)	// Sensitivity for 16-G: 0.732-mg/bit

#define	KGYRSEN245DPS	(0.00875)	// Sensitivity for 245-DPS: 8.75-mdps/bit
#define	KGYRSEN500DPS	(0.0175)	// Sensitivity for 500-DPS: 17.5-mdps/bit
#define	KGYRSEN2000DPS	(0.0700)	// Sensitivity for 2000-DPS: 70.0-mdps/bit

#define	KMAGSEN4GAUSS	(0.14)		// Sensitivity for 4-Gauss: 0.14-Gauss/bit
#define	KMAGSEN8GAUSS	(0.29)		// Sensitivity for 8-Gauss: 0.29-Gauss/bit
#define	KMAGSEN12GAUSS	(0.43)		// Sensitivity for 12-Gauss: 0.43-Gauss/bit
#define	KMAGSEN16GAUSS	(0.58)		// Sensitivity for 16-Gauss: 0.58-Gauss/bit

static	int16_t		v1G;			// 1-G value
static	int16_t		vBiasAcce[3];	// Static bias for the accelerometer
static	int16_t		vBiasGyro[3];	// Static bias for the gyroscope
static	int16_t		vBiasMagn[3];	// Static bias for the magnetometer
static	float64_t	vSensAcce;		// Sensitivity of the accelerometer
static	float64_t	vSensGyro;		// Sensitivity of the gyroscope
static	float64_t	vSensMagn;		// Sensitivity of the magnetometer

// Prototypes
// ==========

static	void	_initSPI(void);
static	void	_initIMU(void);
static	void	_caliIMU(void);
static	void	_writeAcceGyro(uint8_t address, uint8_t *data, uint8_t number);
static	void	_readAcceGyro(uint8_t address, uint8_t *data, uint8_t number);
static	void	_writeMagn(uint8_t address, uint8_t *data, uint8_t number);
static	void	_readMagn(uint8_t address, uint8_t *data, uint8_t number);
static	uint8_t	_writeReadSPI(uint8_t data);

/*
 * \brief stub_imu0_init
 *
 * - Initialize the manager
 *
 */
void	stub_imu0_init(void) {

	RESERVE_SHARED_SPI5(KIMU0);
	_initSPI();
	_initIMU();
	_caliIMU();
	RELEASE_SHARED_SPI5;
}

/*
 * \brief stub_imu0_configure
 *
 * - Configure the manager
 *
 */
int32_t	stub_imu0_configure(const cnfImu0_t *configure) {
	uint8_t		data, reg1, reg6, regx;

// Accelerometer & Gyro
//    011 XX 0 00     = 0x78 - ODR = 119-Hz, XXXX-dps, cut-off = 14-Hz
//    0000 00 00      = 0x00 - OUT LPF1
//    00 00 0000      = 0x00 - No high pass
//    00 111 0 00     = 0x38 - Enable Gyro x, y, z axis
//    00 111 000      = 0x38 - Enable Accelerometer x, y, z axis
//    011 YY 0 11     = 0x73 - ODR = 119-Hz, YYYY-g, anti-aliasing = 50-Hz
//    0 00 00 0 0 0   = 0x00 - No high resolution
//    0 0 1 1 0 1 0 0 = 0x34 - Int active low, Open Drain, 4 wire SPI, auto increment
//    0 0 0 0 0 1 1 0 = 0x04 - Disable i2c, FIFO enable
//    110 11111       = 0xDF - FIFO in continuous mode, max threshold

// Magnetometer
//    0 10 100 00     = 0x50 - High performance, 10-Hz
//    0 XX 0 0 0 0 0  = 0x00 - XX-Gauss
//    1 0 0 0 0 1 0 0 = 0x84 - SPI On, I2C Off, continuous mode
//    0 0 0 0 10 0 0  = 0x08 - High performance

	switch (configure->oAcceMode) {
		case K2G:      { vSensAcce = KACCSEN2G;      reg6 = 0x63; break; }
		case K4G:      { vSensAcce = KACCSEN4G;      reg6 = 0x73; break; }
		case K8G:      { vSensAcce = KACCSEN8G;      reg6 = 0x7B; break; }
		case K16G:     { vSensAcce = KACCSEN16G;     reg6 = 0x6B; break; }
		default:	   { vSensAcce = KACCSEN4G;      reg6 = 0x73; break; }
	}

	switch (configure->oGyroMode) {
		case K245DPS:  { vSensGyro = KGYRSEN245DPS;  reg1 = 0x60; break; }
		case K500DPS:  { vSensGyro = KGYRSEN500DPS;  reg1 = 0x70; break; }
		case K2000DPS: { vSensGyro = KGYRSEN2000DPS; reg1 = 0x78; break; }
		default: 	   { vSensGyro = KGYRSEN2000DPS; reg1 = 0x78; break; }
	}

	switch (configure->oMagnMode) {
		case K4GAUSS:  { vSensMagn = KMAGSEN4GAUSS;  regx = 0x00; break; }
		case K8GAUSS:  { vSensMagn = KMAGSEN8GAUSS;  regx = 0x20; break; }
		case K12GAUSS: { vSensMagn = KMAGSEN12GAUSS; regx = 0x40; break; }
		case K16GAUSS: { vSensMagn = KMAGSEN16GAUSS; regx = 0x60; break; }
		default:	   { vSensMagn = KMAGSEN4GAUSS;  regx = 0x00; break; }
	}

	RESERVE_SHARED_SPI5(KIMU0);
	v1G       = (int16_t)((1.0) / vSensAcce);

	data = reg1; _writeAcceGyro(CTRL_REG1_G,  &data, 1);
	data = 0x00; _writeAcceGyro(CTRL_REG2_G,  &data, 1);
	data = 0x00; _writeAcceGyro(CTRL_REG3_G,  &data, 1);
	data = 0x38; _writeAcceGyro(CTRL_REG4_G,  &data, 1);
	data = 0x38; _writeAcceGyro(CTRL_REG5_XL, &data, 1);
	data = reg6; _writeAcceGyro(CTRL_REG6_XL, &data, 1);
	data = 0x00; _writeAcceGyro(CTRL_REG7_XL, &data, 1);
	data = 0x34; _writeAcceGyro(CTRL_REG8,    &data, 1);
	data = 0x06; _writeAcceGyro(CTRL_REG9,    &data, 1);
	data = 0xDF; _writeAcceGyro(FIFO_CTRL,    &data, 1);

	data = 0x50; _writeMagn(CTRL_REG1_M,      &data, 1);
	data = regx; _writeMagn(CTRL_REG2_M,      &data, 1);
	data = 0x80; _writeMagn(CTRL_REG3_M,      &data, 1);
	data = 0x08; _writeMagn(CTRL_REG4_M,      &data, 1);

	_caliIMU();
	RELEASE_SHARED_SPI5;
	return (KIMU0NOERR);
}

/*
 * \brief stub_imu0_getImu
 *
 * - Get the imu0 data pack
 *
 */
int32_t	stub_imu0_getImu(accePack_t *accelerometer, gyroPack_t *gyroscope, magnPack_t *magnetometer) {
	uint8_t		xsb[2];

	RESERVE_SHARED_SPI5(KIMU0);
	_readAcceGyro(OUT_X_L_XL, xsb, 2); accelerometer->oAcce_X = (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasAcce[0]) * vSensAcce;
	_readAcceGyro(OUT_Y_L_XL, xsb, 2); accelerometer->oAcce_Y = (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasAcce[1]) * vSensAcce;
	_readAcceGyro(OUT_Z_L_XL, xsb, 2); accelerometer->oAcce_Z = (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasAcce[2]) * vSensAcce;

	_readAcceGyro(OUT_X_L_G, xsb, 2);  gyroscope->oGyro_X =     (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasGyro[0]) * vSensGyro;
	_readAcceGyro(OUT_Y_L_G, xsb, 2);  gyroscope->oGyro_Y =     (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasGyro[1]) * vSensGyro;
	_readAcceGyro(OUT_Z_L_G, xsb, 2);  gyroscope->oGyro_Z =     (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasGyro[2]) * vSensGyro;

	_readMagn(OUT_X_L_M, xsb, 2);      magnetometer->oMagn_X =  (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasMagn[0]) * vSensMagn;
	_readMagn(OUT_Y_L_M, xsb, 2);      magnetometer->oMagn_Y =  (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasMagn[1]) * vSensMagn;
	_readMagn(OUT_Z_L_M, xsb, 2);      magnetometer->oMagn_Z =  (float64_t)((int16_t)(xsb[1]<<8 | xsb[0]) - vBiasMagn[2]) * vSensMagn;
	RELEASE_SHARED_SPI5;
	return (KIMU0NOERR);
}

// Local routines
// ==============

/*
 * \brief _initSPI
 *
 */
static	void	_initSPI(void) {

	shared_spi5_deselect(KIMU0);
}

/*
 * \brief _initIMU
 *
 */
static	void	_initIMU(void) {
	uint8_t		data;

// Accelerometer & Gyro
//    011 11 0 00     = 0x78 - ODR = 119-Hz, 2000-dps, cut-off = 14-Hz
//    0000 00 00      = 0x00 - OUT LPF1
//    00 00 0000      = 0x00 - No high pass
//    00 111 0 00     = 0x38 - Enable Gyro x, y, z axis
//    00 111 000      = 0x38 - Enable Accelerometer x, y, z axis
//    011 10 0 11     = 0x73 - ODR = 119-Hz, 4-g, anti-aliasing = 50-Hz
//    0 00 00 0 0 0   = 0x00 - No high resolution
//    0 0 1 1 0 1 0 0 = 0x34 - Int active low, Open Drain, 4 wire SPI, auto increment
//    0 0 0 0 0 1 1 0 = 0x04 - Disable i2c, FIFO enable
//    110 11111       = 0xDF - FIFO in continuous mode, max threshold

// Magnetometer
//    0 10 100 00     = 0x50 - High performance, 10-Hz
//    0 00 0 0 0 0 0  = 0x00 - 4-Gauss
//    1 0 0 0 0 1 0 0 = 0x84 - SPI On, I2C Off, continuous mode
//    0 0 0 0 10 0 0  = 0x08 - High performance

	vSensAcce = KACCSEN4G;
	vSensGyro = KGYRSEN2000DPS;
	vSensMagn = KMAGSEN4GAUSS;
	v1G       = (int16_t)((1.0) / vSensAcce);

	data = 0x78; _writeAcceGyro(CTRL_REG1_G,  &data, 1);
	data = 0x00; _writeAcceGyro(CTRL_REG2_G,  &data, 1);
	data = 0x00; _writeAcceGyro(CTRL_REG3_G,  &data, 1);
	data = 0x38; _writeAcceGyro(CTRL_REG4_G,  &data, 1);
	data = 0x38; _writeAcceGyro(CTRL_REG5_XL, &data, 1);
	data = 0x73; _writeAcceGyro(CTRL_REG6_XL, &data, 1);
	data = 0x00; _writeAcceGyro(CTRL_REG7_XL, &data, 1);
	data = 0x34; _writeAcceGyro(CTRL_REG8,    &data, 1);
	data = 0x06; _writeAcceGyro(CTRL_REG9,    &data, 1);
	data = 0xDF; _writeAcceGyro(FIFO_CTRL,    &data, 1);

	data = 0x50; _writeMagn(CTRL_REG1_M,      &data, 1);
	data = 0x00; _writeMagn(CTRL_REG2_M,      &data, 1);
	data = 0x80; _writeMagn(CTRL_REG3_M,      &data, 1);
	data = 0x08; _writeMagn(CTRL_REG4_M,      &data, 1);
}

/*
 * \brief _caliIMU
 *
 */
static	void	_caliIMU(void) {
	uint8_t		i, xsbAcce[2], xsbGyro[2], xsbMagn[2];
	int16_t		acce[3], gyro[3], magn[3];
	int32_t		summAcce[3], summGyro[3], summMagn[3];

// Waiting for the FIFO full (32 samples)

	kern_suspendProcess(100);

// Calibrate the accelerometer & the gyro

	summAcce[0] = 0; summAcce[1] = 0; summAcce[2] = 0;
	summGyro[0] = 0; summGyro[1] = 0; summGyro[2] = 0;
	summMagn[0] = 0; summMagn[1] = 0; summMagn[2] = 0;
	for (i = 0; i < 32; i++) {

// Read the values
//      The accelerometer                                      // The gyroscope									// The magnetometer

		_readAcceGyro(OUT_X_L_XL, xsbAcce, 2);			       _readAcceGyro(OUT_X_L_G, xsbGyro, 2);			_readMagn(OUT_X_L_M, xsbMagn, 2);
		acce[0] = (int16_t)(xsbAcce[1]<<8 | xsbAcce[0]);       gyro[0] = (int16_t)(xsbGyro[1]<<8 | xsbGyro[0]); magn[0] = (int16_t)(xsbMagn[1]<<8 | xsbMagn[0]);

		_readAcceGyro(OUT_Y_L_XL, xsbAcce, 2);			       _readAcceGyro(OUT_Y_L_G, xsbGyro, 2);			_readMagn(OUT_Y_L_M, xsbMagn, 2);
		acce[1] = (int16_t)(xsbAcce[1]<<8 | xsbAcce[0]);       gyro[1] = (int16_t)(xsbGyro[1]<<8 | xsbGyro[0]); magn[1] = (int16_t)(xsbMagn[1]<<8 | xsbMagn[0]);

		_readAcceGyro(OUT_Z_L_XL, xsbAcce, 2);			       _readAcceGyro(OUT_Z_L_G, xsbGyro, 2);			_readMagn(OUT_Z_L_M, xsbMagn, 2);
		acce[2] = (int16_t)(xsbAcce[1]<<8 | xsbAcce[0]) + v1G; gyro[2] = (int16_t)(xsbGyro[1]<<8 | xsbGyro[0]); magn[2] = (int16_t)(xsbMagn[1]<<8 | xsbMagn[0]);

// Cumulate for the mean

		summAcce[0] += (int32_t)acce[0]; summAcce[1] += (int32_t)acce[1]; summAcce[2] += (int32_t)acce[2];
		summGyro[0] += (int32_t)gyro[0]; summGyro[1] += (int32_t)gyro[1]; summGyro[2] += (int32_t)gyro[2];
		summMagn[0] += (int32_t)magn[0]; summMagn[1] += (int32_t)magn[1]; summMagn[2] += (int32_t)magn[2];
	}

	vBiasAcce[0] = (int16_t)(summAcce[0] >> 5);
	vBiasAcce[1] = (int16_t)(summAcce[1] >> 5);
	vBiasAcce[2] = (int16_t)(summAcce[2] >> 5);
	vBiasGyro[0] = (int16_t)(summGyro[0] >> 5);
	vBiasGyro[1] = (int16_t)(summGyro[1] >> 5);
	vBiasGyro[2] = (int16_t)(summGyro[2] >> 5);
	vBiasMagn[0] = (int16_t)(summMagn[0] >> 5);
	vBiasMagn[1] = (int16_t)(summMagn[1] >> 5);
	vBiasMagn[2] = (int16_t)(summMagn[2] >> 5);
}

/*
 * \brief _writeAcceGyro
 *
 */
static	void	_writeAcceGyro(uint8_t address, uint8_t *data, uint8_t number) {
	uint8_t		i;

// Write the register address
// Then, write 1..n data

	shared_spi5_select(KIMUA);
	_writeReadSPI(address);
	for (i = 0; i < number; i++) {
		_writeReadSPI(*data++);
	}

	shared_spi5_deselect(KIMUA);
}

/*
 * \brief _readAcceGyro
 *
 */
static	void	_readAcceGyro(uint8_t address, uint8_t *data, uint8_t number) {
	uint8_t		i;

// Write the register address
// Then, Read 1..n data

	shared_spi5_select(KIMUA);
	_writeReadSPI(0x80 | address);
	for (i = 0; i < number; i++) {
		*data++ = _writeReadSPI(0x00);
	}

	shared_spi5_deselect(KIMUA);
}

/*
 * \brief _writeMagn
 *
 */
static	void	_writeMagn(uint8_t address, uint8_t *data, uint8_t number) {
	uint8_t		i;

// Write the register address
// Then, write 1..n data

	shared_spi5_select(KIMUM);
	address = (number > 1) ? (0x40 | address) : (address);
	_writeReadSPI(address);
	for (i = 0; i < number; i++) {
		_writeReadSPI(*data++);
	}

	shared_spi5_deselect(KIMUM);
}

/*
 * \brief _readMagn
 *
 */
static	void	_readMagn(uint8_t address, uint8_t *data, uint8_t number) {
	uint8_t		i;

// Write the register address
// Then, Read 1..n data

	shared_spi5_select(KIMUM);
	address = (number > 1) ? (0xC0 | address) : (0x80 | address);
	_writeReadSPI(address);
	for (i = 0; i < number; i++) {
		*data++ = _writeReadSPI(0x00);
	}

	shared_spi5_deselect(KIMUM);
}

/*
 * \brief _writeReadSPI
 *
 */
static	uint8_t	_writeReadSPI(uint8_t data) {
	uint8_t		wRData;

	wRData = data;
	shared_spi5_writeRead(&wRData);
	return (wRData);
}

