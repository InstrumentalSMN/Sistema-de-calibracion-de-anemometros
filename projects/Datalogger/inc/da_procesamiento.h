#ifndef _DA_PROCESAMIENTO_H_
#define _DA_PROCESAMIENTO_H_

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"


/* Agrego un vector para acumular muestras */
/*quiza haga falta pedir memoria para estos Flacos*/

static real32_t AcumIntensidad[20];
static real32_t AcumDireccion[20];
static real32_t AcumPresion[20];
static real32_t Bateria[20];
typedef uint32_t* ptrUnt32_t;


/*Tabla de un elemento por ahora*/

static char *TableToFTP;



/*Region de Procesamiento*/
void opProceso(char * TableToFTP, uint32_t * size, real32_t* AcumIntensidad, real32_t* AcumDireccion, real32_t* AcumPresion, real32_t* Bateria);
extern real32_t maxValue(real32_t myArray[], size_t size);
extern real32_t minValue(real32_t myArray[], size_t size);
extern real32_t AverageValue(real32_t myArray[], size_t size);



#endif



