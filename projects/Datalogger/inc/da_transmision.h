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
#include "../inc/my_spi.h"

#define WEB_SOCK	0
#define DATA_SOCK_FTP	3
#define CTRL_SOCK_FTP_RECV	1
#define DATA_SOCK_FTP_RECV	2
#define _MAX_SS		550
#define USER "estaut"
#define PASS "estacionesautomaticas17"
//#define PATH "EMA_LABO_CLI/SIM800L/AnemometroAeroparque"
#define PATH "/EMA_LABO_CLI/SIM800L/PruebaViaPutty"
//#define PATH "EMA_LABO_CLI/SIM800L/AnemometroPalomar"


extern bool_t KeepAlive();

/*Region de config WebSocket*/

extern void opEnableRxWebSocket();
extern void opReceptionMessage( void *data );

extern void opConfigSocket();
extern bool_t opInitWebSocket();
extern bool_t opConnectToWebSocket();
extern int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort);


/*Region de Transmision*/

extern bool_t opTransmitWebSocketEthernet(int32_t * NumberMesuare);
extern int8_t DesconectarSocket(uint8_t sn);
extern void backUpData();
extern void encodeMessage126(uint8_t * buf, uint8_t * message);
extern void encodeMessage125(uint8_t * buf, uint8_t * message);
#endif



