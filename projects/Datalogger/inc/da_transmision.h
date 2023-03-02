#ifndef _DA_TRANSMISION_H_
#define _DA_TRANSMISION_H_

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
#include "../inc/da_processing.h"

#define CTRL_SOCK_FTP	1
#define DATA_SOCK_FTP	3

/*Region de config GPRS and FTP*/

extern void resetGRPS();
extern bool_t opConfigGPRS();
extern bool_t opConfigFTP();
//extern void KeepAlive();

/*Region de config Socket and FTP*/

extern bool_t opConfigSocketControl();
extern bool_t opConfigSocketData();
extern bool_t opCheckSocketData();
extern bool_t opSetSocketData();

extern bool_t opConfigFTPSocket();
extern void opSetFTPSocket();
extern bool_t opParametersFTPSocket();
extern int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort);

/*Region de Transmision*/
extern bool_t TransmitirFTPViaGPRS(uint32_t * size, int32_t * NumberMesuare);
extern bool_t TransmitirFTPViaEthernet(uint32_t * size, int32_t * NumberMesuare);

#endif



