/*
 * main.c
 *
 *  Created on: 12 may. 2022
 *      Author: Zozimo
 */

/*==================Yo defino[inclusions]=============================================*/
#include "main.h"

#include "sapi.h"       // <= sAPI header
/* Include statechart header file. Be sure you run the statechart C code
 * generation tool!
 */
//Incluyo el .h de la maquina de estado aca y en TimerTicks.c y el TimerTicks.h en el main y en "TimerTicks.c"

#include "..\gen\DataloggerSA5.h"
#include "TimerTicks.h"

/*==================Yo defino[macros and definitions]=================================*/

#define TICKRATE_1MS	(1)				/* 1000 ticks per second */
#define TICKRATE_MS		(TICKRATE_1MS)	/* 1000 ticks per second */

/*==================Yo defino[internal data declaration]==============================*/

volatile bool SysTick_Time_Flag = false;

/*! This is a state machine */
//Declaro la maquina de estado
static DataloggerSA5 statechart;

/*! This is a timed state machine that requires timer services */
//Defino la cantidad de servicios de timer
#define NOF_TIMERS (sizeof(DataloggerSA5TimeEvents)/sizeof(sc_boolean))

//Creo un vector
TimerTicks ticks[NOF_TIMERS];

// Debo Implementar la acciones que se utilizan en Datalogger_SA.c,

//void DataloggerSA5Iface_opLED(const Datalogger_SA* handle, const sc_integer LEDNumber, const sc_boolean State){
//
//	//Esta función es de la biblioteca sapi.h, ver como esta implementada
//	gpioWrite( (LEDR + LEDNumber), State );
//
//}

void dataloggerSA5Iface_opAcumular(const DataloggerSA5* handle){

}
void dataloggerSA5Iface_opEnvioMuestras(const DataloggerSA5* handle){

}
void dataloggerSA5Iface_opProceso(const DataloggerSA5* handle){

}
void dataloggerSA5Iface_opGuardoEnMemoria(const DataloggerSA5* handle){

}
sc_boolean dataloggerSA5Iface_opConfigGPRS(const DataloggerSA5* handle){
	sc_boolean var = TRUE;
	return TRUE;

}
sc_boolean dataloggerSA5Iface_opConfigFTP(const DataloggerSA5* handle){
	sc_boolean var = TRUE;
	return TRUE;

}
void dataloggerSA5Iface_opTransmitirToFTP(const DataloggerSA5* handle){

}
sc_boolean dataloggerSA5Iface_opGetstatusFTP(const DataloggerSA5* handle){
	sc_boolean var = TRUE;
	return TRUE;

}

void dataloggerSA5_setTimer(DataloggerSA5* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic){

	//Esta funcion esta implementada en TimerTicks.h, ver como esta implementada
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);

}

void dataloggerSA5_unsetTimer(DataloggerSA5* handle, const sc_eventid evid){

	//Esta funcion esta implementada en TimerTicks.h, ver como esta implementada
	UnsetTimerTick( ticks, NOF_TIMERS, evid );

}




/**
 * @brief	Hook on Handle interrupt from SysTick timer
 * @return	Nothing
 */
//Esta  funcion es un gancho para manejar las interrupciones del timer, se usa solo en este main
void myTickHook( void *ptr ){

	/* The sysTick Interrupt Handler only set a Flag */
	SysTick_Time_Flag = true;
}

/*! This function scan all EDU-CIAA-NXP buttons (TEC1, TEC2, TEC3 and TEC4),
 *  and return ID of pressed button (TEC1 or TEC2 or TEC3 or TEC4)
 *  or false if no button was pressed.
 */
uint32_t Buttons_GetStatus_(void) {
	uint8_t ret = false;
	uint32_t idx;

	for (idx = 0; idx < 4; ++idx) {
		if (gpioRead( TEC1 + idx ) == 0)
			ret |= 1 << idx;
	}
	return ret;
}

// No necesito mas funciones

int main(void) {

	uint32_t i;

	uint32_t BUTTON_Status;

	/* Generic Initialization */
	// Inicializo la placa EDU-CIAA, ver que hace esta funcion en sapi.h
	boardConfig();
	//uartConfig( UART_485, 9600 );
	uartConfig( UART_USB, 9600 );
	//uartWriteString( UART_USB, "Inicio");
	//uartConfig( UART_485, 9600 );
	/* Init Ticks counter => TICKRATE_MS */
	tickConfig( TICKRATE_MS );

	/* Add Tick Hook */
	//ver como esta implementada tickCallbackSet esta función
	tickCallbackSet( myTickHook, (void*)NULL );

	/* Init Timer Ticks */
	InitTimerTicks( ticks, NOF_TIMERS );

	/* Statechart Initialization */
	dataloggerSA5_init(&statechart);
	dataloggerSA5_enter( &statechart );
	/* LED state is toggled in the main program */
	while (1) {
		/* The uC sleeps waiting for an interruption */
		__WFI();

		/* When a interrupt wakes to the uC, the main program validates it,
		 * checking the waited Flag */

		// SysTick_Time_Flag es un flag que se vincula con la interrupción del timer, que se
		// activa con la funcion myTickHook y se recoge en la funcion tickCallbackSet

		if (SysTick_Time_Flag == true) {

			/* Then reset its Flag */
			SysTick_Time_Flag = false;

			/* Then Update all Timer Ticks */
			UpdateTimers( ticks, NOF_TIMERS );

			/* Then Scan all Timer Ticks */
			for (i = 0; i < NOF_TIMERS; i++) {

				/* Then if there are pending events */
				if (IsPendEvent( ticks, NOF_TIMERS, ticks[i].evid ) == true) {

					/* Then Raise an Event -> Ticks.evid => OK */
					//Esta definida en Datalogger_SA.h e implementada en Datalogger_SA.c
					dataloggerSA5_raiseTimeEvent( &statechart, ticks[i].evid );

					/* Then Mark as Attached -> Ticks.evid => OK */
					// Esta definida en TimerTicks.h
					MarkAsAttEvent( ticks, NOF_TIMERS, ticks[i].evid );
				}
			}
			/* Then Get status of buttons */
//			BUTTON_Status = Buttons_GetStatus_();
//			/* Then if there are a pressed button */
//			if (BUTTON_Status != 0)
//				/* Then Raise an Event -> evTECXOprimodo => OK,
//				 * and Value of pressed button -> viTecla */
//				datalogger_SAIface_raise_evTECXOprimido(&statechart, BUTTON_Status);
//			else
//				/* Then else Raise an Event -> evTECXNoOprimido => OK */
//				datalogger_SAIface_raise_evTECXNoOprimido(&statechart);
//
//			/* Then Run an Cycle of Statechart */
			dataloggerSA5_runCycle(&statechart);		// Run Cycle of Statechart
		}
	}
}


/** @} doxygen end group definition */

/*==================[end of file]============================================*/
