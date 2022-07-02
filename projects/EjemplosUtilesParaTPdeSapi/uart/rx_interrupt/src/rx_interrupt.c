#include "rx_interrupt.h"

void onRx( void *noUsado )
{

	uint8_t receiveByte = uartRxRead( UART_485 );
	//uartWriteByte( UART_USB, receiveByte);
	//uartWriteString( UART_USB, "Entre  \r\n" );
	*ptr = receiveByte;
	ptr++;
	if(receiveByte =='\r'){
		gpioWrite( LED3, ON );
		uartWriteString( UART_USB, uartBuffer);
		ptr=uartBuffer;
		acaproceso
	}
	//cada vez que entra aca guardo el caracter en u string externo y luego proceso ese string externo
//	DataDeltaOhm[0]+=1;
//	DataDeltaOhm[1]+=1;
//	floatToString(DataDeltaOhm[1],miBuffer1,2);
//	uartWriteString( UART_USB, miBuffer1 );
//	uartWriteString( UART_USB, "\r \n" );
//	if(DataDeltaOhm[0] >10){
//		gpioWrite( LED3, ON );
//	}
}
