[1mdiff --git a/projects/Datalogger/src/da_rtc.c b/projects/Datalogger/src/da_rtc.c[m
[1mindex 2a6346b..b393953 100644[m
[1m--- a/projects/Datalogger/src/da_rtc.c[m
[1m+++ b/projects/Datalogger/src/da_rtc.c[m
[36m@@ -58,7 +58,7 @@[m [muint8_t ret5;[m
 [m
 [m
 bool_t opConfigNtpSocketViaTCP(){[m
[31m-[m
[32m+[m[32m//	Configuro los datos de red del cliente y el protocolo y el puerto por el que saldra[m
 	DesconectarSocket(SOCK_NTP);[m
 	//Reset registers[m
 	uint8_t registroModo = getMR();[m
[36m@@ -91,7 +91,7 @@[m [mbool_t opConfigNtpSocketViaTCP(){[m
 }[m
 [m
 bool_t opConfigNtpViaTCP(){[m
[31m-[m
[32m+[m[32m//	Configuro los datos del servidor al cual quiero conectarme[m
 	setSn_DIPR(SOCK_NTP, NTP_destIP);[m
 	setSn_DPORT(SOCK_NTP,_NTP_destport);[m
 	setSn_CR(SOCK_NTP,Sn_CR_CONNECT);[m
[1mdiff --git a/projects/Datalogger/src/da_transmision.c b/projects/Datalogger/src/da_transmision.c[m
[1mindex 5e71790..128f5df 100644[m
[1m--- a/projects/Datalogger/src/da_transmision.c[m
[1m+++ b/projects/Datalogger/src/da_transmision.c[m
[36m@@ -46,14 +46,10 @@[m [mvoid opEnableRxWebSocket(){[m
 [m
 void opReceptionMessage( void *data ){ //Esta se llama  en el callbackSet[m
 	printf("\r\n Llego un mensaje del server");[m
[31m-	printf("\r\n Llego un mensaje del server");[m
 [m
 }[m
 [m
 [m
[31m-[m
[31m-[m
[31m-[m
 void opConfigSocket(){[m
 [m
 	DesconectarSocket(DATA_SOCK_FTP);[m
