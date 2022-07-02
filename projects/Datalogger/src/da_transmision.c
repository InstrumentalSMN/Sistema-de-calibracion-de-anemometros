#include "../inc/da_transmision.h"
#include "sapi.h"
//#include "string.h"

bool_t opConfigGPRS(){
//	uartConfig( UART_232, 115200 );
//	bool_t Status_t  = ERROR;
//	uint8_t dato = 1;
//	uartWriteString( UART_232, "AT\n");/*Los comandos AT van con \n */
//	delay(300);
////	uartReadByte( UART_232, &dato );
////	uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//		//gpioWrite( LED3, ON );
//		//uartWriteByte( UART_USB, dato);
////		uartWriteString( UART_USB, "Respuesta de AT\r\n");
////		uartWriteByte( UART_USB, dato);
////		uartWriteString( UART_USB, "\r\n");
//		Status_t = ERROR;
//		return ERROR;
//	}
//	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//
//	uartWriteString( UART_232, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n");/*Los comandos AT van con \n */
//	delay(300);
////	uartReadByte( UART_232, &dato );
////	uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//		//gpioWrite( LED3, ON );
//		//uartWriteByte( UART_USB, dato);
////		uartWriteString( UART_USB, "Respuesta de AT\r\n");
////		uartWriteByte( UART_USB, dato);
////		uartWriteString( UART_USB, "\r\n");
//		Status_t = ERROR;
//		return ERROR;
//	}
//	uartWriteString( UART_232, "AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"\n");/*Los comandos AT van con \n */
//	delay(300);
////	uartReadByte( UART_232, &dato );
////	uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//	//gpioWrite( LED3, ON );
//	//uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "Respuesta de AT\r\n");
////	uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "\r\n");
//	Status_t = ERROR;
//	return ERROR;
//	}
//	uartWriteString( UART_232, "AT+SAPBR=1,1\n");/*Los comandos AT van con \n */
//	delay(300);
////	uartReadByte( UART_232, &dato );
////	uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//	//gpioWrite( LED3, ON );
//	//uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "Respuesta de AT\r\n");
////	uartWriteByte( UART_USB, dato);
////	uartWriteString( UART_USB, "\r\n");
//	Status_t = ERROR;
//	return ERROR;
//	}
	return OK;
}


//CheckResponse()

bool_t opConfigFTP(){
	return ERROR;
}


