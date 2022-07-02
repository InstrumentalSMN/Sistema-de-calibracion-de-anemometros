#include "../inc/da_adquisicion.h"
#include "sapi.h"
//#include "string.h"

#define MaxVoltajeBatery	3.3//12.23
#define MaxADCValue			1024//997


// Todas las variables o punteros que se declaran en la maquina de estados se usaran en estas funciones
void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n){

	//Esta función es de la biblioteca sapi.h, ver como esta implementada
	gpioWrite( (LEDR + LEDNumber), State );
	*n +=2;
	if((*n) > 4){
		gpioWrite( LEDB, State );
	}
	if((*n) > 7){
		gpioWrite( LEDR, State );
	}
	if((*n) > 10){
		gpioWrite( LEDG, State );
	}

}

void opBufferRS485Reset(){
	/*cuando la llamo reinicia la FIFO*/
	//uartConfig( UART_USB, 115200 ); De Prueba
	uartConfig( UART_485, 9600 );

}

// Funciones de adquicision de datos
void opAdquirirDNB(real32_t* muestraVoltNB ){//puntero a muestra nivel de bateria

	adcConfig( ADC_ENABLE );
	/* Variable para almacenar el valor leido del ADC CH1 */
	uint16_t muestra = 0;
	*muestraVoltNB = 0;
	muestra = adcRead( CH1 );
	*muestraVoltNB = muestra*(MaxVoltajeBatery/MaxADCValue);
	adcConfig( ADC_DISABLE );
}

//void opAdquirirDv2(real32_t* dataWind){
//
//	/* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
//	uartConfig(UART_USB, 9600);
//	// Seteo un callback al evento de recepcion y habilito su interrupcion
//	uartCallbackSet(UART_USB, UART_RECEIVE, opRX, NULL);
//	// Habilito todas las interrupciones de UART_USB
//	uartInterrupt(UART_USB, true);
//
//}
//
//void opRX( void *NoSeUsa ){
////	char c = uartRxRead( UART_USB );
////	printf( "Recibimos <<%c>> por UART\r\n", c );
//	uint8_t receiveByte='*';  //Inicializo sin dato *
//	static char uartBuffer[100];//deberia declaralo afuera
//	char * ptr;
//	ptr = uartBuffer;
//	while (receiveByte !='\r'){
//		receiveByte = uartRxRead(UART_485);
//
//		//uartReadByte(UART_485, &receiveByte);
//		//uartWriteByte( UART_USB, receiveByte);
//		//Verficar si el protocolo que tiene mi DeltaOhm termina con /n
//		//Si hay datos sin leer en la FIFO, los leo
//		*ptr = receiveByte;
//		ptr++;
//	}
//	//uartWriteString( UART_USB, uartBuffer );
//	if(receiveByte == '\r' ){
//		//uartWriteString( UART_USB, uartBuffer );
//		// En esta funcion tengo que validad que sea un string con Datos,
//		// si no hay datos entonce pongo un valor numerico por defecto que en el FTP
//		// se reemplace con un * o un NAN (Un valor numerico que no puede aparecer en el rango de los datos)
//		// tipo -99999
//		gpioWrite( LED1, ON );
//		opPreprocesoDeltaOHM(uartBuffer,dataWind);/*acá parseo el String de datos*/
//	}
//}



//Ver la forma de que esta sea generica
void opAdquirirDV(real32_t* dataWind){

	//bool_t sensorPresent = TRUE;

	//delay(1000);

	uint8_t receiveByte='*';  //Inicializo sin dato *
	static char uartBuffer[100];
	char * ptr;
	ptr = uartBuffer;

	//while (receiveByte !='g' || sensorPresent == TRUE)
	if(uartReadByte(UART_485, &receiveByte)==TRUE){
		gpioWrite( LED1, OFF );
		*ptr = receiveByte;
		ptr++;
		while (receiveByte !='\r'){
			uartReadByte(UART_485, &receiveByte);

			//uartReadByte(UART_485, &receiveByte);
			uartWriteByte( UART_USB, receiveByte);//Si o si para que funcione
			//Verficar si el protocolo que tiene mi DeltaOhm termina con /n
			//Si hay datos sin leer en la FIFO, los leo
			*ptr = receiveByte;
			ptr++;
		}
	}
	//uartWriteString( UART_USB, uartBuffer );
	if(receiveByte == '\r' ){
		//uartWriteString( UART_USB, uartBuffer );
		// En esta funcion tengo que validad que sea un string con Datos,
		// si no hay datos entonce pongo un valor numerico por defecto que en el FTP
		// se reemplace con un * o un NAN (Un valor numerico que no puede aparecer en el rango de los datos)
		// tipo -99999
		gpioWrite( LED1, ON );
		opPreprocesoDeltaOHM(uartBuffer,dataWind);/*acá parseo el String de datos*/
	}


	//Sino Salgo y la estructura que entro no se modifica
}


void opPreprocesoDeltaOHM( char* uartBuffer, real32_t* dataWind){
	//Primero parceo lo que hay en buffer (28.30 359.3 998.3<CR><LF>)
	char *	delimitador = " ";
	char* token;
	char* rest = uartBuffer;
    uint16_t i = 0;
    //Obtengo el largo del vector donde se guardan mis datos de interes en este caso 3. Intensidad, Direccion y Presion
    //uint32_t lenghtDeltaOhm = sizeof(dataWind)/sizeof(dataWind[0]);
    while ((token =  (char *)strtok_r(rest, delimitador, &rest)) !=NULL ){
    	//token es un string asi que puedo comparar contra NAN y no covertir
    	if((memcmp(NAN,token,strlen(NAN))) == 0){
    		dataWind[i] = NoDato;
    		i++;

    	}else{
    		dataWind[i] = (float)atof(token); //Convierto los datos parceados a numeros flotantes
    		i++;

    	}
//		uartWriteString( UART_USB, token );
//		uartWriteString( UART_USB, "\r\n" );
    }

}

void opMuestraDataWind(real32_t* dataWind){


}

void opGuardarMuestras(real32_t* muestraVoltNB, real32_t* dataWind){

	static char miBuffer[100];
	int i;
	int n=7;
	gpioWrite( LEDB, ON );
//int n = sizeof(dataWind) / sizeof(real32_t);
//	for(i = 0; i < n; i++){
//		floatToString(dataWind[i],miBuffer,2);
//		uartWriteString( UART_USB, miBuffer );
//		uartWriteString( UART_USB, "\r\n" );
//	}
//	floatToString(*muestraVoltNB,miBuffer,2);
//	uartWriteString( UART_USB, miBuffer );
//	uartWriteString( UART_USB, "\r\n" );

}

