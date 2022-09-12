#include "da_sa_prueba.h"
#include "auxiliar.h"



void _opLEDprint(uint16_t LEDNumber,  BOOL_8 State){

	real32_t DataDeltaOhm[] = {NAN,NAN,NAN};
	MisDatas[0] = 1996;
	MisDatas[1] = 1996;
	gpioWrite( (LEDR + LEDNumber), State );
	_opParceo(DataDeltaOhm);

}

void _opParceo(real32_t * DataDeltaOhm){

	static char uartBuff[20];
	DataDeltaOhm[0] =128;
	DataDeltaOhm[1] =192;
	DataDeltaOhm[2] =0;

	uartWriteString( UART_USB, "\r\n Imprimo mis datas pasado por argumento al statechart \r\n" );
	floatToString(DataDeltaOhm[0],uartBuff,2);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(DataDeltaOhm[1],uartBuff,2);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(DataDeltaOhm[2],uartBuff,2);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );
	MisDatas[1] = 1586;

	uartWriteString( UART_USB, "\r\n Imprimo mis datas del .h \r\n" );
	floatToString(MisDatas[0],uartBuff,2);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(MisDatas[1],uartBuff,2);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(MisDatas[2],uartBuff,2);
	uartWriteString( UART_USB, uartBuff );
	uartWriteString( UART_USB, "\r\n" );



}
