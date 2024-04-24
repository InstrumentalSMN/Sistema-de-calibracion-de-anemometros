#ifndef _DA_ACQUISITION_H_
#define _DA_ACQUISITION_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"
#include "sapi_uart.h"
#include "common.h"

#define MaxVoltajeBatery	12.43//12.23
#define MaxADCValue			540//997

#define MaxVoltajeAdcTunel	3.552
#define MaxADCValueTunel	1024

#define NoDato  -99999 // No hay dato tambien se usa en procesamiento.c!!!
#define NAN "ND" // tambien se usa en procesamiento.c!!!


typedef enum{HDS500,WMT700,VENTUS}sensor_t;

typedef struct{
	uint16_t Uart;
	uint16_t LED;
	uint32_t BaudRate;
	char Buffer[100];
	char * ptrUartBuffer;
	real32_t DataAnemometer[7];
	real32_t * ptrDataAnemometer;
	sensor_t Sensor;
}amenometerSerialParam_t;


typedef amenometerSerialParam_t * ptrAmenometerSerialParam_t;

/*Array de floats para el DeltaOhm*//*Si hay mas anemometros ver la forma de hacer general esta estructura*/
extern real32_t DataDeltaOhm[];
/*Agrego un array para las muestras instantaneas de datos de viento*/
extern char uartBuffer[100]; //Buffer donde guardamos el string del DeltaOhm
//static char * ptrBuffer=uartBuffer; // un puntero global que apunta a Buffer

/*Region Muestreo */
extern void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n);
/*A futuro esta funcion puede ser un voltaje simple , mas generico, en ese caso puedo pasar el channel*/
extern void opAdquirirDNB(real32_t* muestraVoltNB);
extern void opAdquirirAdcTunel(real32_t* muestraVoltNB);
extern void opBufferRS485Reset(amenometerSerialParam_t * data);
extern void opBufferRS485Off(amenometerSerialParam_t * data);


// Se puede pasar un typedef enum que seleccione un vector de puntero a funciones

extern void opAdquirirDV( void *data );


//Puntero a funcion para selecionar una funcion de procesamiento segun el anemometro conectado
extern void (*opProcesoDatosViento[])(amenometerSerialParam_t * data);/* diccionario de punteros a funcion */



//Funciones para procesar los distintos anemometros
extern void opPreprocesoDeltaOHM(amenometerSerialParam_t * data);
extern void opPreprocesoWMT700(amenometerSerialParam_t * data);
extern void opPreprocesoVentus(amenometerSerialParam_t * data);

extern void opGuardarMuestras(real32_t* muestraVoltNB);





#endif
