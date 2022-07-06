#include "mcp2515.h"
#include "definitions.h"

void configurecanModule();
void getCanMessage();
void sendCanMessage(uint32_t canID, byte* message);

