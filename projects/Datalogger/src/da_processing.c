#include "../inc/da_processing.h"
#include "../inc/da_acquisition.h"
#include "../inc/da_rtc.h"
#include "sapi_rtc.h"

char TableToFTP[10000];
uint32_t next = 0;
//char TableToFTP2[10000];

/*Region Procesamiento */

void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt ){
	static char auxiliarBuffer[100];
//	printf("muestra numero: %04d",*NumMuestra);
	rtcRead( &rtc );
	printf( "%02d/%02d/%04d, %02d:%02d:%02d\r\n",rtc.mday, rtc.month, rtc.year,rtc.hour, rtc.min, rtc.sec );
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


void opProceso(  uint16_t * NumMuestra){

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



	//Armo el string con los datos medidos
	char * auxi = TableToFTP+next; //Reinicializo el apunte a la posicion 0
	rtcRead( &rtc );
	sprintf( auxi,"%02d-%02d-%04d,%02d:%02d:%02d,",
	              rtc.mday, rtc.month, rtc.year,
	              rtc.hour, rtc.min, rtc.sec);
	printf("%f",strlen(auxi));
	auxi = auxi + strlen(auxi);
	//Armo el string con los datos medidos
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

// agregar alguna validacion en caso de que aca size de tabla sea cero.
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



