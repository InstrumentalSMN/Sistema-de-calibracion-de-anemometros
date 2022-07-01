#include "../inc/da_procesamiento.h"
#include "sapi.h"
//#include "string.h"

#define MaxVoltajeBatery	3.3//12.23
#define MaxADCValue			1024//997

void opProceso(char * TableToFTP, uint32_t * size,real32_t* AcumIntensidad, real32_t* AcumDireccion, real32_t* AcumPresion, real32_t* NvBateria){
	char aux[150];
	char miBuffer1[7];
	char miBuffer2[7];
	char miBuffer3[7];
	char miBuffer4[7];
	TableToFTP = aux;
	real32_t Tabla[16];

	float velocidadInst = AcumIntensidad[1];
	float velocidadMax = maxValue(AcumIntensidad,sizeof(AcumIntensidad)/sizeof(AcumIntensidad[0]));
	float velocidadMin = minValue(AcumIntensidad,sizeof(AcumIntensidad)/sizeof(AcumIntensidad[0]));
	float velocidadPromedio = AverageValue(AcumIntensidad,sizeof(AcumIntensidad)/sizeof(AcumIntensidad[0]));

	float direccionInst = AcumDireccion[1];
	float direccionMax = maxValue(AcumDireccion,sizeof(AcumDireccion)/sizeof(AcumDireccion[0]));
	float direccionMin = minValue(AcumDireccion,sizeof(AcumDireccion)/sizeof(AcumDireccion[0]));
	float direccionPromedio = AverageValue(AcumDireccion,sizeof(AcumDireccion)/sizeof(AcumDireccion[0]));

	float presionInst = AcumPresion[1];
	float presionMax = maxValue(AcumPresion,sizeof(AcumPresion)/sizeof(AcumPresion[0]));
	float presionMin = minValue(AcumPresion,sizeof(AcumPresion)/sizeof(AcumPresion[0]));
	float presionPromedio = AverageValue(AcumPresion,sizeof(AcumPresion)/sizeof(AcumPresion[0]));

	float nv_bateriaInst = NvBateria[1];
	float nv_bateriaMax = maxValue(NvBateria,sizeof(NvBateria)/sizeof(NvBateria[0]));
	float nv_bateriaMin = minValue(NvBateria,sizeof(NvBateria)/sizeof(NvBateria[0]));
	float nv_bateriaPromedio = AverageValue(NvBateria,sizeof(NvBateria)/sizeof(NvBateria[0]));

//	real32_t Tabla[16] ={	velocidadInst,velocidadMax,velocidadMin,velocidadPromedio,
//							direccionInst,direccionMax,direccionMin,direccionPromedio,
//							presionInst,presionMax,presionMax,presionMin,presionPromedio,
//							nv_bateriaInst,nv_bateriaMax,nv_bateriaMin,nv_bateriaPromedio};

//	real32_t Tabla[4] ={	velocidadPromedio,
//							direccionPromedio,
//							presionPromedio,
//							nv_bateriaPromedio};
	floatToString(velocidadInst,miBuffer1,2);
	floatToString(direccionInst,miBuffer2,1);
	floatToString(presionInst,miBuffer3,1);
	floatToString(nv_bateriaInst,miBuffer4,2);


	sprintf(TableToFTP, "%s;%s;%s;%s", miBuffer1,miBuffer2 , miBuffer3,miBuffer4);
	//ver si va strlen
	size = sizeof(TableToFTP);
	uartWriteString( UART_USB, TableToFTP );
	uartWriteString( UART_USB, "\r \n" );
//	size_t size = sizeof(Tabla)/sizeof(Tabla[0]);
//	stringToSentToFTP = converTableToStringToSend(Tabla,size);


}


//const char *converTableToStringToSend(real32_t * Tabla,size_t size){
//	static char stringData[200];
//	static char aux[50];
//	int32_t i = 0;
//	for(i=0;i<size;i++){
//		floatToString(Tabla[i],aux,2);
//		strcat(stringData,aux);
//		strcat(stringData,";");
//	}
//
//	uartWriteString( UART_USB, stringData );
//	uartWriteString( UART_USB, "\r\n" );
//	uartWriteString( UART_USB, "\r\n" );
//	return stringData;
//
//}






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
