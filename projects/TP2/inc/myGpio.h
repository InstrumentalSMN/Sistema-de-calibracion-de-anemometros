#ifndef _MYGPIO_H_
#define _MYGPIO_H_

/*==================[inclusions]=============================================*/

#include "myGpio_peripheral_map.h"
#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define gpioConfig  _mygpioInit
#define pinInit     _mygpioInit
#define pinRead     _mygpioRead
#define pinWrite    _mygpioWrite
#define pinValueGet _mygpioRead
#define pinValueSet _mygpioWrite

/*==================[typedef]================================================*/

/* Pin modes */
/*
 *  INPUT  =  0    (No PULLUP or PULLDOWN)
 *  OUTPUT =  1
 *  INPUT_PULLUP = 2
 *  INPUT_PULLDOWN = 3
 *  INPUT_REPEATER = 4 (PULLUP and PULLDOWN)
 *  INITIALIZE = 5
 */
typedef enum {
   GPIO_INPUT, GPIO_OUTPUT,
   GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN,
   GPIO_INPUT_PULLUP_PULLDOWN,
   GPIO_ENABLE
} mygpioInit_t;



/*Creo una estructura de configuracion para los GPIO....*/
typedef struct{
	int8_t scuPinNamePort;
	int8_t scuPinNamePin;
	int8_t func;
	int8_t gpioPort;
	int8_t gpioPin;
}conf_t;




/* ----- Begin Pin Init Structs NXP LPC4337 ----- */

typedef struct {
   int8_t port;
   int8_t pin;
} gpioInitLpc4337_t;

typedef struct {
   pinInitLpc4337_t pinName;
   int8_t func;
   gpioInitLpc4337_t gpio;
} pinInitGpioLpc4337_t;

/* ------ End Pin Init Structs NXP LPC4337 ------ */

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*Mis funciones*/
bool_t _mygpioInit( mygpioMap_t pin, mygpioInit_t config );
bool_t _mygpioRead( mygpioMap_t pin );
bool_t _mygpioWrite( mygpioMap_t pin, bool_t value );
bool_t _mygpioToggle( mygpioMap_t pin );


/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _MYGPIO_H_ */
