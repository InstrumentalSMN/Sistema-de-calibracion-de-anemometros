#include "../inc/controlTunnel.h"
#include "../inc/da_rtc.h"
#include "sapi_rtc.h"

void opConfigPWM(){

	/* Configurar PWM */
	int valor;
	valor = pwmConfig( 0, PWM_ENABLE );
	valor = pwmConfig( PWM3, PWM_ENABLE_OUTPUT );

}

//opGeneratePWM(uint16_t * NumMuestra, real32_t * MuestraAdcTunel, amenometerSerialParam_t * ibc,amenometerSerialParam_t * pat){
void opGeneratePWM(){
	pwmWrite( PWM3, 127);
}

