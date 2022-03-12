/*
; init.
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
; Goal:		Low level init for the uKOS Discovery_476 module.
;
;			!!! This code HAS not to contain static data.
;			!!! It is called before to copy and to initialize
;			!!! the variable into the RAM.
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

LOC_CONST_STRG(aStrApplication[]) =	"init         First hardware initializations.           (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Init\n"
									"====\n\n"

									"This code places in a quite state the hardware resources.\n\n";

// GPIO config macro
// -----------------

#define	CNFGPIO(port, \
				a15, a14, a13, a12, a11, a10, a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, \
				b15, b14, b13, b12, b11, b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0, \
				c15, c14, c13, c12, c11, c10, c9, c8, c7, c6, c5, c4, c3, c2, c1, c0, \
				d15, d14, d13, d12, d11, d10, d9, d8, d7, d6, d5, d4, d3, d2, d1, d0, \
				e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0, \
				f15, f14, f13, f12, f11, f10, f9, f8, f7, f6, f5, f4, f3, f2, f1, f0) \
				GPIO##port->AFR[1] =	(d15<<28) | (d14<<24) | (d13<<20) | (d12<<16) |	\
										(d11<<12) | (d10<<8)  | (d9<<4)   | (d8<<0); \
				GPIO##port->AFR[0] =	(d7<<28)  | (d6<<24)  | (d5<<20)  | (d4<<16)  |	\
										(d3<<12)  | (d2<<8)   | (d1<<4)   | (d0<<0); \
				GPIO##port->OSPEEDR =	(b15<<30) | (b14<<28) | (b13<<26) | (b12<<24) |	\
										(b11<<22) | (b10<<20) | (b9<<18)  | (b8<<16)  |	\
										(b7<<14)  | (b6<<12)  | (b5<<10)  | (b4<<8)   |	\
										(b3<<6)   | (b2<<4)   | (b1<<2)   | (b0<<0); \
				GPIO##port->OTYPER = 	(e15<<15) | (e14<<14) | (e13<<13) | (e12<<12) |	\
										(e11<<11) | (e10<<10) | (e9<<9)   | (e8<<8)   |	\
										(e7<<7)   | (e6<<6)   | (e5<<5)   | (e4<<4)   |	\
										(e3<<3)   | (e2<<2)   | (e1<<1)   | (e0<<0); \
				GPIO##port->MODER = 	(a15<<30) | (a14<<28) | (a13<<26) | (a12<<24) |	\
										(a11<<22) | (a10<<20) | (a9<<18)  | (a8<<16)  |	\
										(a7<<14)  | (a6<<12)  | (a5<<10)  | (a4<<8)   |	\
										(a3<<6)   | (a2<<4)   | (a1<<2)   | (a0<<0); \
				GPIO##port->PUPDR =		(c15<<30) | (c14<<28) | (c13<<26) | (c12<<24) |	\
										(c11<<22) | (c10<<20) | (c9<<18)  | (c8<<16)  |	\
										(c7<<14)  | (c6<<12)  | (c5<<10)  | (c4<<8)   |	\
										(c3<<6)   | (c2<<4)   | (c1<<2)   | (c0<<0); \
				GPIO##port->ODR =		(f15<<15) | (f14<<14) | (f13<<13) | (f12<<12) |	\
										(f11<<11) | (f10<<10) | (f9<<9)   | (f8<<8)   |	\
										(f7<<7)   | (f6<<6)   | (f5<<5)   | (f4<<4)   |	\
										(f3<<3)   | (f2<<2)   | (f1<<1)   | (f0<<0);

// Prototypes
// ==========

static	void	_GPIO_Configuration(void);
static	void	_RCC_Configuration(void);
static	void	_FPU_Configuration(void);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Init, KIDSTARTUP, KINITNUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief init_init
 *
 * - Initialize some basic periphs
 * - GPIO, watchdog, SDRAM
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	init_init(void) {

	_GPIO_Configuration();			// GPIO Config
	_RCC_Configuration();			// Clock Config
	_FPU_Configuration();			// FPU Enabled
}

/*
 * \brief _FPU_Configuration
 *
 * - Enable the FPU
 *
 */
static	void	_FPU_Configuration(void) {

	SCB->CPACR |= ((SCB_CPACR_FPAF<<20) | (SCB_CPACR_FPAF<<22));	// Set CP10 and CP11 Full Access
	FPU->FPCCR |= ((1<<FPU_FPCCR_ASPEN) | (1<<FPU_FPCCR_LSPEN));	// Lazy stacking enable
}

/*
 * \brief _GPIO_Configuration
 *
 * - GPIO configuration
 *
 */
static	void	_GPIO_Configuration(void) {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;		// Turn on the GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;		// Turn on the GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;		// Turn on the GPIOC
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;		// Turn on the GPIOD
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;		// Turn on the GPIOE

// Init all the GPIO A, B, C, D, E

// PA00, IN,  50-MHz, Pull-up	JOY_CENTER	AF15
// PA01, IN,  50-MHz, Pull-up	JOY_CENTER	AF15
// PA02, IN,  50-MHz, Pull-up	JOY_CENTER	AF15
// PA03, IN,  50-MHz, Pull-up	JOY_CENTER	AF15
// PA04, OU,  50-MHz, Push-pull	MFX_WAKEUP	AF15
// PA05, IN,  50-MHz, Pull-up	JOY_CENTER	AF15
// PA06, AL,  50-MHz, Open-D	LCD_SEG23	AF11
// PA07, AL,  50-MHz, Open-D	LCD_SEG0	AF11
// PA08, AL,  50-MHz, Open-D	LCD_COM0	AF11
// PA09, AL,  50-MHz, Open-D	LCD_COM1	AF11
// PA10, AL,  50-MHz, Open-D	LCD_COM2	AF11
// PA11, IN,  50-MHz, Pull-up	OTGFSDM		AF15
// PA12, IN,  50-MHz, Pull-up	OTGFSDP		AF15
// PA13, AL,  50-MHz, Pull-up 	TMS			AF0
// PA14, AL,  50-MHz, Pull-down TCK			AF0
// PA15, AL,  50-MHz, Pull-up	TDI			AF0

	CNFGPIO(A,KAL,KAL,KAL,KIN,KIN,KAL,KAL,KAL,KAL,KAL,KIN,KOU,KIN,KIN,KIN,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KPD,KPU,KPU,KPU,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KPU,KPU,KPU,KPU,
			  A11,A00,A00,A15,A15,A11,A11,A11,A11,A11,A15,A15,A14,A15,A15,A15,
			  KOD,KPP,KPP,KPP,KPP,KOD,KOD,KOD,KOD,KOD,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PB00, AL,  50-MHz, Open-D	LCD_SEG21	AF11
// PB01, AL,  50-MHz, Open-D	LCD_SEG2	AF11
// PB02, OU,  50-MHz, Push-pull	LED_R		AF15	BLED0
// PB03, AL,  50-MHz, Push-pull	TDO			AF0
// PB04, AL,  50-MHz, Open-D	LCD_SEG11	AF11
// PB05, AL,  50-MHz, Open-D	LCD_SEG11	AF11
// PB06, AL,  50-MHz, Pull-up	SCL1		AF4
// PB07, AL,  50-MHz, Pull-up	SDA1		AF4
// PB08, IN,  50-MHz, Pull-up	GYRO_INT	AF15
// PB09, AL,  50-MHz, Push-pull	LCD_COM3	AF11
// PB10, AL,  50-MHz, Pull-up	MFX_SCL2	AF4
// PB11, AL,  50-MHz, Pull-up	MFX_SDA2	AF4
// PB12, AL,  50-MHz, Open-D	LCD_SEG20	AF11
// PB13, AL,  50-MHz, Open-D	LCD_SEG3	AF11
// PB14, AL,  50-MHz, Open-D	LCD_SEG19	AF11
// PB15, AL,  50-MHz, Open-D	LCD_SEG3	AF11

	CNFGPIO(B,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KIN,KAL,KAL,KAL,KAL,KAL,KOU,KAL,KAL,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KNO,KNO,KPU,KPU,KNO,KPU,KPU,KPU,KNO,KNO,KNO,KNO,KNO,KNO,
			  A11,A11,A11,A11,A04,A04,A11,A15,A04,A04,A11,A11,A00,A15,A11,A11,
			  KOD,KOD,KOD,KOD,KPP,KPP,KOD,KPP,KOD,KOD,KOD,KOD,KPP,KPP,KOD,KOD,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PC00, OU,  50-MHz, Push-pull	MAG_CS		AF15
// PC01, IN,  50-MHz, Pull-up	MAG_INT		AF15
// PC02, IN,  50-MHz, Pull-up	MAG_INT		AF15
// PC03, AL,  50-MHz, Open-Dp	LCD_VLCD	AF11
// PC04, AL,  50-MHz, Open-D	LCD_SEG22	AF11
// PC05, AL,  50-MHz, Open-D	LCD_SEG1	AF11
// PC06, AL,  50-MHz, Open-D	LCD_SEG14	AF11
// PC07, AL,  50-MHz, Open-D	LCD_SEG9	AF11
// PC08, AL,  50-MHz, Open-D	LCD_SEG14	AF11
// PC09, IN,  50-MHz, Pull-up	OTGPower	AF15
// PC10, IN,  50-MHz, Pull-up	OTGOver		AF15
// PC11, IN,  50-MHz, Pull-up	OTGVBus		AF15
// PC12, IN,  50-MHz, Pull-up	OTGID		AF15
// PC13, IN,  50-MHz, Pull-up	MFX_IRQ		AF15
// PC14, IN,  50-MHz, -			-			AF15
// PC15, IN,  50-MHz, -			-			AF15

	CNFGPIO(C,KIN,KIN,KIN,KIN,KIN,KIN,KIN,KAL,KAL,KAL,KAL,KAL,KAL,KIN,KIN,KOU,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KPU,KPU,KPU,KPU,KPU,KPU,KPU,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KPU,KNO,
			  A15,A15,A15,A15,A15,A15,A15,A11,A11,A11,A11,A11,A15,A11,A15,A15,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KOD,KOD,KOD,KOD,KOD,KOD,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1);

// PD00, OU,  50-MHz, Push-pull	EXT_RST		AF15
// PD01, AL,  50-MHz, Push-pull	MEM_SCK2	AF05
// PD02, IN,  50-MHz, Pull-up	GYRO_INT1	AF15
// PD03, AL,  50-MHz, Pull-up	MEM_MISO2	AF05
// PD04, AL,  50-MHz, Pull-up	MEM_MOSI2	AF05
// PD05, AL,  50-MHz, Push-pull	USART2_TX	AF07
// PD06, AL,  50-MHz, Push-pull	USART2_RX	AF07
// PD07, OU,  50-MHz, Push-pull	GYRO_CS		AF15
// PD08, AL,  50-MHz, Open-D	LCD_SEG18	AF11
// PD09, AL,  50-MHz, Open-D	LCD_SEG5	AF11
// PD10, AL,  50-MHz, Open-D	LCD_SEG17	AF11
// PD11, AL,  50-MHz, Open-D	LCD_SEG6	AF11
// PD12, AL,  50-MHz, Open-D	LCD_SEG16	AF11
// PD13, AL,  50-MHz, Open-D	LCD_SEG7	AF11
// PD14, AL,  50-MHz, Open-D	LCD_SEG15	AF11
// PD15, AL,  50-MHz, Open-D	LCD_SEG8	AF11

	CNFGPIO(D,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KOU,KAL,KAL,KAL,KAL,KIN,KAL,KOU,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KNO,
			  A11,A11,A11,A11,A11,A11,A11,A11,A15,A07,A07,A05,A05,A15,A05,A15,
			  KOD,KOD,KOD,KOD,KOD,KOD,KOD,KOD,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  1);

// PE00, OU,  50-MHz, Push-pull	XL_CS		AF15
// PE01, IN,  50-MHz, Pull-up	XL_INT		AF15
// PE02, AL,  50-MHz, Push-pull	SAI1_MCK	AF13
// PE03, OU,  50-MHz, Push-pull	AUDIO_RST	AF15
// PE04, AL,  50-MHz, Push-pull	SAI1_FS		AF13
// PE05, AL,  50-MHz, Push-pull	SAI1_SCK	AF13
// PE06, AL,  50-MHz, Push-pull	SAI1_SD		AF13
// PE07, IN,  50-MHz, Pull-up	AUDIO_DIN	AF15
// PE08, OU,  50-MHz, Push-pull	LED_G		AF15	BLED1
// PE09, OU,  50-MHz, Pull-up	AUDIO_CLK	AF15
// PE10, AL,  50-MHz, Push-pull	QSPI_CLK	AF10
// PE11, OU,  50-MHz, Push-pull	QSPI_CS		AF15	BTEST
// PE12, AL,  50-MHz, Push-pull	QSPI_D0		AF10
// PE13, AL,  50-MHz, Push-pull	QSPI_D1		AF10
// PE14, AL,  50-MHz, Push-pull	QSPI_D2		AF10
// PE15, AL,  50-MHz, Push-pull	QSPI_D3		AF10

	CNFGPIO(E,KAL,KAL,KAL,KAL,KOU,KAL,KOU,KOU,KIN,KAL,KAL,KAL,KOU,KAL,KIN,KOU,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KPU,KNO,KNO,KNO,KPU,KNO,
			  A10,A10,A10,A10,A15,A10,A15,A15,A15,A13,A13,A13,A15,A13,A15,A15,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1);

}

/*
 * \brief _RCC_Configuration
 *
 * - Clock, PLL configuration
 *
 */
static	void	_RCC_Configuration(void) {

	RCC->CR      |= 0x00010100;							// HSI & HSE on
	RCC->CFGR     = 0x00000000;							// Reset CFGR register
	RCC->CR      &= 0xFEF7FFFF;							// Reset CSSON and PLLON bits
	RCC->PLLCFGR  = 0x00001000;							// Reset PLLCFGR register
	RCC->CR      &= 0xFFFBFFFF;							// Reset HSEBYP bit
			
// Activate the ART + 4ws (OK for 80-MHz)			

	FLASH->ACR = FLASH_ACR_DCEN							//
			   | FLASH_ACR_ICEN							//
			   | FLASH_ACR_PRFTEN						//
			   | FLASH_ACR_LATENCY_4WS;					// Enable caches and prefetch

// For f(ck in) = 16-MHz
// f(out) = f(vco) / R			f(out) = 80-MHz, R = 4		---> f(vco) = 320-MHz
// f(vco) = f(ck in) * (N/M)	N/M = 40/2 = 20				---> N = 40, M = 2
// f(usb) = f(vco) / Q			Q = 6						---> f(usb) = 53-MHz (USB not possible) !!!

	RCC->PLLCFGR = (0<<27)								// PLLPDIV (not available)
				 | (1<<25)								// R = 4
				 | (1<<24)								// PLLR enabled
				 | (2<<21)								// Q = 6
				 | (1<<20)								// PLLQ enabled
				 | (0<<17)								// P = 6
				 | (1<<16)								// PLLP enabled
				 | (40<<8)								// N = 40
				 | (1<<4)								// M = 2
				 | (2<<0);								// HSI16 as a PLL source
			
	RCC->CFGR = (0<<28)									// MCO (no division)
			  | (5<<24)									// PLL output
			  | (0<<11)									// APB2 bus @ 80/1 = 80-MHz
			  | (0<<8)									// APB1 bus @ 80/1 = 80-MHz
			  | (0<<4)									// HPRE bus @ 80-MHz
			  | (3<<0);									// PLL selected as a system clock

// Enable PLL

	RCC_CR_PLLON_bb = 1;
	while (!RCC_CR_PLLRDY_bb);

// Waiting for the PLL lock

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

