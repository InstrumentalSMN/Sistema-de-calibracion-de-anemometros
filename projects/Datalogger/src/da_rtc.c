#include "../inc/da_rtc.h"//Funciones estadisticas
#include "../inc/da_transmision.h"

uint16_t gmsDatetime[10];
volatile bool Uart_Rs232_Mjs_Flag = false;

void opReadUart(void *noUsado ){

	uint8_t aux = uartRxRead( UART_232 );
	*ptrBufferRtc = aux;
	ptrBufferRtc++;
	gpioWrite( LEDR, OFF );
	if(aux =='\n'){
//	uartWriteString( UART_USB, bufferRtc);
	//	opPreprocesoDeltaOHM();/*acá parseo el String de datos*/
		ptrBufferRtc = bufferRtc; //Reseteo el puntero para el siguiente string
		uartConfig( UART_232, 115200 ); // Limpio mensaje restante en la siguiente linea
		Uart_Rs232_Mjs_Flag = TRUE; // Activo flag que permite validar el mensaje recibido
//	gpioWrite( LEDR, ON );

	}

}

bool_t opSetRtcViaGsm(){


	uartConfig( UART_232, 115200 );
	// Seteo un callback al evento de recepcion y habilito su interrupcion
	uartCallbackSet(UART_232, UART_RECEIVE, opReadUart, NULL);
	// Habilito todas las interrupciones de UART_USB
	uartInterrupt(UART_232, true);


	uartWriteString( UART_USB, "Solicito via GSM la hora\r\n");
//	uartConfig( UART_232, 115200 );
//	uartWriteString( UART_232, "AT+FTPPUT=2,0");
//	uartConfig( UART_232, 115200 );
	uint8_t dato = 1;
	uartWriteString( UART_232, "AT+CIPGSMLOC=2,1");
//	uartWriteString( UART_232, "AT");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	while(Uart_Rs232_Mjs_Flag == FALSE); //Con esto espero que llegue el msj completo sin delays alternos
	if (MyParserToDatetimeGsm(dbuf,remoteIp,&remotePort) == -1){  //pasar esta funcion a socket.h o daregion.h
							//Despues probar de conectarme a ese port que me da y mandar datos
		printf("Bad port syntax\r\n");
		return ERROR;
	}else{
		//Setear el RTC con los datos parseados


	}
	//delay(3000);
	uartWriteString( UART_USB, "Respuesta del sensor\r\n");
//	uartReadByte( UART_232, &dato );
	// Procesar los datos recepcionados.

	uartWriteString( UART_USB, bufferRtc);
	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//		//gpioWrite( LED3, ON );
//		uartWriteString( UART_USB, "Respuesta de AT\r\n");
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//		//uartWriteString( UART_232, "AT+SAPBR=0,1");
//		resetGRPS();
//		return ERROR;
//	}
//	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//
//
//	uartWriteString( UART_232, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"");/*Los comandos AT van con \n */
//	uartWriteString( UART_232, "\r\n");
//	delay(300);
//	uartReadByte( UART_232, &dato );
//	uartWriteByte( UART_USB, dato);
//	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//		gpioWrite( LED3, ON );
//		//uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "Respuesta de AT\r\n");
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//		resetGRPS();
//		return ERROR;
//	}
//

	uartInterrupt(UART_232, FALSE);
	return OK;
}


bool_t opConfigNtpSocket(){

	return ERROR;
}

bool_t opConfigNTP(){

	return ERROR;
}

bool_t opSetRtcViaNtp(){

	return ERROR;

}

void opSetRtcDefault(){

}




