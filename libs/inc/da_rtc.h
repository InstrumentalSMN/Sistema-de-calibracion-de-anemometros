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
#include "sapi_rtc.h"                    // Use RTC peripheral
#include "common.h"
#include "../inc/da_transmision.h"

static char bufferRtc[100]; //Buffer donde guardamos el string del DeltaOhm
static char * ptrBufferRtc = bufferRtc; // un puntero global que apunta a Buffer
extern uint16_t gmsDatetime[100];
extern rtc_t rtc;
extern bool_t opSetRtcViaGsm();
extern bool_t MyParserToDatetimeGsm(char * bufferRtc);

extern bool_t opConfigNtpSocketViaUDP();
extern bool_t opConfigNtpViaUDP();
extern bool_t opSetRtcViaNtpViaUPD();

extern bool_t MyParserToDatetimeNTP(char * bufferRtc);
extern bool_t opConfigNtpSocketViaTCP();
extern bool_t opConfigNtpViaTCP();
extern bool_t opSetRtcViaNtpViaTCP();

extern void opSetRtcDefault();
extern void opReadUart(void *noUsado );


#endif



