#include "mcp2515.h"
#include "definitions.h"

void configurecanModule();
void getCanMessage();
void sendPos2CAN (byte originMod, byte destMod, long valvePos);
void sendCANok(byte originMod, byte destMod);

