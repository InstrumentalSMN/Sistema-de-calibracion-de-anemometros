#include "../inc/da_processing.h"//Funciones estadisticas
#include "../inc/da_transmision.h"
#include "../inc/da_rtc.h"

//#include "my_spi.h"






//client information
uint16_t PortLocal = 30001;
uint16_t PortLocal2 = 30002;
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


//Server information WEBSOCK
uint8_t ipServer[4] = {10,10,13,154};
uint16_t server_port = 8000;

//Server information DATASOCK
uint8_t remoteIp[4];
uint16_t  remotePort;
uint8_t ret5;
char dbuf[550];

//Recepción de mensajes del server WEBSOCKET

void opEnableRxWebSocket(){
////	MySpi_Wiz_Init(SPI0);
//	spiCallbackSet(SPI0,SPP_RECEIVE, opReceptionMessage, NULL );
//	spiInterrupt(SPI0, true);

}

void opReceptionMessage( void *data ){ //Esta se llama  en el callbackSet
	printf("\r\n Llego un mensaje del server");

}


void opConfigSocket(){

	DesconectarSocket(DATA_SOCK_FTP);
	DesconectarSocket(WEB_SOCK);
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

bool_t opInitWebSocket(){

	setSn_MR(WEB_SOCK, Sn_MR_TCP);
	setSn_PORT(WEB_SOCK,PortLocal2);
	setSn_CR(WEB_SOCK,Sn_CR_OPEN);
	ret5 = getSn_SR(WEB_SOCK);
	if(ret5 != SOCK_INIT){
		printf("Salgo error 1\r\n ");
		DesconectarSocket(WEB_SOCK);
		return ERROR;
	}
	setSn_DIPR(WEB_SOCK, ipServer);
	setSn_DPORT(WEB_SOCK, server_port);
	setSn_CR(WEB_SOCK,Sn_CR_CONNECT);
	return OK;

}
// Hay un delay no bloqueante de 50 ms para pasar a la funcion de abajo
bool_t opConnectToWebSocket(){

	ret5 = getSn_SR(WEB_SOCK);
	if(ret5 != SOCK_ESTABLISHED){
		printf("Salgo error 2\r\n");
		DesconectarSocket(WEB_SOCK);
		gpioWrite( GPIO8, ON );
		gpioWrite( GPIO7, OFF );
		return ERROR;

	}
	//msj de conexión
	char aux[200] = "{\'message\':\'hola\'}";
	char host[] = "10.10.13.154";
	char path[] = "/ws/socket-server/";
	char key[] = "x3JJHMbDL1EzLkh9GBhXDw==";
	char header[256];
	sprintf(header,	"GET %s HTTP/1.1\r\n"
					"Host: %s:%d\r\n"
                    "Upgrade: websocket\r\n"
                    "Connection: Upgrade\r\n"
                    "Sec-WebSocket-Key: %s\r\n"
                    "Sec-WebSocket-Version: 13\r\n"
					"\r\n", path, host, server_port, key);
	int32_t a = strlen(header);
	int32_t len = send(WEB_SOCK, header, strlen(header));
	if (a != len){ //valido que se envie correctamente
		printf("Mensaje no enviado\r\n");
		gpioWrite( GPIO8, ON );
		gpioWrite( GPIO7, OFF );
		return ERROR;
	}
//	//Recibo mensaje de handShake si no espero uno ms se rompe, pero con interrupciones ya no tendre ese problema
//	delay(100);
//	long ret = 0;
//	uint16_t size = getSn_RX_RSR(WEB_SOCK);
////	memset(dbuf, 0, _MAX_SS);
//	if(size > _MAX_SS) size = _MAX_SS - 1;
//	ret = recv(WEB_SOCK,dbuf,size);
//	dbuf[ret] = '\0';
//	printf("\r\n%s",dbuf);
//	memset(dbuf, 0, _MAX_SS);
	gpioWrite( GPIO8, OFF );
	gpioWrite( GPIO7, ON );
	return OK;
}

bool_t KeepAlive(uint16_t * sizeBuffServer){

	char aux[] = "{\"message\":\"\"}";;
	char message[140];
	encodeMessage125(aux,message);

	uint32_t len = send(WEB_SOCK, message, strlen(aux) + 6);
	if (len != strlen(aux) + 6){
		printf("No envio bien los datos\r\n");
		return ERROR;
	}
	//Rutina de recepcion de mensajes por polling
	long ret = 0;
	*sizeBuffServer = getSn_RX_RSR(WEB_SOCK);
	memset(dbuf, 0, _MAX_SS);
	if(*sizeBuffServer > _MAX_SS) *sizeBuffServer = _MAX_SS - 1;
	ret = recv(WEB_SOCK,dbuf,*sizeBuffServer);
	dbuf[ret] = '\0';
//	printf("\r\n%s\r\n",dbuf);
 	return OK;
}

bool_t opTransmitWebSocketEthernet(int32_t * NumberMesuare){


//	char prueba[]="holamundobuenasholamundobuenasholamundobuenasholamundobuenasholamundobuenasholamundobuenasholamundobuenasholamundobuenasholab6";
//	uint32_t len = (uint32_t)strlen(prueba);
	uint32_t len = (uint32_t)strlen(TableToFTP);

	char message[600];
	if(len > 125){
		encodeMessage126(TableToFTP,message);
		len = send(WEB_SOCK, message, strlen(TableToFTP) + 8);
		if (len != strlen(TableToFTP) + 8){
			printf("No envio bien los datos\r\n");
			return ERROR;
		}
	}else{

		encodeMessage125(TableToFTP,message);
		len = send(WEB_SOCK, message, strlen(TableToFTP) + 6);
		if (len != strlen(TableToFTP) + 6){
			printf("No envio bien los datos\r\n");
			return ERROR;
		}
	}


//implementar un codigo que extraiga message con strok (JSON) y luego
//trabajar con las interrupciones de recepcion del SPI
//para recibir los mensajes de aplicaiones de forma asincronida
/////////////////////////////////////////////////////
//	if (MyParserToDATASockeyFTP(dbuf,remoteIp,&remotePort) == -1){  //pasar esta funcion a socket.h o daregion.h
//							//Despues probar de conectarme a ese port que me da y mandar datos
//		printf("Salgo error 3\r\n");
//		printf("Bad port syntax\r\n");
//		DesconectarSocket(WEB_SOCK);
//		return ERROR;
//	}
	//limpio la tabla
	memset(TableToFTP, 0, strlen(TableToFTP));
	next = 0;
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
	close(sn);
	return SOCK_OK;

}


void encodeMessage126(uint8_t * buf, uint8_t * message){

	memset(message, 0, sizeof(message));
	message[0] = 0x81; // Opcode 0x1 y datos enmascarados
	//	uint16_taux = ((uint16_t)strlen(aux)) | 0x8000;
	message[1] = 0xFE;
	uint16_t largo = ((uint16_t)strlen(buf));
	message[2] = (largo & 0xFF00)>>8;
	message[3] = (largo & 0x00FF)>>0;
	uint32_t mask_key = 0x12345678; // Clave de codificación
	//Copio la clave
	message[4] = (mask_key & 0xFF000000)>>24;	//0x12
	message[5] = (mask_key & 0x00FF0000)>>16;	//0x34
	message[6] = (mask_key & 0x0000FF00)>>8;	//0x56
	message[7] = (mask_key & 0x000000FF)>>0;	//0x78
	//	uint32_t mask_key_aux = (mask_key & 0x00FF0000)>>16;
	memcpy(message + 8, buf, strlen(buf));
	//	message[6] = message[6]^0x12;
	//	message[7] = message[7]^0x34;
	//	// Copiar los datos sin enmascarar
	int value = 4;
	for (int i = 8; i < strlen(buf) + 8; i++) {
	//		uint8_t a = ((uint8_t*)&mask_key)[i % 4];
	//	    message[i] ^= ((uint8_t*)&mask_key)[i % 4]; // Aplicar XOR con la clave de codificación
		message[i] ^= message[value];
		value++;
		if(value > 7){
			value = 4;
		}
	}




}


void encodeMessage125(uint8_t * buf, uint8_t * message){

		memset(message, 0, sizeof(message));
		message[0] = 0x81; // Opcode 0x1 y datos enmascarados
		//	uint16_taux = ((uint16_t)strlen(aux)) | 0x8000;
		message[1] = ((uint8_t)strlen(buf)) | 0x80; // Longitud de los datos y seteo el bit de enmascaramiento
		uint32_t mask_key = 0x12345678; // Clave de codificación
		//Copio la clave
		message[2] = (mask_key & 0xFF000000)>>24;	//0x12
		message[3] = (mask_key & 0x00FF0000)>>16;	//0x34
		message[4] = (mask_key & 0x0000FF00)>>8;	//0x56
		message[5] = (mask_key & 0x000000FF)>>0;	//0x78
	//	uint32_t mask_key_aux = (mask_key & 0x00FF0000)>>16;
		memcpy(message + 6, buf, strlen(buf));
	//	message[6] = message[6]^0x12;
	//	message[7] = message[7]^0x34;
	//	// Copiar los datos sin enmascarar
		int value = 2;
		for (int i = 6; i < strlen(buf) + 6; i++) {
	//		uint8_t a = ((uint8_t*)&mask_key)[i % 4];
	//	    message[i] ^= ((uint8_t*)&mask_key)[i % 4]; // Aplicar XOR con la clave de codificación
			message[i] ^= message[value];
			value++;
			if(value > 5){
				value = 2;
			}
		}


}

void backUpData(){
	//Backupear los datos en JSON
	uint32_t size = (uint32_t)strlen(TableToFTP);
	TableToFTP[size]='\r';
	TableToFTP[size+1]='\n';
	printf("\r\n Estoy haciendo backup, revisa conexion con el WebSocket");
	next = (uint32_t)strlen(TableToFTP);

}

////Se usa en transmision.c
//int MyParserToDATASockeyFTP(char * arg, uint8_t  * remoteIp ,  uint16_t * remotePort)
//{
//	int i;
//	char* tok=0;
//	strtok(arg,"(");
//	for (i = 0; i < 4; i++)
//	{
//		if(i==0) tok = strtok(NULL,",\r\n");
//		else	 tok = strtok(NULL,",");
//		remoteIp[i] = (uint8_t)atoi(tok);
//		if (!tok){
//			printf("bad pport : %s\r\n", arg);
//			return -1;
//		}
//	}
//	*remotePort = 0;
//	for (i = 0; i < 2; i++){
//		tok = strtok(NULL,",\r\n");
//		*remotePort <<= 8;
//		*remotePort += atoi(tok);
//		if (!tok){
//			printf("bad pport : %s\r\n", arg);
//			return -1;
//		}
//	}
////	printf("ip : %d.%d.%d.%d, port : %d\r\n", remoteIp[0], remoteIp[1], remoteIp[2], remoteIp[3], *remotePort);
//	return 0;
//}




