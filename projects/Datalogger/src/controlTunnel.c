#include "../inc/controlTunnel.h"
#include "../inc/da_rtc.h"
#include "sapi_rtc.h"

//Esta variable global se actualizara con los datos recibidos desde el servidor con los valores de referencia para el controlador
//gestionados su recepcion en da_configDatalogger

real32_t referenceVelocity_ts = 0;

void opConfigPWM(){

	/* Configurar PWM */
	int valor;
	valor = pwmConfig( 0, PWM_ENABLE );
	valor = pwmConfig( PWM3, PWM_ENABLE_OUTPUT );
	pwmWrite( PWM3, 255);

}

//opGeneratePWM(uint16_t * NumMuestra, real32_t * MuestraAdcTunel, amenometerSerialParam_t * ibc,amenometerSerialParam_t * pat){
void opGeneratePWM(){
	pwmWrite( PWM3, 0);
}

void opMesuareWindPAT(amenometerSerialParam_t * pat,real32_t * VelPat, real32_t * VelRef ){
	char auxBuffer[7];
	*VelRef = referenceVelocity_ts;
	floatToString(*VelRef,auxBuffer,3);
	printf("Mido Viento Ref %s\r\n",auxBuffer );

	*VelPat = pat->DataAnemometer[0] * NUDO2MS;
	floatToString(*VelPat,auxBuffer,3);
	printf("Mido Viento Pat %s\r\n",auxBuffer );
//	return OK;

}
void configPID(pidController_t * pid){
	pid->kp = 3;
	pid->kd = 0.5;
	pid->ki = 0;
	pid->Ts = 1;
	pid->lastError = 0;
	pid->integral = 0;
	pwmConfig( 0, PWM_ENABLE );
	pwmConfig( PWM3, PWM_ENABLE_OUTPUT );
	pwmWrite( PWM3, 0);
}
void pidControlTunnel( pidController_t * pid, real32_t * VelPat, real32_t * VelRef ,int32_t * pwmValue){
	char auxBuffer[7];
	// Término proporcional
	pid->error = *VelRef - *VelPat;
	// Término integral
	pid->integral += pid->error * pid->Ts;
	// Término derivativo
	pid->derivative = (pid->error - pid->lastError) / pid->Ts;
	// Actualizar el último error
    pid->lastError = pid->error;
	// Calculo de accion de control
	real32_t u_c = pid->kp* pid->error + pid->ki * pid->integral + pid->kd * pid->derivative;

	int32_t u_c_Redon = redondeo(u_c);
//	*pwmValue = (*pwmValue) + (int)floor(u_c);
	*pwmValue = (*pwmValue) + u_c_Redon;
	if (*pwmValue > 255){
		*pwmValue = 255;
	}
	if(*pwmValue < 0){
		*pwmValue = 0;
	}
	pwmWrite( PWM3, *pwmValue);
//	El pwm deberia ser como un acumulador que tiene rango de cero a 255 y me va aumentando o disminuyendo en funcion del signo
//	del error multiplicado por su valor proporcional, podria hacer todas esa cuenta flotante y luego el valor acumulado mandarlo
//	al PWM del tunel
	floatToString(pid->error,auxBuffer,3);
	printf("ERROR  %s\r\n",auxBuffer );
	floatToString(u_c,auxBuffer,3);
	printf("ACCION DE CONTROL sin redondear %s\r\n",auxBuffer );
	printf("Valor de accion de control u_c %d \r\n", u_c_Redon );
	printf("Valor de PWM %d \r\n", *pwmValue );
//	return OK;
}

int32_t redondeo(real32_t num) {
    // Ajustar el número para que sea positivo antes del redondeo
	int32_t signo = (num >= 0) ? 1 : -1;
    num = fabs(num);

    // Redondear el número
    int32_t entero = round(num);

    // Restaurar el signo original
    return entero * signo;
}
