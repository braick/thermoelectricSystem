#include <Arduino.h>
#include <WiFi.h>
#include "canBusCom.h"

SystemSensors systemSensorsTCP;
QueueHandle_t systemSensorsQueue = xQueueCreate(1,sizeof(systemSensorsTCP));

extern QueueHandle_t CANCallOutQueue;

extern  uint8_t noRespArray[255];

extern uint8_t systemSMHT, systemSMLT;


void TCPframeProcces(byte *inputBuffer,uint8_t *numOfBytesToSend,byte* outputBuffer)
{
    *numOfBytesToSend = 1;
    memcpy(outputBuffer,numOfBytesToSend,sizeof(uint8_t));
    outputBuffer++;
    byte *ptr = &inputBuffer[0];
    byte emptyBytes[8] = {0};
    switch (*ptr)
    {
    case valveCompDir1:
    {
        ptr++;
        uint8_t valveCMD = (uint8_t)*ptr;
        if (*ptr==modSetPtValve)
        {
          ptr++;
          int sptOUT = (int)*ptr;
          byte spFrameOUT[8];
          memcpy(spFrameOUT,&sptOUT,sizeof(int));
          //xSemaphoreTake(CANMutex,100);
          sendCANMsgToQueue(CCUDir,valveCompDir1,valveCMD,spFrameOUT,true);
          //xSemaphoreGive(CANMutex);
        }
        else
        {
          sendCANMsgToQueue(CCUDir,valveCompDir1,valveCMD,emptyBytes,true);
        }
        break;
    }
    case valveCompDir2:
    {
        ptr++;
        uint8_t valveCMD = (uint8_t)*ptr;
        if (*ptr==modSetPtValve)
        {
          ptr++;
          int sptOUT = (int)*ptr;
          byte spFrameOUT[8];
          memcpy(spFrameOUT,&sptOUT,sizeof(int));
          sendCANMsgToQueue(CCUDir,valveCompDir2,valveCMD,spFrameOUT,true);
        }
        else
        {
          sendCANMsgToQueue(CCUDir,valveCompDir2,valveCMD,emptyBytes,true);
        }
        break;
        break;
    }
    case sendSensorsData:
    {
        //Serial.println("send sensors TCP command");
        if (xQueuePeek(systemSensorsQueue, &systemSensorsTCP,0) !=pdTRUE) 
        {
            *outputBuffer = nothigToRead;
            break;
        }
        *numOfBytesToSend = 1 + pressSensorNum*(sizeof(float)) + 2*tempSensorNum*(sizeof(float)) + 2*sizeof(long);
        outputBuffer--;
        *outputBuffer = *numOfBytesToSend;
        outputBuffer++;
        *outputBuffer = sendSensorsData;
        outputBuffer++;
        for (size_t i = 0; i < pressSensorNum; i++)
        {
            memcpy(outputBuffer,&systemSensorsTCP.pressureArray[i],sizeof(float));
            outputBuffer+=sizeof(float);
        }
        for (size_t i = 0; i < tempSensorNum; i++)
        {
            memcpy(outputBuffer,&systemSensorsTCP.tempArrayMod1[i],sizeof(float));
            outputBuffer+=sizeof(float);
        }
        for (size_t i = 0; i < tempSensorNum; i++)
        {
            memcpy(outputBuffer,&systemSensorsTCP.tempArrayMod2[i],sizeof(float));
            outputBuffer+=sizeof(float);
        }
        memcpy(outputBuffer,&systemSensorsTCP.valve1Pos,sizeof(long));
        outputBuffer+=sizeof(long);
        memcpy(outputBuffer,&systemSensorsTCP.valve2Pos,sizeof(long));
        break;
    }
    case systemsStatusReq:
    {
      *numOfBytesToSend = 2 + numOfSysModules;
      outputBuffer--;
      *outputBuffer = *numOfBytesToSend;
      outputBuffer++;
      *outputBuffer = systemsStatusReq;
      outputBuffer++;

      *outputBuffer = noRespArray[valveCompDir1];
      outputBuffer++;
      *outputBuffer = noRespArray[valveCompDir2];
      outputBuffer++;
      *outputBuffer = noRespArray[tempModDir1];
      outputBuffer++;
      *outputBuffer = noRespArray[tempModDir2];
      outputBuffer++;
      *outputBuffer = noRespArray[pressureModDir];
      outputBuffer++;
      *outputBuffer = noRespArray[200];
      outputBuffer++;
      
      break;
    }
    case relaysTest:
    {
      ptr++;
      systemSMHT = *ptr; 
      //Serial.print("control state changed to:");
      //Serial.println(*ptr);
      break;
    }
    default:
    {
        break;
    }
    }
}



TaskHandle_t  communicationTaskHandle = NULL;
/**
 * @brief Tarea de comunicaciones -> Se encargara de:
 * Conectar el modulo al punto de acceso y mantener la conexion 
 * Gestioanr la conexion con el cliente
 * Gestionar las peticiones del cliente:
 *  -Recepcion de la configuracion de los sensores
 *  -Recepcion de los parametros del controlador
 *  -Recepcion del setpoint
 *  -Envio de datos de telemetria de la valvula
 *  -Envio de datos de los sensores
 * 
 * @param parameters 
 */
void communication (void* parameters)
{
TickType_t xLastWakeTime = xTaskGetTickCount();
const TickType_t xFrequency = 50;

///definir parametros de conexion con el AP
const char* ssid     = "TEGCS";
const char* password = "RECUPERA";
IPAddress local_IP(192, 168, 4, 3);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) 
{
    Serial.println("STA Failed to configure");
}
WiFi.mode(WIFI_STA);
WiFi.setSleep(false); // Desactiva la suspensión de wifi en modo STA para mejorar la velocidad de respuesta
TickType_t timeOut = 0;
////definir parametros del servidor
WiFiServer tcpServerObj;
WiFiClient client;
bool printFlag = true;
#define ApConexion 0
#define waitingForClient 1
#define waitingForData 2
uint8_t stateMachineHandle = 0;
uint16_t clientCallTimeOut = clientDisconnectTimeout;
for(;;)
{
  switch (stateMachineHandle)
  {
    case ApConexion://conexion con el Ap
    { 
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println("");
        Serial.println("Connected to:");
        Serial.print("Ssid: ");
        Serial.println(ssid);
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        Serial.println("Starting tcp server al port 80");
        tcpServerObj.begin(80);

        stateMachineHandle = waitingForClient;
        break;
      }

      if(xTaskGetTickCount() >= timeOut)//si se ha superado del timeoout 
      {
        timeOut = xTaskGetTickCount() + 5000;
        WiFi.begin(ssid, password);
        Serial.println("");
        Serial.println("");
        Serial.print("Connecting to: ");
        Serial.println(ssid);
        break;
      }
      Serial.print(".");
      break;
    } 
    case waitingForClient://esperando cliente
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("WARNING: Conexion lost");
        client.stop();
        stateMachineHandle = ApConexion;
        break;
      }

      client = tcpServerObj.available();
      if(client)
      {
        Serial.println("Cliente conectado");
        printFlag = true;
        stateMachineHandle = waitingForData;
        break;
      }

      if (printFlag)
      {
        Serial.println("Waiting for client");
        printFlag = false;
      }
      break;
    }  
    case waitingForData://esperando datos
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("WARNING: Conexion lost");
        client.stop();
        stateMachineHandle = ApConexion;
        break;
      }

      if (!client.connected())
      {
        Serial.println("Client disconnect");
        client.stop();
        stateMachineHandle = waitingForClient;
        break;
      }

      if (!client.available())
      {
        //Serial.print("Waiting data at time: ");
        //Serial.println(xTaskGetTickCount());
        clientCallTimeOut--;
        if (clientCallTimeOut<=0)
        {
          client.stop();
          stateMachineHandle = waitingForClient;
          clientCallTimeOut = clientDisconnectTimeout;
          Serial.println("Client timeout");

        }
        break;
      }
      clientCallTimeOut = clientDisconnectTimeout;

      //cliente conectado y datos en el buffer
      //Serial.println("Reciving data...");
      byte inputBuffer[client.available()];//crea un array de bytes del tamaño del buffer de datos 
      int inputDataNum = client.available();
      //Serial.print(inputDataNum);
      //Serial.println(" bytes");
      byte outputBuffer [255] = {0x00};//array de la respuesta
      uint8_t numOfBytesToSend = (uint8_t)1;
      *outputBuffer = numOfBytesToSend;
      for (int i = 0; i < inputDataNum; i++)
      {
        inputBuffer[i] = client.read();
        //Serial.print(i);
        //Serial.print(": ");
        //Serial.println(inputBuffer[i],HEX);
      }
      //Serial.println((uint8_t)outputBuffer[0]);
      TCPframeProcces(inputBuffer,&numOfBytesToSend,outputBuffer);
      //Serial.println((uint8_t)outputBuffer[0]);
      //outputBuffer[1] = 0xA0;
      client.write(outputBuffer, numOfBytesToSend + 1);
      break;//break del caso 3 de la maquina de estados
    }  
    default://default de la maquina de estados 
    {
      break;
    }
  }
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
}  
}
