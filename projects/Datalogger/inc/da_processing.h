#ifndef _DA_PROCESSING_H_
#define _DA_PROCESSING_H_

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"
#include "common.h"



/*Buffer para las estadisticas del DeltaOHM*/
static real32_t AcumIntensidad[100];
static real32_t AcumDireccion[100];
static real32_t AcumPresion[100];
static real32_t AcumTemp[100];
static real32_t NvBateria[100];

/*Tabla de un elemento por ahora para transmitir al FTP*/

extern char TableToFTP[150];

/*Region Procesamiento */



extern void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt);
extern void opProceso( uint32_t * size);



extern real32_t maxValue(real32_t myArray[], size_t size);
extern real32_t minValue(real32_t myArray[], size_t size);
extern real32_t AverageValue(real32_t myArray[], size_t size);



#endif



