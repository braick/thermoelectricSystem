#include "mcp2515.h"
#include "definitions.h"
struct can_frame pressure2CanMsg (byte originMod, byte destMod, float press, uint8_t sensorNum);
void configurecanModule();
uint8_t getCanMessage();
void sendCanMessage(float press, uint8_t sensorNum);
