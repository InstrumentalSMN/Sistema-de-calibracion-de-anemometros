#ifndef _DA_FUNC_PROCESS_H_
#define _DA_FUNC_PROCESS_H_

/*==================[inclusions]=============================================*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "sapi_boards.h"
#include "sapi.h"


extern real32_t maxValue(real32_t myArray[], size_t size);
extern real32_t minValue(real32_t myArray[], size_t size);
extern real32_t AverageValue(real32_t myArray[], size_t size);



#endif



