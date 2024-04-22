/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Date: 2016-05-02 */

#ifndef _MY_SPI_H_
#define _MY_SPI_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/
typedef enum{
   SPP_RECEIVE,
   SPP_TRANSMITER
} spiEvents_t;

/*==================[external functions definition]==========================*/

bool_t MySpi_Wiz_Init( spiMap_t spi );

bool_t MySpiInit( spiMap_t spi );

bool_t MySpiRead( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize );

bool_t MySpiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize);


//-------------------------------------------------------------
// Interrupts
//-------------------------------------------------------------

// SPI Global Interrupt Enable/Disable
void spiInterrupt( spiMap_t ssp, bool_t enable );

// SPI Interrupt event Enable and set a callback
void spiCallbackSet( spiMap_t ssp, spiEvents_t event, callBackFuncPtr_t callbackFunc, void* callbackParam );

// SPI Interrupt event Disable
void spiCallbackClr( spiMap_t ssp, spiEvents_t event );

//// UART Set Pending Interrupt. Useful to force first character in tx transmission
//void spiSetPendingInterrupt(uartMap_t uart);
//
//// UART Clear Pending Interrupt.
//void spiClearPendingInterrupt(uartMap_t uart);

/*==================[ISR external functions declaration]======================*/

/* 0x28 0x000000A0 - Handler for ISR SSP1 (IRQ 24) */
void SSP1_IRQHandler(void);


//#endif /* SAPI_USE_INTERRUPTS */





/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif






/*==================[end of file]============================================*/
#endif /* _MY_SPI_H_ */
