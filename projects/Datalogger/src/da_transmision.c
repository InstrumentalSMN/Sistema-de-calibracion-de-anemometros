#include "../inc/da_processing.h"//Funciones estadisticas
#include "../inc/da_transmision.h"
#include "../inc/da_rtc.h"

//#include "my_spi.h"






//client information
uint16_t PortLocal = 30001;
uint8_t mac[6] = { 0x18, 0x66, 0xDA, 0x01, 0x02, 0x03 };
uint8_t mac1[6];
//uint8_t IPLocal[4] = {10,10,13,121};
uint8_t IPLocal[4] = {10,10,13,70}; // IP EMA instrumental
//uint8_t IPLocal[4] = {192,168,0,22}; //casa
uint8_t IPLocal1[4];
uint8_t Gat[4] = {10,10,13,1};
//uint8_t Gat[4] = {192,168,0,1}; //casa
uint8_t Gat1[4];
uint8_t MASKSUB[4] = {255,255,255,0};
uint8_t MASKSUB1[4];


//Server information CONTROLSOCK
//uint8_t _FTP_destip[4] = {129,6,15,29}; //NIST NTP
uint8_t _FTP_destip_Local[4] = {192,168,5,5};
//uint8_t _FTP_destip[4] = {10,10,13,157};
uint8_t _FTP_destip_Global[4] = {200,16,116,5};
//uint16_t _FTP_destport = 37; //NTP port
uint16_t _FTP_destport = 21;
//Server information DATASOCK
uint8_t remoteIp[4];
uint16_t  remotePort;
char auxiliarBuffer[50];
uint8_t ret1;
uint8_t ret2;
uint8_t ret3;
uint8_t ret4;
uint8_t ret5;
uint8_t dat[500];
char dbuf[550];



void opConfigSocket(){


	close(DATA_SOCK_FTP);
	close(CTRL_SOCK_FTP);
//	disconnect(CTRL_SOCK_FTP);//Fundamental desconectame si salgo con ERROR
//	disconnect(DATA_SOCK_FTP);
	//	Configurar Cliente
	uint8_t registroModo = getMR();
	setMR(MR_RST);
	registroModo = getMR();
	setGAR(Gat);
	getGAR(Gat1);
	setSHAR(mac);
	getSHAR(mac1);
	setSUBR(MASKSUB);
	getSUBR(MASKSUB1);
	setSIPR(IPLocal);
	getSIPR(IPLocal1);


}

bool_t opSetFTPSocketCtrl(){
	//Conecto al FTP
	//Configurar Socket de control para el FTP
//	ftpc_init(IPLocal);
//	ftpc_run(auxiliarBuffer);

	setSn_MR(CTRL_SOCK_FTP, Sn_MR_TCP);
	setSn_PORT(CTRL_SOCK_FTP,PortLocal);
	setSn_CR(CTRL_SOCK_FTP,Sn_CR_OPEN);
//	delay(5); // Necesario para recibir respuestas
//	ret1 = getSn_MR(CTRL_SOCK_FTP);
//	ret2 = getSn_PORT(CTRL_SOCK_FTP);
	ret5 = getSn_SR(CTRL_SOCK_FTP);
	if(ret5 != SOCK_INIT){
		printf("\r\n Salgo error 1");
		DesconectarSocket(CTRL_SOCK_FTP);
		return ERROR;
	}
	setSn_DIPR(CTRL_SOCK_FTP, _FTP_destip_Local);
	setSn_DPORT(CTRL_SOCK_FTP, _FTP_destport);
	setSn_CR(CTRL_SOCK_FTP,Sn_CR_CONNECT);
	return OK;

}
// Hay un delay no bloqueante de 50 ms para pasar a la funcion de abajo
bool_t opParametersFTPSocket(){

//	ret4 = getSn_DPORT(CTRL_SOCK_FTP);
	ret5 = getSn_SR(CTRL_SOCK_FTP);
	//	delay(2000);
	//	ret5 = getSn_SR(CTRL_SOCK_FTP);
	if(ret5 != SOCK_ESTABLISHED){
		printf("\r\n Salgo error 2");
		DesconectarSocket(CTRL_SOCK_FTP);
		return ERROR;
		//gpioWrite( LED2, ON );
	}
	char mystr[200];
	sprintf(mystr,"USER %s\r\n",USER);
	send(CTRL_SOCK_FTP, mystr, strlen(mystr));
//	setSn_CR(CTRL_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(CTRL_SOCK_FTP));

	sprintf(mystr,"PASS %s\r\n",PASS);
	send(CTRL_SOCK_FTP, mystr, strlen(mystr));
//	setSn_CR(CTRL_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(CTRL_SOCK_FTP));

	sprintf(mystr,"PASV\r\n"); // Pasive mode
	send(CTRL_SOCK_FTP, mystr, strlen(mystr));
//	setSn_CR(CTRL_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(CTRL_SOCK_FTP));

	sprintf(mystr,"TYPE A\r\n"); //Ascii mode
	send(CTRL_SOCK_FTP, mystr, strlen(mystr));
//	setSn_CR(CTRL_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(CTRL_SOCK_FTP));

	sprintf(mystr,"CWD %s\r\n",PATH); //Eligo la ruta
	send(CTRL_SOCK_FTP, mystr, strlen(mystr));
//	setSn_CR(CTRL_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(CTRL_SOCK_FTP));

	//	floatToString(_FTP_destip_Local[0],auxiliarBuffer,0);
	//	uartWriteString( UART_USB, "\r\n ServerFTP \t" );
	//	uartWriteString( UART_USB, auxiliarBuffer );
	//	uartWriteString( UART_USB, "\r\n" );
	//	floatToString(ret4,auxiliarBuffer,0);
	//	uartWriteString( UART_USB, "\r\nDestPort \t" );
	//	uartWriteString( UART_USB, auxiliarBuffer );
	//	uartWriteString( UART_USB, "\r\n" );
	//	floatToString(ret5,auxiliarBuffer,0);
	//	uartWriteString( UART_USB, "\r\n StatusRegister \t" );
	//	uartWriteString( UART_USB, auxiliarBuffer );
	//	uartWriteString( UART_USB, "\r\n" );

	return OK;
}


// Hay un delay no bloqueante de 50 ms para pasar a la funcion de abajo

bool_t opSetSocketData(){
	long ret = 0;
//	delay(10);//Necesario para recibir una respuesta del Server
	uint16_t size = getSn_RX_RSR(CTRL_SOCK_FTP);

	//Recibo MSJ del socket de control e imprimo
	memset(dbuf, 0, _MAX_SS);
	if(size > _MAX_SS) size = _MAX_SS - 1;
	ret = recv(CTRL_SOCK_FTP,dbuf,size);
	dbuf[ret] = '\0';
//	printf("Rcvd Command fot Control Sock: %s\r\n", dbuf);
	if (MyParserToDATASockeyFTP(dbuf,remoteIp,&remotePort) == -1){  //pasar esta funcion a socket.h o daregion.h
							//Despues probar de conectarme a ese port que me da y mandar datos
		printf("\r\n Salgo error 3");
		printf("Bad port syntax\r\n");
//		close(DATA_SOCK_FTP);
		DesconectarSocket(CTRL_SOCK_FTP);
		return ERROR;
	}

	//Configurar Socket de control para el FTP

	setSn_MR(DATA_SOCK_FTP, Sn_MR_TCP);
	setSn_PORT(DATA_SOCK_FTP,PortLocal);
	setSn_CR(DATA_SOCK_FTP,Sn_CR_OPEN);
//	delay(5); // Necesario para recibir respuestas
	ret3 = getSn_SR(DATA_SOCK_FTP);
	if(ret3 != SOCK_INIT){
		printf("\r\n Salgo error 4");
		DesconectarSocket(CTRL_SOCK_FTP);
		DesconectarSocket(DATA_SOCK_FTP);
		return ERROR;
	}
//	//Conecto al FTP
	setSn_DIPR(DATA_SOCK_FTP, remoteIp);
	setSn_DPORT(DATA_SOCK_FTP, remotePort);
	setSn_CR(DATA_SOCK_FTP,Sn_CR_CONNECT);
	return OK;
}
// Hay un delay no bloqueante de 50 ms para pasar a la funcion de abajo
bool_t opCheckSocketData(){
//	ret1 = getSn_MR(DATA_SOCK_FTP);
//	ret2 = getSn_PORT(DATA_SOCK_FTP);
//	ret4 = getSn_DPORT(DATA_SOCK_FTP);
	ret5 = getSn_SR(DATA_SOCK_FTP);
	if(ret5 != SOCK_ESTABLISHED){
		printf("\r\n Salgo error 5");
		DesconectarSocket(DATA_SOCK_FTP);
		DesconectarSocket(CTRL_SOCK_FTP);
		return ERROR;
		//gpioWrite( LED2, ON );
	}
	return OK;
}


void OpenFileFTP( int32_t * NumberMesuare){


	char mystr[200];
	char aux[200];
	sprintf(mystr, "ST%d", *NumberMesuare);
//	rtcRead( &rtc );
//	sprintf( mystr,"%02d-%02d-%04d,%02d:%02d:%02d",
//	              rtc.mday, rtc.month, rtc.year,
//	              rtc.hour, rtc.min, rtc.sec);
	//Agrego el numero de medicion A futuro se debera agregar el RTC
	sprintf(aux,"%s-%s", mystr,"DeltaOHM_022274.txt");
	sprintf(mystr,"STOR %s\r\n", aux);
	send(CTRL_SOCK_FTP,mystr, strlen(mystr));


//	setSn_CR(CTRL_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(CTRL_SOCK_FTP));

	// en la variable de estructura rtc te queda la fecha/hora actual

	      // Envio por UART de forma humanamente legible
	      // %02d == %d y ademas completa con 2 0 a izquierda


//	uartWriteString( UART_USB, TableToFTP );
//	uartWriteString( UART_USB, "\r \n" );

}
//bool_t TransmitirFTPViaEthernet(uint32_t * size){
bool_t TransmitirFTPViaEthernet(){

//	char ParaEnviar[*size];

//	sprintf(ParaEnviar, "%s", TableToFTP);
	//Lo deberia hacer en transmision

//	sprintf(dat,"%s",ParaEnviar); //Eligo la ruta
	uint32_t size = (uint32_t)strlen(TableToFTP);
//	delay(100);
	send(DATA_SOCK_FTP, TableToFTP, size);
//	uint32_t size = (uint32_t)strlen(TableToFTP);
	if((uint32_t)strlen(TableToFTP) == 0){
		printf("\r\n Esta vacio el string por algun motivo");
	}
	rtcRead( &rtc );
		printf( "\r \n Hora de transmision: %02d/%02d/%04d, %02d:%02d:%02d\r\n",
			              rtc.mday, rtc.month, rtc.year,
			              rtc.hour, rtc.min, rtc.sec );
//	setSn_CR(DATA_SOCK_FTP,Sn_CR_SEND);
//	while(getSn_CR(DATA_SOCK_FTP));
//	close(CTRL_SOCK_FTP);

//	Mientras este abierto el dataSock puedo seguir cargando datos
//	al archivo por medio del data_sock_ftp, cuando hago el close

//	close(DATA_SOCK_FTP);
//	close(CTRL_SOCK_FTP);
//	disconnect(CTRL_SOCK_FTP);
//	disconnect(DATA_SOCK_FTP);
	DesconectarSocket(DATA_SOCK_FTP);
	DesconectarSocket(CTRL_SOCK_FTP);
	//limpio la tabla
	memset(TableToFTP, 0, size);
	next = 0;

//	gpioWrite( LEDG, ON );
//	delay(1000);
//	gpioWrite( LEDG, OFF );
	return OK;
}


int8_t DesconectarSocket(uint8_t sn){
//	CHECK_SOCKNUM();
//	CHECK_SOCKMODE(Sn_MR_TCP);
	setSn_CR(sn,Sn_CR_DISCON); // Con este comando me deja cerrar el FTP data y puedo abrir el archivo
	while(getSn_CR(sn));
	setSn_CR(sn,Sn_CR_CLOSE);
   /* wait to process the command... */
	while( getSn_CR(sn) );
	/* clear all interrupt of the socket. */
	setSn_IR(sn, 0xFF);
	while(getSn_SR(sn) != SOCK_CLOSED);
	return SOCK_OK;

}



void backUpData(){
	uint32_t size = (uint32_t)strlen(TableToFTP);
	TableToFTP[size]='\r';
	TableToFTP[size+1]='\n';
	next = (uint32_t)strlen(TableToFTP);

}



//Se usa en transmision.c
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
//	printf("ip : %d.%d.%d.%d, port : %d\r\n", remoteIp[0], remoteIp[1], remoteIp[2], remoteIp[3], *remotePort);
	return 0;
}




/* Region Config GPRS and FTP*/

// Reinicio GRPS

void resetGRPS(){

	gpioWrite( GPIO2, OFF );
	//	printf("\r\n entro");
	delay(2000);
	gpioWrite( GPIO2, ON );
//	delay(1000);
	gpioWrite( LED1, OFF );
	gpioWrite( LED2, OFF );

}




bool_t opConfigGPRS(){

	uartWriteString( UART_USB, "ConfiguroGPRS\r\n");
//	uartConfig( UART_232, 115200 );
//	uartWriteString( UART_232, "AT+FTPPUT=2,0");
	uartConfig( UART_232, 115200 );
	uint8_t dato = 1;
	uartWriteString( UART_232, "AT");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		//uartWriteString( UART_232, "AT+SAPBR=0,1");
		resetGRPS();
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS


	uartWriteString( UART_232, "AT+SAPBR=3,1,\"Contype\",\"GPRS\"");/*Los comandos AT van con \n */
	uartWriteString( UART_232, "\r\n");
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//Poner APN en el inicio

	uartWriteString( UART_232, "AT+SAPBR=3,1,\"APN\",\"igprs.claro.com.ar\"");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS

	uartWriteString( UART_232, "AT+SAPBR=1,1");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(1000);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		uartWriteString( UART_232, "AT+SAPBR=0,1");
		uartWriteString( UART_232, "\r\n");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS

	uartWriteString( UART_232, "AT+SAPBR=2,1");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");
	gpioWrite( LED1, ON );/*GPRS Config OK*/
//	return OK;
	return ERROR;
}



bool_t opConfigFTP(){

	uartWriteString( UART_USB, "ConfiguroFTP\r\n");
//	uartConfig( UART_232, 115200 );
	uint8_t dato = 1;
	char aux[200];
	char aux1[200];
//	uartWriteString( UART_232, "AT");
//	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
//		delay(300);
//		uartReadByte( UART_232, &dato );
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//
//		if(dato != '0'){
//			//gpioWrite( LED3, ON );
//			uartWriteString( UART_USB, "Respuesta de AT\r\n");
//			uartWriteByte( UART_USB, dato);
//			uartWriteString( UART_USB, "\r\n");
//			//uartWriteString( UART_232, "AT+SAPBR=0,1");
//			return ERROR;
//		}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS

	uartWriteString( UART_232, "AT+FTPCID=1");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	//Convierto la IP a un string
	floatToString(_FTP_destip_Global[0],aux1,0);
	sprintf(aux, "AT+FTPSERV=\"%s", aux1);
	floatToString(_FTP_destip_Global[1],aux1,0);
	sprintf(aux, "%s.%s",aux, aux1);
	floatToString(_FTP_destip_Global[2],aux1,0);
	sprintf(aux, "%s.%s",aux, aux1);
	floatToString(_FTP_destip_Global[3],aux1,0);
	sprintf(aux, "%s.%s\"",aux, aux1);
	printf("Esta es la IP: %s\r\n",aux);

	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//	uartWriteString( UART_232, "AT+FTPSERV=\"200.16.116.5\"");
	uartWriteString( UART_232, aux);
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	sprintf(aux, "AT+FTPUN=\"%s\"", USER);
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//	uartWriteString( UART_232, "AT+FTPUN=\"estaut\"");
	uartWriteString( UART_232,aux);
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	sprintf(aux, "AT+FTPPW=\"%s\"", PASS);
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//	uartWriteString( UART_232, "AT+FTPPW=\"estacionesautomaticas17\"");
	uartWriteString( UART_232, aux);
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	sprintf(aux, "AT+FTPPUTPATH=\"%s/\"", PATH);
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
//	uartWriteString( UART_232,"AT+FTPPUTPATH=\"/EMA_LABO_CLI/SIM800L/AnemometroAeroparque/\"");
	uartWriteString( UART_232,aux);
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS


	gpioWrite( LED2, ON );/*FTP Config ok*/
//	return ERROR;
	return OK;
}







bool_t TransmitirFTPViaGPRS( uint32_t * size, int32_t * NumberMesuare){

	char mystr[20];
	char aux[200];
//

	uartConfig( UART_232, 115200 );
	uint8_t dato = 1;

	uartWriteString( UART_232, "AT");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
		//uartWriteString( UART_232, "AT+SAPBR=0,1");

	}
	sprintf(mystr, "%d", *NumberMesuare);
	sprintf(aux,"%s%s-%s","AT+FTPPUTNAME=\"", mystr,"DeltaOHM_022274.txt\"");
	uartWriteString( UART_USB, aux );
	uartWriteString( UART_USB, "\r\n");
	uartWriteString( UART_USB, "TransmitoMediciones\r\n");

	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	//Configuramos el nombre del archivos
	uartWriteString( UART_232, aux);/*Nombre del archivo */
	uartWriteString( UART_232, "\r\n");
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	//Configuramos el nombre del archivos
	uartWriteString( UART_232, "AT+FTPPUT =1");/*Abro Sesion */
	uartWriteString( UART_232, "\r\n");
	delay(11000);//Importante esperara acá
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato == '4'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT+FTPPUT =1\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	sprintf(mystr, "%d", *size);
	sprintf(aux,"%s%s","AT+FTPPUT=2,",mystr);
	//uartWriteString( UART_USB, aux ); /*Pido enviar *size elementos*/
	uartWriteString( UART_USB, aux );
	uartWriteString( UART_USB, "\r\n");
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	uartWriteString( UART_232, aux );/*Pido enviar *size elementos*/
	uartWriteString( UART_232, "\n");
	delay(11000);//Importante esperara acá
	if(dato == '4'){
		//gpioWrite( LED2, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_232, "AT+FTPPUT=2,0");
		uartWriteString( UART_232, "\n");
		uartWriteString( UART_USB, "Respuesta de AT+FTPPUT=size\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	uartWriteString( UART_USB, "\r \n Mi tabla por GPRS:-------" );
	uartWriteString( UART_USB, TableToFTP );
	uartWriteString( UART_USB, "\r \n" );
	char ParaEnviar[*size];
	sprintf(ParaEnviar, "%s", TableToFTP);
	uartWriteString( UART_USB, ParaEnviar);
//	uartWriteString( UART_USB, "\r\n");
//	uartWriteString( UART_232, ParaEnviar );
	/*envio*/
//	uartWriteString( UART_232, "12345");
//	uartWriteString( UART_232, "\n");
	uartWriteString( UART_232, ParaEnviar);
	uartWriteString( UART_232, "\n");
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");


	delay(500);
	uartConfig( UART_232, 115200 );
	uartWriteString( UART_USB, "AT+FTPPUT=2,0" );
	uartWriteString( UART_USB, "\r\n");
	uartWriteString( UART_232, "AT+FTPPUT=2,0");
	uartWriteString( UART_232, "\r\n");/*(No mas datos cierro) */
	delay(2000);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		//gpioWrite( LED3, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_USB, "No pude cerrarRespuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		resetGRPS();
		return ERROR;
	}
	delay(300);
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	uartWriteString( UART_USB, "AT+SAPBR=0,1 respuesta" );
	uartWriteString( UART_USB, "\r\n");
	uartWriteString( UART_232, "AT+SAPBR=0,1");
	uartWriteString( UART_232, "\r\n");/*(No mas datos cierro) */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

//	if(dato != '+' || dato != '0' ){
//		//gpioWrite( LED3, ON );
//		//uartWriteByte( UART_USB, dato);
//		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
//		uartWriteString( UART_USB, "No pude cerrarRespuesta de AT\r\n");
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//		return ERROR;
//	}
	gpioWrite( LEDB, ON );
	delay(1000);
	gpioWrite( LEDB, OFF );
	return OK;

}



