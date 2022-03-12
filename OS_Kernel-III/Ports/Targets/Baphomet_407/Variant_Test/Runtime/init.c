/*
; init.
; =====

;------------------------------------------------------------------------
; Author:	Franzi Edo.	The 2006-06-28
; Modifs:	Franzi Edo.	The 2014-02-25	Add 5.95ns to the PSRAM timing
;
; SVN:
; $Author:: efr               $:  Author of last commit
; $Rev:: 185                  $:  Revision of last commit
; $Date:: 2017-08-17 15:20:55#$:  Date of last commit
;
; Project:	uKOS
; Goal:		Low level init for the uKOS Baphomet_407 module.
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

// FSMC & RAM
// ----------

#define KBURSTLENCON	0x0007			//
#define KBURSTWRAPNO	0x0008			//
#define KDRVSTRGTH1_2	0x0010			//
#define KWAITCONFONE	0x0100			//
#define KWAITPOLARLOW	0x0000			//
#define KWAITPOLARHIGH	0x0400			//
#define KLATENCY3		0x1800			//
#define KINKLATENCYVAR	0x0000			//
#define KMODESYNC		0x0000			//
#define KMODESAYNC		0x8000			//

#define KPSRAMBCR		1
#define KPSRAMASYNC		(KMODESAYNC | KLATENCY3 | KWAITPOLARHIGH | KWAITCONFONE | KDRVSTRGTH1_2 | KBURSTWRAPNO | KBURSTLENCON)
#define KPSRAMSYNC		(KBURSTLENCON | KBURSTWRAPNO | KDRVSTRGTH1_2 | KWAITCONFONE| KWAITPOLARLOW | KLATENCY3 | KINKLATENCYVAR | KMODESYNC)

#define BRAMENABLE		9				// PORT F 9, RAM Enable (for IS66WVE4M16)
#define BRAMCRE			9				// PORT F 9, CRE Enable (for MT45W8MW16)

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
static	void	_FSMC_Configuration(void);
static	void	_FPU_Configuration(void);
static	void	_FLASHLatency(uint32_t frequency);
static	void	_writeConfigPSRAM(uint16_t regID, uint16_t regValue);

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
	_FSMC_Configuration();			// FSMC Config
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
// PA01, AL,  50-MHz, Pull-up	USART4_RX	AF8
// PA02, AL,  50-MHz, Push-pull	USART2_TX	AF7
// PA03, AL,  50-MHz, Pull-up	USART2_RX	AF7
// PA04, OU,  50-MHz, Push-pull	SPI1_NSS	AF14	/BSELADT7320
// PA05, AL,  50-MHz, Push-pull	SPI1_SCK	AF5
// PA06, AL,  50-MHz, Push-pull	DCMI_PIXCK	AF13
// PA07, OU,  50-MHz, Push-pull	OI0			AF14
// PA08, IN,  50-MHz, Pull-up	IO3			AF14
// PA09, IN,  50-MHz, Pull-up	VBUS_FS!!	AF14
// PA10, AL,  50-MHz, Pull-up	USART1_RX	AF7
// PA11, IN,  50-MHz, Pull-up	OTG_DM!!	AF14
// PA12, IN,  50-MHz, Pull-up	OTG_DP!!	AF14
// PA13, AL,  50-MHz, Pull-up 	TMS			AF0
// PA14, AL,  50-MHz, Pull-down TCK			AF0
// PA15, AL,  50-MHz, Pull-up	TDI			AF0

	CNFGPIO(A,KAL,KAL,KAL,KIN,KIN,KAL,KIN,KIN,KOU,KAL,KAL,KOU,KAL,KAL,KAL,KAL,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KPU,KPD,KPU,KPU,KPU,KPU,KPU,KPU,KNO,KNO,KNO,KNO,KPU,KNO,KPU,KNO,
			  A00,A00,A00,A14,A14,A07,A14,A14,A14,A13,A05,A14,A07,A07,A08,A08,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0);

// PB00, IN,  50-MHz, Pull-down	SW1			AF14
// PB01, OU,  50-MHz, Push-pull	OI1			AF14
// PB02, IN,  50-MHz, Pull-up	Boot1		AF14
// PB03, AL,  50-MHz, Push-pull	TDO			AF0
// PB04, AL,  50-MHz, Pull-up	SPI1_MISO	AF5
// PB05, AL,  50-MHz, Push-pull	SPI1_MOSI	AF5
// PB06, AL,  50-MHz, Push-pull	USART1_TX	AF7
// PB07, AL,  99-MHz, Push-pull	FSMC_NL		AF12
// PB08, AL,  50-MHz, Open DU	I2C1_SCL	AF4
// PB09, AL,  50-MHz, Open DU	I2C1_SDA	AF4
// PB10, AL,  50-MHz, Push-pull	USART3_TX	AF7
// PB11, AL,  50-MHz, Pull-up	USART3_RX	AF7
// PB12, OU,  50-MHz, Push-pull	SPI2_NSS	AF14	/BSELEEPROM
// PB13, AL,  50-MHz, Push-pull	SPI2_SCK	AF5
// PB14, AL,  50-MHz, Pull-up	SPI2_MISO	AF5
// PB15, AL,  50-MHz, Push-pull	SPI2_MOSI	AF5

	CNFGPIO(B,KAL,KAL,KAL,KOU,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KIN,KOU,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K99,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KPU,KNO,KNO,KPU,KNO,KPU,KPU,KNO,KNO,KNO,KPU,KNO,KPU,KNO,KPD,
			  A05,A05,A05,A14,A07,A07,A04,A04,A12,A07,A05,A05,A00,A14,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KOD,KOD,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0);

// PC00, IN,  50-MHz, Pull-up	IO4			AF14
// PC01, OU,  50-MHz, Push-pull	OI5			AF14
// PC02, IN,  50-MHz, Pull-up	GND			AF14
// PC03, IN,  50-MHz, Pull-up	GND			AF14
// PC04, IN,  50-MHz, Pull-up	FTDI_nRF	AF14	BIRQUSBR
// PC05, IN,  50-MHz, Pull-up	FTDI_nWR	AF14	BIRQUSBW
// PC06, AL,  50-MHz, Push-pull	USART6_TX	AF8
// PC07, AL,  50-MHz, Pull-up	USART6_RX	AF8
// PC08, AL,  50-MHz, Pull-up	SDIO_D0		AF12
// PC09, OU,  50-MHz, Push-pull	SPI3_NSS	AF14	/BSELSDCARD
// PC10, AL,  50-MHz, Push-pull	SPI3_SCK	AF6
// PC11, AL,  50-MHz, Pull-up	SPI3_MISO	AF6
// PC12, AL,  50-MHz, Push-pull	SPI3_MOSI	AF6
// PC13, IN,  50-MHz, Pull-down	SW0			AF14
// PC14, AL,  50-MHz, -			OSC			AF0
// PC15, AL,  50-MHz, -			OSC			AF0

	CNFGPIO(C,KAL,KAL,KIN,KAL,KAL,KAL,KOU,KAL,KAL,KAL,KIN,KIN,KIN,KIN,KOU,KIN,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KPD,KNO,KPU,KNO,KNO,KPU,KPU,KNO,KPU,KPU,KPU,KPU,KNO,KPU,
			  A00,A00,A14,A06,A06,A06,A14,A12,A08,A08,A14,A14,A14,A14,A14,A14,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PD00, AL,  99-MHz, Push-pull	FSMC_D2		AF12
// PD01, AL,  99-MHz, Push-pull	FSMC_D3		AF12
// PD02, AL,  50-MHz, Push-pull	SDIO_CMD	AF12
// PD03, AL,  99-MHz, Push-pull	FSMC_CLK	AF12
// PD04, AL,  99-MHz, Push-pull	FSMC_NOE	AF12
// PD05, AL,  99-MHz, Push-pull	FSMC_NWE	AF12
// PD06, AL,  99-MHz, Pull-up	FSMC_NWAIT	AF12
// PD07, AL,  99-MHz, Push-pull	FSMC_NE1	AF12
// PD08, AL,  99-MHz, Push-pull	FSMC_D13	AF12
// PD09, AL,  99-MHz, Push-pull	FSMC_D14	AF12
// PD10, AL,  99-MHz, Push-pull	FSMC_D15	AF12
// PD11, AL,  99-MHz, Push-pull	FSMC_A16	AF12
// PD12, AL,  99-MHz, Push-pull	FSMC_A17	AF12
// PD13, AL,  99-MHz, Push-pull	FSMC_A18	AF12
// PD14, AL,  99-MHz, Push-pull	FSMC_D0		AF12
// PD15, AL,  99-MHz, Push-pull	FSMC_D1		AF12

	CNFGPIO(D,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,
			  K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K50,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,
			  A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PE00, AL,  99-MHz, Push-pull	FSMC_NBL0	AF12
// PE01, AL,  99-MHz, Push-pull	FSMC_NBL1	AF12
// PE02, AL,  99-MHz, Push-pull	FSMC_A23	AF12
// PE03, AL,  99-MHz, Push-pull	FSMC_A19	AF12
// PE04, AL,  99-MHz, Push-pull	FSMC_A20	AF12
// PE05, AL,  99-MHz, Push-pull	FSMC_A21	AF12
// PE06, AL,  99-MHz, Push-pull	FSMC_A22	AF12
// PE07, AL,  99-MHz, Push-pull	FSMC_D4		AF12
// PE08, AL,  99-MHz, Push-pull	FSMC_D5		AF12
// PE09, AL,  99-MHz, Push-pull	FSMC_D6		AF12
// PE10, AL,  99-MHz, Push-pull	FSMC_D7		AF12
// PE11, AL,  99-MHz, Push-pull	FSMC_D8		AF12
// PE12, AL,  99-MHz, Push-pull	FSMC_D9		AF12
// PE13, AL,  99-MHz, Push-pull	FSMC_D10	AF12
// PE14, AL,  99-MHz, Push-pull	FSMC_D11	AF12
// PE15, AL,  99-MHz, Push-pull	FSMC_D12	AF12

	CNFGPIO(E,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,
			  K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KPU,
			  A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PF00, AL,  99-MHz, Push-pull	FSMC_A0		AF12
// PF01, AL,  99-MHz, Push-pull	FSMC_A1		AF12
// PF02, AL,  99-MHz, Push-pull	FSMC_A2		AF12
// PF03, AL,  99-MHz, Push-pull	FSMC_A3		AF12
// PF04, AL,  99-MHz, Push-pull	FSMC_A4		AF12
// PF05, AL,  99-MHz, Push-pull	FSMC_A5		AF12
// PF06, OU,  50-MHz, Push-pull	LED0		AF14
// PF07, IN,  50-MHz, Pull-down	SW3			AF14
// PF08, OU,  50-MHz, Push-pull	LED1		AF14
// PF09, OU,  50-MHz, Push-pull	CRE			AF14
// PF10, IN,  50-MHz, Pull-down	SW2			AF14
// PF11, AL,  50-MHz, Push-pull	DCMI_D12	AF13
// PF12, AL,  99-MHz, Push-pull	FSMC_A6		AF12
// PF13, AL,  99-MHz, Push-pull	FSMC_A7		AF12
// PF14, AL,  99-MHz, Push-pull	FSMC_A8		AF12
// PF15, AL,  99-MHz, Push-pull	FSMC_A9		AF12

	CNFGPIO(F,KAL,KAL,KAL,KAL,KAL,KIN,KOU,KOU,KIN,KOU,KAL,KAL,KAL,KAL,KAL,KAL,
			  K99,K99,K99,K99,K50,K50,K50,K50,K50,K50,K99,K99,K99,K99,K99,K99,
			  KNO,KNO,KNO,KNO,KNO,KPD,KNO,KNO,KPD,KNO,KNO,KNO,KNO,KNO,KNO,KNO,
			  A12,A12,A12,A12,A13,A14,A14,A14,A14,A14,A12,A12,A12,A12,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PG00, AL,  99-MHz, Push-pull	FSMC_A10	AF12
// PG01, AL,  99-MHz, Push-pull	FSMC_A11	AF12
// PG02, AL,  99-MHz, Push-pull	FSMC_A12	AF12
// PG03, AL,  99-MHz, Push-pull	FSMC_A13	AF12
// PG04, AL,  99-MHz, Push-pull	FSMC_A14	AF12
// PG05, AL,  99-MHz, Push-pull	FSMC_A15	AF12
// PG06, IN,  50-MHz, Pull-up	IO2			AF14
// PG07, OU,  50-MHz, Push-pull	TP3			AF14
// PG08, OU,  50-MHz, Push-pull	TP4			AF14
// PG09, AL,  99-MHz, Push-pull	FSMC_NE2	AF12
// PG10, AL,  99-MHz, Push-pull	FSMC_NE3	AF12
// PG11, OU,  50-MHz, Push-pull	LED2		AF14
// PG12, OU,  50-MHz, Push-pull	LED3		AF14
// PG13, AL,  99-MHz, Push-pull	FSMC_A24	AF12
// PG14, IN,  50-MHz, Pull-up	CD			AF12
// PG15, OU,  50-MHz, Push-pull	LED4		AF14

	CNFGPIO(G,KOU,KIN,KAL,KOU,KOU,KAL,KAL,KOU,KOU,KIN,KAL,KAL,KAL,KAL,KAL,KAL,
			  K50,K50,K99,K50,K50,K99,K99,K50,K50,K50,K99,K99,K99,K99,K99,K99,
			  KNO,KPU,KNO,KNO,KNO,KPU,KPU,KNO,KNO,KPU,KNO,KNO,KNO,KNO,KNO,KNO,
			  A14,A14,A12,A14,A14,A12,A12,A14,A14,A14,A12,A12,A12,A12,A12,A12,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);

// PH00, AL,  50-MHz, -			OSC			AF0
// PH01, AL,  50-MHz, -			OSC			AF0
// PH02, OU,  50-MHz, Push-pull	LED5		AF14
// PH03, OU,  50-MHz, Push-pull	LED6		AF14
// PH04, AL,  50-MHz, Open DU	I2C2_SCL	AF4
// PH05, AL,  50-MHz, Open DU	I2C2_SDA	AF4
// PH06, OU,  50-MHz, Push-pull	OI6			AF14
// PH07, OU,  50-MHz, Push-pull	OI7			AF14
// PH08, AL,  50-MHz, Push-pull	DCMI_HSYNC	AF13
// PH09, AL,  50-MHz, Push-pull	DCMI_D0		AF13
// PH10, AL,  50-MHz, Push-pull	DCMI_D1		AF13
// PH11, AL,  50-MHz, Push-pull	DCMI_D2		AF13
// PH12, AL,  50-MHz, Push-pull	DCMI_D3		AF13
// PH13, IN,  50-MHz, Pull-up	-			AF14
// PH14, AL,  50-MHz, Push-pull	DCMI_D4		AF13
// PH15, AL,  50-MHz, Push-pull	DCMI_D11	AF13

	CNFGPIO(H,KAL,KAL,KIN,KAL,KAL,KAL,KAL,KAL,KOU,KOU,KAL,KAL,KOU,KOU,KAL,KAL,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KNO,KNO,KPU,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KPU,KPU,KNO,KNO,KNO,KNO,
			  A13,A13,A14,A13,A13,A13,A13,A13,A14,A14,A04,A04,A14,A14,A00,A00,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KOD,KOD,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0);

// PI00, AL,  50-MHz, Push-pull	DCMI_D13	AF13
// PI01, AL,  50-MHz, Push-pull	DCMI_D8		AF13
// PI02, AL,  50-MHz, Push-pull	DCMI_D9		AF13
// PI03, AL,  50-MHz, Push-pull	DCMI_D10	AF13
// PI04, AL,  50-MHz, Push-pull	DCMI_D5		AF13
// PI05, AL,  50-MHz, Push-pull	DCMI_VSYNC	AF13
// PI06, AL,  50-MHz, Push-pull	DCMI_D6		AF13
// PI07, AL,  50-MHz, Push-pull	DCMI_D7		AF13
// PI08, OU,  50-MHz, Push-pull	LED7		AF14
// PI09, IN,  50-MHz, Pull-up	-			AF14
// PI10, IN,  50-MHz, Pull-up	-			AF14
// PI11, IN,  50-MHz, Pull-up	-			AF14
// PI12, IN,  50-MHz, Pull-up	-			AF14
// PI13, IN,  50-MHz, Pull-up	-			AF14
// PI14, IN,  50-MHz, Pull-up	-			AF14
// PI15, IN,  50-MHz, Pull-up	-			AF14

	CNFGPIO(I,KIN,KIN,KIN,KIN,KIN,KIN,KIN,KOU,KAL,KAL,KAL,KAL,KAL,KAL,KAL,KAL,
			  K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,K50,
			  KPU,KPU,KPU,KPU,KPU,KPU,KPU,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,KNO,
			  A14,A14,A14,A14,A14,A14,A14,A14,A13,A13,A13,A13,A13,A13,A13,A13,
			  KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,KPP,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0);
}

/*
 * \brief _FSMC_Configuration
 *
 * - FSMC configuration
 *
 */
static	void	_FSMC_Configuration(void) {

    RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN;		// Turn on the FSMCEN

// FSMC bank 1 & CE1 configuration in the asynchronous mode
// - Address/Data non multiplexed
// - FTDI (250-ns)
// - 8 bits
// - Write operations are enabled for the bank by the FSMC (default after reset)
// - Write operations are always performed in asynchronous mode

	FSMC_Bank1->BCR1 = FSMC_BCR_WRAPMOD		// Wrap mode
					 | FSMC_BCR_WREN;		// Enable the write enable
 
	FSMC_Bank1->BTR1 = (0<<28)				// Access mode A
					 | (0<<24)				// DATLAT (do not care)
					 | (0<<20)				// CLKDIV (do not care)
					 | (0<<16)				// BUSTURN of 0 clks
					 | (35<<8)				// DATAST of 35 clks (Data phase duration)
					 | (0<<4)				// ADDHLD (do not care)
					 | (7<<0);				// ADDSET 7 of clks (Address setup)
    
    FSMC_Bank1->BCR1 |= FSMC_BCR_MBKEN;		// Memory bank enable

// FSMC bank 1 & CE3 configuration in the asynchronous mode
// - Address/Data non multiplexed
// - EXTENSION (250-ns)
// - 8 bits
// - Write operations are enabled for the bank by the FSMC (default after reset)
// - Write operations are always performed in asynchronous mode

	FSMC_Bank1->BCR3 = FSMC_BCR_WRAPMOD		// Wrap mode
					 | FSMC_BCR_WREN;		// Enable the write enable
 
	FSMC_Bank1->BTR3 = (0<<28)				// Access mode A
					 | (0<<24)				// DATLAT (do not care)
					 | (0<<20)				// CLKDIV (do not care)
					 | (0<<16)				// BUSTURN of 0 clks
					 | (35<<8)				// DATAST of 35 clks (Data phase duration)
					 | (0<<4)				// ADDHLD (do not care)
					 | (7<<0);				// ADDSET 7 of clks (Address setup)
    
    FSMC_Bank1->BCR3 |= FSMC_BCR_MBKEN;		// Memory bank enable

// FSMC bank 1 & CE2 configuration in the asynchronous mode
// - Address/Data non multiplexed
// - PSRAM (70-ns)
// - 16 bits (default after reset)
// - Write operations are enabled for the bank by the FSMC (default after reset)
// - Write operations are always performed in asynchronous mode

// Activate the RAM array 

	GPIOF->ODR |= (1<<BRAMENABLE);

	FSMC_Bank1->BCR2 = FSMC_BCR_WRAPMOD		// Wrap mode
					 | FSMC_BCR_MTYP_0		// The device is a PSRAM
					 | FSMC_BCR_MWID_0		// Bus width 16-bits
					 | FSMC_BCR_WREN;		// Enable the write enable
 
	FSMC_Bank1->BTR2 = (0<<28)				// Access mode A
					 | (0<<24)				// DATLAT (do not care)
					 | (0<<20)				// CLKDIV (do not care)
					 | (0<<16)				// BUSTURN of 0 clks
					 | (11<<8)				// DATAST of 11 clks (Data phase duration)
					 | (0<<4)				// ADDHLD (do not care)
					 | (1<<0);				// ADDSET 1 of clks (Address setup)

    FSMC_Bank1->BCR2 |= FSMC_BCR_MBKEN;		// Memory bank enable

	_writeConfigPSRAM(KPSRAMBCR, KPSRAMASYNC);
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

// For f(ck in) = 16-MHz
// f(out) = f(vco) / P			f(out) = 168-MHz, P = 2		---> f(vco) = 336-MHz
// f(vco) = f(ck in) * (N/M)	N/M = 336/16 = 21			---> N = 168, M = 8
// f(usb) = f(vco) / Q			Q = 7						---> f(usb) = 48-MHz

	RCC->PLLCFGR = 0x20000000							// Init the register
				 | (7<<24)								// Q = 7
				 | (1<<22)								// HSE as a PLL source
				 | (0<<16)								// P = 2
				 | (168<<6)								// N = 168
				 | (8<<0);								// M = 8
			
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

/*
 * \brief _writeConfigPSRAM
 *
 * - PSRAM configuration
 *
 */
static	void	_writeConfigPSRAM(uint16_t regID, uint16_t regValue) {
				uint16_t	*maxAdd;
	volatile	uint16_t	dummy __attribute__ ((unused));

// Specify max address: 8Mix16b: 0x7FFFFF << 1 = 0xFFFFFE
// Read two times at max address

	maxAdd = (uint16_t *)((uint32_t)&_stEXRAM + 0xFFFFFE);

// Read at the max address
// Read at the max address
// Write register address
// Write configuration register

	dummy   = *maxAdd;		//
	dummy   = *maxAdd;		// Dummy reads
	*maxAdd = regID;		// Write the register ID
	*maxAdd = regValue;		// Write the register value
}

/*
 * \brief _readBusIO
 *
 * - Read from the FMC external bus 
 *   workaround for accessing the bus via a program executed in RAM
 *
 */
void	_readBusIO(uint8_t	offset, uint8_t *data) {
	uint8_t		dummy __attribute__ ((unused));

		*data = *(LOCBUSINT+offset);
		dummy = *(LOCBUSINT);
}

/*
 * \brief _writeBusIO
 *
 * - Write to the FMC external bus 
 *   workaround for accessing the bus via a program executed in RAM
 *
 */
void	_writeBusIO(uint8_t	offset, uint8_t data) {
	uint8_t		dummy __attribute__ ((unused));

		*(LOCBUSINT+offset) = data;
		dummy = *(LOCBUSINT);
}

