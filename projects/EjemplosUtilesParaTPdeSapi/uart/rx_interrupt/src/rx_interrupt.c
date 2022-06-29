#include "rx_interrupt.h"

void onRx( void *noUsado )
{
	char c = uartRxRead( UART_USB );
	printf( "Recibimos <<%c>> por UART\r\n", c );
}
