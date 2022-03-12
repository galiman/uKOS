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
; Goal:		Low level init for the uKOS Discovery_407 module.
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

// PA00, IN,  50-MHz, Pull-up	SW Push		AF14
// PA01, OU,  50-MHz, Push-pull	TEST_SCOPE	AF14	
// PA02, AL,  50-MHz, Push-pull	USART2_TX	AF7
// PA03, AL,  50-MHz, Pull-up	USART2_RX	AF7
// PA04, OU,  50-MHz, Push-pull	Audio DAC	AF14
// PA05, AL,  50-MHz, Push-pull	SPI1_SCK	AF5
// PA06, AL,  50-MHz, Pull-up	SOI1_MISO	AF5
// PA07, AL,  50-MHz, Push-pull	SOI1_MOSI	AF5
// PA08, IN,  50-MHz, Pull-up	-			AF14
// PA09, IN,  Analog, -			VBUS_FS		AF14
// PA10, AL,  50-MHz, Push-pull OTG_ID		AF10
// PA11, AL,  50-MHz, Push-pull OTG_DM		AF10
// PA12, AL,  50-MHz, Push-pull OTG_DP		AF10
// PA13, AL,  50-MHz, Pull-up 	TMS			AF0
// PA14, AL,  50-MHz, Pull-down TCK			AF0
// PA15, AL,  50-MHz, Pull-up	TDI			AF0

	CNFGPIO(A,KAL,KAL,KAL,KAL,KAL,KAL,KAN,KIN,KAL,KAL,KAL,KOU,KAL,KAL,KOU,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KPU,KPD,KPU,KNO,KNO,KNO,KPU,KPU,KNO,KPU,KNO,KNO,KNO,KNO,KNO,KPD,
			  A00,A00,A00,A10,A10,A10,A14,A14,A05,A05,A05,A14,A07,A07,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PB00, IN,  50-MHz, Pull-up	-			AF14
// PB01, IN,  50-MHz, Pull-up	-			AF14
// PB02, IN,  50-MHz, Pull-up	-			AF14
// PB03, AL,  50-MHz, Push-pull	TDO			AF0
// PB04, AL,  50-MHz, Pull-up	RST			AF14
// PB05, IN,  50-MHz, Pull-up	-			AF14
// PB06, AL,  50-MHz, Push-pull	I2C1_SCL	AF4
// PB07, IN,  50-MHz, Pull-up	-			AF14
// PB08, IN,  50-MHz, Pull-up	-			AF14
// PB09, AL,  50-MHz, Pull-up	I2C1_SDA	AF4
// PB10, AL,  50-MHz, Push-pull	I2S2_CK		AF5
// PB11, IN,  50-MHz, Pull-up	-			AF14
// PB12, OU,  50-MHz, Push-pull	SPI2_NSS	AF14	/BSELEEPROM
// PB13, AL,  50-MHz, Push-pull	SPI2_SCK	AF5
// PB14, AL,  50-MHz, Pull-up	SPI2_MISO	AF5
// PB15, AL,  50-MHz, Push-pull	SPI2_MOSI	AF5

	CNFGPIO(B,KAL,KAL,KAL,KOU,KIN,KAL,KAL,KIN,KIN,KAL,KIN,KAL,KAL,KIN,KIN,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KPU,KNO,KNO,KPU,KNO,KPU,KPU,KPU,KNO,KPU,KPU,KNO,KPU,KPU,KPU,
			  A05,A05,A05,A14,A14,A05,A04,A14,A14,A04,A14,A14,A00,A14,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PC00, IN,  50-MHz, Pull-up	-			AF14
// PC01, OU,  50-MHz, Push-pull	LED0		AF14
// PC02, OU,  50-MHz, Push-pull	LED1		AF14
// PC03, AL,  50-MHz, Push-pull	I2S2_SD		AF5
// PC04, AN,  50-MHz, Pull-up	ANA			AF14
// PC05, IN,  50-MHz, Pull-up	-			AF14
// PC06, IN,  50-MHz, Pull-up	-			AF14
// PC07, AL,  50-MHz, Push-pull	I2S3_MCK	AF6
// PC08, AL,  50-MHz, Pull-up	-			AF14
// PC09, OU,  50-MHz, Push-pull	SPI3_NSS	AF14
// PC10, AL,  50-MHz, Push-pull	SPI3_SCK	AF6
// PC11, AL,  50-MHz, Pull-up	SPI3_MISO	AF6
// PC12, AL,  50-MHz, Push-pull	SPI3_MOSI	AF6
// PC13, OU,  50-MHz, Push-pull	LED3		AF14
// PC14, AL,  50-MHz, -			OSC			AF0
// PC15, AL,  50-MHz, -			OSC			AF0

	CNFGPIO(C,KAL,KAL,KOU,KAL,KAL,KAL,KOU,KAL,KAL,KIN,KIN,KAN,KAL,KOU,KOU,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KNO,KNO,KPU,KNO,KNO,KPU,KNO,KPU,KPU,KPU,KNO,KNO,KNO,KPU,
			  A00,A00,A14,A06,A06,A06,A14,A14,A06,A14,A14,A14,A05,A14,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PD00, IN,  50-MHz, Pull-down	SW0			AF14
// PD01, IN,  50-MHz, Pull-down	SW1			AF14
// PD02, AL,  50-MHz, Pull-up	SDIO_CMD	AF12
// PD03, IN,  50-MHz, Pull-down	SW2			AF14
// PD04, IN,  50-MHz, Pull-down	SW3			AF14
// PD05, AN,  50-MHz, Pull-up	I Fault		AF14
// PD06, OU,  50-MHz, Push-pull	LED5		AF14
// PD07, OU,  50-MHz, Push-pull	LED4		AF14
// PD08, AL,  50-MHz, Push-pull	USART3_TX	AF7
// PD09, AL,  50-MHz, Pull-up	USART3_RX	AF7
// PD10, OU,  50-MHz, Push-pull	LED6		AF14
// PD11, OU,  50-MHz, Push-pull	LED7		AF14
// PD12, OU,  50-MHz, Push-pull	LED Green	AF14
// PD13, OU,  50-MHz, Push-pull	LED Orange	AF14
// PD14, OU,  50-MHz, Push-pull	LED Red		AF14
// PD15, OU,  50-MHz, Push-pull	LED Blue	AF14

	CNFGPIO(D,KOU,KOU,KOU,KOU,KOU,KOU,KAL,KAL,KOU,KOU,KAN,KIN,KIN,KAL,KIN,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KNO,KNO,KNO,KNO,KPU,KPU,KNO,KNO,KPU,KPD,KPD,KPU,KPD,KPD,
			  A14,A14,A14,A14,A14,A14,A07,A07,A14,A14,A14,A14,A14,A12,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PE00, IN,  50-MHz, Pull-up	MEMS_INT1	AF14
// PE01, IN,  50-MHz, Pull-up	MEMS_INT2	AF14
// PE02, IN,  50-MHz, Pull-up	SDIO_CARD	AF14	/BNOSDCARD
// PE03, OU,  50-MHz, Push-pull	SPI1_CS		AF14
// PE04, IN,  50-MHz, Pull-up	-			AF14
// PE05, IN,  50-MHz, Pull-up	-			AF14
// PE06, OU,  50-MHz, Push-pull	LED2		AF14
// PE02, IN,  50-MHz, Pull-up	-			AF14
// PE02, IN,  50-MHz, Pull-up	-			AF14
// PE02, IN,  50-MHz, Pull-up	-			AF14
// PE10, IN,  50-MHz, Pull-up	-			AF14
// PE11, IN,  50-MHz, Pull-up	-			AF14
// PE12, IN,  50-MHz, Pull-up	-			AF14
// PE13, IN,  50-MHz, Pull-up	-			AF14
// PE14, IN,  50-MHz, Pull-up	-			AF14
// PE15, IN,  50-MHz, Pull-up	-			AF14

	CNFGPIO(E,KIN,KIN,KIN,KIN,KIN,KIN,KIN,KIN,KIN,KOU,KIN,KIN,KOU,KIN,KIN,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KPU,KPU,KPU,KPU,KPU,KPU,KPU,KPU,KPU,KNO,KPU,KPU,KNO,KPU,KPU,KPU,
			  A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0);
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
// f(out) = f(vco) / P			f(out) = 168-MHz, P = 2		---> f(vco) = 336-MHz
// f(vco) = f(ck in) * (N/M)	N/M = 336/8 = 42			---> N = 168, M = 4
// f(usb) = f(vco) / Q			Q = 7						---> f(usb) = 48-MHz

	RCC->PLLCFGR = 0x20000000							// Init the register
				 | (7<<24)								// Q = 7
				 | (1<<22)								// HSE as a PLL source
				 | (0<<16)								// P = 2
				 | (168<<6)								// N = 168
				 | (4<<0);								// M = 4
			
	RCC->CFGR = (0<<30)									// Sysclk selected
			  | (0<<27)									// MCO2 (no division)
			  | (0<<24)									// MCO1 (no division)
			  | (0<<23)									// I2S uses a PLL
			  | (0<<21)									// MCO1, HSI source
			  | (0<<16)									// No RTC clock
			  | (4<<13)									// APB2 bus @ 168/2 = 84-MHz
			  | (5<<10)									// APB1 bus @ 168/4 = 42-MHz
			  | (0<<4)									// HPRE bus @ 168-MHz
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

