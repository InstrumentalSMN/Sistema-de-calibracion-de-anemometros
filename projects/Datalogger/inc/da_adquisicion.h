#ifndef _DA_ADQUISICION_H_
#define _DA_ADQUISICION_H_

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"

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

/* Define a Char DataType*/
typedef char CHAR;
/* Define Boolean Data Type */
typedef bool booleano_t;


/* Define real Data Types (floating point) */
typedef float  real32_t;
//typedef double real64_t; // In LPC4337 float = double (Floating Point single precision, 32 bits)

/* Define Tick Data Type */
typedef uint64_t tick_t;

/*Agrego un array para datos de viento*/

static real32_t DataDeltaOhm[] = {NoDato,NoDato,NoDato,NoDato,NoDato,NoDato,NoDato};

/*Si hay mas anemometros ver la forma de hacer general esta estructura*/

/* type of Pointers */



/*==================Prototipos para la maquina de Estados=========================*/

/*Region Muestreo */
extern void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n);
/*A futuro esta funcion puede ser un voltaje simple , mas generico, en ese caso puedo pasar el channel*/
extern void opAdquirirDNB(real32_t* muestraVoltNB);
extern void opBufferRS485Reset();
extern void opAdquirirDV(real32_t* dataWind);
extern void opPreprocesoDeltaOHM(char* uartBuffer, real32_t* dataWind);
extern void opGuardarMuestras(real32_t* muestraVoltNB, real32_t* dataWind);
extern void opMuestraDataWind(real32_t* dataWind);
/*
 * Function Pointer definition
 * --------------------------------------
 * param:  void * - For passing arguments
 * return: bool_t - For Error Reports
 */
typedef bool_t (*sAPI_FuncPtr_t)(void *);

/*
 * Function Pointer definition
 * --------------------------------------
 * param:  void
 * return: void
 */
typedef void (*callBackFuncPtr_t)(void *);

/*==================[external functions declaration]=========================*/

/*
 * Null Function Pointer definition
 * --------------------------------------
 * param:  void * - Not used
 * return: bool_t - Return always true
 */
bool_t sAPI_NullFuncPtr(void *);

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_DATATYPES_H_*/

///** LPC type for character type */
//typedef char CHAR;
//
///** LPC type for 8 bit unsigned value */
//typedef uint8_t UNS_8;
//
///** LPC type for 8 bit signed value */
//typedef int8_t INT_8;
//
///** LPC type for 16 bit unsigned value */
//typedef uint16_t UNS_16;
//
///** LPC type for 16 bit signed value */
//typedef int16_t INT_16;
//
///** LPC type for 32 bit unsigned value */
//typedef uint32_t UNS_32;
//
///** LPC type for 32 bit signed value */
//typedef int32_t INT_32;
//
///** LPC type for 64 bit signed value */
//typedef int64_t INT_64;
//
///** LPC type for 64 bit unsigned value */
//typedef uint64_t UNS_64;
//
//#ifdef __CODE_RED
//#define BOOL_32 bool
//#define BOOL_16 bool
//#define BOOL_8  bool
//#else
///** 32 bit boolean type */
//typedef bool BOOL_32;
//
///** 16 bit boolean type */
//typedef bool BOOL_16;
//
///** 8 bit boolean type */
//typedef bool BOOL_8;
