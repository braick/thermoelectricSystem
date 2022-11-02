#include "mcp2515.h"
#include "definitions.h"

void configurecanModule();
void getCanMessage();
void sendPos2CAN (byte originMod, byte destMod, long valvePos);
void sendCANok(byte originMod, byte destMod);
void sendCANMsg(byte originMod, byte destMod, uint8_t cmd, byte *dataOut);
void serialFlush();
void sendCANMsgToSerial (can_frame canMsg);
void serialRecieve ();
