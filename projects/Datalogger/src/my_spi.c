/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
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
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 2016-05-02   v0.0.1   ENP   First version
 */

/*==================[inclusions]=============================================*/

#include "../inc/my_spi.h"

#include "chip.h"
#include "wizchip_conf.h"
#include "sapi.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
//#ifdef SAPI_USE_INTERRUPTS
static callBackFuncPtr_t rxIsrCallbackSSP1 = 0;
static void* rxIsrCallbackSSP1Params = NULL;

//static callBackFuncPtr_t txIsrCallbackUART0 = 0;
//static void* txIsrCallbackUART0Params = NULL;
//static callBackFuncPtr_t txIsrCallbackUART2 = 0;
//static void* txIsrCallbackUART2Params = NULL;
//static callBackFuncPtr_t txIsrCallbackUART3 = 0;
//static void* txIsrCallbackUART3Params = NULL;
//#endif /* SAPI_USE_INTERRUPTS */
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions declaration]=========================*/
//#ifdef SAPI_USE_INTERRUPTS
static void sspProcessIRQ(spiMap_t spi );

/*==================[internal functions definition]==========================*/
static void sspProcessIRQ( spiMap_t spi )
{
//   uint8_t status = Chip_UART_ReadLineStatus( lpcUarts[uart].uartAddr );
//   SSP_STATUS_T Stat;
//   Chip_SSP_GetStatus(LPC_SSP1,Stat);
	// Rx Interrupt
	if(spi == SPI0){
		(*rxIsrCallbackSSP1)(rxIsrCallbackSSP1Params);
	}
//
//   if(status & UART_LSR_RDR) { // uartRxReady
//      // Execute callback
//      if( ( spi == SPI0 ) && (rxIsrCallbackUART0 != 0) )
//         (*rxIsrCallbackUART0)(rxIsrCallbackUART0Params); //Cambie esto en teoria ahora puedo enviar argumentos !!
//
//      if( ( uart == UART_USB )  && (rxIsrCallbackUART2 != 0) )
//         (*rxIsrCallbackUART2)(0);
//
//      if( ( uart == UART_232 )  && (rxIsrCallbackUART3 != 0) )
//         (*rxIsrCallbackUART3)(rxIsrCallbackUART3Params);
//   }

//   // Tx Interrupt
//   if( ( status & UART_LSR_THRE ) && // uartTxReady
//       ( Chip_UART_GetIntsEnabled( lpcUarts[uart].uartAddr ) & UART_IER_THREINT ) ) {
//
//      // Execute callback
//      if( ( uart == UART_GPIO ) && (txIsrCallbackUART0 != 0) )
//         (*txIsrCallbackUART0)(0);
//
//      if( ( uart == UART_USB )  && (txIsrCallbackUART2 != 0) )
//         (*txIsrCallbackUART2)(0);
//
//      if( ( uart == UART_232 )  && (txIsrCallbackUART3 != 0) )
//         (*txIsrCallbackUART3)(txIsrCallbackUART3);
//   }
}
//#endif /* SAPI_USE_INTERRUPTS */

/*==================[external functions definition]==========================*/



bool_t MySpi_Wiz_Init( spiMap_t spi )
{

   bool_t retVal = TRUE;

   if( spi == SPI0 ) {

	   reg_wizchip_cs_cbfunc(NULL, NULL);
	   reg_wizchip_spi_cbfunc(NULL, NULL);

      // Configure SPI pins for each board

      #if BOARD==ciaa_nxp||edu_ciaa_nxp
         /* Set up clock and power for SSP1 module */
         // Configure SSP SSP1 pins
         Chip_SCU_PinMuxSet( 0xF, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // SSP1_SCK
         Chip_SCU_PinMuxSet( 0x1, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); // SSP1_MISO
         Chip_SCU_PinMuxSet( 0x1, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC5)); // SSP1_MOSI

         // Initialize SSP Peripheral
         Chip_SSP_Init( LPC_SSP1 );
         Chip_SSP_Enable( LPC_SSP1 );
         Chip_SCU_PinMuxSet( 0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // Pin for SPI SS configured as GPIO output with pull-up
         Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);
         gpioWrite( GPIO0, ON );
      #endif

//      #if BOARD==ciaa_nxp
//         Chip_SCU_PinMuxSet( 0x6, 7, (SCU_MODE_PULLUP | SCU_MODE_FUNC4) ); // Pin for SPI SS configured as GPIO output with pull-up
//         Chip_GPIO_SetPinDIROutput( LPC_GPIO_PORT, 5, 15 );
//      #elif BOARD==edu_ciaa_nxp
//         Chip_SCU_PinMuxSet( 0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // Pin for SPI SS configured as GPIO output with pull-up
//         Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);
//      #elif BOARD==ciaa_z3r0
//         #error CIAA-Z3R0
//      #elif BOARD==pico_ciaa
//         #error PicoCIAA
//      #else
//         #error BOARD compile variable must be defined
//      #endif

   } else {
      retVal = FALSE;
   }

   return retVal;
}

bool_t MySpiInit( spiMap_t spi )
{

   bool_t retVal = TRUE;

   if( spi == SPI0 ) {

      // Configure SPI pins for each board

      #if BOARD==ciaa_nxp||edu_ciaa_nxp
         /* Set up clock and power for SSP1 module */
         // Configure SSP SSP1 pins
         Chip_SCU_PinMuxSet( 0xF, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // SSP1_SCK
         Chip_SCU_PinMuxSet( 0x1, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); // SSP1_MISO
         Chip_SCU_PinMuxSet( 0x1, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC5)); // SSP1_MOSI

         // Initialize SSP Peripheral
         Chip_SSP_Init( LPC_SSP1 );
         Chip_SSP_Enable( LPC_SSP1 );
      #endif

      #if BOARD==ciaa_nxp
         Chip_SCU_PinMuxSet( 0x6, 7, (SCU_MODE_PULLUP | SCU_MODE_FUNC4) ); // Pin for SPI SS configured as GPIO output with pull-up
         Chip_GPIO_SetPinDIROutput( LPC_GPIO_PORT, 5, 15 );
      #elif BOARD==edu_ciaa_nxp
         Chip_SCU_PinMuxSet( 0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // Pin for SPI SS configured as GPIO output with pull-up
         Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);
      #elif BOARD==ciaa_z3r0
         #error CIAA-Z3R0
      #elif BOARD==pico_ciaa
         #error PicoCIAA
      #else
         #error BOARD compile variable must be defined
      #endif

   } else {
      retVal = FALSE;
   }

   return retVal;
}


bool_t MySpiRead( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize )
{
//	uartWriteString( UART_USB, "\r\n" );
//	uartWriteString( UART_USB, "Entre SPI Respuesta Socket\r\n" );
   bool_t retVal = TRUE;

   Chip_SSP_DATA_SETUP_T xferConfig;

   xferConfig.tx_data = NULL;
   xferConfig.tx_cnt  = 0;
   xferConfig.rx_data = buffer;
   xferConfig.rx_cnt  = 0;
   xferConfig.length  = bufferSize;

   if( spi == SPI0 ) {
     Chip_SSP_RWFrames_Blocking( LPC_SSP1, &xferConfig );
//     Chip_SSP_Int_RWFrames8Bits( LPC_SSP1, &xferConfig );
   } else {
      retVal = FALSE;
   }

   return retVal;
}


bool_t MySpiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize)
{

   bool_t retVal = TRUE;

   Chip_SSP_DATA_SETUP_T xferConfig;

   xferConfig.tx_data = buffer;
   xferConfig.tx_cnt  = 0;
   xferConfig.rx_data = NULL;
   xferConfig.rx_cnt  = 0;
   xferConfig.length  = bufferSize;

   if( spi == SPI0 ) {
	   Chip_SSP_RWFrames_Blocking( LPC_SSP1, &xferConfig );
//	   Chip_SSP_Int_RWFrames8Bits( LPC_SSP1, &xferConfig );
   } else {
      retVal = FALSE;
   }

   return retVal;
}


// SPI Global Interrupt Enable/Disable
void spiInterrupt( spiMap_t ssp, bool_t enable )
{
//	las funcones NVIC estan en core_cm4.h
   if( enable ) {
      // Interrupt Priority for SSP channel
//	  SSP1_IRQn esta en cmsis_43xx.h
      NVIC_SetPriority( SSP1_IRQn, 6 ); // FreeRTOS Requiere prioridad >= 5 (numero mas alto, mas baja prioridad)
      // Enable Interrupt for SSP channel
      NVIC_EnableIRQ( SSP1_IRQn );
   } else {
      // Disable Interrupt for UART channel
      NVIC_DisableIRQ( SSP1_IRQn);
   }
}

// SPI Interrupt event Enable and set a callback
void spiCallbackSet( spiMap_t spi, spiEvents_t event, callBackFuncPtr_t callbackFunc, void* callbackParam )
{
   switch(event){

      case SPP_RECEIVE:
         // Enable UART Receiver Buffer Register Interrupt
         //intMask = UART_IER_RBRINT;

         // Enable UART Receiver Buffer Register Interrupt and Enable UART line
         //status interrupt. LPC43xx User manual page 1118
//         intMask = UART_IER_RBRINT | UART_IER_RLSINT;

         if( callbackFunc != 0 ) {
            // Set callback
        	 if( spi == SPI0 ){
               rxIsrCallbackSSP1 = callbackFunc;
               rxIsrCallbackSSP1Params = callbackParam;
            }
         } else{
            return;
         }
      break;

      case SPP_TRANSMITER:
         // Enable THRE irq (TX)
//         intMask = UART_IER_THREINT;

//         if( callbackFunc != 0 ) {
//
//            // Set callback
//            if( (uart == UART_GPIO) || (uart == UART_485) ){
//               txIsrCallbackUART0 = callbackFunc;
//               txIsrCallbackUART0Params = callbackParam;
//            }
//            if( (uart == UART_USB) || (uart == UART_ENET) ){
//               txIsrCallbackUART2 = callbackFunc;
//               txIsrCallbackUART2Params = callbackParam;
//            }
//            if( uart == UART_232 ){
//               txIsrCallbackUART3 = callbackFunc;
//               txIsrCallbackUART3Params = callbackParam;
//            }
//         } else{
//            return;
//         }
      break;

      default:
         return;
   }

   // Enable UART Interrupt
   Chip_SSP_Int_Enable(LPC_SSP1);
}

// SPI Interrupt event Disable
void spiCallbackClr( spiMap_t ssp, spiEvents_t event )
{
//   uint32_t intMask;
//
//   switch(event){
//
//      case UART_RECEIVE:
//         // Enable UART Receiver Buffer Register Interrupt
//         //intMask = UART_IER_RBRINT;
//
//         // Enable UART Receiver Buffer Register Interrupt and Enable UART line
//         //status interrupt. LPC43xx User manual page 1118
//         intMask = UART_IER_RBRINT | UART_IER_RLSINT;
//      break;
//
//      case UART_TRANSMITER_FREE:
//         // Enable THRE irq (TX)
//         intMask = UART_IER_THREINT;
//      break;
//
//      default:
//         return;
//   }
//
//   // Disable UART Interrupt
//   Chip_UART_IntDisable(lpcUarts[uart].uartAddr, intMask);
}

//// UART Set Pending Interrupt. Useful to force first character in tx transmission
//void uartSetPendingInterrupt(uartMap_t uart) {
//   NVIC_SetPendingIRQ(lpcUarts[uart].uartIrqAddr);
//}
//
//// UART Clear Pending Interrupt.
//void uartClearPendingInterrupt(uartMap_t uart) {
//   NVIC_ClearPendingIRQ(lpcUarts[uart].uartIrqAddr);
//}


/*==================[ISR external functions definition]======================*/
//#ifdef SAPI_USE_INTERRUPTS

__attribute__ ((section(".after_vectors")))

// UART0 (GPIO1 y GPIO2 or RS485/Profibus)
// 0x28 0x000000A0 - Handler for ISR UART0 (IRQ 24)
void SSP1_IRQHandler(void)
{
	uint8_t status = Chip_SSP_GetIntStatus(LPC_SSP1);

    if (status & (1 << 0)) { // Verificar si la interrupción es por transmisión (TX)
        // Realizar acciones de transmisión
        printf("\r\n tranmiti algo");
    }
    if (status & (1 << 1)) { // Verificar si la interrupción es por recepción (RX)
    	uint16_t data = Chip_SSP_ReceiveFrame(LPC_SSP1); // Leer dato recibido del registro de datos del SSP1
    	printf("\r\n recibi algo");
    	        // Realizar acciones con el dato recibido
    	        // ...
    }
	sspProcessIRQ( SPI0 );

}

//#endif /* SAPI_USE_INTERRUPTS */


/** @} doxygen end group definition */
/*==================[end of file]============================================*/
