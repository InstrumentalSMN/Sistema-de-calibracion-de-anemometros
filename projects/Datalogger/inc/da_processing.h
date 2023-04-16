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
#include "../inc/da_acquisition.h"



/*Buffer para las estadisticas del DeltaOHM*/
static real32_t AcumIntensidadPat[100];
static real32_t AcumDireccionPat[100];
static real32_t AcumIntensidadIBC[100];
static real32_t AcumDireccionIBC[100];
static real32_t AcumPresionPat[100];
static real32_t AcumTempPat[100];
static real32_t NvBateria[100];


/*Tabla de un elemento por ahora para transmitir al FTP*/

extern char TableToFTP[10000];
extern uint32_t next;

/*Region Procesamiento */



extern void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt, amenometerSerialParam_t ibc ,amenometerSerialParam_t pat);
//extern void opProceso( uint32_t * size);
extern void opProceso(  uint16_t * NumMuestra, int32_t * NumMedicion);



extern real32_t maxValue(real32_t myArray[], size_t size);
extern real32_t minValue(real32_t myArray[], size_t size);
extern real32_t AverageValue(real32_t myArray[], size_t size);



#endif



