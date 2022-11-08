#include "../inc/da_rtc.h"//Funciones estadisticas
#include "../inc/da_transmision.h"
#include "sapi_rtc.h"                    // Use RTC peripheral
uint16_t gmsDatetime[100];
volatile bool Uart_Rs232_Mjs_Flag = false;

// Crear estructura RTC
   rtc_t rtc;

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

//int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort)
//+CIPGSMLOC: 0,2022/09/29,14:29:07
//+CIPGSMLOC: 601

bool_t MyParserToDatetimeGsm(char * bufferRtc){

	int i,j;
	char* tok=0;
	tok = strtok(bufferRtc," ");
	if(tok == NULL){return ERROR;}
	printf("1ero strk: %s\r\n", tok);
	tok = strtok(NULL,",");
	if(tok == NULL){return ERROR;}
	printf("2do strk: %s\r\n", tok);
//	Valido con este segundo que puede ser 0 -->ok o 601 --Error
	if((uint8_t)atoi(tok) != FALSE){

		printf("ERROR NO SIGNAL GPRS\r\n");
		return ERROR;

	}
	for (i = 0; i < 2; i++){

		tok = strtok(NULL,"/");
		if(tok == NULL){return ERROR;}
		printf("Fecha: %s\r\n", tok);
		gmsDatetime[i] = (uint16_t)atoi(tok);
	}
	tok = strtok(NULL,",");
	if(tok == NULL){return ERROR;}
	gmsDatetime[i] = (uint16_t)atoi(tok);
	i++;
	printf("Fecha: %s\r\n", tok);

	for (j = 0; j < 2; j++){

		tok = strtok(NULL,":");
		if(tok == NULL){return ERROR;}
		printf("Hora: %s\r\n", tok);
		gmsDatetime[i] = (uint16_t)atoi(tok);
		i++;
	}
	tok = strtok(NULL,"\r\n");
	if(tok == NULL){return ERROR;}
	printf("Hora: %s\r\n", tok);
	gmsDatetime[i] = (uint16_t)atoi(tok);
	return TRUE;

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
//	uartWriteString( UART_232, "AT+SAPBR=2,1");
//	uartWriteString( UART_232, "AT");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	while(Uart_Rs232_Mjs_Flag == FALSE); //Con esto espero que llegue el msj completo sin delays alternos
	printf("Respuesta del GPRS: %s \r\n",bufferRtc);
	//Crear esta funcion!!! MyParserToDatetimeGsm
	if (MyParserToDatetimeGsm(bufferRtc) == ERROR){  //pasar esta funcion a socket.h o daregion.h
							//Despues probar de conectarme a ese port que me da y mandar datos
		printf("No pude obtener la hora por GSM\r\n");
		return ERROR;
	}else{
		//Setear el RTC con los datos parseados
		printf("Si pude obtener la hora por GSM: %s \r\n",bufferRtc);
		uartInterrupt(UART_232, FALSE);
		// Completar estructura RTC
		rtc.year = gmsDatetime[0];
		rtc.month = (uint8_t)gmsDatetime[1];
		rtc.mday = (uint8_t)gmsDatetime[2];
		rtc.wday = 3;
		rtc.hour = (uint8_t)gmsDatetime[3];
		rtc.min = (uint8_t)gmsDatetime[4];
		rtc.sec= (uint8_t)gmsDatetime[5];
		rtcInit();
		rtcWrite( &rtc );
		return OK;
	}
//	//delay(3000);
//	uartWriteString( UART_USB, "Respuesta del sensor\r\n");
////	uartReadByte( UART_232, &dato );
//	// Procesar los datos recepcionados.
//
//	uartWriteString( UART_USB, bufferRtc);
//	uartWriteString( UART_USB, "\r\n");
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




