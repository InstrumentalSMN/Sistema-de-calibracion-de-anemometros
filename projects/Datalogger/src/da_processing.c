#include "../inc/da_processing.h"
#include "../inc/da_rtc.h"
#include "sapi_rtc.h"

char TableToFTP[10000];
uint32_t next = 0;
//char TableToFTP2[10000];

/*Region Procesamiento */

void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt, amenometerSerialParam_t ibc, amenometerSerialParam_t pat){
	static char auxiliarBuffer[100];
//	printf("muestra numero: %04d",*NumMuestra);
	rtcRead( &rtc );
	printf( "%02d/%02d/%04d, %02d:%02d:%02d\r\n",rtc.mday, rtc.month, rtc.year,rtc.hour, rtc.min, rtc.sec );
	AcumIntensidadPat[*NumMuestra] = pat.DataAnemometer[0];
	AcumDireccionPat[*NumMuestra] = pat.DataAnemometer[1];
	AcumPresionPat[*NumMuestra] = pat.DataAnemometer[2];
	AcumTempPat[*NumMuestra] = pat.DataAnemometer[3];
	AcumIntensidadIBC[*NumMuestra] = ibc.DataAnemometer[0];
	AcumDireccionIBC[*NumMuestra] = ibc.DataAnemometer[1];
	NvBateria[*NumMuestra] = *MuestraVolt;

//	uartWriteString( UART_USB, "Presion-------------------:" );
//	floatToString(AcumDireccion[*NumMuestra],auxiliarBuffer,2);
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );
}


void opProceso(  uint16_t * NumMuestra, int32_t * NumMedicion){

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


	float velocidadInst = AcumIntensidadPat[(*NumMuestra)-1];
	float velocidadMax = maxValue(AcumIntensidadPat,(size_t)*NumMuestra);
	float velocidadMin = minValue(AcumIntensidadPat,(size_t)*NumMuestra);
	float velocidadPromedio = AverageValue(AcumIntensidadPat,(size_t)*NumMuestra);

	float direccionInst = AcumDireccionPat[(*NumMuestra)-1];
	float direccionMax = maxValue(AcumDireccionPat,(size_t)*NumMuestra);
	float direccionMin = minValue(AcumDireccionPat,(size_t)*NumMuestra);
	float direccionPromedio = AverageValue(AcumDireccionPat,(size_t)*NumMuestra);

	float velocidadInstIBC = AcumIntensidadIBC[(*NumMuestra)-1];
	float velocidadMaxIBC = maxValue(AcumIntensidadIBC,(size_t)*NumMuestra);
	float velocidadMinIBC = minValue(AcumIntensidadIBC,(size_t)*NumMuestra);
	float velocidadPromedioIBC = AverageValue(AcumIntensidadIBC,(size_t)*NumMuestra);

	float direccionInstIBC = AcumDireccionIBC[(*NumMuestra)-1];
	float direccionMaxIBC = maxValue(AcumDireccionIBC,(size_t)*NumMuestra);
	float direccionMinIBC = minValue(AcumDireccionIBC,(size_t)*NumMuestra);
	float direccionPromedioIBC = AverageValue(AcumDireccionIBC,(size_t)*NumMuestra);
//Reemplazar esto con datos de un sensor de presion alterno
	float presionInst = AcumPresionPat[(*NumMuestra)-1];
	float presionMax = maxValue(AcumPresionPat,(size_t)*NumMuestra);
	float presionMin = minValue(AcumPresionPat,(size_t)*NumMuestra);
	float presionPromedio = AverageValue(AcumPresionPat,(size_t)*NumMuestra);
//Reemplazar esto con datos de un sensor de temperatura alterno
	float TempInst = AcumTempPat[(*NumMuestra)-1];
	float TempMax = maxValue(AcumTempPat,(size_t)*NumMuestra);
	float TempMin = minValue(AcumTempPat,(size_t)*NumMuestra);
	float TempPromedio = AverageValue(AcumTempPat,(size_t)*NumMuestra);

	float nv_bateriaInst = NvBateria[(*NumMuestra)-1];
	float nv_bateriaMax = maxValue(NvBateria,(size_t)*NumMuestra);
	float nv_bateriaMin = minValue(NvBateria,(size_t)*NumMuestra);
	float nv_bateriaPromedio = AverageValue(NvBateria,(size_t)*NumMuestra);

	real32_t Tabla_Mediciones[16] = {	velocidadInst,velocidadMin,velocidadMax,velocidadPromedio,
							velocidadInstIBC,velocidadMinIBC,velocidadMaxIBC,velocidadPromedioIBC,
							direccionInstIBC,direccionMinIBC,direccionMaxIBC,direccionPromedioIBC,
							direccionInst,direccionMin,direccionMax,direccionPromedio
							};

//	real32_t Tabla[4] ={	velocidadPromedio,
//							direccionPromedio,
//							presionPromedio,
//							nv_bateriaPromedio};



	//Armo el string con los datos medidos
	char * auxi = TableToFTP+next; //Reinicializo el apunte a la posicion 0
	sprintf(auxi, "{\"message\":\"M;%d;", *NumMedicion);
	auxi = auxi + strlen(auxi);
	sprintf( auxi,"%02d-%02d-%04d,%02d:%02d:%02d,",
	              rtc.mday, rtc.month, rtc.year,
	              rtc.hour, rtc.min, rtc.sec);
	printf("%f",strlen(auxi));
	auxi = auxi + strlen(auxi);
	//Armo el string con los datos medidos
	for (i = 0; i<sizeof(Tabla_Mediciones)/sizeof(Tabla_Mediciones[0]);i++){

		if (Tabla_Mediciones[i] == NoDato ){
				sprintf(miBuffer1,"%s","NAN");
			}else{
				floatToString(Tabla_Mediciones[i],miBuffer1,2);

			}
//		printf("\r\ncantidad de bytes %d\r\n",strlen(miBuffer1));
		sprintf(auxi, "%s;\"}", miBuffer1);
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



