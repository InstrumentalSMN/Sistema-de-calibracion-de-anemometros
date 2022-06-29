/* Copyright 2015, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Date: 2015-09-23 */

#ifndef _DA_SA_PRUEBA_H_
#define _DA_SA_PRUEBA_H_

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include <stdint.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"


/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

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

#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE   (!FALSE)
#endif

// From: https://es.coursera.org/lecture/embedded-software-hardware/9-register-definition-files-6pqVq

//  __I Defines 'read only' permissions: volatile const
//  __O Defines 'write only' permissions: volatile
// __IO Defines 'read / write' permissions: volatile

#define HW_REG_8_R(x)     (*((__I  uint8_t *)(x)))
#define HW_REG_16_R(x)    (*((__I uint16_t *)(x)))
#define HW_REG_32_R(x)    (*((__I uint32_t *)(x)))

#define HW_REG_8_W(x)     (*((__O  uint8_t *)(x)))
#define HW_REG_16_W(x)    (*((__O uint16_t *)(x)))
#define HW_REG_32_W(x)    (*((__O uint32_t *)(x)))

#define HW_REG_8_RW(x)    (*((__IO  uint8_t *)(x)))
#define HW_REG_16_RW(x)   (*((__IO uint16_t *)(x)))
#define HW_REG_32_RW(x)   (*((__IO uint32_t *)(x)))

// Example:
//#define REG_NAME   (HW_REG_32_RW(0x4544555))

/*==================[typedef]================================================*/

/* Define Boolean Data Type */
typedef uint8_t bool_t;

/* Define real Data Types (floating point) */
typedef float  real32_t;
//typedef double real64_t; // In LPC4337 float = double (Floating Point single precision, 32 bits)

/* Define Tick Data Type */
typedef uint64_t tick_t;
typedef bool BOOL_8;

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

/* Estructuras*/
#define NAN  -99999 //No hay dato
typedef struct{
	 real32_t Intensidad;
	 real32_t Direccion;
	 real32_t Presion;

}dataWind_t;

/* type of Pointers */

typedef  int* ptrInt;
typedef char* ptrChar;
typedef float* ptfFloat;
typedef char* StringData;
typedef uint16_t* ptrUnt16_t;
typedef dataWind_t* ptrDataWind_t;

//Se comporta como una variable global para la maquina de estados, esto es genial ya que conserva su estado
static  real32_t MisDatas[] = {NAN,NAN,NAN}; // el0,ele1,ele2,



/*==================Prototipos=========================*/

extern void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n, real32_t * DataDeltaOhm );

extern void _opLEDprint(uint16_t LEDNumber,  BOOL_8 State, real32_t * DataDeltaOhm);

extern void _opParceo(real32_t * DataDeltaOhm);




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
