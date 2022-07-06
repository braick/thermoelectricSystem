#include <Arduino.h>
//#include "definitions.h"
#include "motorDriverModule.h"
#include "canBusCom.h"

valveControlSM SM = stop;
bool calibrationFlag = true;
unsigned long T1=0, T0=0;

int setPt = 0;
long sensorV = 0;
void setup() 
{
  Serial.begin(9600);
  configurecanModule();
  configureDriverOutputs();
  //configureDriverModule();
}

void loop() 
{
  
  getCanMessage();

  switch (SM)
  {
  case stop:
  {
    openValve();
    break;
  }
  case calibrate:
  {
    calibrateValve();
    calibrationFlag = true;
    SM=stop;
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
      SM = calibrate;
      break;
    }
    
    T1=millis();
    if (T1-T0>Ts)
    {
      T0 = T1;

      setPt = constrain(setPt, 0, 70);
      sensorV =  PWMcontrolActionCalc (setPt,0);
      
    }
    
    break;
  }
  default:
    SM = stop;
    break;
  }
}