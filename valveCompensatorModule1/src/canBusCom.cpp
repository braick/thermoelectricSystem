#include "canBusCom.h"


struct can_frame canMsg;
MCP2515 mcp2515(9);

extern float setPt;


void configurecanModule()
{
    mcp2515.reset();
    mcp2515.setConfigMode();
    mcp2515.setBitrate(CAN_125KBPS);
  
    mcp2515.setFilterMask(MCP2515::MASK0, true, 0xff00);
    mcp2515.setFilter(MCP2515::RXF0, true, 0xA0<<8);
    mcp2515.setNormalMode();

}

uint8_t getCanMessage()
{
if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) 
  {
    memcpy(&setPt,canMsg.data,sizeof(float));
    Serial.println("Message recived");
  }
  return 3;
}

void sendCanMessage(uint32_t canID, byte* message)
{
    
}

