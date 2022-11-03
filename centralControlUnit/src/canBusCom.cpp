#include "canBusCom.h"


struct can_frame canMsgR, canMsgT;
MCP2515 mcp2515(5);

SystemSensors systemSensorsCAN, auxSensor;
QueueHandle_t systemSensorsQueue = xQueueCreate(1,sizeof(systemSensorsCAN));
uint8_t noRespArray [255] = {0};

QueueHandle_t CANCallOutQueue = xQueueCreate(50,sizeof(canMsgT));
QueueHandle_t CANCallInQueue = xQueueCreate(10,sizeof(canMsgR));

bool noResponseFlag = false;
uint8_t noRespCount = 0;
byte emptyData = 0;
bool queueSendFlag = false;

void configureCANModule(){
  mcp2515.reset();
  mcp2515.setConfigMode();
  mcp2515.setBitrate(CAN_125KBPS);
  
  mcp2515.setFilterMask(MCP2515::MASK0, true, 0xff00);
  mcp2515.setFilter(MCP2515::RXF0, true, CCUDir<<8);
  Serial.println("CAN module configured");  
  mcp2515.setNormalMode();
}


void sendCANMsgToQueue (byte originMod, byte destMod, uint8_t cmd, byte *dataOut, bool sendToFront)
{
    struct can_frame canMsg;
    unsigned long canIdLong = 0;
    byte canIDByte[4];
    /*
    Serial.println("");
    Serial.print(originMod, HEX);
    Serial.print(":");
    Serial.println(destMod, HEX);
    */
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
      
      if (xQueueSend(CANCallOutQueue,&canMsg,100)==pdTRUE)
      {
        //Serial.print("Added to queue: ");
        //Serial.println(canMsg.can_id, HEX);
      } 
    }
}

void CANframeProcess(can_frame canMsg)
{
  byte canCheck = 0;
  byte origMod = 0;
  byte comand = 0;

  canCheck = canMsg.can_id>>24 & 0xff;
  origMod = canMsg.can_id>>16 & 0xff;
  comand = canMsg.can_id & 0xff;
  //Serial.println(comand,HEX);
  if (canCheck!=0x80)
  {
    return;
  }
  
  switch (origMod)
  {
  case linkDir:
  {
    Serial.println("Link msg rec");
  }
  case valveCompDir1:
  {
    switch (comand)
    {
    case 0x04:
      long valveDataIn;
      memcpy(&valveDataIn,canMsg.data,sizeof(long));
      systemSensorsCAN.valve1Pos = valveDataIn;
      break;
    case 0x00:
      //Serial.println("Valve 1 stop recived");
      break;
    case 0x03:
      //Serial.println("Valve 1 start recived");
      break;
    case 0x05:
      //Serial.println("Valve 1 modif. SP recived");
      break;
    default:
      break;
    }
    
    break;
  }
  case valveCompDir2:
  {
    switch (comand)
    {
    case 0x04:
      long valveDataIn;
      memcpy(&valveDataIn,canMsg.data,sizeof(long));
      systemSensorsCAN.valve2Pos = valveDataIn;
      break;
    case 0x00:
      //Serial.println("Valve 2 stop recived");
      break;
    case 0x03:
      //Serial.println("Valve 2 start recived");
      break;
    case 0x05:
      //Serial.println("Valve 2 modif. SP recived");
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
    systemSensorsCAN.pressureArray[comand] = pressDataIn;
    break;
  }
  case tempModDir1:
  {
    float tempDataIn = 0;
    memcpy(&tempDataIn, canMsg.data,sizeof(float));
    systemSensorsCAN.tempArrayMod1[comand] = tempDataIn;
    break;
  }
  case tempModDir2:
  {
    float tempDataIn = 0;
    memcpy(&tempDataIn, canMsg.data,sizeof(float));
    systemSensorsCAN.tempArrayMod2[comand] = tempDataIn;
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
  bool callFlag = false;

  configureCANModule();
  intializeTasks2();
  for (size_t i = 0; i < 255; i++)
  {
    noRespArray[i] = 0xff;
  }
  for(;;)
  {
    if (!callFlag && (xQueueReceive(CANCallOutQueue,&canMsgT,100) == pdTRUE))
    {
      mcp2515.sendMessage(&canMsgT);
      callTimeout0 = xTaskGetTickCount();
      callFlag = true;
      //Serial.print("MSG sent: ");
      //Serial.println(canMsgT.can_id, HEX);
    }

    vTaskDelayUntil( &xLastWakeTime, xFrequency );
  
    
    if (mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) 
    {
      //Serial.print("Can msg recived: ");
      //Serial.println(canMsgR.can_id, HEX);
      CANframeProcess(canMsgR);
      if (canMsgIdMatch(canMsgR, canMsgT))
      {
        //Serial.println("CAN msg match with call");
        callFlag = false;
        noRespArray[canMsgT.can_id>>8 & 0xff] = 0;
        xQueueOverwrite(systemSensorsQueue,&systemSensorsCAN);
      }
      continue;
    }
    if (!callFlag)
    {
      //Serial.println("not call");
      continue;
    }
    callTimeout1 = xTaskGetTickCount();
    if ((callTimeout1 - callTimeout0)<50)
    {
      //Serial.println("time out not reached");
      continue;
    }
    //Serial.println("response time out");
    callFlag = false;
    if (noRespArray[canMsgT.can_id>>8 & 0xff] <= 3)
    {
      noRespArray[canMsgT.can_id>>8 & 0xff]++;
      xQueueSend(CANCallOutQueue,&canMsgT,100);
    }
  }
}


void samplingCallsTask (void* parameters)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 250;
  uint8_t valve1Counter=0, valve2Counter=0, pressModCounter=0, tempMod1Counter=0,tempMod2Counter=0;
  for(;;)
  {
    if (noRespArray[valveCompDir1]<=3)
    {
      sendCANMsgToQueue(CCUDir,valveCompDir1,4,&emptyData,false);
    }
    else
    {
      if ((valve1Counter++)>8)
      {
        sendCANMsgToQueue(CCUDir,valveCompDir1,4,&emptyData,false);
        valve1Counter = 0;
      }
    }

    if (noRespArray[valveCompDir2]<=3)
    {
      sendCANMsgToQueue(CCUDir,valveCompDir2,4,&emptyData,false);
    }
    else
    {
      if ((valve2Counter++)>8)
      {
        sendCANMsgToQueue(CCUDir,valveCompDir2,4,&emptyData,false);
        valve2Counter = 0;
      }
    }

    if (noRespArray[pressureModDir]<=3)
    {
      for (size_t i = 0; i < 8; i++)
      {
        sendCANMsgToQueue(CCUDir,pressureModDir,i,&emptyData,false);
      }
    }
    else
    {
      if ((pressModCounter++)>8)
      {
        sendCANMsgToQueue(CCUDir,pressureModDir,0,&emptyData,false);
        pressModCounter = 0;
      }
    }

    if (noRespArray[tempModDir1]<=3)
    {
      for (size_t i = 0; i < 8; i++)
      {
        sendCANMsgToQueue(CCUDir,tempModDir1,i,&emptyData,false);
      }
    }
    else
    {
      if ((tempMod1Counter++)>8)
      {
        sendCANMsgToQueue(CCUDir,tempModDir1,0,&emptyData,false);
        tempMod1Counter = 0;
      }
    }
    
    if (noRespArray[tempModDir2]<=3)
    {
      for (size_t i = 0; i < 8; i++)
      {
        sendCANMsgToQueue(CCUDir,tempModDir2,i,&emptyData,false);
      }
    }
    else
    {
      if ((tempMod2Counter++)>8)
      {
        sendCANMsgToQueue(CCUDir,tempModDir2,0,&emptyData,false);
        tempMod2Counter = 0;
      }
    }

    vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}