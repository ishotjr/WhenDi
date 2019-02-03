/*
 * Copyright (c) 2018 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file port_interrupts.h
 * This is the header file for the Port Interrupts module
 */

#ifndef _PORT_INTERRUPTS_H_
#define _PORT_INTERRUPTS_H_

/*!
 * @addtogroup port_interrupts Port Interrupts
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Enable IRQs on PORTA for sensors with interrupt lines and push button switch SW1
 *
 * @return None
 *
 */
void PORT_IRQ_EnablePortAIrq(void);

/*!
 * @brief Enable IRQs on PORTB for external modules interrupt lines
 *
 * @return None
 *
 */
void PORT_IRQ_EnablePortBIrq(void);

/*!
 * @brief Enable IRQs on PORTC for sensors interrupt lines
 *
 * @return None
 *
 */
void PORT_IRQ_EnablePortCIrq(void);

/*!
 * @brief Enable IRQs on PORTD for sensors interrupt lines
 *
 * @return None
 *
 */
void PORT_IRQ_EnablePortDIrq(void);

/*!
 * @brief Enable IRQs on PORTE for push button switches SW2, SW3, SW4
 *
 * @return None
 *
 */
void PORT_IRQ_EnablePortEIrq(void);

/*!
 * @brief IRQ handler for PORTE for push button switches SW2, SW3, SW4
 *
 * @return None
 *
 */
void PORTE_IRQHandler(void);

/*! @}*/

#if defined(__cplusplus)
}
#endif

#endif /* _PORT_INTERRUPTS_H_ */
