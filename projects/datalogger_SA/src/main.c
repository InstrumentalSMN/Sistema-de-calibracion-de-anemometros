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

#include "Datalogger_SA.h"
#include "TimerTicks.h"

/*==================Yo defino[macros and definitions]=================================*/

#define TICKRATE_1MS	(1)				/* 1000 ticks per second */
#define TICKRATE_MS		(TICKRATE_1MS)	/* 1000 ticks per second */

/*==================Yo defino[internal data declaration]==============================*/

volatile bool SysTick_Time_Flag = false;

/*! This is a state machine */
//Declaro la maquina de estado
static Datalogger_SA statechart;

/*! This is a timed state machine that requires timer services */
//Defino la cantidad de servicios de timer
// Datalogger_SATimeEvents es una estructura definida en Datalogger_SA.h, la genera Yakindu
// Y sc_boolean esta definida en sc_types.h, la genera Yakindu
#define NOF_TIMERS (sizeof(Datalogger_SATimeEvents)/sizeof(sc_boolean))

//Creo un vector
TimerTicks ticks[NOF_TIMERS];

// Debo Implementar la acciones que se utilizan en Datalogger_SA.c,

void datalogger_SAIface_opLED(const Datalogger_SA* handle, const sc_integer LEDNumber, const sc_boolean State){

	//Esta función es de la biblioteca sapi.h, ver como esta implementada
	gpioWrite( (LEDR + LEDNumber), State );

}

void datalogger_SA_setTimer(Datalogger_SA* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic){

	//Esta funcion esta implementada en TimerTicks.h, ver como esta implementada
	SetNewTimerTick(ticks, NOF_TIMERS, evid, time_ms, periodic);

}

void datalogger_SA_unsetTimer(Datalogger_SA* handle, const sc_eventid evid){

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


// No necesito mas funciones

int main(void) {

	uint32_t i;

	/* Generic Initialization */
	// Inicializo la placa EDU-CIAA, ver que hace esta funcion en sapi.h
	boardConfig();

	/* Init Ticks counter => TICKRATE_MS */
	tickConfig( TICKRATE_MS );

	/* Add Tick Hook */
	//ver como esta implementada tickCallbackSet esta función
	tickCallbackSet( myTickHook, (void*)NULL );

	/* Init Timer Ticks */
	InitTimerTicks( ticks, NOF_TIMERS );

	/* Statechart Initialization */
	datalogger_SA_init(&statechart);
	datalogger_SA_enter( &statechart );
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
					datalogger_SA_raiseTimeEvent( &statechart, ticks[i].evid );

					/* Then Mark as Attached -> Ticks.evid => OK */
					// Esta definida en TimerTicks.h
					MarkAsAttEvent( ticks, NOF_TIMERS, ticks[i].evid );
				}
			}

			/* Then Run an Cycle of Statechart */
			datalogger_SA_runCycle(&statechart);		// Run Cycle of Statechart
		}
	}
}


/** @} doxygen end group definition */

/*==================[end of file]============================================*/
