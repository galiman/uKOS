/*
; LSM9DS1.
; ========

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2013-03-08
; Modifs:
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 162                  $:  Revision of last commit
; $Date:: 2017-06-27 17:52:59#$:  Date of last commit
;
; Project:	uKOS
; Goal:		ST LSM9DS1 iNEMO equates.
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

#ifndef	__LSM9DS1__
#define	__LSM9DS1__

// Accelerometer/Gyroscope registers

#define	ACT_THS					0x04									//
#define	ACT_DUR					0x05									//
#define	INT_GEN_CFG_XL			0x06									//
#define	INT_GEN_THS_X_XL		0x07									//
#define	INT_GEN_THS_Y_XL		0x08									//
#define	INT_GEN_THS_Z_XL		0x09									//
#define	INT_GEN_DUR_XL			0x0A									//
#define	REFERENCE_G				0x0B									//
#define	INT1_CTRL				0x0C									//
#define	INT2_CTRL				0x0D									//
#define	WHO_AM_I				0x0F									//
#define	CTRL_REG1_G				0X10									//
#define	CTRL_REG2_G				0X11									//
#define	CTRL_REG3_G				0X12									//
#define	ORIENT_CFG_G			0X13									//
#define	INT_GEN_SRC_G			0X14									//
#define	OUT_TEMP_L				0x15									//
#define	OUT_TEMP_H				0x16									//
#define	STATUS_REG1				0x17									//
#define	OUT_X_L_G				0x18									//
#define	OUT_X_H_G				0x19									//
#define	OUT_Y_L_G				0x1A									//
#define	OUT_Y_H_G				0x1B									//
#define	OUT_Z_L_G				0x1C									//
#define	OUT_Z_H_G				0x1D									//
#define	CTRL_REG4_G				0X1E									//
#define	CTRL_REG5_XL			0X1F									//
#define	CTRL_REG6_XL			0X20									//
#define	CTRL_REG7_XL			0X21									//
#define	CTRL_REG8				0X22									//
#define	CTRL_REG9				0X23									//
#define	CTRL_REG10				0X24									//
#define	INT_GEN_SRC_XL			0X26									//
#define	STATUS_REG2				0X27									//
#define	OUT_X_L_XL				0x28									//
#define	OUT_X_H_XL				0x29									//
#define	OUT_Y_L_XL				0x2A									//
#define	OUT_Y_H_XL				0x2B									//
#define	OUT_Z_L_XL				0x2C									//
#define	OUT_Z_H_XL				0x2D									//
#define	FIFO_CTRL				0X2E									//
#define	FIFO_SRC				0X2F									//
#define	INT_GEN_CFG_G			0X30									//
#define	INT_GEN_THS_X_GL		0X31									//
#define	INT_GEN_THS_X_GH		0X32									//
#define	INT_GEN_THS_Y_GL		0X33									//
#define	INT_GEN_THS_Y_GH		0X34									//
#define	INT_GEN_THS_Z_GL		0X35									//
#define	INT_GEN_THS_Z_GH		0X36									//
#define	INT_GEN_DUR_G			0X37									//

// Magnetometer registers

#define	OFFSET_X_REG_L_M		0X05									//
#define	OFFSET_X_REG_H_M		0X06									//
#define	OFFSET_Y_REG_L_M		0X07									//
#define	OFFSET_Y_REG_H_M		0X08									//
#define	OFFSET_Z_REG_L_M		0X09									//
#define	OFFSET_Z_REG_H_M		0X0A									//
#define	WHO_AM_I_M				0x0F									//
#define	CTRL_REG1_M				0X20									//
#define	CTRL_REG2_M				0X21									//
#define	CTRL_REG3_M				0X22									//
#define	CTRL_REG4_M				0X23									//
#define	CTRL_REG5_M				0X24									//
#define	STATUS_REG_M			0x27									//
#define	OUT_X_L_M				0x28									//
#define	OUT_X_H_M				0x29									//
#define	OUT_Y_L_M				0x2A									//
#define	OUT_Y_H_M				0x2B									//
#define	OUT_Z_L_M				0x2C									//
#define	OUT_Z_H_M				0x2D									//
#define	INT_CFG_M				0X30									//
#define	INT_SRC_M				0X31									//
#define	INT_THS_L_M				0X32									//
#define	INT_THS_H_M				0X33									//
#endif

