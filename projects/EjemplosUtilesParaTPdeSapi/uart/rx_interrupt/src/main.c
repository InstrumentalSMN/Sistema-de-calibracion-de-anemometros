#include "rx_interrupt.h"


int main(void)
{
   /* Inicializar la placa */
   boardConfig();

   /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_USB, 9600);
   uartConfig(UART_485, 9600);
   // Seteo un callback al evento de recepcion y habilito su interrupcion
   uartCallbackSet(UART_485, UART_RECEIVE, onRx, NULL);
   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_485, true);
   
   while(TRUE) {
      // Una tarea muy bloqueante para demostrar que la interrupcion funcina
      gpioToggle(LEDB);
      delay(1000);
   }
   return 0;
}
