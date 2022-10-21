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
      xQueueSendToFront(CANCallOutQueue, &canMsg,100);
    }else
    {
      xQueueSend(CANCallOutQueue,&canMsg,100);
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
    case 0x04:
      long valveDataIn;
      memcpy(&valveDataIn,canMsg.data,sizeof(long));
      systemSensorsCAN.valve1Pos = valveDataIn;
      break;
    case 0x00:
      Serial.println("Valve 1 stop recived");
    break;
    case 0x03:
      Serial.println("Valve 1 start recived");
    break;
    case 0x05:
      Serial.println("Valve 1 modif. SP recived");
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
    //Serial.print("Press: ");
    //Serial.print(command);
    //Serial.print(" :");
    //Serial.println(pressDataIn);
    break;
  }
  case tempModDir1:
  {
    float tempDataIn = 0;
    memcpy(&tempDataIn, canMsg.data,sizeof(float));
    systemSensorsCAN.tempArrayMod1[command] = tempDataIn;
    //Serial.print("Temp: ");
    //Serial.print(command);
    //Serial.print(" :");
    //Serial.println(tempDataIn);
    break;
  }
  case tempModDir2:
  {
    float tempDataIn = 0;
    memcpy(&tempDataIn, canMsg.data,sizeof(float));
    systemSensorsCAN.tempArrayMod2[command] = tempDataIn;
    break;
  }
  default:
  {
    break;
  }
  }
}

bool canMsgIdMatch(can_frame canR, can_frame canT)
{
  //Serial.print("T:");
  //Serial.println(canT.can_id, HEX);
  //Serial.print("R:");
  //Serial.println(canR.can_id, HEX);
  byte dMod, mID;
  dMod = (canR.can_id >> 16) & 0xff;
  mID = canR.can_id & 0xff;
  if (dMod != ((canT.can_id>>8) & 0xff))
  {
    //Serial.println("false 1");
    return false;
  }
  if (mID != (canT.can_id & 0xff))
  {
    //Serial.println("false 2");
    return false;
  }
  //Serial.println("true");
  return true;
}


TaskHandle_t  samplingCallsTaskHandle = NULL;
TaskHandle_t  canBusComTaskHandle = NULL;
void canBusTask (void* parameters)
{
TickType_t xLastWakeTime = xTaskGetTickCount();
const TickType_t xFrequency = 10;
TickType_t callTimeout0 = 0, callTimeout1 = 0;
//vTaskSuspend(NULL);
configureCANModule();
intializeTasks2();
for (size_t i = 0; i < 255; i++)
{
  noRespArray[i] = 0xff;
}
for(;;)
{
    if ((mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) && canMsgIdMatch(canMsgR, canMsgT)) 
    {
      callFlag = false;
      //Serial.print("Response module: ");
      //Serial.println(canMsgR.can_id,HEX);
      CANframeProcess(canMsgR);
      noRespArray[canMsgT.can_id>>8 & 0xff] = 0;
      xQueueOverwrite(systemSensorsQueue,&systemSensorsCAN);
    }
    else
    {
      //Serial.println("No response");
      callTimeout1 = xTaskGetTickCount();
      if (callFlag && (callTimeout1 - callTimeout0)>50)
      {
        callFlag = false;
        if (noRespArray[canMsgT.can_id>>8 & 0xff] < 255)
        {
          noRespArray[canMsgT.can_id>>8 & 0xff]++;
          if (noRespArray[canMsgT.can_id>>8 & 0xff] <=1)
          {
            xQueueSend(CANCallOutQueue,&canMsgT,100);
          }
        }
      }
    }
    
  if ((xQueueReceive(CANCallOutQueue,&canMsgT,5) == pdTRUE) && !callFlag)
  {
    callFlag = true;
    mcp2515.sendMessage(&canMsgT);
    callTimeout0 = xTaskGetTickCount();
    //Serial.print("Call module: ");
    //Serial.println(canMsgT.can_id, HEX);
  }
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
}
  
}


void samplingCallsTask (void* parameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 250;
  for(;;)
  {
    for (size_t i = 0; i < 8; i++)
    {
      sendCANMsgToQueue(CCUDir,pressureModDir,i,&emptyData,false);
    }
    for (size_t i = 0; i < 8; i++)
    {
      sendCANMsgToQueue(CCUDir,tempModDir1,i,&emptyData,false);
    }
    sendCANMsgToQueue(CCUDir,valveCompDir1,4,&emptyData,false);
    vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}