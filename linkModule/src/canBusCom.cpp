#include "canBusCom.h"

can_frame canMsgR,canMsgT;
MCP2515 mcp2515(10);
byte byteArrayTx[8] = {0x00};

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
    mcp2515.setNormalMode();

}

void getCanMessage()
{
if (mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) 
  {
    byteArrayTx[0] = 0xff;
    byteArrayTx[1] = (canMsgR.can_id>>16) & 0xff;
    byteArrayTx[2] = (canMsgR.can_id>>8) & 0xff;
    byteArrayTx[3] = canMsgR.can_id & 0xff;
    if (canMsgR.can_dlc >= 4)
    {
      memcpy(&byteArrayTx[4], canMsgR.data,4);
    }
    else
    {
      byteArrayTx[4] = 0;
      byteArrayTx[5] = 0;
      byteArrayTx[6] = 0;
      byteArrayTx[7] = 0;
    }
    if(Serial.availableForWrite()>7)
    {
      Serial.write(byteArrayTx,8);
    }
  }
}

