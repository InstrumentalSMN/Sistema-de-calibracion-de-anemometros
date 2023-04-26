#ifndef _DA_CONFIGDATALOGGER_H_
#define _DA_CONFIGDATALOGGER_H_

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
#include "../inc/da_acquisition.h"
#include "socket.h"
#include "w5100.h"
#include "../inc/my_spi.h"


typedef struct{
	char command[25];
	uint16_t indexCommand;
	uint8_t startMesuare;
	amenometerSerialParam_t AnemoIBC;
	amenometerSerialParam_t AnemoPAT;
	//uint16_t IntervaloTabla;
	//uint16_t IntervaloMuestra;
}config_t;


typedef config_t * ptrConfig_t;

extern char *diccCommands[];

extern bool_t (*diccProcesos[])(config_t * commandConfig);/* diccionario de punteros a funcion */
typedef bool_t(*funcion_t)(config_t * commandConfig);/*tipo de dato puntero a funcion*/

/*===================================PROTOTIPOS_DE_FUNCIONES_COMMANDS=============================*/
bool_t start(config_t * commandConfig);
bool_t stop( config_t * commandConfig);
bool_t setIBC(config_t * commandConfig);
bool_t setPAT(config_t * commandConfig);
bool_t setTimes(config_t * commandConfig);

/*=======================================================================================================*/

extern bool_t opProccessMessageFromServer( uint16_t * sizeBuffServer,config_t * commandConfig );
extern bool_t myParserDataFromWebSocket(char * dbuf, uint16_t * sizeBuffServer, config_t * commandConfig);

#endif



