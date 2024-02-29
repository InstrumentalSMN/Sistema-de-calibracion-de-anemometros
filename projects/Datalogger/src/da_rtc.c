#include "../inc/da_rtc.h"
#include "../inc/da_transmision.h"
#include "../inc/w5100.h"
#include "socket.h"
#include "../inc/sntp.h"

uint16_t gmsDatetime[100];
uint16_t ntpDatetime[100];
volatile bool Uart_Rs232_Mjs_Flag = false;

// Crear estructura RTC
   rtc_t rtc;



#define NTP_PACKET_SIZE		48
#define SOCK_NTP			2
//ntpformat NTPformat;
datetime Nowdatetime;
uint8_t ntpmessage[48];
uint8_t *data_buf;
uint8_t NTP_SOCKET;
uint8_t time_zone_NTP = 18; //Hora de argentina UTC -3
uint16_t ntp_retry_cnt_NTP=0; //counting the ntp retry number

//client information
uint16_t PortLocal_NTP = 30009;
uint8_t mac_NTP[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
uint8_t mac_NTP1[6];
//uint8_t IPLocal_NTP[4] = {10,10,13,121};
uint8_t IPLocal_NTP[4] = {10,10,13,70}; // IP EMA instrumental
//uint8_t IPLocal[4] = {192,168,0,22}; //casa
uint8_t IPLocal_NTP1[4];
uint8_t Gat_NTP[4] = {10,10,13,1};
//uint8_t Gat[4] = {192,168,0,1}; //casa
uint8_t Gat_NTP1[4];
uint8_t MASKSUB_NTP[4] = {255,255,255,0};
uint8_t MASKSUB_NTP1[4];

// Server NTP information
//uint8_t NTP_destIP[4] = {129,6,15,29};
//uint8_t NTP_destIP[4] = {132,163,96,5};
uint8_t NTP_destIP[4] = {128,138,140,44};
//uint8_t NTP_destIP[4] = {128,138,141,172};

uint16_t _NTP_destport = 13; //  puerto para NTP por medio de UPD
char BufferDayTime[100];


uint8_t ret1;
uint8_t ret2;
uint8_t ret3;
uint8_t ret4;
uint8_t ret5;





bool_t opConfigNtpSocketViaTCP(){

	DesconectarSocket(SOCK_NTP);
	//Reset registers
	uint8_t registroModo = getMR();
	setMR(MR_RST);
	registroModo = getMR();
	setGAR(Gat_NTP);
	getGAR(Gat_NTP1);
	setSHAR(mac_NTP);
	getSHAR(mac_NTP1);
	setSUBR(MASKSUB_NTP);
	getSUBR(MASKSUB_NTP1);
	setSIPR(IPLocal_NTP);
	getSIPR(IPLocal_NTP1);

	//Configurar Socket de control para el NTP

	setSn_MR(SOCK_NTP, Sn_MR_TCP);
	setSn_PORT(SOCK_NTP,PortLocal_NTP);
	setSn_CR(SOCK_NTP,Sn_CR_OPEN);
	delay(15); // Necesario para recibir respuestas
	ret1 = getSn_MR(SOCK_NTP);
	ret2 = getSn_PORT(SOCK_NTP);
	ret5 = getSn_SR(SOCK_NTP);
	if(ret5 != SOCK_INIT){
		disconnect(SOCK_NTP);
		return ERROR;
	}

	return OK;
}

bool_t opConfigNtpViaTCP(){

	setSn_DIPR(SOCK_NTP, NTP_destIP);
	setSn_DPORT(SOCK_NTP,_NTP_destport);
	setSn_CR(SOCK_NTP,Sn_CR_CONNECT);
	ret4 = getSn_DPORT(SOCK_NTP);
	ret5 = getSn_SR(SOCK_NTP);
//	printf("\n\r Puerto TCP Seteado",ret4);
	uint16_t size = 0;
	//validar con un delay puede evitar que se rompa cuando esta mal el puerto
	while(size == 0){
		send(SOCK_NTP, "\n", strlen("\n"));
		size = getSn_RX_RSR(SOCK_NTP);
//		printf("%d",size);
	}
	wiz_recv_data(SOCK_NTP, BufferDayTime,  size);
//	disconnect(SOCK_NTP);
	return OK;

}

bool_t opSetRtcViaNtpViaTCP(){

	if (MyParserToDatetimeNTP(BufferDayTime) == ERROR){  //pasar esta funcion a socket.h o daregion.h
							//Despues probar de conectarme a ese port que me da y mandar datos
		printf("No pude obtener la hora por Ethernet\r\n");
		return ERROR;
	}else{
		//Setear el RTC con los datos parseados
		printf("Si pude obtener la hora por Ethernet:\r\n");
		// Completar estructura RTC
		rtc.year = ntpDatetime[0];
		rtc.month = (uint8_t)ntpDatetime[1];
		rtc.mday = (uint8_t)ntpDatetime[2];
//		rtc.wday = 3;
		rtc.hour = (uint8_t)ntpDatetime[3];
		rtc.min = (uint8_t)ntpDatetime[4];
		rtc.sec= (uint8_t)ntpDatetime[5];
		rtcInit();
		rtcWrite( &rtc );
		DesconectarSocket(SOCK_NTP);
		close(SOCK_NTP);
		return OK;
	}

}

//59992 23-02-17 17:47:01 00 0 0 831.5 UTC(NIST) *


bool_t MyParserToDatetimeNTP(char * bufferRtc){

	int i,j;
	char* tok=0;
	tok = strtok(bufferRtc," ");
	if(tok == NULL){return ERROR;}
//	printf("1ero strk: %s\r\n", tok);
	for (i = 0; i < 2; i++){

		tok = strtok(NULL,"-");
		if(tok == NULL){return ERROR;}
		printf("Fecha: %s\r\n", tok);
		if(i == 0){
			ntpDatetime[i] = (uint16_t)atoi(tok)+2000;
		}else
			ntpDatetime[i] = (uint16_t)atoi(tok);
	}
	tok = strtok(NULL," ");
	if(tok == NULL){return ERROR;}
	ntpDatetime[i] = (uint16_t)atoi(tok);
	i++;
	printf("Fecha: %s\r\n", tok);

	for (j = 0; j < 2; j++){

		tok = strtok(NULL,":");
		if(tok == NULL){return ERROR;}
		printf("Hora: %s\r\n", tok);
		ntpDatetime[i] = (uint16_t)atoi(tok);
		i++;
	}
	tok = strtok(NULL," ");
	if(tok == NULL){return ERROR;}
	printf("Hora: %s\r\n", tok);
	ntpDatetime[i] = (uint16_t)atoi(tok);
	return TRUE;

}

void opSetRtcDefault(){

//	while(1){
//		rtcRead( &rtc );
//		printf( "%02d/%02d/%04d, %02d:%02d:%02d\r\n",
//		rtc.mday, rtc.month, rtc.year,
//		rtc.hour, rtc.min, rtc.sec );
//		// Note: printf() use sAPI UART_USB (Chip USART2 on EDU-CIAA-NXP) at 115200, 8N1
//
//		delay(1000);
//	}

	// Completar estructura RTC
	rtc.year = 2023;
	rtc.month = 4;
	rtc.mday = 18;
	rtc.wday = 4;
	rtc.hour = 9;
	rtc.min = 7;
	rtc.sec= 0;

	// Inicializar RTC
	rtcInit();
	rtcWrite( &rtc );

}


void opReadUart(void *noUsado ){

	uint8_t aux = uartRxRead( UART_232 );
	*ptrBufferRtc = aux;
	ptrBufferRtc++;
	gpioWrite( LEDR, OFF );
	if(aux =='\n'){
//	uartWriteString( UART_USB, bufferRtc);
	//	opPreprocesoDeltaOHM();/*acá parseo el String de datos*/
		ptrBufferRtc = bufferRtc; //Reseteo el puntero para el siguiente string
		uartConfig( UART_232, 115200 ); // Limpio mensaje restante en la siguiente linea
		Uart_Rs232_Mjs_Flag = TRUE; // Activo flag que permite validar el mensaje recibido
//	gpioWrite( LEDR, ON );

	}

}

//int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort)
//+CIPGSMLOC: 0,2022/09/29,14:29:07
//+CIPGSMLOC: 601

bool_t MyParserToDatetimeGsm(char * bufferRtc){

	int i,j;
	char* tok=0;
	tok = strtok(bufferRtc," ");
	if(tok == NULL){return ERROR;}
	printf("1ero strk: %s\r\n", tok);
	tok = strtok(NULL,",");
	if(tok == NULL){return ERROR;}
	printf("2do strk: %s\r\n", tok);
//	Valido con este segundo que puede ser 0 -->ok o 601 --Error
	if((uint8_t)atoi(tok) != FALSE){

		printf("ERROR NO SIGNAL GPRS\r\n");
		return ERROR;

	}
	for (i = 0; i < 2; i++){

		tok = strtok(NULL,"/");
		if(tok == NULL){return ERROR;}
		printf("Fecha: %s\r\n", tok);
		gmsDatetime[i] = (uint16_t)atoi(tok);
	}
	tok = strtok(NULL,",");
	if(tok == NULL){return ERROR;}
	gmsDatetime[i] = (uint16_t)atoi(tok);
	i++;
	printf("Fecha: %s\r\n", tok);

	for (j = 0; j < 2; j++){

		tok = strtok(NULL,":");
		if(tok == NULL){return ERROR;}
		printf("Hora: %s\r\n", tok);
		gmsDatetime[i] = (uint16_t)atoi(tok);
		i++;
	}
	tok = strtok(NULL,"\r\n");
	if(tok == NULL){return ERROR;}
	printf("Hora: %s\r\n", tok);
	gmsDatetime[i] = (uint16_t)atoi(tok);
	return TRUE;

}


bool_t opSetRtcViaGsm(){


	uartConfig( UART_232, 115200 );
	// Seteo un callback al evento de recepcion y habilito su interrupcion
	uartCallbackSet(UART_232, UART_RECEIVE, opReadUart, NULL);
	// Habilito todas las interrupciones de UART_USB
	uartInterrupt(UART_232, true);


	uartWriteString( UART_USB, "Solicito via GSM la hora\r\n");
//	uartConfig( UART_232, 115200 );
//	uartWriteString( UART_232, "AT+FTPPUT=2,0");
//	uartConfig( UART_232, 115200 );
	uint8_t dato = 1;
	uartWriteString( UART_232, "AT+CIPGSMLOC=2,1");
//	uartWriteString( UART_232, "AT+SAPBR=2,1");
//	uartWriteString( UART_232, "AT");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	while(Uart_Rs232_Mjs_Flag == FALSE); //Con esto espero que llegue el msj completo sin delays alternos
	printf("Respuesta del GPRS: %s \r\n",bufferRtc);
	//Crear esta funcion!!! MyParserToDatetimeGsm
	if (MyParserToDatetimeGsm(bufferRtc) == ERROR){  //pasar esta funcion a socket.h o daregion.h
							//Despues probar de conectarme a ese port que me da y mandar datos
		printf("No pude obtener la hora por GSM\r\n");
		return ERROR;
	}else{
		//Setear el RTC con los datos parseados
		printf("Si pude obtener la hora por GSM: %s \r\n",bufferRtc);
		uartInterrupt(UART_232, FALSE);
		// Completar estructura RTC
		rtc.year = gmsDatetime[0];
		rtc.month = (uint8_t)gmsDatetime[1];
		rtc.mday = (uint8_t)gmsDatetime[2];
		rtc.wday = 3;
		rtc.hour = (uint8_t)gmsDatetime[3];
		rtc.min = (uint8_t)gmsDatetime[4];
		rtc.sec= (uint8_t)gmsDatetime[5];
		rtcInit();
		rtcWrite( &rtc );
		return OK;
	}
//	//delay(3000);
//	uartWriteString( UART_USB, "Respuesta del sensor\r\n");
////	uartReadByte( UART_232, &dato );
//	// Procesar los datos recepcionados.
//
//	uartWriteString( UART_USB, bufferRtc);
//	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//		//gpioWrite( LED3, ON );
//		uartWriteString( UART_USB, "Respuesta de AT\r\n");
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//		//uartWriteString( UART_232, "AT+SAPBR=0,1");
//		resetGRPS();
//		return ERROR;
//	}
//	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//
//
//	uartWriteString( UART_232, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"");/*Los comandos AT van con \n */
//	uartWriteString( UART_232, "\r\n");
//	delay(300);
//	uartReadByte( UART_232, &dato );
//	uartWriteByte( UART_USB, dato);
//	uartWriteString( UART_USB, "\r\n");
//
//	if(dato != '0'){
//		gpioWrite( LED3, ON );
//		//uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "Respuesta de AT\r\n");
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//		resetGRPS();
//		return ERROR;
//	}
//


}

//bool_t opConfigNtpSocketViaUDP(){
//
////	setMR(mr)(0x00);
////	//Reset registers
//	uint8_t registroModo = getMR();
//	setMR(MR_RST);
//	registroModo = getMR();
//	setGAR(Gat_NTP);
//	getGAR(Gat_NTP1);
//	setSHAR(mac_NTP);
//	getSHAR(mac_NTP1);
//	setSUBR(MASKSUB_NTP);
//	getSUBR(MASKSUB_NTP1);
//	setSIPR(IPLocal_NTP);
//	getSIPR(IPLocal_NTP1);
//// msj para el servidor preconfigurado
////	NTPformat.dstaddr[0] = NTP_destIP[0];
////	NTPformat.dstaddr[1] = NTP_destIP[1];
////	NTPformat.dstaddr[2] = NTP_destIP[2];
////	NTPformat.dstaddr[3] = NTP_destIP[3];
////	data_buf = dbuf;
////
////	uint8_t Flag;
////	NTPformat.leap = 0;           /* leap indicator */
////	NTPformat.version = 4;        /* version number */
////	NTPformat.mode = 3;           /* mode */
////	NTPformat.stratum = 0;        /* stratum */
////	NTPformat.poll = 0;           /* poll interval */
////	NTPformat.precision = 0;      /* precision */
////	NTPformat.rootdelay = 0;      /* root delay */
////	NTPformat.rootdisp = 0;       /* root dispersion */
////	NTPformat.refid = 0;          /* reference ID */
////	NTPformat.reftime = 0;        /* reference time */
////	NTPformat.org = 0;            /* origin timestamp */
////	NTPformat.rec = 0;            /* receive timestamp */
////	NTPformat.xmt = 1;            /* transmit timestamp */
////
////	Flag = (NTPformat.leap<<6)+(NTPformat.version<<3)+NTPformat.mode; //one byte Flag
//////	memcpy(ntpmessage,(void const*)(&Flag),1);
//////
////
//
//
//	  memset(dbuf, 0, NTP_PACKET_SIZE);
//	  // Initialize values needed to form NTP request
//	  // (see URL above for details on the packets)
//	  dbuf[0] = 0b11011011;   // LI, Version, Mode
//	  dbuf[1] = 2;     // Stratum, or type of clock
//	  dbuf[2] = 10;     // Polling Interval
//	  dbuf[3] = 0xEC;  // Peer Clock Precision
//	  // 8 bytes of zero for Root Delay & Root Dispersion
//	  dbuf[12]  = 0;
//	  dbuf[13]  = 0;
//	  dbuf[14]  = 0;
//	  dbuf[15]  = 0;
//
//
//
//	//Configurar Socket de control para el NTP
//
//	setSn_MR(SOCK_NTP, Sn_MR_UDP);
//	setSn_PORT(SOCK_NTP,PortLocal_NTP);
//	setSn_CR(SOCK_NTP,Sn_CR_OPEN);
//	ret5 = getSn_SR(SOCK_NTP);
//	if(ret5 != SOCK_UDP){
//		disconnect(SOCK_NTP);
//		return ERROR;
//	}
//
//	delay(15); // Necesario para recibir respuestas
//	ret1 = getSn_MR(SOCK_NTP);
//	ret2 = getSn_PORT(SOCK_NTP);
//
//
//
//	return OK;
////	return ERROR;
//
//}
//
//bool_t opConfigNtpViaUDP(){
//
//	uint16_t size = getSn_RX_RSR(SOCK_NTP);
//	uint16_t RSR_len;
//	setSn_DIPR(SOCK_NTP, NTP_destIP);
//	setSn_DPORT(SOCK_NTP,_NTP_destport);
//	setSn_CR(SOCK_NTP,Sn_CR_CONNECT);
//
////	delay(5000); // Necesario para recibir respuestas
//	ret4 = getSn_DPORT(SOCK_NTP);
//	ret5 = getSn_SR(SOCK_NTP);
//	printf("\n\r Puerto UDP Seteado %d",ret4);
////	setSn_CR(SOCK_NTP,Sn_CR_CONNECT);
//
////	if(ret5 != SOCK_ESTABLISHED){ // SOCK_ESTABLISHED = 23
////		disconnect(SOCK_NTP);
////		return ERROR;
////		//gpioWrite( LED2, ON );
////	}
//
//	delay(1000);//Necesario para recibir una respuesta del Server
//
////	printf("%d",size);
//
////	//Enviamos el primer requerimiento
//////	delay(25000);
//	uint16_t dato = sizeof(dbuf);
////	int32_t largo = sendto(SOCK_NTP, dbuf, sizeof(dbuf), NTP_destIP, 123);
//	ret5 = getSn_SR(SOCK_NTP);
////	wiz_recv_data(SOCK_NTP, dbuf,  sizeof(dbuf));
//	wiz_send_data(SOCK_NTP, dbuf, sizeof(dbuf));
//	delay(20000);
//	size = getSn_RX_RSR(SOCK_NTP);
//	wiz_recv_data(SOCK_NTP, dbuf,  sizeof(dbuf));
//
//	datetime *time;
//	get_seconds_from_ntp_server(dbuf,40);
//	time->yy = Nowdatetime.yy;
//	time->mo = Nowdatetime.mo;
//	time->dd = Nowdatetime.dd;
//	time->hh = Nowdatetime.hh;
//	time->mm = Nowdatetime.mm;
//	time->ss = Nowdatetime.ss;
//
//
//
//
//
//
//	while(size==0){
////		wiz_send_data(SOCK_NTP, dbuf, sizeof(dbuf));
//
//		delay(1000);
//		size = getSn_RX_RSR(SOCK_NTP);
//		printf(" \n\r Espero respuesta");
//	}
//	size = getSn_RX_RSR(SOCK_NTP);
//////	setSn_CR(SOCK_NTP,Sn_CR_SEND);
//////	while(getSn_CR(SOCK_NTP));
////	printf("\n\r cantidad de datos  %d",largo);
////
//////	printf("\n\r cantidad de datos  %d",getSn_RX_RSR(SOCK_NTP));
////	while(getSn_RX_RSR(SOCK_NTP) == 0){
////		sendto(SOCK_NTP, dbuf, sizeof(dbuf), NTP_destIP, 123);
////		delay(10000);
////		printf("\r\n Espero respuesta del server");
////	}
////	RSR_len = getSn_RX_RSR(SOCK_NTP);
//////	if ((RSR_len = getSn_RX_RSR(SOCK_NTP)) > 0){
//////		printf("\r\n Tenemos datos del servidor NTP\r\n");
//////	}
//
//
//
//	return ERROR;
//
//}
//
//
//bool_t opSetRtcViaNtpViaUPD(){
//
//	return ERROR;
//
//}
//
