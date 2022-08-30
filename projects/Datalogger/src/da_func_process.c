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

int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort)
{
	int i;
	char* tok=0;
	strtok(arg,"(");
	for (i = 0; i < 4; i++)
	{
		if(i==0) tok = strtok(NULL,",\r\n");
		else	 tok = strtok(NULL,",");
		remoteIp[i] = (uint8_t)atoi(tok);
		if (!tok){
			printf("bad pport : %s\r\n", arg);
			return -1;
		}
	}
	*remotePort = 0;
	for (i = 0; i < 2; i++){
		tok = strtok(NULL,",\r\n");
		*remotePort <<= 8;
		*remotePort += atoi(tok);
		if (!tok){
			printf("bad pport : %s\r\n", arg);
			return -1;
		}
	}
	printf("ip : %d.%d.%d.%d, port : %d\r\n", remoteIp[0], remoteIp[1], remoteIp[2], remoteIp[3], *remotePort);
	return 0;
}
