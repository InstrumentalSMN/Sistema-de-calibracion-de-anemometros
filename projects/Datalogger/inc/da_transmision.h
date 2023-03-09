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
#include "socket.h"
#include "w5100.h"

#define CTRL_SOCK_FTP	1
#define DATA_SOCK_FTP	3
#define _MAX_SS		512
#define USER "estaut"
#define PASS "estacionesautomaticas17"
//#define PATH "EMA_LABO_CLI/SIM800L/AnemometroAeroparque"
#define PATH "/EMA_LABO_CLI/SIM800L/PruebaViaPutty"
//#define PATH "EMA_LABO_CLI/SIM800L/AnemometroPalomar"

/*Region de config GPRS and FTP*/

extern void resetGRPS();
extern bool_t opConfigGPRS();
extern bool_t opConfigFTP();
//extern void KeepAlive();

/*Region de config Socket and FTP*/

extern void opConfigSocket();
extern bool_t opConfigSocketData();
extern bool_t opCheckSocketData();
extern bool_t opSetSocketData();

extern bool_t opConfigFTPSocket();
extern bool_t opSetFTPSocketCtrl();
extern bool_t opParametersFTPSocket();
extern int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort);

/*Region de Transmision*/
extern bool_t TransmitirFTPViaGPRS(uint32_t * size, int32_t * NumberMesuare);
extern void OpenFileFTP( int32_t * NumberMesuare);
extern bool_t TransmitirFTPViaEthernet();
extern int8_t DesconectarSocket(uint8_t sn);
extern void backUpData();

#endif



