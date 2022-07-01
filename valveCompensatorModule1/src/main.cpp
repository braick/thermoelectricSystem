#include <Arduino.h>
#include "definitions.h"
#include "motorDriverModule.h"
#include "canBusCom.h"

uint8_t state = 0;
bool calibrationFlag = true;
unsigned long T1=0, T0=0;

float setPt = 0;

long sensorV = 0;
void setup() 
{
  Serial.begin(9600);
  configurecanModule();
  configureDriverModule();
}

void loop() 
{
  
  state = getCanMessage();

  switch (state)
  {
  case stop:
  {
    break;
  }
  case calibrate:
  {

    calibrationFlag = true;
    break;
  }
  case monitorin:
  {
    break;
  }
  case positionControl:
  {
    if (!calibrationFlag)
    {
      break;
    }
    
    T1=millis();
    if (T1-T0>Ts)
    {
      T0 = T1;

      setPt = constrain(setPt, 0, 70);
      sensorV =  PWMcontrolActionCalc ((int)setPt,0);
      
    }
    
    break;
  }
  default:
    state = 0;
    break;
  }
}