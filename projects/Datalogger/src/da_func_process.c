#include "../inc/da_func_process.h"



/*Funciones para Estadisticas*/

real32_t maxValue(real32_t myArray[], size_t size) {
    /* enforce the contract */
    //assert(myArray && size);
    size_t i;
    real32_t maxValue = myArray[0];

    for (i = 1; i < size; ++i) {
        if ( myArray[i] > maxValue ) {
            maxValue = myArray[i];
        }
    }
    return maxValue;
}


real32_t minValue(real32_t myArray[], size_t size) {
    /* enforce the contract */
    //assert(myArray && size);
    size_t i;
    real32_t minValue = myArray[0];

    for (i = 1; i < size; ++i) {
        if ( myArray[i] < minValue ) {
        	minValue = myArray[i];
        }
    }
    return minValue;
}


real32_t AverageValue(real32_t myArray[], size_t size){

	real32_t sum,avg;
	int i;
	sum = avg = 0;
	for(i = 0; i < size; i++) {
	   sum = sum + myArray[i];
	}
	avg = (float)sum / i;
	return avg;

}
