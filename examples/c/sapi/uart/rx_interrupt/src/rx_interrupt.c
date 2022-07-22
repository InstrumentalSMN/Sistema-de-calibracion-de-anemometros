#include "sapi.h"
#include "sapi_uart.h"

void onRx( void *noUsado )
{
   uint8_t dato = uartRxRead( UART_232 );
   uartWriteByte( UART_USB, dato);
}

int main(void)
{
   /* Inicializar la placa */
   boardConfig();

   /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_USB, 9600);
   uartConfig(UART_232, 115200);
   // Seteo un callback al evento de recepcion y habilito su interrupcion
   uartCallbackSet(UART_232, UART_RECEIVE, onRx, NULL);
   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_232, true);
   
//   uartWriteString( UART_232, "AT");
//   uartWriteString( UART_232, "\n");
//   delay(5000);
//	uartWriteString( UART_232, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"");/*Los comandos AT van con \n */
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+SAPBR=1,1");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+SAPBR=2,1");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPCID=1");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPSERV=\"ftp.smn.gov.ar\"");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPUN=\"estaut\"");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPPW=\"estacionesautomaticas17\"");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232,"AT+FTPPUTPATH=\"/\"");
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPPUTNAME=\"DatosAnemometroPruebawewew12345.txt\"");/*Nombre del archivo */
//	uartWriteString( UART_232, "\r\n");
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPPUT =1");/*Abro Sesion */
//	delay(5000);
//	uartWriteString( UART_232, "AT+FTPPUT=2,4" );
//	uartWriteString( UART_232, "1234dsds" );
//	uartWriteString( UART_232, "\n");
//	uartWriteString( UART_232, "AT+FTPPUT=2,0");
//	uartWriteString( UART_232, "\r\n");
   while(TRUE) {
	   uartWriteString( UART_232, "AT");
	     uartWriteString( UART_232, "\n");
	     delay(5000);
	   	uartWriteString( UART_232, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"");/*Los comandos AT van con \n */
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+SAPBR=1,1");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+SAPBR=2,1");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPCID=1");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPSERV=\"ftp.smn.gov.ar\"");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPUN=\"estaut\"");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPPW=\"estacionesautomaticas17\"");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232,"AT+FTPPUTPATH=\"/\"");
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPPUTNAME=\"DatosAnemometroPrueb.txt\"");/*Nombre del archivo */
	   	uartWriteString( UART_232, "\r\n");
	   	delay(5000);
	   	uartWriteString( UART_USB, "AT+FTPPUT =1\r\n");
	   	uartWriteString( UART_232, "AT+FTPPUT =1");
	   	uartWriteString( UART_232, "\r\n");/*Abro Sesion */
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPPUT=2,4" );
	   	delay(5000);
	   	uartWriteString( UART_232, "1234" );
	   	uartWriteString( UART_232, "\n");
	   	delay(5000);
	   	uartWriteString( UART_232, "AT+FTPPUT=2,0");
	   	uartWriteString( UART_232, "\r\n");
      // Una tarea muy bloqueante para demostrar que la interrupcion funciona
      gpioToggle(LEDB);
      delay(5000);
   }
   return 0;
}
