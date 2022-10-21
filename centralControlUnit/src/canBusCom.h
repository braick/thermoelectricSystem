#include <Arduino.h>
#include <SPI.h>
#include "mcp2515.h"
#include "definitions.h"
#include "tasks.h"

void configureCANModule();
void sendCANMsgToQueue (byte originMod, byte destMod, uint8_t cmd, byte *dataOut, bool sendToFront);
void CANframeProcess(can_frame canMsg);
bool canMsgIdMatch(can_frame canR, can_frame canT);