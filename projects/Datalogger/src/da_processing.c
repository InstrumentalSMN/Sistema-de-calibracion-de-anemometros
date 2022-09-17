#include "../inc/da_processing.h"
#include "../inc/da_acquisition.h"

char TableToFTP[150];
//char TableToFTP2[10000];

/*Region Procesamiento */

void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt ){
	static char auxiliarBuffer[100];
	AcumIntensidad[*NumMuestra] = DataDeltaOhm[0];
	AcumDireccion[*NumMuestra] = DataDeltaOhm[1];
	AcumPresion[*NumMuestra] = DataDeltaOhm[2];
	AcumTemp[*NumMuestra] = DataDeltaOhm[3];
	NvBateria[*NumMuestra] = *MuestraVolt;
//	uartWriteString( UART_USB, "Presion-------------------:" );
//	floatToString(AcumDireccion[*NumMuestra],auxiliarBuffer,2);
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );
}


void opProceso( uint32_t * size, uint16_t * NumMuestra){

	uint32_t i = 0;
	//char aux[150];
	char miBuffer1[7];
	char miBuffer2[7];
	char miBuffer3[7];
	char miBuffer4[7];
	char miBuffer5[7];
	//TableToFTP = aux;
	//real32_t Tabla[16];

//	uartWriteString( UART_USB, "Numero de muestras-------------------:" );
//	floatToString(*NumMuestra,miBuffer1,0);
//	uartWriteString( UART_USB, miBuffer1 );
//	uartWriteString( UART_USB, "\r\n" );


	float velocidadInst = AcumIntensidad[1];
	float velocidadMax = maxValue(AcumIntensidad,(size_t)*NumMuestra);
	float velocidadMin = minValue(AcumIntensidad,(size_t)*NumMuestra);
	float velocidadPromedio = AverageValue(AcumIntensidad,(size_t)*NumMuestra);

	float direccionInst = AcumDireccion[1];
	float direccionMax = maxValue(AcumDireccion,(size_t)*NumMuestra);
	float direccionMin = minValue(AcumDireccion,(size_t)*NumMuestra);
	float direccionPromedio = AverageValue(AcumDireccion,(size_t)*NumMuestra);

	float presionInst = AcumPresion[1];
	float presionMax = maxValue(AcumPresion,(size_t)*NumMuestra);
	float presionMin = minValue(AcumPresion,(size_t)*NumMuestra);
	float presionPromedio = AverageValue(AcumPresion,(size_t)*NumMuestra);

	float TempInst = AcumTemp[1];
	float TempMax = maxValue(AcumTemp,(size_t)*NumMuestra);
	float TempMin = minValue(AcumTemp,(size_t)*NumMuestra);
	float TempPromedio = AverageValue(AcumTemp,(size_t)*NumMuestra);

	float nv_bateriaInst = NvBateria[1];
	float nv_bateriaMax = maxValue(NvBateria,(size_t)*NumMuestra);
	float nv_bateriaMin = minValue(NvBateria,(size_t)*NumMuestra);
	float nv_bateriaPromedio = AverageValue(NvBateria,(size_t)*NumMuestra);

	real32_t Tabla_10min[20] ={	velocidadInst,velocidadMin,velocidadMax,velocidadPromedio,
							direccionInst,direccionMin,direccionMax,direccionPromedio,
							presionInst,presionMin,presionMax,presionPromedio,
							TempInst,TempMin,TempMax,TempPromedio,
							nv_bateriaInst,nv_bateriaMin,nv_bateriaMax,nv_bateriaPromedio};

//	real32_t Tabla[4] ={	velocidadPromedio,
//							direccionPromedio,
//							presionPromedio,
//							nv_bateriaPromedio};
	char * auxi = TableToFTP; //Reinicializo el apunte a la posicion 0
	for (i = 0; i<sizeof(Tabla_10min)/sizeof(Tabla_10min[0]);i++){

		if (Tabla_10min[i] == NoDato ){
				sprintf(miBuffer1,"%s","NAN");
			}else{
				floatToString(Tabla_10min[i],miBuffer1,2);

			}
//		printf("\r\ncantidad de bytes %d\r\n",strlen(miBuffer1));
		sprintf(auxi, "%s;", miBuffer1);
		auxi = auxi + strlen(miBuffer1)+1;//hacia falta el + 1 por \0

	}


//	if (velocidadInst == NoDato ){
//		sprintf(miBuffer1,"%s","NAN");
//	}else{
//		floatToString(velocidadInst,miBuffer1,2);
//	}
//	if (direccionInst == NoDato ){
//		sprintf(miBuffer2,"%s","NAN");
//	}else{
//		floatToString(direccionInst,miBuffer2,1);
//	}
//	if (presionInst == NoDato ){
//		sprintf(miBuffer3,"%s","NAN");
//	}else{
//		floatToString(presionInst,miBuffer3,1);
//	}
//	if (TempInst == NoDato ){
//		sprintf(miBuffer4,"%s","NAN");
//	}else{
//		floatToString(TempInst,miBuffer4,2);
//	}
//	if (nv_bateriaInst == NoDato ){
//		sprintf(miBuffer5,"%s","NAN");
//	}else{
//		floatToString(nv_bateriaInst,miBuffer5,2);
//	}
//
//	sprintf(TableToFTP, "%s;%s;%s;%s;%s", miBuffer1,miBuffer2 , miBuffer3,miBuffer4,miBuffer5);
	//ver si va strlen
	*size = (uint32_t)strlen(TableToFTP);
	uartWriteString( UART_USB, "\r \n Mi tabla:-------\r\n" );
	uartWriteString( UART_USB, TableToFTP );
	uartWriteString( UART_USB, "\r \n" );
//	gpioWrite( LED3, ON );
//	size_t size = sizeof(Tabla)/sizeof(Tabla[0]);
//	stringToSentToFTP = converTableToStringToSend(Tabla,size);


}






/*Funciones para Estadisticas*/

real32_t maxValue(real32_t myArray[], size_t size) {
    /* enforce the contract */
    //assert(myArray && size);
    size_t i;
    real32_t maxValue = myArray[0];

    for (i = 1; i < size; ++i) {
        if ( myArray[i] > maxValue ) {
            maxValue = myArray[i];
        }
    }
    return maxValue;
}


real32_t minValue(real32_t myArray[], size_t size) {
    /* enforce the contract */
    //assert(myArray && size);
    size_t i;
    real32_t minValue = myArray[0];

    for (i = 1; i < size; ++i) {
        if ( myArray[i] < minValue ) {
        	minValue = myArray[i];
        }
    }
    return minValue;
}


real32_t AverageValue(real32_t myArray[], size_t size){

	real32_t sum,avg;
	int i;
	sum = avg = 0;
	for(i = 0; i < size; i++) {
	   sum = sum + myArray[i];
	}
	avg = (float)sum / i;
	return avg;

}



