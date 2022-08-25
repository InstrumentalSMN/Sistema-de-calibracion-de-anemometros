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

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

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
   } else {
      retVal = FALSE;
   }

   return retVal;
}

//int8_t socket(uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag)
//{
//	CHECK_SOCKNUM();
//	switch(protocol)
//	{
//      case Sn_MR_TCP :
//         {
//            //M20150601 : Fixed the warning - taddr will never be NULL
//		    /*
//            uint8_t taddr[4];
//            getSIPR(taddr);
//            */
//            uint32_t taddr;
//            getSIPR((uint8_t*)&taddr);
//            if(taddr == 0) return SOCKERR_SOCKINIT;
//	    break;
//         }
//      case Sn_MR_UDP :
//      case Sn_MR_MACRAW :
//	  case Sn_MR_IPRAW :
//         break;
//   #if ( _WIZCHIP_ < 5200 )
//      case Sn_MR_PPPoE :
//         break;
//   #endif
//      default :
//         return SOCKERR_SOCKMODE;
//	}
//	//M20150601 : For SF_TCP_ALIGN & W5300
//	//if((flag & 0x06) != 0) return SOCKERR_SOCKFLAG;
//	if((flag & 0x04) != 0) return SOCKERR_SOCKFLAG;
//#if _WIZCHIP_ == 5200
//   if(flag & 0x10) return SOCKERR_SOCKFLAG;
//#endif
//
//	if(flag != 0)
//	{
//   	switch(protocol)
//   	{
//   	   case Sn_MR_TCP:
//   		  //M20150601 :  For SF_TCP_ALIGN & W5300
//          #if _WIZCHIP_ == 5300
//   		     if((flag & (SF_TCP_NODELAY|SF_IO_NONBLOCK|SF_TCP_ALIGN))==0) return SOCKERR_SOCKFLAG;
//          #else
//   		     if((flag & (SF_TCP_NODELAY|SF_IO_NONBLOCK))==0) return SOCKERR_SOCKFLAG;
//          #endif
//
//   	      break;
//   	   case Sn_MR_UDP:
//   	      if(flag & SF_IGMP_VER2)
//   	      {
//   	         if((flag & SF_MULTI_ENABLE)==0) return SOCKERR_SOCKFLAG;
//   	      }
//   	      #if _WIZCHIP_ == 5500
//      	      if(flag & SF_UNI_BLOCK)
//      	      {
//      	         if((flag & SF_MULTI_ENABLE) == 0) return SOCKERR_SOCKFLAG;
//      	      }
//   	      #endif
//   	      break;
//   	   default:
//   	      break;
//   	}
//   }
//	close(sn);
//	//M20150601
//	#if _WIZCHIP_ == 5300
//	   setSn_MR(sn, ((uint16_t)(protocol | (flag & 0xF0))) | (((uint16_t)(flag & 0x02)) << 7) );
//    #else
//	   setSn_MR(sn, (protocol | (flag & 0xF0)));
//    #endif
//	if(!port)
//	{
//	   port = sock_any_port++;
//	   if(sock_any_port == 0xFFF0) sock_any_port = SOCK_ANY_PORT_NUM;
//	}
//   setSn_PORT(sn,port);
//   setSn_CR(sn,Sn_CR_OPEN);
//   while(getSn_CR(sn));
//   //A20150401 : For release the previous sock_io_mode
//   sock_io_mode &= ~(1 <<sn);
//   //
//	sock_io_mode |= ((flag & SF_IO_NONBLOCK) << sn);
//   sock_is_sending &= ~(1<<sn);
//   sock_remained_size[sn] = 0;
//   //M20150601 : repalce 0 with PACK_COMPLETED
//   //sock_pack_info[sn] = 0;
//   sock_pack_info[sn] = PACK_COMPLETED;
//   //
//   while(getSn_SR(sn) == SOCK_CLOSED);
//   return (int8_t)sn;
//}

/*==================[ISR external functions definition]======================*/



/** @} doxygen end group definition */
/*==================[end of file]============================================*/
