#include "../inc/da_regions.h"
#include "../inc/da_func_process.h"//Funciones estadisticas
#include "socket.h"
#include "w5100.h"
#include "ftpc.h"
//#include "my_spi.h"

#define MaxVoltajeBatery	12.43//12.23
#define MaxADCValue			540//997
#define SOCK	1
//client information
uint16_t PortLocal = 30001;
uint8_t mac[6] = { 0x18, 0x66, 0xDA, 0x01, 0x02, 0x03 };
uint8_t mac1[6];
uint8_t IPLocal[4] = {10,10,13,121};
//uint8_t IPLocal[4] = {192,168,0,22};
uint8_t IPLocal1[4];
uint8_t Gat[4] = {10,10,13,1};
//uint8_t Gat[4] = {192,168,0,1};
uint8_t Gat1[4];
uint8_t MASKSUB[4] = {255,255,255,0};
uint8_t MASKSUB1[4];


////Server information
uint8_t _FTP_destip[4] = {129,6,15,29}; //NIST NTP
//uint8_t _FTP_destip[4] = {192,168,5,5};
//uint8_t _FTP_destip[4] = {10,10,13,157};
//uint8_t _FTP_destip[4] = {200,16,116,5};
uint16_t _FTP_destport = 37; //NTP port
//uint16_t _FTP_destport = 21;


bool_t opConfigSOCKET(){
	char auxiliarBuffer[50];
//	Configurar Cliente
	setIMR(0x00);
	//Reset registers
	setIMR(MR_RST);
	uint8_t registroModo = getIMR();
	setGAR(Gat);
	getGAR(Gat1);
	setSHAR(mac);
	getSHAR(mac1);
	setSUBR(MASKSUB);
	getSUBR(MASKSUB1);
	setSIPR(IPLocal);
	getSIPR(IPLocal1);



//	ftpc_init(IPLocal);
//	ftpc_run(auxiliarBuffer);

	setSn_MR(SOCK, Sn_MR_TCP);
	setSn_PORT(SOCK,PortLocal);
	setSn_CR(SOCK,Sn_CR_OPEN);
//
//	//Conecto al FTP
	setSn_DIPR(SOCK, _FTP_destip);
	setSn_DPORT(SOCK, _FTP_destport);
	setSn_CR(SOCK,Sn_CR_CONNECT);

	delay(1500);


	//	socket(CTRL_SOCK, Sn_MR_TCP, FTP_destport, 0x0);
//	connect(CTRL_SOCK, FTP_destip, FTP_destport))
//	//setSn_CR(sn,Sn_CR_CONNECT);
//	setSn_DIPR(CTRL_SOCK,FTP_destip);
	getSn_DIPR(SOCK, _FTP_destip);
	uint8_t ret = getSn_MR(SOCK);
	uint16_t ret2 = getSn_PORT(SOCK);
//	uint8_t ret3 = getSn_CR(SOCK);
	uint16_t ret4 = getSn_DPORT(SOCK);
	uint8_t ret5 = getSn_SR(SOCK);
//	delay(1000);
	uint16_t ret6 = getSn_RX_RSR(SOCK);





//	uint8_t ret5 = getSn_SR(SOCK);
//	connect(CTRL_SOCK, FTP_destip, FTP_destport)
//	// see status register
////	setSn_DIPR(CTRL_SOCK, FTP_destip);
//	setSn_CR(CTRL_SOCK,0x02);

//	setGAR(FTP_ip);
//	getGAR(FTP_ip);


	floatToString(_FTP_destip[0],auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\n ServerFTP \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );
//	floatToString(FTP_ip[0],auxiliarBuffer,0);
//	uartWriteString( UART_USB, "\r\n" );
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );
	floatToString(ret,auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\nModo  \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(ret2,auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\nPuerto \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );
//	floatToString(ret3,auxiliarBuffer,0);
//	uartWriteString( UART_USB, "\r\n" );
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );
	floatToString(ret4,auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\nDestPort \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(ret5,auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\n StatusRegister \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );
	floatToString(ret6,auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\nRX Received Size \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );
	/*-------------------------------*/
	floatToString(ret6,auxiliarBuffer,0);
	uartWriteString( UART_USB, "\r\nRX Received Size \t" );
	uartWriteString( UART_USB, auxiliarBuffer );
	uartWriteString( UART_USB, "\r\n" );

//	char auxiliarBuffer[7];
//	int8_t retSocket;
//	uint8_t ret;
//	retSocket = socket(CTRL_SOCK, Sn_MR_TCP, FTP_destport, 0x0);
//	ret = getSn_MR(CTRL_SOCK);
////	ret = getSn_SR(CTRL_SOCK);
//	floatToString(ret,auxiliarBuffer,0);
//	uartWriteString( UART_USB, "\r\n" );
////	uartWriteString( UART_USB, "Respuesta Socket\r\n" );
//	uartWriteString( UART_USB, auxiliarBuffer );


	return ERROR;
}
bool_t opConfigFTPSocket(){
	return ERROR;
}

bool_t TransmitirFTPViaEthernet(uint32_t * size, int32_t * NumberMesuare){
	return ERROR;
}


// Todas las variables o punteros que se declaran en la maquina de estados se usaran en estas funciones
void _opLED(  uint16_t LEDNumber,  BOOL_8 State, uint16_t * n){

	//Esta funci�n es de la biblioteca sapi.h, ver como esta implementada
	gpioWrite( (LEDR + LEDNumber), State );
	*n +=2;
	if((*n) > 4){
		gpioWrite( LEDB, State );
	}
	if((*n) > 7){
		gpioWrite( LEDR, State );
	}
	if((*n) > 10){
		gpioWrite( LEDG, State );
	}

}

/*Region Muestreo */
// Funciones de adquicision de datos
void opAdquirirDNB(real32_t* muestraVoltNB ){//puntero a muestra nivel de bateria

	adcConfig( ADC_ENABLE );
	/* Variable para almacenar el valor leido del ADC CH1 */
	uint16_t muestra = 0;
	*muestraVoltNB = 0;
	muestra = adcRead( CH1 );
	*muestraVoltNB = muestra*(MaxVoltajeBatery/MaxADCValue);
	adcConfig( ADC_DISABLE );
}


void opBufferRS485Reset(){
	/*cuando la llamo reinicia la FIFO*/
	//uartConfig( UART_USB, 115200 ); De Prueba
	uartConfig( UART_485, 9600 );
	// Seteo un callback al evento de recepcion y habilito su interrupcion
	uartCallbackSet(UART_485, UART_RECEIVE, opAdquirirDV2, NULL);
	// Habilito todas las interrupciones de UART_USB
	uartInterrupt(UART_485, true);

}

void opAdquirirDV2( void *noUsado ) //Esta se llama  en el callbackSet
{
	uint8_t receiveByte = uartRxRead( UART_485 );
	//uartWriteByte( UART_USB, receiveByte);
	//uartWriteString( UART_USB, "Entre  \r\n" );
	*ptr = receiveByte;
	ptr++;
	gpioWrite( LED3, OFF );
	if(receiveByte =='\r'){
		//uartWriteString( UART_USB, uartBuffer);

		opPreprocesoDeltaOHM2();/*ac� parseo el String de datos*/
		ptr=uartBuffer; //Reseteo el puntero para el siguiente string
	gpioWrite( LED3, ON );
	}

}
void opPreprocesoDeltaOHM2(){
	//Primero parceo lo que hay en buffer (28.30 359.3 998.3<CR><LF>)
	char *	delimitador = " ";
	char* token;
	char* rest = uartBuffer;
    uint16_t i = 0;
    static char auxiliarBuffer[100];
    //Obtengo el largo del vector donde se guardan mis datos de interes en este caso 3. Intensidad, Direccion y Presion
    //uint32_t lenghtDeltaOhm = sizeof(dataWind)/sizeof(dataWind[0]);
    while ((token =  (char *)strtok_r(rest, delimitador, &rest)) !=NULL ){
    	//token es un string asi que puedo comparar contra NAN y no covertir
    	if((memcmp(NAN,token,strlen(NAN))) == 0){
    		DataDeltaOhm[i] = NoDato;
    		i++;
    	}else{
    		DataDeltaOhm[i] = (float)atof(token); //Convierto los datos parceados a numeros flotantes
    		i++;

    	}
//    	floatToString(DataDeltaOhm[i],auxiliarBuffer,2);
//		uartWriteString( UART_USB, auxiliarBuffer );
//		uartWriteString( UART_USB, "\r\n" );
    }

}

void opGuardarMuestras2(real32_t* muestraVoltNB){

	static char auxiliarBuffer[100];
	int i;
	int n=7;
	//gpioToggle( LED1 );

	//gpioWrite( LEDB, ON );
//int n = sizeof(dataWind) / sizeof(real32_t);
//	for(i = 0; i < n; i++){
//		floatToString(DataDeltaOhm[i],auxiliarBuffer,2);
//		uartWriteString( UART_USB, auxiliarBuffer );
//		uartWriteString( UART_USB, "\r\n" );
//	}
//	floatToString(*muestraVoltNB,auxiliarBuffer,2);
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );

}


/*Region Procesamiento */

void opAcumular(uint16_t * NumMuestra,real32_t * MuestraVolt ){
	static char auxiliarBuffer[100];
	AcumIntensidad[*NumMuestra] = DataDeltaOhm[0];
	AcumDireccion[*NumMuestra] = DataDeltaOhm[1];
	AcumPresion[*NumMuestra] = DataDeltaOhm[2];
	AcumTemp[*NumMuestra] = DataDeltaOhm[3];
	NvBateria[*NumMuestra] = *MuestraVolt;
//	uartWriteString( UART_USB, "Presion:" );
//	floatToString(AcumPresion[*NumMuestra],auxiliarBuffer,2);
//	uartWriteString( UART_USB, auxiliarBuffer );
//	uartWriteString( UART_USB, "\r\n" );
}


void opProceso( uint32_t * size){
	//char aux[150];
	char miBuffer1[7];
	char miBuffer2[7];
	char miBuffer3[7];
	char miBuffer4[7];
	char miBuffer5[7];
	//TableToFTP = aux;
	//real32_t Tabla[16];

	float velocidadInst = AcumIntensidad[1];
	float velocidadMax = maxValue(AcumIntensidad,sizeof(AcumIntensidad)/sizeof(AcumIntensidad[0]));
	float velocidadMin = minValue(AcumIntensidad,sizeof(AcumIntensidad)/sizeof(AcumIntensidad[0]));
	float velocidadPromedio = AverageValue(AcumIntensidad,sizeof(AcumIntensidad)/sizeof(AcumIntensidad[0]));

	float direccionInst = AcumDireccion[1];
	float direccionMax = maxValue(AcumDireccion,sizeof(AcumDireccion)/sizeof(AcumDireccion[0]));
	float direccionMin = minValue(AcumDireccion,sizeof(AcumDireccion)/sizeof(AcumDireccion[0]));
	float direccionPromedio = AverageValue(AcumDireccion,sizeof(AcumDireccion)/sizeof(AcumDireccion[0]));

	float presionInst = AcumPresion[1];
	float presionMax = maxValue(AcumPresion,sizeof(AcumPresion)/sizeof(AcumPresion[0]));
	float presionMin = minValue(AcumPresion,sizeof(AcumPresion)/sizeof(AcumPresion[0]));
	float presionPromedio = AverageValue(AcumPresion,sizeof(AcumPresion)/sizeof(AcumPresion[0]));

	float TempInst = AcumTemp[1];
	float TempMax = maxValue(AcumTemp,sizeof(AcumTemp)/sizeof(AcumTemp[0]));
	float TempMin = minValue(AcumTemp,sizeof(AcumTemp)/sizeof(AcumTemp[0]));
	float TempPromedio = AverageValue(AcumTemp,sizeof(AcumTemp)/sizeof(AcumTemp[0]));

	float nv_bateriaInst = NvBateria[1];
	float nv_bateriaMax = maxValue(NvBateria,sizeof(NvBateria)/sizeof(NvBateria[0]));
	float nv_bateriaMin = minValue(NvBateria,sizeof(NvBateria)/sizeof(NvBateria[0]));
	float nv_bateriaPromedio = AverageValue(NvBateria,sizeof(NvBateria)/sizeof(NvBateria[0]));

//	real32_t Tabla[16] ={	velocidadInst,velocidadMax,velocidadMin,velocidadPromedio,
//							direccionInst,direccionMax,direccionMin,direccionPromedio,
//							presionInst,presionMax,presionMax,presionMin,presionPromedio,
//							nv_bateriaInst,nv_bateriaMax,nv_bateriaMin,nv_bateriaPromedio};

//	real32_t Tabla[4] ={	velocidadPromedio,
//							direccionPromedio,
//							presionPromedio,
//							nv_bateriaPromedio};
	if (velocidadInst == NoDato ){
		sprintf(miBuffer1,"%s","NAN");
	}else{
		floatToString(velocidadInst,miBuffer1,2);
	}
	if (direccionInst == NoDato ){
		sprintf(miBuffer2,"%s","NAN");
	}else{
		floatToString(direccionInst,miBuffer2,1);
	}
	if (presionInst == NoDato ){
		sprintf(miBuffer3,"%s","NAN");
	}else{
		floatToString(presionInst,miBuffer3,1);
	}
	if (TempInst == NoDato ){
		sprintf(miBuffer4,"%s","NAN");
	}else{
		floatToString(TempInst,miBuffer4,2);
	}
	if (nv_bateriaInst == NoDato ){
		sprintf(miBuffer5,"%s","NAN");
	}else{
		floatToString(nv_bateriaInst,miBuffer5,2);
	}

	sprintf(TableToFTP, "%s;%s;%s;%s;%s", miBuffer1,miBuffer2 , miBuffer3,miBuffer4,miBuffer5);
	//ver si va strlen
	*size = (uint32_t)strlen(TableToFTP);
//	uartWriteString( UART_USB, TableToFTP );
//	uartWriteString( UART_USB, "\r \n" );
//	gpioWrite( LED3, ON );
//	size_t size = sizeof(Tabla)/sizeof(Tabla[0]);
//	stringToSentToFTP = converTableToStringToSend(Tabla,size);


}


/* Region Config GPRS and FTP*/

bool_t opConfigGPRS(){
	gpioWrite( LED1, OFF );
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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS


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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS

	uartWriteString( UART_232, "AT+SAPBR=1,1");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		uartWriteString( UART_232, "AT+SAPBR=0,1");
		uartWriteString( UART_232, "\r\n");//Cierro portadora y salgo
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
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
	return OK;
}
bool_t opConfigFTP(){
	gpioWrite( LED2, OFF );
	uartWriteString( UART_USB, "ConfiguroFTP\r\n");
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
			return ERROR;
		}
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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	uartWriteString( UART_232, "AT+FTPSERV=\"ftp.smn.gov.ar\"");
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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	uartWriteString( UART_232, "AT+FTPUN=\"estaut\"");
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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	uartWriteString( UART_232, "AT+FTPPW=\"estacionesautomaticas17\"");
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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	uartWriteString( UART_232,"AT+FTPPUTPATH=\"/EMA_LABO_CLI/SIM800L/AnemometroAeroparque/\"");
	uartWriteString( UART_232, "\r\n");/*Los comandos AT van con \n */
	delay(300);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

	if(dato != '0'){
		uartWriteString( UART_USB, "Respuesta de AT\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS


	gpioWrite( LED2, ON );/*FTP Config ok*/
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
		return ERROR;
	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	//Configuramos el nombre del archivos
	uartWriteString( UART_232, "AT+FTPPUT =1");/*Abro Sesion */
	uartWriteString( UART_232, "\r\n");
	delay(11000);//Importante esperara ac�
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
	delay(10000);//Importante esperara ac�
	if(dato == '4'){
		//gpioWrite( LED2, ON );
		//uartWriteByte( UART_USB, dato);
		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
		uartWriteString( UART_232, "AT+FTPPUT=2,0");
		uartWriteString( UART_232, "\n");
		uartWriteString( UART_USB, "Respuesta de AT+FTPPUT=size\r\n");
		uartWriteByte( UART_USB, dato);
		uartWriteString( UART_USB, "\r\n");
		return ERROR;
	}
	char ParaEnviar[*size];
	sprintf(ParaEnviar, "%s", TableToFTP);
	uartWriteString( UART_USB, ParaEnviar);
	uartWriteString( UART_USB, "\r\n");
	uartWriteString( UART_232, ParaEnviar );
	/*envio*/
//	uartWriteString( UART_232, "12345");
//	uartWriteString( UART_232, "\n");
	uartWriteString( UART_232, ParaEnviar);
	uartWriteString( UART_232, "\n");
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");



	uartConfig( UART_232, 115200 );
	uartWriteString( UART_USB, "AT+FTPPUT=2,0" );
	uartWriteString( UART_USB, "\r\n");
	uartWriteString( UART_232, "AT+FTPPUT=2,0");
	uartWriteString( UART_232, "\r\n");/*(No mas datos cierro) */
	delay(2000);
	uartReadByte( UART_232, &dato );
	uartWriteByte( UART_USB, dato);
	uartWriteString( UART_USB, "\r\n");

//	if(dato != '0'){
//		//gpioWrite( LED3, ON );
//		//uartWriteByte( UART_USB, dato);
//		//uartWriteString( UART_232, "AT+SAPBR=0,1");//Cierro portadora y salgo
//		uartWriteString( UART_USB, "No pude cerrarRespuesta de AT\r\n");
//		uartWriteByte( UART_USB, dato);
//		uartWriteString( UART_USB, "\r\n");
//		return ERROR;
//	}
	uartConfig( UART_232, 115200 ); //Limpio la Uart FIFOS
	gpioWrite( LEDB, ON );
	delay(1000);
	gpioWrite( LEDB, OFF );

	return OK;

}



