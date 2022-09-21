#include "canBusCom.h"


struct can_frame canMsgR, canMsgT;
MCP2515 mcp2515(5);

int command = 0;
SystemSensors systemSensorsCAN, auxSensor;
extern QueueHandle_t systemSensorsQueue;
uint8_t noRespArray [255] = {0};

QueueHandle_t CANCallOutQueue = xQueueCreate(50,sizeof(canMsgT));
QueueHandle_t CANCallInQueue = xQueueCreate(10,sizeof(canMsgR));

bool noResponseFlag = false;
bool callFlag = false;
uint8_t noRespCount = 0;
byte emptyData = 0;
bool queueSendFlag = false;
void configureCANModule(){
  mcp2515.reset();
  mcp2515.setConfigMode();
  mcp2515.setBitrate(CAN_125KBPS);
  
  mcp2515.setFilterMask(MCP2515::MASK0, true, 0xff00);
  mcp2515.setFilter(MCP2515::RXF0, true, CCUDir<<8);
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
  
  mcp2515.setNormalMode();
}


void sendCANMsgToQueue (byte originMod, byte destMod, uint8_t cmd, byte *dataOut, bool sendToFront)
{
    struct can_frame canMsg;
    unsigned long canIdLong = 0;
    byte canIDByte[4];

    canIDByte[3] = 0x80;
    canIDByte[2] = originMod;
    canIDByte[1] = destMod;
    canIDByte[0]= cmd;
    
    memcpy(&canIdLong,canIDByte,sizeof(unsigned long));
    canMsg.can_id = canIdLong;
    
    canMsg.can_dlc = 8;
    memcpy(canMsg.data,dataOut,sizeof(int));
    
    if (sendToFront)
    {
      xQueueSendToFront(CANCallOutQueue, &canMsg,10);
    }else
    {
      xQueueSend(CANCallOutQueue,&canMsg,10);
    }
    //mcp2515.sendMessage(&canMsg);
}

void CANframeProcess(can_frame canMsg)
{
  byte origMod = 0;
  byte comand = 0;

  origMod = canMsg.can_id>>16 & 0xff;
  command = canMsg.can_id & 0xff;
  //Serial.println(command,HEX);

  switch (origMod)
  {
  case valveCompDir1:
  {
    switch (command)
    {
    case 0x00:
      long valveDataIn;
      memcpy(&valveDataIn,canMsg.data,sizeof(long));
      systemSensorsCAN.valve1Pos = valveDataIn;
      break;
    case 0xA0:
      //Serial.println("Valve 1 ok recived");
      break;
    default:
      break;
    }
    
    break;
  }
  case valveCompDir2:
  {
    switch (command)
    {
    case 0x00:
      long valveDataIn;
      memcpy(&valveDataIn,canMsg.data,sizeof(long));
      systemSensorsCAN.valve2Pos = valveDataIn;
      break;
    case 0xA0:
      //setpoint modified
      break;
    default:
      break;
    }
    break;
  }
  case pressureModDir:
  {
    float pressDataIn = 0;
    memcpy(&pressDataIn, canMsg.data,sizeof(float));
    systemSensorsCAN.pressureArray[command] = pressDataIn;
    break;
  }
  case tempModDir1:
  {
    /* code */
    break;
  }
  case tempModDir2:
  {
    /* code */
    break;
  }
  default:
  {
    break;
  }
  }
}


TaskHandle_t  samplingCallsTaskHandle = NULL;
TaskHandle_t  canBusComTaskHandle = NULL;
void canBusTask (void* parameters)
{
TickType_t xLastWakeTime = xTaskGetTickCount();
const TickType_t xFrequency = 10;
//vTaskSuspend(NULL);
configureCANModule();
intializeTasks2();
for (size_t i = 0; i < 255; i++)
{
  noRespArray[i] = 0xff;
}

for(;;)
{
    if ((mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) && ((canMsgT.can_id>>8 & 0xff) == (canMsgR.can_id>>16 & 0xff))) 
    {
      CANframeProcess(canMsgR);
      noRespArray[canMsgT.can_id>>8 & 0xff] = 0;
      xQueueOverwrite(systemSensorsQueue,&systemSensorsCAN);
    }
    else
    {
      if (noRespArray[canMsgT.can_id>>8 & 0xff] < 255)
      {
        noRespArray[canMsgT.can_id>>8 & 0xff]++;
        if (noRespArray[canMsgT.can_id>>8 & 0xff] <=1)
        {
          xQueueSend(CANCallOutQueue,&canMsgT,5);
        }
      }
    }
  
  if (xQueueReceive(CANCallOutQueue,&canMsgT,5) == pdTRUE)
  {
    mcp2515.sendMessage(&canMsgT);
    //Serial.println("call");
  }
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
}
  
}


void samplingCallsTask (void* parameters)
{
TickType_t xLastWakeTime = xTaskGetTickCount();
const TickType_t xFrequency = 300;
for(;;)
{

  //lectura de los sensores de presion

  for (size_t i = 0; i < 8; i++)
  {
    sendCANMsgToQueue(CCUDir,pressureModDir,i,&emptyData,false);
  }
  sendCANMsgToQueue(CCUDir,valveCompDir1,4,&emptyData,false);
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
}
  
}