#ifndef ARDUINO_LIBRARY
#define ARDUINO_LIBRARY
#include <Arduino.h>
#endif

#ifndef I2C_LIBRARY
#define I2C_LIBRARY
#include <Wire.h>
#endif


#define ADC1 0x48 // direccion del ADC // conectado ADR a GND
#define ADC2 0x49 // direccion del ADC // conectado ADR a Vcc

#define ch0 0b01000000 // canal 0
#define ch1 0b01010000 // canal 1
#define ch2 0b01100000 // canal 2
#define ch3 0b01110000 // canal 3

#define maxV6   0b00000000 //fondo de escala a 6.144v
#define maxV4   0b00000010 //fondo de escala a 4.096v
#define maxV2   0b00000100 //fondo de escala a 2.048v
#define maxV1   0b00000110 //fondo de escala a 1.024v
#define maxV05  0b00001000 //fondo de escala a 0.512v
#define maxV02  0b00001010 //fondo de escala a 0.256v

#define maxV6Val   6144//mV
#define maxV4Val   4096//mV
#define maxV2Val   2048//mV
#define maxV1Val   1024//mV
#define maxV05Val  512//mV
#define maxV02Val  256//mV

#define max15BitVal 32768 

#define singleConversionMode 0b00000001
#define continuousConversionMode 0b00000000

#define sps8    0b00000000
#define sps16   0b00100000
#define sps32   0b01000000
#define sps64   0b01100000
#define sps128  0b10000000
#define sps250  0b11000000
#define sps475  0b11000000
#define sps860  0b11100000


void configureADC (byte ADCDirecction, byte chanel, byte maxVolt, byte conversionMode, byte SPS);
float ADCRead (byte ADCDirecction, float fullScale);
void insertVoltage2Press();
void configureSensors();
void readSensors();


#define sampligDelay 5

#define moveMeanSize 20
#define sensorsNum 8
struct pressSensor
{
    float mesures[moveMeanSize] = {0.0};
    float measuresMean = {0.0};
    float m = 0, b = 0;
};
