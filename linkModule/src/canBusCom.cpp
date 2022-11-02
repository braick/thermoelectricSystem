#include "canBusCom.h"

can_frame canMsgR,canMsgT;
MCP2515 mcp2515(10);
byte byteArrayTx[8] = {0x00};
byte testByteArray[8] = {0x00};

extern int setPt;
extern valveControlSM SM;
extern long valvePosition1;

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

void serialRecieve ()
{
  byte reciveSerialBuffer[8] = {0x00};
  if (Serial.available() < 8)
  {
    //Serial.write(testByteArray,8);
    return;
  }
  if (Serial.readBytes(reciveSerialBuffer,8) != 8)
  {
    //Serial.write(testByteArray,8);
    return;
  }
  if (reciveSerialBuffer[0] != 0xff)
  {
    //Serial.write(testByteArray,8);
    void serialFlush();
    return;
  }
  if (reciveSerialBuffer[1] != 0x00)
  {
    //Serial.write(testByteArray,8);
    void serialFlush();
    return;
  }
  //Serial.write(testByteArray,8);
  sendCANMsg(reciveSerialBuffer[1],reciveSerialBuffer[2],reciveSerialBuffer[3],&reciveSerialBuffer[4]);
}


void sendCANMsg(byte originMod, byte destMod, uint8_t cmd, byte *dataOut)
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
    sendCANMsgToSerial(canMsg);

}

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

void sendCANMsgToSerial (can_frame canMsg)
{
  byteArrayTx[0] = 0xff;
  byteArrayTx[1] = (canMsg.can_id>>16) & 0xff;
  byteArrayTx[2] = (canMsg.can_id>>8) & 0xff;
  byteArrayTx[3] = canMsg.can_id & 0xff;
  if (canMsg.can_dlc >= 4)
  {
    memcpy(&byteArrayTx[4], canMsg.data,4);
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

void getCanMessage()
{
if (mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) 
  {
    sendCANMsgToSerial(canMsgR);
  }
}

