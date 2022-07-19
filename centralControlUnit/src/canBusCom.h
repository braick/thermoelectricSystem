#include <Arduino.h>
#include <SPI.h>
#include "mcp2515.h"
#include "definitions.h"
#include "tasks.h"

void configureCANModule();
void sendCANMsg (byte originMod, byte destMod, uint8_t cmd, byte *dataOut);
void CANframeProcess(can_frame canMsg);