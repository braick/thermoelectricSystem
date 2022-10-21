#include <Arduino.h>
#include "canBusCom.h"

extern byte byteArrayTx[8];
//byte buffArray[8] = {0xf0, 0xf1, 0xf2, 0xf3,0xf4,0xf5,0xf6,0xf7};

void setup() {
  Serial.begin(115200);
  configurecanModule();
}

void loop() {
  getCanMessage();
  //Serial.write(buffArray,8);
  //delay(1);
  }