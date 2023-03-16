#include "da_acquisition.h"

//real32_t DataDeltaOhm[] = {NoDato,NoDato,NoDato,NoDato,NoDato,NoDato,NoDato};
//char uartBuffer[100];
//char * ptrBuffer=uartBuffer;

/*Region Muestreo */

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


void opBufferRS485Reset(amenometerSerialParam_t * data){
	data->ptrUartBuffer = data->Buffer;
//	((amenometerSerialParam_t *)data)->Uart = 1;
	uint16_t miUart = data->Uart;
//	uint16_t miLed = ((amenometerSerialParam_t *)data)->LED;
	/*cuando la llamo reinicia la FIFO*/
	//uartConfig( UART_USB, 115200 ); De Prueba
	uartConfig((UART_GPIO+miUart),data->BaudRate );
	// Seteo un callback al evento de recepcion y habilito su interrupcion
	uartCallbackSet( (UART_GPIO+miUart), UART_RECEIVE, opAdquirirDV,(void*)data);
	// Habilito todas las interrupciones de UART_USB
	uartInterrupt( (UART_GPIO+miUart), true);

}

void opAdquirirDV( void *data ){ //Esta se llama  en el callbackSet
//	((amenometerSerialParam_t *)data)->Uart = 5;
	uint16_t miUart = ((amenometerSerialParam_t *)data)->Uart;
	uint16_t miLed = ((amenometerSerialParam_t *)data)->LED;
//	if(miUart==1461){
//		miUart=5;
//	}

//	uartMap_t miUart=((uartMapAnemo2_t *)noUsado)->Uart;
	uint8_t receiveByte = uartRxRead( (UART_GPIO+miUart));
	//uartWriteByte( UART_USB, receiveByte);
	//uartWriteString( UART_USB, "Entre  \r\n" );
	*(((amenometerSerialParam_t *)data)->ptrUartBuffer) = receiveByte;
	(((amenometerSerialParam_t *)data)->ptrUartBuffer)++;
	gpioWrite((LEDR+((amenometerSerialParam_t *)data)->LED), OFF );
//	gpioWrite(LEDR, OFF );
	if(receiveByte =='\r'){
//		uartWriteString( UART_USB, uartBuffer);

		opPreprocesoDeltaOHM((amenometerSerialParam_t *)data);/*acá parseo el String de datos*/
		(((amenometerSerialParam_t *)data)->ptrUartBuffer)=(((amenometerSerialParam_t *)data)->Buffer); //Reseteo el puntero para el siguiente string
	gpioWrite( (LEDR+((amenometerSerialParam_t *)data)->LED), ON );
//	gpioWrite( LEDR, ON );
	}

}
void opPreprocesoDeltaOHM(amenometerSerialParam_t * data){
	//Primero parceo lo que hay en buffer (28.30 359.3 998.3<CR><LF>)
	char *	delimitador = " ";
	char* token;
	char* rest = data->Buffer;
    uint16_t i = 0;
    static char auxiliarBuffer[100];
    //Obtengo el largo del vector donde se guardan mis datos de interes en este caso 3. Intensidad, Direccion y Presion
    //uint32_t lenghtDeltaOhm = sizeof(dataWind)/sizeof(dataWind[0]);
    while ((token =  (char *)strtok_r(rest, delimitador, &rest)) !=NULL ){
    	//token es un string asi que puedo comparar contra NAN y no covertir
    	if((memcmp(NAN,token,strlen(NAN))) == 0){
    		data->DataAnemometer[i] = NoDato;
    		i++;
    	}else{
    		data->DataAnemometer[i] = (float)atof(token); //Convierto los datos parceados a numeros flotantes
    		i++;

    	}
//    	floatToString(DataDeltaOhm[i-1],auxiliarBuffer,2);
//		uartWriteString( UART_USB, auxiliarBuffer );
//		uartWriteString( UART_USB, "\r\n" );
    }

}

void opGuardarMuestras(real32_t* muestraVoltNB){

	static char auxiliarBuffer[100];
	int i;
	int n=7;
	//gpioToggle( LED1 );

	//gpioWrite( LEDB, ON );
//int n = sizeof(dataWind) / sizeof(real32_t);
//	for(i = 0; i < n; i++){
//		floatToString(DataDeltaOhm[i],auxiliarBuffer,2);
//		uartWriteString( UART_USB, auxiliarBuffer );
//		uartWriteString( UART_USB, "\r\n" );
//	}
//	floatToString(*muestraVoltNB,auxiliarBuffer,2);
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );

}
