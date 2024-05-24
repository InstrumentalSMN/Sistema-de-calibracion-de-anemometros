#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"
#include "sapi_uart.h"
#include "w5100.h"
#include <math.h>

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
// Constantes
#define NUDO2MS 0.514444
/*==================[typedef]================================================*/

/*Agrego algún tipo de dato particular para el string de los sensores*/
typedef  int* ptrInt;
typedef char* ptrChar;
typedef float* ptrfFloat;
typedef char* StringData;
typedef uint8_t * ptrUnt8_t;
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


//estructuras para  tranferir a la funcion de interrupcion de las uarts que reciben los datos de los anemometros
// Especial atencion en como se carga en la maquina de estados estas variables

#endif
