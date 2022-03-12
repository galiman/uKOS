/*
; exce.
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
; Goal:		Exceptions for the LAP_icycam_103 module.
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

LOC_CONST_STRG(aStrApplication[]) =	"exce         Exception management.                     (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Exce\n"
									"====\n\n"

									"This code manages the spurious exceptions.\n\n";

enum	{
		SPP = 0,
		BASEPRI,
		r04, r05, r06, r07, r08, r09, r10, r11, DLR, DR3, r00, r01, r02, r03, r12, LR, PC, xPSR,
		DUMMY,
		ENDREG
		};

#if (defined (__NO_KERNEL__))
					void		(*vExce_indExcVectors[KNBEXCEPTIONS])(void);
					void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
		static		uint32_t	vI, *vPtr, vStack, vStackCoreDump[ENDREG];
		static		char_t		*vException;

#else
					void		(*vExce_indExcVectors[KNBEXCEPTIONS])(void);
					void		(*vExce_indIntVectors[KNBINTERRUPTIONS])(void);
		static		uint32_t	vI, *vPtr, vStack, vStackCoreDump[ENDREG];
		static		char_t		*vException, vString[20], *vNextFree;
extern	volatile	proc_t		*vKern_current;
extern				trace_t		vTraceFifo[KSZFIFO];
#endif

// Prototypes
// ==========

extern	void	pbus_read(uint16_t address, uint8_t *data);
extern	void	pbus_write(uint16_t address, uint8_t data);
extern	void	cmns_send(uint32_t commManager, const char_t *ascii);
extern	void	cmns_wait(uint32_t us);
static	void	_setLEDs(uint8_t ledNb);
static	void	_clrLEDs(uint8_t ledNb);
static	void	_cpyLEDs(uint8_t value);
static	void	_set_BASEPRI(uint32_t priority);
static	void	_displayExeptions(uint8_t exceptionNb);
static	void	_displayInterruptions(uint8_t exceptionNb);
static	void	_printStackFrame(uint32_t *stackCoreDump);
static	void	_printRegister0(uint32_t commDev, char_t *reg, uint32_t *value);
static	void	_printRegister1(uint32_t commDev, char_t *reg, uint32_t *value);
static	int32_t	_cnvtValBin32ToHexAscii(char_t *ascii, const uint32_t *binary, char_t **buffer);

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Exce, KIDSTARTUP, KEXCENUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief exce_init
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	exce_init(void) {
	uint8_t		nbExceptions, nbInterruptions;

	for (nbExceptions = 0; nbExceptions < KNBEXCEPTIONS; nbExceptions++) {
		vExce_indExcVectors[nbExceptions] = (void (*)(void))_displayExeptions;
	}

	for (nbInterruptions = 0; nbInterruptions < KNBINTERRUPTIONS; nbInterruptions++) {
		vExce_indIntVectors[nbInterruptions] = (void (*)(void))_displayInterruptions;
	}

	_set_BASEPRI(KINT_LEVEL_PERIPH<<4);
	SCB->AIRCR = AIRCR_VECTKEY_MASK | 0x0300;
}

// Local routines
// ==============

/*
 * \brief _displayExeptions
 *
 */
static	void	_displayExeptions(uint8_t exceptionNb) __attribute__ ((naked));
static	void	_displayExeptions(uint8_t exceptionNb) {

// Save the registers r4..r11 and the basepri
// r0 contains the stack

	__asm__ volatile (
	"cpsid		i					\n"		//
	"mrs		r0,msp				\n"		// r0 = stack
	"mrs		r1,basepri			\n"		// r1 = basepri
	"stmdb		r0!,{r1,r4-r11}		\n"		// Save the register list
	"stmdb		r0!,{r14}			\n"		// Save the EXCEPTION return
	"msr		msp,r0				\n"		//
	"str		r0,%0				\n"		// Save the stack of the old process
	:
	: "m" (vStack)
	:
	);

	vPtr = (uint32_t *)(vStack);
	for (vI = SPP; vI < ENDREG; vI++) {
		vStackCoreDump[vI] = *vPtr++;
	}

// Print the exception
// -------------------

	cmns_send(KDEF0, "\n\n\n\n");
	cmns_send(KDEF0, "System dead! Core DUMP!!\n");
	cmns_send(KDEF0, "========================\n\n");
	switch (exceptionNb) {
		case  0: { vException = "NMIException";			break; }
		case  1: { vException = "HardFaultException";	break; }
		case  2: { vException = "MemManageException";	break; }
		case  3: { vException = "BusFaultException";	break; }
		case  4: { vException = "UsageFaultException";	break; }
		case  5: { vException = "Reserved 5";			break; }
		case  6: { vException = "Reserved 6";			break; }
		case  7: { vException = "Reserved 7";			break; }
		case  8: { vException = "Reserved 8";			break; }
		case  9: { vException = "SVCHandler";			break; }
		case 10: { vException = "DebugMonitor";			break; }
		case 11: { vException = "Reserved 11";			break; }
		case 12: { vException = "PendSVCHandler";		break; }
		case 13: { vException = "SysTickHandler";		break; }
		default: { vException = "Unknown";				break; }
	}
	cmns_send(KDEF0, vException);
	cmns_send(KDEF0, "\n");

	#if (defined (__NO_KERNEL__))
	cmns_send(KDEF0, "No uKOS micro Kernel\n");

	#else
	cmns_send(KDEF0, vKern_current->oSpecification.oIdentifier);
	cmns_send(KDEF0, "\n\n");

	for (vI = 0; vI < KSZFIFO; vI++) {
		if (vTraceFifo[vI].oIdentifier != NULL) {
			_cnvtValBin32ToHexAscii(vString, &vTraceFifo[vI].oParameter, &vNextFree);

			cmns_send(KDEF0, vTraceFifo[vI].oIdentifier); cmns_send(KDEF0, "  -->  "); cmns_send(KDEF0, vString); cmns_send(KDEF0, "\n");
		}
	}
	cmns_send(KDEF0, "\n");
	#endif

	_printStackFrame(vStackCoreDump);

// Signal an exception by the LED 0

	_cpyLEDs(0xFF);
	while (TRUE) {
		cmns_wait(20000000);
		_setLEDs(0);
		cmns_wait(20000000);
		_clrLEDs(0);
	}
}

/*
 * \brief _displayInterruptions
 *
 */
static	void	_displayInterruptions(uint8_t exceptionNb) __attribute__ ((naked));
static	void	_displayInterruptions(uint8_t exceptionNb) {

// Save the registers r4..r11 and the basepri
// r0 contains the stack

	__asm__ volatile (
	"cpsid		i					\n"		//
	"mrs		r0,msp				\n"		// r0 = stack
	"mrs		r1,basepri			\n"		// r1 = basepri
	"stmdb		r0!,{r1,r4-r11}		\n"		// Save the register list
	"stmdb		r0!,{r14}			\n"		// Save the EXCEPTION return
	"msr		msp,r0				\n"		//
	"str		r0,%0				\n"		// Save the stack of the old process
	:
	: "m" (vStack)
	:
	);

	vPtr = (uint32_t *)(vStack);
	for (vI = SPP; vI < ENDREG; vI++) {
		vStackCoreDump[vI] = *vPtr++;
	}

	cmns_send(KDEF0, "\n\n\n\n");
	cmns_send(KDEF0, "System dead! Core DUMP!!\n");
	cmns_send(KDEF0, "========================\n\n");
	switch (exceptionNb) {
		case  0: { vException = "WWDG_IRQn";			break; }
		case  1: { vException = "PVD_IRQn";				break; }
		case  2: { vException = "TAMPER_IRQn";			break; }
		case  3: { vException = "RTC_IRQn";				break; }
		case  4: { vException = "FLASH_IRQn";			break; }
		case  5: { vException = "RCC_IRQn";				break; }
		case  6: { vException = "EXTI0_IRQn";			break; }
		case  7: { vException = "EXTI1_IRQn";			break; }
		case  8: { vException = "EXTI2_IRQn";			break; }
		case  9: { vException = "EXTI3_IRQn";			break; }
		case 10: { vException = "EXTI4_IRQn";			break; }
		case 11: { vException = "DMA1_Stream1_IRQn";	break; }
		case 12: { vException = "DMA1_Stream2_IRQn";	break; }
		case 13: { vException = "DMA1_Stream3_IRQn";	break; }
		case 14: { vException = "DMA1_Stream4_IRQn";	break; }
		case 15: { vException = "DMA1_Stream5_IRQn";	break; }
		case 16: { vException = "DMA1_Stream6_IRQn";	break; }
		case 17: { vException = "DMA1_Channel7_IRQn";	break; }
		case 18: { vException = "ADC1_2_IRQn";			break; }
		case 19: { vException = "USB_HP_CAN1_TX_IRQn";	break; }
		case 20: { vException = "USB_LP_CAN1_RX0_IRQn";	break; }
		case 21: { vException = "CAN1_RX1_IRQn";		break; }
		case 22: { vException = "CAN1_SCE_IRQn";		break; }
		case 23: { vException = "EXTI9_5_IRQn";			break; }
		case 24: { vException = "TIM1_BRK_IRQn";		break; }
		case 25: { vException = "TIM1_UP_IRQn";			break; }
		case 26: { vException = "TIM1_TRG_COM_IRQn";	break; }
		case 27: { vException = "TIM1_CC_IRQn";			break; }
		case 28: { vException = "TIM2_IRQn";			break; }
		case 29: { vException = "TIM3_IRQn";			break; }
		case 30: { vException = "TIM4_IRQn";			break; }
		case 31: { vException = "I2C1_EV_IRQn";			break; }
		case 32: { vException = "I2C1_ER_IRQn";			break; }
		case 33: { vException = "I2C2_EV_IRQn";			break; }
		case 34: { vException = "I2C2_ER_IRQn";			break; }
		case 35: { vException = "SPI1_IRQn";			break; }
		case 36: { vException = "SPI2_IRQn";			break; }
		case 37: { vException = "USART1_IRQn";			break; }
		case 38: { vException = "USART2_IRQn";			break; }
		case 39: { vException = "USART3_IRQn";			break; }
		case 40: { vException = "EXTI15_10_IRQn";		break; }
		case 41: { vException = "RTCAlarm_IRQn";		break; }
		case 42: { vException = "USBWakeUp_IRQn";		break; }
		case 43: { vException = "TIM8_BRK_IRQn";		break; }
		case 44: { vException = "TIM8_UP_IRQn";			break; }
		case 45: { vException = "TIM8_TRG_COM_IRQn";	break; }
		case 46: { vException = "TIM8_CC_IRQn";			break; }
		case 47: { vException = "ADC3_IRQn";			break; }
		case 48: { vException = "FSMC_IRQn";			break; }
		case 49: { vException = "SDIO_IRQn";			break; }
		case 50: { vException = "TIM5_IRQn";			break; }
		case 51: { vException = "SPI3_IRQn";			break; }
		case 52: { vException = "UART4_IRQn";			break; }
		case 53: { vException = "UART5_IRQn";			break; }
		case 54: { vException = "TIM6_IRQn";			break; }
		case 55: { vException = "TIM7_IRQn";			break; }
		case 56: { vException = "DMA2_Channel1_IRQn";	break; }
		case 57: { vException = "DMA2_Channel2_IRQn";	break; }
		case 58: { vException = "DMA2_Channel3_IRQn";	break; }
		case 59: { vException = "DMA2_Channel4_5_IRQn";	break; }
		default: { vException = "Unknown";				break; }
	}
	cmns_send(KDEF0, vException);
	cmns_send(KDEF0, "\n");

	#if (defined (__NO_KERNEL__))
	cmns_send(KDEF0, "No uKOS micro Kernel\n");

	#else
	cmns_send(KDEF0, vKern_current->oSpecification.oIdentifier);
	cmns_send(KDEF0, "\n\n");

	for (vI = 0; vI < KSZFIFO; vI++) {
		if (vTraceFifo[vI].oIdentifier != NULL) {
			_cnvtValBin32ToHexAscii(vString, &vTraceFifo[vI].oParameter, &vNextFree);

			cmns_send(KDEF0, vTraceFifo[vI].oIdentifier); cmns_send(KDEF0, "  -->  "); cmns_send(KDEF0, vString); cmns_send(KDEF0, "\n");
		}
	}
	cmns_send(KDEF0, "\n");
	#endif

	_printStackFrame(vStackCoreDump);

// Signal an interruption by the LED 1

	_cpyLEDs(0xFF);
	while (TRUE) {
		cmns_wait(20000000);
		_setLEDs(1);
		cmns_wait(20000000);
		_clrLEDs(1);
	}
}

/*
 * \brief _printStackFrame
 *
 * - Print the stack frame of the machine
 *
 *   - Stack frame organization
 *
 *     - xPSR
 *     - PC
 *     - LR
 *     - r12
 *     - r3, r2, r1, r0
 *     - DLR
 *     - DR3  8 bytes due to the "push	{r3, lr}" instruction of the exception handler
 *     - r11, r10, r9, r8, r7, r6, r5, r4
 *     - BASPRI
 *     - SP
 *
 *     - void	Exception_XYZ(void) {
 *     - 140:	b508    push	{r3, lr}
 *     - void	(*go)(uint8_t exceptionNb);
 *
 *     - go = vExce_indExcVectors[XYZ];
 *     - ...
 *     - (*go)(exceptionNb);
 *     - ...
 *
 */
static	void	_printStackFrame(uint32_t *stackCoreDump) {

	cmns_send(KDEF0, "CPU registers\n");
	_printRegister0(KDEF0, "r00      = 0x", &stackCoreDump[r00]); 	_printRegister1(KDEF0, "    r08      = 0x", &stackCoreDump[r08]);
	_printRegister0(KDEF0, "r01      = 0x", &stackCoreDump[r01]);	_printRegister1(KDEF0, "    r09      = 0x", &stackCoreDump[r09]);
	_printRegister0(KDEF0, "r02      = 0x", &stackCoreDump[r02]);	_printRegister1(KDEF0, "    r10      = 0x", &stackCoreDump[r10]);
	_printRegister0(KDEF0, "r03      = 0x", &stackCoreDump[r03]);	_printRegister1(KDEF0, "    r11      = 0x", &stackCoreDump[r11]);
	_printRegister0(KDEF0, "r04      = 0x", &stackCoreDump[r04]);	_printRegister1(KDEF0, "    r12      = 0x", &stackCoreDump[r12]);
	_printRegister0(KDEF0, "r05      = 0x", &stackCoreDump[r05]);	_printRegister1(KDEF0, "    r13 (SP) = 0x", &stackCoreDump[SPP]);
	_printRegister0(KDEF0, "r06      = 0x", &stackCoreDump[r06]);	_printRegister1(KDEF0, "    r14 (LR) = 0x", &stackCoreDump[LR]);
	_printRegister0(KDEF0, "r07      = 0x", &stackCoreDump[r07]);	_printRegister1(KDEF0, "    r15 (PC) = 0x", &stackCoreDump[PC]);
	_printRegister1(KDEF0, "xPSR     = 0x", &stackCoreDump[xPSR]);
	_printRegister1(KDEF0, "BASEPRI  = 0x", &stackCoreDump[BASEPRI]);
}

/*
 * \brief _printRegisterX
 *
 * - x = 0, without CR, x = 1, with CR
 *
 * - Conversion of a 32-bit binary value to a decimal ASCII buffer and print it
 *   - 32-bit input value:       ASCII output buffer:
 *   - (.32) -0x92A				 (ASCII) -0x92ACRLF\0
 *
 */
static	void	_printRegister0(uint32_t commDev, char_t *reg, uint32_t *value) {
	char_t	ascii[25], *wkAscii;

	_cnvtValBin32ToHexAscii(ascii, value, &wkAscii);
	cmns_send(commDev, reg); cmns_send(commDev, ascii);
}

static	void	_printRegister1(uint32_t commDev, char_t *reg, uint32_t *value) {
	char_t	ascii[25], *wkAscii;

	_cnvtValBin32ToHexAscii(ascii, value, &wkAscii);
	cmns_send(commDev, reg); cmns_send(commDev, ascii); cmns_send(commDev, "\n");
}

/*
 * \brief _cnvtValBin32ToHexAscii
 *
 * - 32-bit input value:	ASCII output buffer:
 * - (.32) -0x92A			(ASCII) -0x92A
 *
 */
static	char_t	*_cnvtInt32HASCII(uint32_t binary, char_t *ascii, uint32_t nbDigits) {
			uint32_t	i, number;
	const	char_t		tabHX[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

	for (i = 0; i < nbDigits; i++) {
		number = binary & 0xF;
		binary >>= 4;
		ascii[nbDigits-1-i] = tabHX[number];
	}
	return (ascii+nbDigits);
}

static	int32_t	_cnvtValBin32ToHexAscii(char_t *ascii, const uint32_t *binary, char_t **buffer) {

	*buffer = (_cnvtInt32HASCII(*binary, ascii, 8));
	**buffer = '\0';
	return (KSTRGNOERR);
}

/*
 * \brief _set_BASEPRI
 *
 * - Set the basepri register
 *
 */
static	void	_set_BASEPRI(uint32_t priority) {

	__asm__ volatile ("MSR basepri, %0" : : "r" (priority));
}

/*
 * \brief _setLEDs
 *
 * - Turn on a LED
 *
 */
void	_setLEDs(uint8_t ledNb) {
	uint8_t		led;

	pbus_read(LOCLEDS, &led);
	led |= (1<<ledNb);
	pbus_write(LOCLEDS, led);
}

/*
 * \brief _clrLEDs
 *
 * - Turn off a LED
 *
 */
void	_clrLEDs(uint8_t ledNb) {
	uint8_t		led;

	pbus_read(LOCLEDS, &led);
	led &= ~(1<<ledNb);
	pbus_write(LOCLEDS, led);
}

/*
 * \brief _cpyLEDs
 *
 * - Write on the LEDs
 *
 */
void	_cpyLEDs(uint8_t value) {
	uint8_t		led, mask;

	mask = 0x01;
	for (led = 0; led < 8; led++) {
		(value & mask) ? _setLEDs(led) : _clrLEDs(led);
		mask = mask<<1;
	}
}

