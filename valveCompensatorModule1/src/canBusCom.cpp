#include "canBusCom.h"

can_frame canMsgR,canMsgT;
MCP2515 mcp2515(9);

extern int setPt;
extern valveControlSM SM;
extern long valvePosition1;

void valvePos2CANFrame (byte originMod, byte destMod, long valvePos, can_frame *canMsg)
{
    //struct can_frame canMsg;
    unsigned long canIdLong = 0;
    byte canIDByte[4];

    canIDByte[3] = 0x80;
    canIDByte[2] = originMod;
    canIDByte[1] = destMod;
    canIDByte[0]= 0x00;

    memcpy(&canIdLong,canIDByte,sizeof(unsigned long));
    (*canMsg).can_id = canIdLong;
    
    (*canMsg).can_dlc = 8;
    memcpy((*canMsg).data, &valvePos, sizeof(long));
}

void configurecanModule()
{
    mcp2515.reset();
    mcp2515.setConfigMode();
    mcp2515.setBitrate(CAN_125KBPS);
  
    mcp2515.setFilterMask(MCP2515::MASK0, true, 0xff00);
    mcp2515.setFilter(MCP2515::RXF0, true, valveCompDir1<<8);
    mcp2515.setNormalMode();

}

void getCanMessage()
{
if (mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) 
  {
    //Serial.println("Message recived");
    uint8_t cmd = canMsgR.can_id & 0xff;

    switch (cmd)
    {
    case stopCMD:
    {
      SM = stop;
      break;
    }
    case calibrateCMD:
    {
      SM = calibrate;
      break;
    }
    case monitorinCMD:
    { 
      SM = monitorin;
      break;
    }
    case positionControlCMD:
    {
      SM = positionControl;
      break;
    }
    case sendPositionCMD:
    { 
      valvePos2CANFrame(valveCompDir1,CCUDir,valvePosition1,&canMsgT);
      mcp2515.sendMessage(&canMsgT);
      break;
    }
    case modSetPtCMD:
    {
      memcpy(&setPt,canMsgR.data,sizeof(int));
      break;
    }
    default:
    {
      break;
    }
    }
  }
}

void sendCanMessage(uint32_t canID, byte* message)
{
    
}

