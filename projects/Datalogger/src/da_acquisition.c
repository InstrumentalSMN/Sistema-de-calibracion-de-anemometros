#include "da_acquisition.h"


// Diccionario de punteros a funcion para procesar en función del DeltaOHM elegido
void (*opProcesoDatosViento[])(amenometerSerialParam_t * data)={
								opPreprocesoDeltaOHM,
								opPreprocesoWMT700,
								opPreprocesoVentus};

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

void opBufferRS485Off(amenometerSerialParam_t * data){

	// Dehabilito todas las interrupciones de UART_USB
		uint16_t miUart = data->Uart;
		uartInterrupt( (UART_GPIO+miUart), false);
}


void opBufferRS485Reset(amenometerSerialParam_t * data){
	data->ptrUartBuffer = data->Buffer;
//	((amenometerSerialParam_t *)data)->Uart = 1;
	uint16_t miUart = data->Uart;
	/*cuando la llamo reinicia la FIFO*/
	//uartConfig( UART_USB, 115200 ); De Prueba
	uartConfig((UART_GPIO+miUart),data->BaudRate );
	// Seteo un callback al evento de recepcion y habilito su interrupcion
	uartCallbackSet( (UART_GPIO+miUart), UART_RECEIVE, opAdquirirDV,(void*)data);
	// Habilito todas las interrupciones de UART_USB
	uartInterrupt( (UART_GPIO+miUart), true);

}

void opAdquirirDV( void *data ){ //Esta se llama  en el callbackSet
	uint16_t miUart = ((amenometerSerialParam_t *)data)->Uart;
	uint16_t miLed = ((amenometerSerialParam_t *)data)->LED;
	uint8_t receiveByte = uartRxRead( (UART_GPIO+miUart));
	*(((amenometerSerialParam_t *)data)->ptrUartBuffer) = receiveByte;
	(((amenometerSerialParam_t *)data)->ptrUartBuffer)++;
	gpioWrite((LEDR+((amenometerSerialParam_t *)data)->LED), OFF );
	if(receiveByte =='\r'){
//		printf("%s",(((amenometerSerialParam_t *)data)->Buffer));
		opProcesoDatosViento[(((amenometerSerialParam_t *)data)->Sensor)]((amenometerSerialParam_t *)data);
		(((amenometerSerialParam_t *)data)->ptrUartBuffer)=(((amenometerSerialParam_t *)data)->Buffer); //Reseteo el puntero para el siguiente string
	gpioWrite( (LEDR+((amenometerSerialParam_t *)data)->LED), ON );
	}

}
// Parser para anemometros
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
//    printf("Parcee los datos del DeltaOHM");

}


void opPreprocesoWMT700(amenometerSerialParam_t * data){
	//Primero parceo lo que hay en buffer $--MWV,021,R,003.34,N,A*14
	int i,j;
	char* tok=0;
	char* rest = data->Buffer;
	tok = strtok(rest,",");
//	if(tok == NULL){return ERROR;}
//	printf("1ero strk: %s\r\n", tok);
	tok = strtok(NULL,",");
//	if(tok == NULL){return ERROR;}
	data->DataAnemometer[1] = (float)atof(tok);
	tok = strtok(NULL,",");
	tok = strtok(NULL,",");
	data->DataAnemometer[0] = (float)atof(tok);
//	printf("Parcee los datos del WMT700");

}
void opPreprocesoVentus(amenometerSerialParam_t * data){
//	Agregar en :
//	char *diccSensoresDeViento[]
//   typedef enum{HDS500,WMT700,VENTUS}sensor_t;
//   *opProcesoDatosViento[])(amenometerSerialParam_t * data)={
//   								opPreprocesoDeltaOHM,
//   								opPreprocesoWMT700};


	printf("\r\n Desarrollar el Parcer para ventus");

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
