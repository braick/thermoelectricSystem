#include <SPI.h>
#include <Arduino.h>

#define A_Pin 3
#define B_Pin 4
#define C_Pin 5
#define cs_Pin 9

void configThModModule();
void muxSelect(uint8_t outputNumSelect);
float readThermocouple(uint8_t pin);