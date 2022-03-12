/*
; cmns.
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
; Goal:		Some common routines used in many modules.
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

// Module strings
// ==============

// ----------------------------------I------------I-----------------------------------------I--------------I

LOC_CONST_STRG(aStrApplication[]) =	"cmns         Minimal I/O (not under uKOS).             (c) EFr-2017";
LOC_CONST_STRG(aStrHelp[])        = "Cmns\n"
									"====\n\n"

									"This code provides some minimal I/O.\n\n";

// Module specifications (See the uKOS_module.h)
// =============================================

MODULE(Cmns, KIDSTARTUP, KCMNSNUM, NULL, "1.0", (1<<BSHOW));

/*
 * \brief cmns_init
 *
 * \param[in]	-
 * \param[out]	-
 *
 */
void	cmns_init(void) {

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    USART2->BRR	 = BAUDRATE(KFREQUENCY_APB1, KDEFAULT_BAUDRATE);
	USART2->CR1	 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
	USART2->ISR &= ~USART_ISR_RXNE;
	USART2->ISR &= ~USART_ISR_TXE;

    USART3->BRR	 = BAUDRATE(KFREQUENCY_APB1, KDEFAULT_BAUDRATE);
	USART3->CR1	 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
	USART3->ISR &= ~USART_ISR_RXNE;
	USART3->ISR &= ~USART_ISR_TXE;
}

/*
 * \brief cmns_send
 *
 * \param[in]	commManager	Communication manager
 * \param[out]	*ascii		Ptr on the ascii buffer
 *
 */
void	cmns_send(uint32_t commManager, const char_t *ascii) {
	uint8_t		data;

	switch (commManager) {

// UART 0 device

		default:
		case KURT0: {
			while (TRUE) {
				while (!(USART2->ISR & USART_ISR_TXE));

				data = *ascii++;
				if (data == '\0') return;

				USART2->TDR = (uint16_t)data;
			}
			break;
		}

// UART 1 device

		case KURT1: {
			while (TRUE) {
				while (!(USART3->ISR & USART_ISR_TXE));

				data = *ascii++;
				if (data == '\0') return;

				USART3->TDR = (uint16_t)data;
			}
			break;
		}
	}
}

/*
 * \brief cmns_receive
 *
 * \param[in]	commManager	Communication manager
 * \param[out]	*data		Data received
 *
 */
void	cmns_receive(uint32_t commManager, uint8_t *data) {

	switch (commManager) {

// UART 0 device

		default:
		case KURT0: {
			while (!(USART2->ISR & USART_ISR_RXNE));

			*data = (uint8_t)USART2->RDR;
			break;
		}

// UART 1 device

		case KURT1: {
			while (!(USART3->ISR & USART_ISR_RXNE));

			*data = (uint8_t)USART3->RDR;
			break;
		}
	}
}

/*
 * \brief cmns_wait
 *
 * \param[in]	us		Delay in us
 * \param[out]	-
 *
 */
void	cmns_wait(uint32_t us) {
	volatile	uint32_t	time;

	for (time = 0; time < us; time++) { NOP; }
}

