#include "mcp2515.h"

void configurecanModule();
uint8_t getCanMessage();
void sendCanMessage(uint32_t canID, byte* message);

