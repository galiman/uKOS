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
; Goal:		Low level init for the uKOS LAM_icyM3 module.
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

// FSMC
// ----

#define KBURSTLEN4		0x0001			//
#define KBURSTLEN8		0x0002			//
#define KBURSTLEN16		0x0003			//
#define KBURSTLEN32		0x0004			//
#define KBURSTLENCON	0x0007			//
#define KBURSTLENMASK	0xFFF8			//

#define KBURSTWRAPNO	0x0008			//
#define KBURSTWRAPYES	0x0000			//

#define KDRVSTRGTHFULL	0x0000			//
#define KDRVSTRGTH1_2	0x0010			//
#define KDRVSTRGTH1_4	0x0020			//
#define KDRVSTRGTHMASK	0xFFCF			//

#define KWAITCONFDELAY	0x0000			//
#define KWAITCONFONE	0x0100			//

#define KWAITPOLARLOW	0x0000			//
#define KWAITPOLARHIGH	0x0400			//

#define KLATENCY8		0x0000			//
#define KLATENCY1		0x0800			//
#define KLATENCY2		0x1000			//
#define KLATENCY3		0x1800			//
#define KLATENCY4		0x2000			//
#define KLATENCY5		0x2800			//
#define KLATENCY6		0x3000			//
#define KLATENCY7		0x3800			//
#define KLATENCYMASK	0xC7FF			//

#define KINKLATENCYVAR	0x0000			//
#define KINKLATENCYFIX	0x4000			//

#define KMODESYNC		0x0000			//
#define KMODESAYNC		0x8000			//

#define KPSRAMBCR		1
#define KPSRAMASYNC		(KMODESAYNC | KLATENCY3 | KWAITPOLARHIGH | KWAITCONFONE | KDRVSTRGTH1_2 | KBURSTWRAPNO | KBURSTLENCON)
#define KPSRAMSYNC		(KBURSTLENCON | KBURSTWRAPNO | KDRVSTRGTH1_2 | KWAITCONFONE| KWAITPOLARLOW | KLATENCY3 | KINKLATENCYVAR | KMODESYNC)

// GPIO
// ----

#define	BBGRNT			5		// Pseudo Bus Grant
#define	BBREQ			6		// Pseudo Bus Request

// Configure macro

#define	CNFGPIO(port, \
				a15, a14, a13, a12, a11, a10, a9, a8, a7, a6, a5, a4, a3, a2, a1, a0, \
				b15, b14, b13, b12, b11, b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0) \
				GPIO##port->CRH = 	(a15<<28) | (a14<<24) | (a13<<20) | (a12<<16) |	\
									(a11<<12) | (a10<<8)  | (a9<<4)   | (a8<<0); \
				GPIO##port->CRL = 	(a7<<28)  | (a6<<24)  | (a5<<20)  | (a4<<16)  |	\
									(a3<<12)  | (a2<<8)   | (a1<<4)   | (a0<<0); \
				GPIO##port->ODR =	(b15<<15) | (b14<<14) | (b13<<13) | (b12<<12) |	\
									(b11<<11) | (b10<<10) | (b9<<9)   | (b8<<8)   |	\
									(b7<<7)   | (b6<<6)   | (b5<<5)   | (b4<<4)   |	\
									(b3<<3)   | (b2<<2)   | (b1<<1)   | (b0<<0);

// Prototypes
// ==========

static	void	_GPIO_Configuration(void);
static	void	_FSMC_Configuration(void);
static	void	_RCC_Configuration(void);
static	void	_RTC_Configuration(void);
static	bool_t	_waitForHSEStartUp(void);
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
	_RTC_Configuration();			// RTC Config
}

/*
 * \brief _RCC_Configuration
 *
 * - Clock, PLL configuration
 *
 */
static	void	_RCC_Configuration(void) {
	uint32_t	tmp = 0;

// RCC system reset(for debug purpose)

	RCC->CR   |= RCC_CR_HSION;		// Set HSION bit
	RCC->CFGR &= 0xF8FF0000;		// Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
	RCC->CR   &= 0xFEF6FFFF;		// Reset HSEON, CSSON and PLLON bits
	RCC->CR   &= 0xFFFBFFFF;		// Reset HSEBYP bit
	RCC->CFGR &= 0xFF80FFFF;		// Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
	RCC->CIR   = 0x009F0000;		// Disable all interrupts and clear pending bits

// Enable HSE
  
	RCC->CR &= 0xFFFEFFFF;			// Reset HSEON bit
	RCC->CR &= 0xFFFBFFFF;			// Reset HSEBYP bit
	RCC->CR |= RCC_CR_HSEON;		// Set HSEON bit

// Wait till HSE is ready

	if (_waitForHSEStartUp()) {

// Enable Prefetch Buffer with 2 wait states
  
		FLASH->ACR &= 0xFFFFFFEF;														//
  		FLASH->ACR |= FLASH_ACR_PRFTBE;													// Enable or disable the Prefetch Buffer
		tmp  = FLASH->ACR & (FLASH_ACR_PRFTBS | FLASH_ACR_PRFTBE | FLASH_ACR_HLFCYA);	//
		tmp |= FLASH_ACR_LATENCY_2;														//
		FLASH->ACR = tmp;																// Sets the Latency value

// HCLK = SYSCLK

		tmp  = RCC->CFGR & 0xFFFFFF0F;					//
		tmp |= RCC_CFGR_HPRE_DIV1;						//
		RCC->CFGR = tmp;								// Set HPRE[3:0] bits according to RCC_SYSCLK value

// PCLK2 = HCLK and PCLK1 = HCLK/2

		tmp  = RCC->CFGR & 0xFFFFC7FF;					//
		tmp |= (RCC_CFGR_PPRE1_DIV1<<3);				//
		RCC->CFGR = tmp;								// Set PPRE2[2:0] bits according to RCC_HCLK value
		tmp  = RCC->CFGR & 0xFFFFF8FF;					//
		tmp |= RCC_CFGR_PPRE1_DIV2;						//
		RCC->CFGR = tmp;								// Set PPRE1[2:0] bits according to RCC_HCLK value

// ADCCLK = PCLK2/4

		tmp  = RCC->CFGR & 0xFFFF3FFF;					//
		tmp |= RCC_CFGR_ADCPRE_DIV4;					//
		RCC->CFGR = tmp;								// Set ADCPRE[1:0] bits according to RCC_PCLK2 value

// PLLCLK = 8-MHz * 9 = 72-MHz and enable PLL & Wait till PLL is ready

  		tmp  = RCC->CFGR & 0xFFC0FFFF;					//
  		tmp |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;		//
  		RCC->CFGR = tmp;								// Set the PLL configuration bits
		RCC->CR = RCC->CR | RCC_CR_PLLON;				//
		while (!(RCC->CR & RCC_CR_PLLRDY));				// PLL enabled

// Select PLL as system clock source and wait till PLL is used as system clock source

		tmp  = RCC->CFGR & 0xFFFFFFFC;								//
		tmp |= RCC_CFGR_SW_PLL;										//
		RCC->CFGR = tmp;											// Set SW[1:0] bits according to RCC_SYSCLKSource value
		while ((RCC->CFGR & 0x0000000C) != RCC_CFGR_SWS_1);			//
	}
}

/*
 * \brief _RTC_Configuration
 *
 * - RTC configuration: clk @ 32768-Hz
 *
 */
static	void	_RTC_Configuration(void) {
	uint16_t	cntH, cntL;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;					// Turn on the Power interface

// Enable the acces to the RTC registers

	PWR->CR |= PWR_CR_DBP;								// Enable access to RTC, BDC registers
	RCC->BDCR |= RCC_BDCR_LSEON;						// Enable LSE
	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0);			// Waiting for LSERDY = 1 (LSE is ready)

	RCC->BDCR |= (RCC_BDCR_RTCSEL_0 | RCC_BDCR_RTCEN);	// Set RTC clock source, enable RTC clock
	cntL = RTC->CNTL;
	cntH = RTC->CNTH;
	if ((cntH == 0) && (cntL == 0)) {

// Initialize the clock

		RTC->CRL  |=  RTC_CRL_CNF;						// Set configuration mode
		RTC->PRLH  = (KRTCCLK>>16) & 0x00FF;			// Set prescaler load register high
		RTC->PRLL  =  KRTCCLK;							// Set prescaler load register low
		RTC->CNTH  = (KTIME>>16);						// Set counter high
		RTC->CNTL  =  KTIME;							// Set counter low
		RTC->CRL  &= ~RTC_CRL_CNF;						// Reset configuration mode
		while ((RTC->CRL & RTC_CRL_RTOFF) == 0);		// Wait until write is finished
	}
}

/*
 * \brief _GPIO_Configuration
 *
 * - GPIO configuration
 *
 */
static	void	_GPIO_Configuration(void) {
	uint32_t	tmp;

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		// Turn on the GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;		// Turn on the GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;		// Turn on the GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;		// Turn on the GPIOD
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;		// Turn on the GPIOE
	RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;		// Turn on the GPIOF
	RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;		// Turn on the GPIOG
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;		// Turn on the GPIOO

// Init all the GPIO AD, CONTROL BUS,

// PA00, IN,  Pull-up									input
// PA01, IN,  Analog				Pot 	   ADC0		input
// PA02, AF,  50-MHz, push pull		USART2 	   TX		output
// PA03, IN,  Pull-up				USART2	   RX		input
// PA04, IN,  Pull-up				SPI1	   SS		input
// PA05, AF,  50-MHz, push pull		SPI1	   CK		input
// PA06, IN,  Pull-up				SPI1	   MISO		input
// PA07, AF,  50-MHz, push pull		SPI1	   MOSI		output
// PA08, OUT, 50-MHz, push pull		J8 bus 	   Reset	output
// PA09, AF,  50-MHz, push pull		USART1 	   TX		output
// PA10, IN,  Pull-up				USART1	   RX		input
// PA11, AF,  50-MHz, push pull		USB 	   DM		input
// PA12, AF,  50-MHz, push pull		USB 	   DP		input
// PA13, IN,  Pull-up				JTAG	   JTMS		input
// PA14, IN,  Pull-down				JTAG	   JTCK		input
// PA15, IN,  Pull-up				JTAG	   JTDI		input

	CNFGPIO(A,KPULUPINP,KPULUPINP,KPULUPINP,KPPALT50M,KPPALT50M,KPULUPINP,KPPALT50M,KPPOUT50M,KPPALT50M,KPULUPINP,KPPALT50M,KPULUPINP,KPULUPINP,KPPALT50M,KANALOINP,KPULUPINP,
			  1,        0,        1,        0,        0,        1,        0,        0,        0,        1,        0,        1,        1,        0,        1,        1);

// PB00, OUT, 50-MHz, push pull		pseudo bus CS		output
// PB01, OUT, 50-MHz, push pull		- 					output
// PB02, IN,  Pull-up				BOOT1				input
// PB03, OUT, 50-MHz, push pull     JTAG	   JTDO		output
// PB04, IN,  Pull-up				SPI3	   SS		input
// PB05, OUT, 50-MHz, push pull		pseudo bus BGRNT	output
// PB06, IN,  Pull-up				pseudo bus BREQ		input
// PB07, AF,  50-MHz, push pull		FSMC	   ADV		output
// PB08, IN,  Pull-up				CAN 	   RD		input
// PB09, AF,  50-MHz, push pull		CAN		   TD		output
// PB10, AF,  50-MHz, open drain	I2C		   SCL		output
// PB11, AF,  50-MHz, open drain	I2C		   SDA		output
// PB12, IN,  Pull-up				SPI2	   SS		input
// PB13, AF,  50-MHz, push pull		SPI2	   CK		input
// PB14, IN,  Pull-up				SPI2	   MISO		input
// PB15, AF,  50-MHz, push pull		SPI2	   MOSI		output

	CNFGPIO(B,KPPALT50M,KPULUPINP,KPPALT50M,KPULUPINP,KPPALT50M,KPPALT50M,KPPALT50M,KPULUPINP,KPPALT50M,KPULUPINP,KPPOUT50M,KPULUPINP,KPPOUT50M,KPULUPINP,KPPOUT50M,KPPOUT50M,
			  0,        1,        0,        1,        0,        0,        0,        1,        0,        1,        0,        1,        0,        1,        0,        1);

// PC00, IN,  Floating				pseudo bus D00		input
// PC01, IN,  Floating				pseudo bus D01		input
// PC02, IN,  Floating				pseudo bus D02		input
// PC03, IN,  Floating				pseudo bus D03		input
// PC04, IN,  Floating				pseudo bus D04		input
// PC05, IN,  Floating				pseudo bus D05		input
// PC06, IN,  Floating				pseudo bus D06		input
// PC07, IN,  Floating				pseudo bus D07		input
// PC00, OUT, 50-MHz, push pull		pseudo bus D00		output
// PC01, OUT, 50-MHz, push pull		pseudo bus D01		output
// PC02, OUT, 50-MHz, push pull		pseudo bus D02		output
// PC03, OUT, 50-MHz, push pull		pseudo bus D03		output
// PC04, OUT, 50-MHz, push pull		pseudo bus D04		output
// PC05, OUT, 50-MHz, push pull		pseudo bus D05		output
// PC06, OUT, 50-MHz, push pull		pseudo bus D06		output
// PC07, OUT, 50-MHz, push pull		pseudo bus D07		output
// PC08, AF,  50-MHz, push pull		SDIO 	   D00		output
// PC09, AF,  50-MHz, push pull		SDIO 	   D01		output
// PC10, AF,  50-MHz, push pull		SDIO 	   D02		output
// PC11, AF,  50-MHz, push pull		SDIO 	   D03		output
// PC12, AF,  50-MHz, push pull		SDIO 	   CK		output
// PC13, AF,  50-MHz, push pull		SDIO 	   Tamper	output
// PC14, AF,  50-MHz, push pull		Osc in				input
// PC15, AF,  50-MHz, push pull		Osc out				output

	CNFGPIO(C,KPPALT50M,KFLOAOINP,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KFLOAOINP,KFLOAOINP,KFLOAOINP,KFLOAOINP,KFLOAOINP,KFLOAOINP,KFLOAOINP,KFLOAOINP,
			  0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0);

// PD00, AF,  50-MHz, push pull		FSMC 	   D02		output
// PD01, AF,  50-MHz, push pull		FSMC 	   D03		output
// PD02, AF,  50-MHz, push pull		SDIO 	   CMD		output
// PD03, AF,  50-MHz, push pull		FSMC 	   CLK		output
// PD04, AF,  50-MHz, push pull		FSMC 	   NOE		output
// PD05, AF,  50-MHz, push pull		FSMC 	   NWE		output
// PD06, IN,  Pull-up				FSMC 	   NWAIT	input
// PD07, AF,  50-MHz, push pull		FSMC 	   NCE2		output
// PD08, AF,  50-MHz, push pull		FSMC 	   D13		output
// PD09, AF,  50-MHz, push pull		FSMC 	   D14		output
// PD10, AF,  50-MHz, push pull		FSMC 	   D15		output
// PD11, AF,  50-MHz, push pull		FSMC 	   A16		output
// PD12, AF,  50-MHz, push pull		FSMC 	   A17		output
// PD13, AF,  50-MHz, push pull		FSMC 	   A18		output
// PD14, AF,  50-MHz, push pull		FSMC 	   D00		output
// PD15, AF,  50-MHz, push pull		FSMC 	   D01		output

	CNFGPIO(D,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPULUPINP,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,
			  0,        0,        0,        0,        0,        0,        0,        0,        0,        1,        0,        0,        0,        0,        0,        0);

// PE00, AF,  50-MHz, push pull		FSMC 	   NBL0		output
// PE01, AF,  50-MHz, push pull		FSMC 	   NBL1		output
// PE02, AF,  50-MHz, push pull		FSMC 	   A23		output
// PE03, AF,  50-MHz, push pull		FSMC 	   A19		output
// PE04, AF,  50-MHz, push pull		FSMC 	   A20		output
// PE05, AF,  50-MHz, push pull		FSMC 	   A21		output
// PE06, AF,  50-MHz, push pull		FSMC 	   A22		output
// PE07, AF,  50-MHz, push pull		FSMC 	   D04		output
// PE08, AF,  50-MHz, push pull		FSMC 	   D05		output
// PE09, AF,  50-MHz, push pull		FSMC 	   D06		output
// PE10, AF,  50-MHz, push pull		FSMC 	   D07		output
// PE11, AF,  50-MHz, push pull		FSMC 	   D08		output
// PE12, AF,  50-MHz, push pull		FSMC 	   D09		output
// PE13, AF,  50-MHz, push pull		FSMC 	   D10		output
// PE14, AF,  50-MHz, push pull		FSMC 	   D11		output
// PE15, AF,  50-MHz, push pull		FSMC 	   D12		output

	CNFGPIO(E,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,
			  0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0);

// PF00, AF,  50-MHz, push pull		FSMC 	   A00		output
// PF01, AF,  50-MHz, push pull		FSMC 	   A01		output
// PF02, AF,  50-MHz, push pull		FSMC 	   A02		output
// PF03, AF,  50-MHz, push pull		FSMC 	   A03		output
// PF04, AF,  50-MHz, push pull		FSMC 	   A04		output
// PF05, AF,  50-MHz, push pull		FSMC 	   A05		output
// PF06, IN,  Analog				icycam 	   ADC0		input
// PF07, IN,  Analog				icycam 	   ADC1		input
// PF08, OUT, 50-MHz, push pull		pseudo bus AD0		output
// PF09, OUT, 50-MHz, push pull		pseudo bus AD1		output
// PF10, OUT, 50-MHz, push pull		pseudo bus AD2		output
// PF11, OUT, 50-MHz, push pull		pseudo bus RW		output
// PF12, AF,  50-MHz, push pull		FSMC 	   A06		output
// PF13, AF,  50-MHz, push pull		FSMC 	   A07		output
// PF14, AF,  50-MHz, push pull		FSMC 	   A08		output
// PF15, AF,  50-MHz, push pull		FSMC 	   A09		output

	CNFGPIO(F,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPOUT50M,KPPOUT50M,KPPOUT50M,KPPOUT50M,KANALOINP,KANALOINP,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,
			  0,        0,        0,        0,        1,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0,        0);

// PG00, AF,  50-MHz, push pull		FSMC 	   A10		output
// PG01, AF,  50-MHz, push pull		FSMC 	   A11		output
// PG02, AF,  50-MHz, push pull		FSMC 	   A12		output
// PG03, AF,  50-MHz, push pull		FSMC 	   A13		output
// PG04, AF,  50-MHz, push pull		FSMC 	   A14		output
// PG05, AF,  50-MHz, push pull		FSMC 	   A15		output
// PG06, IN,  Pull-up				FSMC 	   INT2		input
// PG07, IN,  Pull-up				FSMC 	   INT3		input
// PG08, OUT, 50-MHz, push pull		FSMC 	   CRE		output
// PG09, AF,  50-MHz, push pull		FSMC 	   CE3		output
// PG10, OUT, 50-MHz, push pull		J8 bus 	   ET		output
// PG11, IN,  Pull-up				SPI2	   SS2		input
// PG12, IN,  Pull-up				SPI2	   SS1		input
// PG13, AF,  50-MHz, push pull		FSMC 	   A24		output
// PG14, IN,  Pull-up				FSMC 	   INT2		input
// PG15, IN,  Pull-up				FSMC 	   INT0		input

	CNFGPIO(G,KPULUPINP,KPULUPINP,KPPALT50M,KPULUPINP,KPULUPINP,KPPOUT50M,KPPALT50M,KPPOUT50M,KPULUPINP,KPULUPINP,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,KPPALT50M,
			  1,        1,        0,        1,        1,        0,        0,        0,        1,        1,        0,        0,        0,        0,        0,        0);

	tmp = (0x0000000F)<<(0x04 * (BBREQ & 0x03));
	AFIO->EXTICR[BBREQ>>0x02] &= ~tmp;
	AFIO->EXTICR[BBREQ>>0x02] |= ((AFIO_EXTICR1_EXTI0_PB)<<(0x04 * (BBREQ & 0x03)));

	tmp = EXTI_BASE;
	EXTI->IMR &= ~(1<<BBREQ);
	EXTI->EMR &= ~(1<<BBREQ);
	tmp += EXTI_Mode_Interrupt;
	*(volatile uint32_t *)tmp |= (1<<BBREQ);

	EXTI->RTSR &= ~(1<<BBREQ);
	EXTI->FTSR &= ~(1<<BBREQ);
	EXTI->RTSR |=  (1<<BBREQ);
	EXTI->FTSR |=  (1<<BBREQ);
}

/*
 * \brief _FSMC_Configuration
 *
 * - FSMC configuration
 *
 */
static	void	_FSMC_Configuration(void) {

    RCC->AHBENR |= RCC_AHBENR_FSMCEN;			// Turn on the FSMCEN

// 1st - FSMC configuration in the asynchronous mode
//		- Memory bank is disabled
//		- Address/Data non multiplexed
//		- PSRAM (Cellular RAM: CRAM)
//		- 16 bits (default after reset)
//		- NOR Flash memory access is disabled
//		- Burst access mode disabled (default after reset)
//		- NWAIT active low (default after reset)
//		- Direct wrapped burst is not enabled (default after reset)
//		- NWAIT signal is active one data cycle before wait state (default after reset)
//		- Write operations are enabled for the bank by the FSMC (default after reset)
//		- NWAIT signal is disabled
//		- Values inside FSMC_BWTR register are not taken into account (default after reset)
//		- Write operations are always performed in asynchronous mode

	FSMC_Bank1->BCR2 = FSMC_BCR_MTYP_0			// The device is a PSRAM
					 | FSMC_BCR_MWID_0			// Bus width 16-bits
					 | FSMC_BCR_WREN;			// Enable the write enable
 
	FSMC_Bank1->BTR2 = (0<<28)					// Access mode A
					 | (0<<24)					// DATLAT (do not care)
					 | (0<<20)					// CLKDIV (do not care)
					 | (0<<16)					// BUSTURN of 0 clks
					 | (3<<8)					// DATAST of 3 clks
					 | (0<<4)					// ADDHLD (do not care)
					 | (5<<0);					// ADDSET 5 of clks
     
    FSMC_Bank1->BCR2 |= FSMC_BCR_MBKEN;			// Memory bank enable

	_writeConfigPSRAM(KPSRAMBCR, KPSRAMASYNC);

	#if 0
	_writeConfigPSRAM(KPSRAMBCR, KPSRAMSYNC);

// 2nd - FSMC configuration in the synchronous mode
//		- Memory bank is disabled
//		- Address/Data non multiplexed
//		- PSRAM (Cellular RAM: CRAM)
//		- 16 bits (default after reset)
//		- NOR Flash memory access is disabled
//		- Burst access mode enable
//		- NWAIT active low (default after reset)
//		- Direct wrapped burst is not enabled (default after reset)
//		- NWAIT signal is active one data cycle before wait state (default after reset)
//		- Write operations are enabled for the bank by the FSMC (default after reset)
//		- NWAIT signal is enabled
//		- Values inside FSMC_BWTR register are not taken into account (default after reset)
//		- Write operations are performed in synchronous mode

	FSMC_Bank1->BCR2 = FSMC_BCR_MTYP_0			// The device is a PSRAM
					 | FSMC_BCR_MWID_0			// Bus width 16-bits
					 | FSMC_BCR_BURSTEN			// Burst access mode
					 | FSMC_BCR_WAITEN			// Wait enable bit
					 | FSMC_BCR_CBURSTRW		// Write burst enable
					 | FSMC_BCR_WREN;			// Enable the write enable

	FSMC_Bank1->BTR2 = (0<<28)					// Access mode A
					 | (0<<24)					// DATLAT of 2 clks
					 | (1<<20)					// CLKDIV of 2 clks
					 | (0<<16)					// BUSTURN of 0 clks
					 | (3<<8)					// DATAST of 3 clks
					 | (5<<4)					// ADDHLD of 5 clks
					 | (5<<0);					// ADDSET 5 of clks
	#endif
}

// Local routines
// ==============

/*
 * \brief _waitForHSEStartUp
 *
 * - Clock, PLL configuration
 *
 */
static	bool_t	_waitForHSEStartUp(void) {
	volatile	uint32_t	startUpCounter = 0x0500;
  
// Wait till HSE is ready and if Time out is reached exit

	do {
		startUpCounter--;  
	} while ((startUpCounter != 0) && !(RCC->CR & RCC_CR_HSERDY));
  
	if (RCC->CR & RCC_CR_HSERDY) return (TRUE);
	return (FALSE);
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

