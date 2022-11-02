#include "canBusCom.h"

can_frame canMsgR,canMsgT;
MCP2515 mcp2515(9);

extern int setPt;
extern valveControlSM SM;
extern long valvePosition1;

void sendPos2CAN (byte originMod, byte destMod, long valvePos)
{
    //struct can_frame canMsg;
    unsigned long canIdLong = 0;
    byte canIDByte[4];

    canIDByte[3] = 0x80;
    canIDByte[2] = originMod;
    canIDByte[1] = destMod;
    canIDByte[0]= 0x04;

    memcpy(&canIdLong,canIDByte,sizeof(unsigned long));
    canMsgT.can_id = canIdLong;
    
    canMsgT.can_dlc = 8;
    memcpy(canMsgT.data, &valvePos, sizeof(long));
    mcp2515.sendMessage(&canMsgT);
}

void sendCANok(byte originMod, byte destMod)
{
  unsigned long canIdLong = 0;
  byte canIDByte[4];
  canIDByte[3] = 0x80;
  canIDByte[2] = originMod;
  canIDByte[1] = destMod;
  canIDByte[0]= canMsgR.can_id & 0xff;

  memcpy(&canIdLong,canIDByte,sizeof(unsigned long));
  canMsgT.can_id = canIdLong;

  canMsgT.can_dlc = 8;
  canMsgT.data[0] = 0x01;
  mcp2515.sendMessage(&canMsgT);
}

void configurecanModule()
{
    mcp2515.reset();
    mcp2515.setConfigMode();
    mcp2515.setBitrate(CAN_125KBPS);
  
    mcp2515.setFilterMask(MCP2515::MASK0, true, 0xff00);
    mcp2515.setFilter(MCP2515::RXF0, true, valveCompDir2<<8);
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
      sendCANok(valveCompDir2,CCUDir);
      break;
    }
    case calibrateCMD:
    {
      SM = calibrate;
      sendCANok(valveCompDir2,CCUDir);
      break;
    }
    case monitorinCMD:
    { 
      SM = monitorin;
      //sendCANok(valveCompDir2,CCUDir);
      break;
    }
    case positionControlCMD:
    {
      SM = positionControl;
      sendCANok(valveCompDir2,CCUDir);
      break;
    }
    case sendPositionCMD:
    { 
      sendPos2CAN(valveCompDir2,CCUDir,valvePosition1);
      
      break;
    }
    case modSetPtCMD:
    {
      memcpy(&setPt,canMsgR.data,sizeof(int));
      sendCANok(valveCompDir2,CCUDir);
      break;
    }
    default:
    {
      break;
    }
    }
  }
}

