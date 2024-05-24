#ifndef _CONTROLTUNNEL_H_
#define _CONTROLTUNNEL_H_

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

// Definición de la estructura del controlador PID
typedef struct {
	real32_t kp;  // Ganancia proporcional
	real32_t ki;  // Ganancia integral
	real32_t kd;  // Ganancia derivativa
	real32_t error;
	real32_t lastError;  // Último error
	real32_t integral;    // Suma acumulada de errores
	real32_t derivative;
	real32_t Ts;

} pidController_t;
typedef pidController_t * ptrPidController_t;

extern real32_t referenceVelocity_ts;
 /*Region ControlPID */
extern void opConfigPWM();
extern void opGeneratePWM();
extern void opMesuareWindPAT(amenometerSerialParam_t * pat, real32_t * VelPat, real32_t * VelRef );
extern void configPID(pidController_t * pid);
extern void pidControlTunnel(pidController_t * pid, real32_t * VelPat, real32_t * VelRef, int32_t * pwmLevel );
extern int32_t redondeo(real32_t num);
#endif



