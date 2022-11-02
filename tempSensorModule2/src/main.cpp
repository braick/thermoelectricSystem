#include <Arduino.h>
#include "conversorRead.h"
#include "canBusCom.h"
#include "definitions.h"

unsigned long t1 = 0, t0 = 0, dt = 0;

uint8_t cmd = 255;

float tempData[8]={0};
void setup()
{
  Serial.begin(9600);
  configurecanModule();
  configThModModule();
  //Serial.println("setup");
}

void loop() 
{
  t1 = millis();
  dt = t1-t0;

  if (dt>tempSamplingPeriod)
  {
    t0 = t1;
    for (size_t i = 0; i < 8; i++)
    {
      muxSelect(i);
      tempData[i] =  readThermocouple(cs_Pin);
      //Serial.println(tempData[i]);
    }
  }

  cmd = getCanMessage();
  if (cmd!=255)
  {
    sendCanMessage(tempData[cmd],cmd);
  }
} 