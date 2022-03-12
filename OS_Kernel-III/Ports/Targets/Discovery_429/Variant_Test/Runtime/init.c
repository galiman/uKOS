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
; Goal:		Low level init for the uKOS Discovery_429 module.
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

extern	uint32_t	_stEXRAM, _lnEXRAM;

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
static	void	_FMC_Configuration(void);
static	void	_FPU_Configuration(void);
static	void	_FLASHLatency(uint32_t frequency);

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
	_FMC_Configuration();			// FMC Config
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

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;		// Turn on the GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;		// Turn on the GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;		// Turn on the GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;		// Turn on the GPIOD
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;		// Turn on the GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;		// Turn on the GPIOF
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;		// Turn on the GPIOG
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;		// Turn on the GPIOH
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;		// Turn on the GPIOI

// Init all the GPIO A, B, C, D, E, F, G, H, I

// PA00, AL,  50-MHz, Push-pull	USART4_TX	AF8
// PA01, IN,  50-MHz, Pull-up	MEMS_INT1	AF15
// PA02, IN,  50-MHz, Pull-up	MEMS_INT2	AF15
// PA03, AL,  99-MHz, Push-pull	LCD-B5		AF14
// PA04, AL,  99-MHz, Push-pull	LCD-VSYNC	AF14
// PA05, IN,  50-MHz, Pull-up	SDIO_CARD	AF15	/BNOSDCARD
// PA06, AL,  99-MHz, Push-pull	LCD-G2		AF14
// PA07, OU,  50-MHz, Push-pull	ASP/RST		AF15
// PA08, AL,  50-MHz, Pull-up	SCL3		AF04
// PA09, AL,  50-MHz, Push-pull	USART1_TX	AF7
// PA10, AL,  50-MHz, Pull-up	USART1_RX	AF7
// PA11, AL,  99-MHz, Push-pull	LCD-R4		AF14
// PA12, AL,  99-MHz, Push-pull	LCD-R5		AF14
// PA13, AL,  50-MHz, Pull-up 	TMS			AF0
// PA14, AL,  50-MHz, Pull-down TCK			AF0
// PA15, AL,  50-MHz, Pull-up	TDI			AF0

	CNFGPIO(A,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KOU,KAL,KIN,KAL,KAL,KIN,KIN,KAL,
			  K50,K50,K50,K99,K99,K50,K50,K50,K50,K99,K50,K99,K99,K50,K50,K50,
			  KPU,KPD,KPU,KNO,KNO,KPU,KNO,KPU,KNO,KPU,KPU,KNO,KNO,KPU,KPU,KNO,
			  A00,A00,A00,A14,A14,A07,A07,A04,A15,A14,A15,A14,A14,A15,A15,A08,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KOD,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0);

// PB00, AL,  99-MHz, Push-pull	LCD-R5		AF09
// PB01, AL,  99-MHz, Push-pull	LCD-R5		AF09
// PB02, IN,  50-MHz, Pull-up	-			AF14
// PB03, AL,  50-MHz, Push-pull	TDO			AF0
// PB04, OU,  50-MHz, Push-pull	LED2		AF15
// PB05, AL,  99-MHz, Push-pull	SDCKE1		AF12
// PB06, AL,  99-MHz, Push-pull	SDNEE1		AF12
// PB07, OU,  50-MHz, Push-pull	LED1		AF15
// PB08, AL,  99-MHz, Push-pull	LCD-B6		AF14
// PB09, AL,  99-MHz, Push-pull	LCD-B7		AF14
// PB10, AL,  99-MHz, Push-pull	LCD-G4		AF14
// PB11, AL,  99-MHz, Push-pull	LCD-G5		AF14
// PB12, AL,  50-MHz, Push-pull	OTGFSID		AF12
// PB13, AL,  50-MHz, Push-pull	VBUS		AF12
// PB14, AL,  50-MHz, Push-pull	DM			AF12
// PB15, AL,  50-MHz, Push-pull	DP			AF12

	CNFGPIO(B,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KOU,KAL,KAL,KOU,KAL,KIN,KAL,KAL,
			  K50,K50,K50,K50,K99,K99,K99,K99,K50,K99,K99,K50,K50,K50,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KNO,
			  A12,A12,A12,A12,A14,A14,A14,A14,A15,A12,A12,A15,A00,A15,A09,A09,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PC00, AL,  99-MHz, Push-pull	SDNWE		AF12
// PC01, OU,  50-MHz, Push-pull	CS			AF15
// PC02, OU,  50-MHz, Push-pull	CSX			AF15
// PC03, OU,  50-MHz, Push-pull	LED7		AF15
// PC04, OU,  50-MHz, Push-pull	PSO			AF15
// PC05, OU,  50-MHz, Push-pull	QC			AF15
// PC06, AL,  99-MHz, Push-pull	LCD-HSYNC	AF14
// PC07, AL,  99-MHz, Push-pull	LCD-G6		AF14
// PC08, OU,  50-MHz, Push-pull	LED5		AF15
// PC09, AL,  50-MHz, Pull-up	SDA3		AF04
// PC10, AL,  99-MHz, Push-pull	LCD-R2		AF14
// PC11, AL,  50-MHz, Pull-up	USART4_RX	AF8
// PC12, AL,  50-MHz, Push-pull	USART5_TX	AF8
// PC13, OU,  50-MHz, Push-pull	CSCARD		AF15	/BSELSDCARD
// PC14, AL,  50-MHz, -			OSC			AF0
// PC15, AL,  50-MHz, -			OSC			AF0

	CNFGPIO(C,KAL,KAL,KOU,KAL,KAL,KAL,KAL,KOU,KAL,KAL,KOU,KOU,KOU,KOU,KOU,KAL,
			  K50,K50,K50,K50,K50,K99,K50,K50,K99,K99,K50,K50,K50,K50,K50,K99,
			  KNO,KNO,KNO,KNO,KPU,KNO,KNO,KPU,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,
			  A00,A00,A15,A08,A08,A14,A04,A15,A14,A14,A15,A15,A15,A15,A15,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KOD,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  1,  0,  0,  0,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0);

// PD00, AL,  99-MHz, Push-pull	D2			AF12
// PD01, AL,  99-MHz, Push-pull	D3			AF12
// PD02, AL,  50-MHz, Pull-up	USART5_RX	AF8
// PD03, AL,  99-MHz, Push-pull	LCD-G7		AF14
// PD04, OU,  50-MHz, Push-pull	LED4		AF15
// PD05, OU,  50-MHz, Push-pull	LED3		AF15
// PD06, AL,  99-MHz, Push-pull	LCD-G7		AF14
// PD07, OU,  50-MHz, Push-pull	OutTest		AF15	BTEST_SCOPE
// PD08, AL,  99-MHz, Push-pull	D13			AF12
// PD09, AL,  99-MHz, Push-pull	D14			AF12
// PD10, AL,  99-MHz, Push-pull	D15			AF12
// PD11, OU,  50-MHz, Push-pull	TE			AF15
// PD12, OU,  50-MHz, Push-pull	RDX			AF15
// PD13, OU,  50-MHz, Push-pull	WRX			AF15
// PD14, AL,  99-MHz, Push-pull	D0			AF12
// PD15, AL,  99-MHz, Push-pull	D1			AF12

	CNFGPIO(D,KAL,KAL,KOU,KOU,KOU,KAL,KAL,KAL,KOU,KAL,KOU,KOU,KAL,KAL,KAL,KAL,
			  K99,K99,K50,K50,K50,K99,K99,K99,K50,K99,K50,K50,K99,K50,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KNO,
			  A12,A12,A15,A15,A15,A12,A12,A12,A15,A14,A15,A15,A14,A08,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PE00, AL,  99-MHz, Push-pull	NBL0		AF12
// PE01, AL,  99-MHz, Push-pull	NBL0		AF12
// PE02, AL,  50-MHz, Push-pull	SCK4		AF05
// PE03, OU,  50-MHz, Push-pull	LED0		AF15
// PE04, OU,  50-MHz, Push-pull	NSS4		AF15	/BSELEEPROM
// PE05, AL,  50-MHz, Pull-up	MISO4		AF05
// PE06, AL,  50-MHz, Push-pull	MOSI4		AF05
// PE07, AL,  99-MHz, Push-pull	D4			AF12
// PE08, AL,  99-MHz, Push-pull	D5			AF12
// PE09, AL,  99-MHz, Push-pull	D6			AF12
// PE10, AL,  99-MHz, Push-pull	D7			AF12
// PE11, AL,  99-MHz, Push-pull	D8			AF12
// PE12, AL,  99-MHz, Push-pull	D9			AF12
// PE13, AL,  99-MHz, Push-pull	D0			AF12
// PE14, AL,  99-MHz, Push-pull	D11			AF12
// PE15, AL,  99-MHz, Push-pull	D12			AF12

	CNFGPIO(E,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KOU,KOU,KAL,KAL,KAL,
			  K99,K99,K99,K99,K99,K99,K99,K99,K99,K50,K50,K50,K50,K50,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KNO,KNO,KNO,KNO,
			  A12,A12,A12,A12,A12,A12,A12,A12,A12,A05,A05,A15,A15,A05,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0);

// PF00, AL,  99-MHz, Push-pull	A0			AF12
// PF01, AL,  99-MHz, Push-pull	A1			AF12
// PF02, AL,  99-MHz, Push-pull	A2			AF12
// PF03, AL,  99-MHz, Push-pull	A3			AF12
// PF04, AL,  99-MHz, Push-pull	A4			AF12
// PF05, AL,  99-MHz, Push-pull	A5			AF12
// PF06, OU,  50-MHz, Push-pull	LED6		AF15
// PF07, AL,  50-MHz, Push-pull	SCK5		AF05
// PF08, AL,  50-MHz, Pull-up	MISO5		AF05
// PF09, AL,  50-MHz, Push-pull	MOSI5		AF05
// PF10, AL,  99-MHz, Push-pull	LCD_DE		AF14
// PF11, AL,  99-MHz, Push-pull	SDNRAS		AF12
// PF12, AL,  99-MHz, Push-pull	A6			AF12
// PF13, AL,  99-MHz, Push-pull	A7			AF12
// PF14, AL,  99-MHz, Push-pull	A8			AF12
// PF15, AL,  99-MHz, Push-pull	A9			AF12

	CNFGPIO(F,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KOU,KAL,KAL,KAL,KAL,KAL,KAL,
			  K99,K99,K99,K99,K99,K99,K50,K50,K50,K50,K99,K99,K99,K99,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,
			  A12,A12,A12,A12,A12,A14,A05,A05,A05,A15,A12,A12,A12,A12,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PG00, AL,  99-MHz, Push-pull	A10			AF12
// PG01, AL,  99-MHz, Push-pull	A11			AF12
// PG02, IN,  50-MHz, Pull-down	SW1			AF15
// PG03, IN,  50-MHz, Pull-down	SW0			AF15
// PG04, AL,  99-MHz, Push-pull	BA0			AF12
// PG05, AL,  99-MHz, Push-pull	BA1			AF12
// PG06, AL,  99-MHz, Push-pull	LCD-G7		AF14
// PG07, AL,  99-MHz, Push-pull	LCD-CLK		AF14
// PG08, AL,  99-MHz, Push-pull	SDCLK		AF12
// PG09, OU,  50-MHz, Push-pull	-			AF15
// PG10, AL,  99-MHz, Push-pull	LCD_G3		AF09
// PG11, AL,  99-MHz, Push-pull	LCD_B3		AF14
// PG12, AL,  99-MHz, Push-pull	LCD_B4		AF09
// PG13, OU,  50-MHz, Push-pull	LED Green	AF15
// PG14, OU,  50-MHz, Push-pull	LED Red		AF15
// PG15, AL,  99-MHz, Push-pull	SDNCAS		AF12

	CNFGPIO(G,KAL,KOU,KOU,KAL,KAL,KAL,KOU,KAL,KAL,KAL,KAL,KAL,KIN,KIN,KAL,KAL,
			  K99,K50,K50,K99,K99,K99,K50,K99,K99,K99,K99,K99,K50,K50,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPD,KPD,KNO,KNO,
			  A12,A15,A15,A09,A14,A09,A15,A12,A14,A14,A12,A12,A15,A15,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PH00, AL,  50-MHz, -			OSC			AF0
// PH01, AL,  50-MHz, -			OSC			AF0
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15
// PH05, OU,  50-MHz, Push-pull	-			AF15

	CNFGPIO(H,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KOU,KAL,KAL,
			  K50,K50,K50,K50,K50,K50,K50,K99,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,
			  A15,A15,A15,A15,A15,A15,A15,A15,A15,A15,A15,A15,A15,A15,A00,A00,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);
}

/*
 * \brief _RCC_Configuration
 *
 * - Clock, PLL configuration
 *
 */
static	void	_RCC_Configuration(void) {

// Enable HSE clock
// Configure Flash latency for desired frequency

	RCC_CR_HSEON_bb = 1;
	_FLASHLatency(KFREQUENCY_AHB);

// For f(ck in) = 8-MHz
// f(out) = f(vco) / P			f(out) = 180-MHz, P = 2		---> f(vco) = 360-MHz
// f(vco) = f(ck in) * (N/M)	N/M = 360/8 = 45			---> N = 180, M = 4
// f(usb) = f(vco) / Q			Q = 7						---> f(usb) = 51.42-MHz (USB not possible) !!!

	RCC->PLLCFGR = 0x20000000							// Init the register
				 | (7<<24)								// Q = 7
				 | (1<<22)								// HSE as a PLL source
				 | (0<<16)								// P = 2
				 | (180<<6)								// N = 180
				 | (4<<0);								// M = 4
			
	RCC->CFGR = (0<<30)									// Sysclk selected
			  | (0<<27)									// MCO2 (no division)
			  | (0<<24)									// MCO1 (no division)
			  | (0<<23)									// I2S uses a PLL
			  | (0<<21)									// MCO1, HSI source
			  | (0<<16)									// No RTC clock
			  | (4<<13)									// APB2 bus @ 180/2 = 90-MHz
			  | (5<<10)									// APB1 bus @ 180/4 = 45-MHz
			  | (0<<4)									// HPRE bus @ 180-MHz
			  | (2<<0)									// PLL used as a Sysclk
			  | (0<<0);									// HSI selected as a system clock in a special conditions

// Waiting for stable clock and enable PLL

	while (!RCC_CR_HSERDY_bb);
	RCC_CR_PLLON_bb = 1;

// Waiting for the PLL lock

	while (!RCC_CR_PLLRDY_bb);

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

/*
 * \brief _FMC_Configuration
 *
 * - FMC controller configuration
 *
 */
static	void	_FMC_Configuration(void) {
	volatile	uint32_t	i, *memory;

	RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN;	

// FMC bank 5-6 & CE1 configuration in the synchronous mode
// - SDRAM is a IS42S16400J-7 speed grade, connected to bank 2 (0xD0000000)
//   Some bits in SDCR[1] are don't care, and the have to be set in SDCR[0],
//   they aren't just don't care, the controller will fail if they aren't at 0

	FMC_Bank5_6->SDCR[0] = (1<<12)						// Burst (see SDRam mode register)
						 | (2<<10);						// SDRAM runs @ 90-MHz

	FMC_Bank5_6->SDCR[1] = (0<<9)						// Write allowed
						 | (2<<7)						// CAS latency 2 cycles
						 | (1<<6)						// 4 internal banks
						 | (1<<4)						// 16-bit data bus
						 | (1<<2)						// 12-bit row address
						 | (0<<0);						// 8-bit column address

// One SDRAM clock cycle is 1/90-MHz = 11.1-ns
// Some bits in SDTR[1] are don't care, and the have to be set in SDTR[0],
// they aren't just don't care, the controller will fail if they aren't at 0

	FMC_Bank5_6->SDTR[0] = ((2-1)<<20)					// 2 cycle TRP (22.2-ns > 15-ns)
						 | ((6-1)<<12);					// 6 cycle TRC (66.6-ns > 63-ns)

	FMC_Bank5_6->SDTR[1] = ((2-1)<<24)					// 2 cycle TRCD (22.2-ns > 15-ns)
						 | ((2-1)<<16)					// 2 cycle TWR
						 | ((4-1)<<8)					// 4 cycle TRAS (44.4-ns > 42-ns)
						 | ((7-1)<<4)					// 7 cycle TXSR (77.7-ns > 70-ns)
						 | ((2-1)<<0);					// 2 cycle TMRD
	
	FMC_Bank5_6->SDCMR = (1<<3)							// Command on bank 2
					   | (1<<0);						// MODE = 001, Clock Configuration Enable

	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);			// Waiting for the ready

// ST and SDRAM datasheet agree a delay > 100-us

	for (i = 0; i < 10000000; i++);

	FMC_Bank5_6->SDCMR = (1<<3)							// Command on bank 2
					   | (2<<0);						// MODE = 010, PALL (“All Bank Precharge”) command

	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);			// Waiting for the ready

	FMC_Bank5_6->SDCMR = ((2-1)<<5)         			// NRFS = 2 SDRAM datasheet says "at least two AUTO REFRESH cycles"
					   | (1<<3)							// Command on bank 2
					   | (3<<0);						// MODE = 011, Auto-refresh command

	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);			// Waiting for the ready

// SDRam mode register
// Mode: 11 10 09 08   07 06 05 04   03 02 01 00
//        -  -  1  0    0  0  1  0    0  0  0  0
//
// M9      = 1		Single location access
// M8 - M7 = 00		Standard operation
// M6 - M4 = 010	CAS latency 2
// M3	   = 0		Sequential
// M2 - M0 = 011	Burst length 8

	FMC_Bank5_6->SDCMR = (0x220<<9)						// MRD = 0x220: CAS latency = 2 burst len = 1
					   | (1<<3)							// Command on bank 2
					   | (4<<0);						// MODE = 100, Load Mode Register

	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);			// Waiting for the ready

// 64-ms/4096 = 15.625-us
// 15.625-us * 90-MHz = 1406-20 = 1386

	FMC_Bank5_6->SDRTR = (1386<<1);						// Refresh timer count
	while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);			// Waiting for the ready

	MEMO_SYNC_BARRIER;
	DATA_SYNC_BARRIER;
	INST_SYNC_BARRIER;

// Remapping of the SDRAM (from 0xD0000000 -> 0x90000000)

	SYSCFG->MEMRMP = 0x400;								//

// Clear the SDRAM

	memory = (volatile uint32_t *)&_stEXRAM;
	for (i = 0; i < (uint32_t)&_lnEXRAM; i += 4) {
		*memory++ = 0x00000000;
	}
}

// Local routines
// ==============

/*
 * \brief _FLASHLatency
 *
 * - FLASH configuration
 *
 */
static	void	_FLASHLatency(uint32_t frequency) {
	uint32_t	waitStates;

// Calculate wait_states (30M is valid for 2.7V to 3.6V voltage range, use 24M for 2.4V to 2.7V, 18M for 2.1V to 2.4V or 16M for 1.8V to 2.1V)
// Trim to max allowed value - 7

	waitStates  = frequency / 30000000;
	waitStates &= 7;

	FLASH->ACR = waitStates;							// Set wait_states, disable all caches and prefetch
	FLASH->ACR = FLASH_ACR_DCRST						//
			   | FLASH_ACR_ICRST						//
			   | waitStates;							// Reset caches

	FLASH->ACR = FLASH_ACR_DCEN							//
			   | FLASH_ACR_ICEN							//
			   | FLASH_ACR_PRFTEN						//
			   | waitStates;							// Enable caches and prefetch
}

