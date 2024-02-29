#include "../inc/da_processing.h"//Funciones estadisticas
#include "../inc/da_transmision.h"
#include "../inc/da_rtc.h"
#include "../inc/da_configDatalogger.h"



//Este diccionario tiene los comandos que enviar la aplicacione WEB, se obtiene el numero y se lo manda al diccionario de
//punteros a funcion para establecer la operacion seteada

char *diccCommands[]={
				"start",
				"stop",
				"setIBC",
				"setPAT",
				"setTimes",
				"NAN"};


char *diccSensoresDeViento[]={
				"HDS500",
				"WMT700",
				"VENTUS"};


bool_t (*diccProcesos[])(config_t * commandConfig)={
							start,
							stop,
							setIBC,
							setPAT,
							setTimes};


/*implementacion de funciones*/
bool_t start(config_t * commandConfig){
//	printf("\r\n%s",commandConfig->command);
	commandConfig->startMesuare  = TRUE;
	return OK;
}
bool_t stop( config_t * commandConfig){
	commandConfig->startMesuare  = FALSE;
	return OK;
}


bool_t setIBC(config_t * commandConfig){
	printf("\r\n%s",commandConfig->command);
	char* tok=0;
	tok = strtok(commandConfig->command,";");
	if(tok == NULL){return ERROR;}
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->AnemoIBC.Uart = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->AnemoIBC.LED = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->AnemoIBC.BaudRate = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	int j;
    for (j = 0; diccSensoresDeViento[j] != NULL; j++) {
        if (strcmp(diccSensoresDeViento[j], tok) == 0) {
            break;
        }
    }
    printf("\r\nValor del sensor IBC %d",j);
//	deberia agregar un paso extra donde a traves del string WMT700 me de en tipo enumerativo
//	commandConfig->AnemoIBC.Sensor = atoi(tok);
	commandConfig->AnemoIBC.Sensor = j;
//	setIBC;5;3;9600;0 (232,LED1,BAUD9600,DELTA_OHM)
	return OK;
}

bool_t setPAT(config_t * commandConfig){
	//	setPAT;1;4;9600;1 (485,LED2,BAUD9600,WMT700)
	printf("\r\n%s",commandConfig->command);
	char* tok=0;
	tok = strtok(commandConfig->command,";");
	if(tok == NULL){return ERROR;}
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->AnemoPAT.Uart = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->AnemoPAT.LED = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->AnemoPAT.BaudRate = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	int j;
	for (j = 0; diccSensoresDeViento[j] != NULL; j++) {
		if (strcmp(diccSensoresDeViento[j], tok) == 0) {
			break;
		}
	}
	printf("\r\nValor del sensor PAT %d",j);
	commandConfig->AnemoPAT.Sensor = j;
//	commandConfig->AnemoPAT.Sensor = atoi(tok);
	return OK;
}


bool_t setTimes(config_t * commandConfig){
//	setTimes;1;10 (tiempoMuestra;tiempoTabla)
//	printf("\r\n%s",commandConfig->command);


//	printf("\r\n%s",commandConfig->command);
	char* tok=0;
	tok = strtok(commandConfig->command,";");
	if(tok == NULL){return ERROR;}
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->IntervaloMuestra = atoi(tok);
	tok = strtok(NULL,";");
	if(tok == NULL){return ERROR;}
	commandConfig->IntervaloTabla = atoi(tok);
	return OK;
}




bool_t opProccessMessageFromServer(uint16_t * sizeBuffServer,config_t * commandConfig ){

//	setIBC;5;3;9600;0 (232,LED1,BAUD9600,DELTA_OHM)
	memset(commandConfig->command, 0, strlen(commandConfig->command));
	funcion_t procesoCommand;
	if(!myParserDataFromWebSocket(dbuf, sizeBuffServer,commandConfig)) return FALSE;//me devuelve el comando y atributos
//	printf("\r\n%s\r\n",commandConfig->command);
	procesoCommand = diccProcesos[commandConfig->indexCommand]; //inicializo el puntero a función.
	procesoCommand(commandConfig);
	return OK;
}



//Se usa para procesar los datos recibidos del servidor

bool_t myParserDataFromWebSocket(char * dbuf, uint16_t * sizeBuffServer, config_t * commandConfig)
{
//	setIBC;5;3;9600;0
//	\2010{\"type\": \"chat\", \"message\": \"setIBC;5;3;9600;0\"}
//	${"type": "chat", "message": "start"}

	printf("\r\n%s\r\n",dbuf);
	printf("\r\n%02d\r\n",* sizeBuffServer);


	uint16_t i,j;
	char* tok=0;
	tok = strtok(dbuf," \" ");
	if(tok == NULL){return ERROR;}
//	int a = memcmp(tok,"HTTP/1.1",strlen("HTTP/1.1"));
	if(!memcmp(tok,"HTTP/1.1",strlen("HTTP/1.1"))) return ERROR;

//	tok = strtok(NULL," \" "); //itero hasta 6 o 7 y saco el mensaje con un for luego puedo borrar el buf y
//	sobreecribir el comando
//	if(tok == NULL){return ERROR;}
//	tok = strtok(NULL," \" ");
//	if(tok == NULL){return ERROR;}
//	tok = strtok(NULL," \" ");
//	if(tok == NULL){return ERROR;}
//	printf("\r\n%s\r\n",tok);
	for (i = 0; i < 7; i++){

		tok = strtok(NULL," \" ");
		if(tok == NULL){return ERROR;}
//		printf("data %s\r\n", tok);
//		if(i == 0){
//			ntpDatetime[i] = (uint16_t)atoi(tok)+2000;
//		}else
//			ntpDatetime[i] = (uint16_t)atoi(tok);
	}
	sprintf(commandConfig->command, tok, strlen(tok));
	char aux[strlen(commandConfig->command)];
	sprintf(aux,commandConfig->command,strlen(commandConfig->command));
	tok=0;
	tok = strtok(aux,";");
	if(tok == NULL){return ERROR;}
//	uint16_t sizeDic = (uint16_t)(sizeof(diccCommands)/sizeof(diccCommands[0]));
//	printf("02%d",b);
	for(commandConfig->indexCommand=0;memcmp(diccCommands[commandConfig->indexCommand],tok,strlen(tok));commandConfig->indexCommand++);
//	printf("%d",commandConfig->indexCommand);

	return OK;
}
