#ifndef ARDUINO_LIBRARY
#define ARDUINO_LIBRARY
#include <Arduino.h>
#endif
#ifndef SPI_LIB
#define SPI_LIB
#include <SPI.h>
#endif

#include "MCP3208.h"

#define PWM_pin  5 //pin para el pwm del motor
#define bridge_pin_1 6//pin sentido del puento
#define bridge_pin_2 8

void calibrateValve();
void configureDriverOutputs();
long PWMcontrolActionCalc (int closeAngle, uint8_t controlState);
void openValve();