#include "conversorRead.h"


void configThModModule()
{
    SPI.begin();
    pinMode(A_Pin,OUTPUT);
    pinMode(B_Pin,OUTPUT);
    pinMode(C_Pin,OUTPUT);
    pinMode(cs_Pin,OUTPUT);
    digitalWrite(A_Pin,LOW);
    digitalWrite(B_Pin,LOW);
    digitalWrite(C_Pin,LOW);
    digitalWrite(cs_Pin,HIGH);
}

void muxSelect(uint8_t outputNumSelect)
{
  uint8_t A, B, C;
  uint8_t mask = 0b00000001;
  A = outputNumSelect & mask;
  B = (outputNumSelect>>1) & mask;
  C = (outputNumSelect>>2) & mask;

  digitalWrite(A_Pin, A);
  digitalWrite(B_Pin, B);
  digitalWrite(C_Pin, C);
}

float readThermocouple(uint8_t pin)
{
  uint16_t tempCounts = 0;
  uint8_t thermoOK = 1;
  #define tempSteps 0.25
  float temp = 0;

  SPI.beginTransaction(SPISettings(100000,MSBFIRST,SPI_MODE1));
  digitalWrite(pin,LOW);
  tempCounts = SPI.transfer16(0x00);
  
  
  digitalWrite(pin,HIGH);
  SPI.endTransaction();
  
 
  thermoOK = (tempCounts >> 2) & 0x01;
  if(thermoOK != 0)
  {
    temp = -1;
  }
  else
  {
    temp = (tempCounts >> 3) * tempSteps;
  }
  //Serial.println(tempCounts,BIN);
  return temp;
}