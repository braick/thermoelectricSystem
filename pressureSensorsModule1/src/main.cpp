#include <Arduino.h>
#include "ADCModule.h"
#include "canBusCom.h"


uint8_t cmd = 255;

extern pressSensor pressureSensors[sensorsNum];
unsigned long t1, t0;

void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  configureSensors();
  configurecanModule();
}

void loop() 
{
  t1 = millis();
  if (t1-t0>300)
  {
    t0 = t1;
    readSensors();
  }
  cmd = getCanMessage();
  if (cmd!=255)
  {
    sendCanMessage(pressureSensors[cmd].measuresMean,cmd);
  }  
}