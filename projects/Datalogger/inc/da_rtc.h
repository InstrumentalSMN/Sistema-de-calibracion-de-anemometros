#ifndef _DA_RTC_H_
#define _DA_RTC_H_

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
#include "common.h"
#include "../inc/da_transmision.h"

static char bufferRtc[100]; //Buffer donde guardamos el string del DeltaOhm
static char * ptrBufferRtc = bufferRtc; // un puntero global que apunta a Buffer
extern uint16_t gmsDatetime[10];
extern bool_t opSetRtcViaGsm();

extern bool_t opConfigNtpSocket();
extern bool_t opConfigNTP();
extern bool_t opSetRtcViaNtp();

extern void opSetRtcDefault();
extern void opReadUart(void *noUsado );


#endif



