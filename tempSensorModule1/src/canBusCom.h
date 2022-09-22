#include "mcp2515.h"
#include "definitions.h"
struct can_frame temp2CanMsg (byte originMod, byte destMod, float temp, uint8_t sensorNum);
void configurecanModule();
uint8_t getCanMessage();
void sendCanMessage(float temp, uint8_t sensorNum);
