#include "canBusCom.h"
#include "ADCModule.h"
struct can_frame canMsgR, canMsgT;
MCP2515 mcp2515(10);

extern pressSensor pressureSensors;

void configurecanModule()
{
    mcp2515.reset();
    mcp2515.setConfigMode();
    mcp2515.setBitrate(CAN_125KBPS);
  
    mcp2515.setFilterMask(MCP2515::MASK0, true, 0xff00);
    mcp2515.setFilter(MCP2515::RXF0, true, pressureModDir<<8);
    mcp2515.setNormalMode();

}

uint8_t getCanMessage()
{
if (mcp2515.readMessage(&canMsgR) == MCP2515::ERROR_OK) 
  {
    Serial.println("Message recived");
    uint8_t cmd = canMsgR.can_id & 0xff;
    cmd = constrain(cmd,0,7);
    return cmd;
  }
  return 255;
}

void sendCanMessage(float press, uint8_t sensorNum)
{
  canMsgT = pressure2CanMsg(pressureModDir,CCUDir,press,sensorNum);
  mcp2515.sendMessage(&canMsgT);
}

struct can_frame pressure2CanMsg (byte originMod, byte destMod, float press, uint8_t sensorNum)
{
    struct can_frame canMsg;
    unsigned long canIdLong = 0;
    byte canIDByte[4];

    canIDByte[3] = 0x80;
    canIDByte[2] = originMod;
    canIDByte[1] = destMod;
    canIDByte[0]= (byte)sensorNum;

    memcpy(&canIdLong,canIDByte,sizeof(unsigned long));
    canMsg.can_id = canIdLong;
    
    canMsg.can_dlc = 8;
    memcpy(canMsg.data, &press, sizeof(float));
    
    return canMsg;
}

