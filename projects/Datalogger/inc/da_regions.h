#ifndef _DA_REGIONS_H_
#define _DA_REGIONS_H_

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"
#include "sapi_uart.h"

/*==================[macros]=================================================*/

/* Functional states */
#ifndef ON
#define ON     1
#endif
#ifndef OFF
#define OFF    0
#endif

/* Electrical states */
#ifndef HIGH
#define HIGH   1
#endif
#ifndef LOW
#define LOW    0
#endif

/* Logical states */

////#ifndef FALSE
//#define FALSE  0
////#endif
////#ifndef TRUE
//#define TRUE   (!FALSE)
////#endif

#define NoDato  -99999 // No hay dato
#define NAN "ND"

/*==================[typedef]================================================*/

/*Agrego algún tipo de dato particular para el string de los sensores*/
typedef  int* ptrInt;
typedef char* ptrChar;
typedef float* ptrfFloat;
typedef char* StringData;
typedef uint16_t* ptrUnt16_t;
typedef float* ptrReal32_t;
typedef uint32_t* ptrUint32_t;
typedef int32_t* ptrInt32_t;

/* Define a Char DataType*/
typedef char CHAR;
/* Define Boolean Data Type */
typedef bool booleano_t;

/* Define real Data Types (floating point) */
typedef float  real32_t;
//typedef double real64_t; // In LPC4337 float = double (Floating Point single precision, 32 bits)

/* Define Tick Data Type */
typedef uint64_t tick_t;

/*Nivel de bateria se declara en la yakindu*/

/*Agrego un array para las muestras instantaneas de datos de viento*/
static char uartBuffer[100]; //Buffer donde guardamos el string del DeltaOhm
static char * ptr=uartBuffer;


/*Array de floats para el DeltaOhm*//*Si hay mas anemometros ver la forma de hacer general esta estructura*/
static real32_t DataDeltaOhm[] = {NoDato,NoDato,NoDato,NoDato,NoDato,NoDato,NoDato};



/*Buffer para las estadisticas del DeltaOHM*/
static real32_t AcumIntensidad[20];
static real32_t AcumDireccion[20];
static real32_t AcumPresion[20];
static real32_t AcumTemp[20];
static real32_t NvBateria[20];



/*Tabla de un elemento por ahora para transmitir al FTP*/

static char TableToFTP[40];


/*==================Prototipos para la maquina de Estados=========================*/

/*Region Muestreo */
extern void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n);
/*A futuro esta funcion puede ser un voltaje simple , mas generico, en ese caso puedo pasar el channel*/
extern void opAdquirirDNB(real32_t* muestraVoltNB);
extern void opBufferRS485Reset();
//extern void opAdquirirDV(real32_t* dataWind);
//extern void opPreprocesoDeltaOHM(char* uartBuffer, real32_t* dataWind);
//extern void opGuardarMuestras(real32_t* muestraVoltNB, real32_t* dataWind);
extern void opMuestraDataWind(real32_t* dataWind);

extern void opAdquirirDV2( void *noUsado );
extern void opPreprocesoDeltaOHM2();
extern void opGuardarMuestras2(real32_t* muestraVoltNB);



/*Region Procesamiento */





extern void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt);
extern void opProceso( uint32_t * size);

/*Region de config GPRS and FTP*/


extern bool_t opConfigGPRS();
extern bool_t opConfigFTP();
extern void KeepAlive();

/*Region de Transmision*/
extern bool_t TransmitirFTP(uint32_t * size, int32_t * NumberMesuare);

#endif



