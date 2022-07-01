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
#include "sapi_datatypes.h"

typedef uint32_t* ptrUint32_t;

extern bool_t opConfigGPRS();
extern bool_t opConfigFTP();
extern void TransmitirFTP(char * TableToFTP, uint32_t * size);



#endif



