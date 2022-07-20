#include "canBusCom.h"


struct can_frame canMsgR, canMsgT;
MCP2515 mcp2515(5);

int command = 0;
SystemSensors systemSensorsCAN;
extern QueueHandle_t systemSensorsQueue;
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


void sendCANMsg (byte originMod, byte destMod, uint8_t cmd, byte *dataOut)
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
    mcp2515.sendMessage(&canMsg);
    //Serial.println("Message sent");
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
    long valveDataIn;
    memcpy(&valveDataIn,canMsg.data,sizeof(long));
    systemSensorsCAN.valve1Pos = valveDataIn;
    //Serial.println(valveDataIn);
    
    break;
  }
  case valveCompDir2:
  {
    /* code */
    break;
  }
  case pressureModDir:
  {
    float pressDataIn = 0;
    memcpy(&pressDataIn, canMsg.data,sizeof(float));
    systemSensorsCAN.pressureArray[command] = pressDataIn;
    /*
    Serial.print("Sensor: ");
    Serial.print(command);
    Serial.print("   ");
    Serial.print("Presion: ");
    Serial.print(pressDataIn);
    Serial.println("");*/
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
for(;;)
{
  if (queueSendFlag)
  {
    if (xQueueSend(systemSensorsQueue,&systemSensorsCAN,0) == pdTRUE)
    {
      queueSendFlag = false;
    }
    
  }

  
  
  if (mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) 
  {
    CANframeProcess(canMsgR);
    vTaskResume(samplingCallsTaskHandle);
    callFlag = false;
    noRespCount = 0;
    //Serial.println("message recived");

  }else
  {
   if (callFlag)
   {
    noRespCount++;
   }
   
  }

  if (callFlag && (noRespCount>2))
  {
    vTaskResume(samplingCallsTaskHandle);
    noRespCount = 0;
    callFlag = false;
    //Serial.println("NO response");
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
  sendCANMsg(CCUDir,pressureModDir,0,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,1,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,2,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,3,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,4,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,5,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,6,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,pressureModDir,7,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  sendCANMsg(CCUDir,valveCompDir1,4,&emptyData);
  callFlag = true;
  vTaskSuspend(samplingCallsTaskHandle);
  queueSendFlag = true;
  //Serial.println("message sent");
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
}
  
}