#include "auxiliar.h"
//#include "da_sa_prueba.h"

void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n, real32_t * DataDeltaOhm){

	//Esta función es de la biblioteca sapi.h, ver como esta implementada
	gpioWrite( (LEDR + LEDNumber), State );
	*n +=2;/*inicializo acá el putero*/
	if((*n) > 4){
		gpioWrite( LEDB, State );
	}
	if((*n) > 7){
		gpioWrite( LEDR, State );
	}
	if((*n) > 10){
		gpioWrite( LEDG, State );
	}
	DataDeltaOhm[0] += 1;
	DataDeltaOhm[1] += 2;
	DataDeltaOhm[2] += 3;
}


