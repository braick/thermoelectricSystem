#include "ADCModule.h"
#include "calibrationParamers.h"

float sensorVoltage [sensorsNum] = {0.0};
pressSensor pressureSensors[sensorsNum];



/**
 * @brief Funcion para configurar los parametros del ADC
 * 
 * @param ADCDirecction Direccion del ADC en el bus I2C
 * @param chanel Canal de lectura 
 * @param maxVolt Fondo de escala - Configuracion del PGA
 * @param conversionMode Tipo de conversion- Continua/Power saving
 * @param SPS Muestreo
 */
void configureADC (byte ADCDirecction, byte chanel, byte maxVolt, byte conversionMode, byte SPS)
{
    byte configByte1 = chanel | maxVolt | conversionMode;
    byte configByte2 =  SPS;
    //Serial.println(configByte1, BIN);
    //Serial.println(configByte2, BIN);
    Wire.beginTransmission(ADCDirecction);
    Wire.write(0x01);//apuntar el registro de configuracion de la daq
    Wire.write(configByte1);
    Wire.write(configByte2);
    Wire.endTransmission();
    Wire.beginTransmission(ADCDirecction);
    Wire.write(0x00);//apuntar al registro de la ultima conversion
    Wire.endTransmission();
}


/**
 * @brief Funcion para leer el valor de TENSION del registro de la ADC 
 * 
 * @param ADCDirecction Direcccion del ADC dentro del bus
 * @param fullScale Fondo de escala para la conversion de bits a voltaje
 * @return float 
 */
float ADCRead (byte ADCDirecction, float fullScale)
{
    uint16_t bitValue = 0;
    double doubleValue = 0.0;
    byte readBuffer[2]= {0x00,0x00};
    Wire.requestFrom(ADCDirecction,(uint8_t)2);
    readBuffer[1] = Wire.read();
    readBuffer[0] = Wire.read();
    Wire.endTransmission();
    
    bitValue =  readBuffer[1] << 8 | readBuffer[0];
    if (bitValue > max15BitVal)
    {
      bitValue = 0;
    }
    doubleValue = (float)bitValue * (fullScale/(float)max15BitVal);

    return doubleValue;
}


void configureSensors()
{
  pressureSensors[0].m = S1_m;
  pressureSensors[0].b = S1_b;
  
  pressureSensors[1].m = S2_m;
  pressureSensors[1].b = S2_b;
  
  pressureSensors[2].m = S3_m;
  pressureSensors[2].b = S3_b;
  
  pressureSensors[3].m = S4_m;
  pressureSensors[3].b = S4_b;
  
  pressureSensors[4].m = S5_m;
  pressureSensors[4].b = S5_b;
  
  pressureSensors[5].m = S6_m;
  pressureSensors[5].b = S6_b;
  
  pressureSensors[6].m = S7_m;
  pressureSensors[6].b = S7_b;
  
  pressureSensors[7].m = S8_m;
  pressureSensors[7].b = S8_b;
}
void readSensors()

{
  configureADC(ADC1,ch0,maxV6,continuousConversionMode,sps250);
  configureADC(ADC2,ch0,maxV6,continuousConversionMode,sps250);
  delay(sampligDelay);
  sensorVoltage[0] = ADCRead(ADC1, maxV6Val);
  sensorVoltage[4] = ADCRead(ADC2, maxV6Val);

  configureADC(ADC1,ch1,maxV6,continuousConversionMode,sps250);
  configureADC(ADC2,ch1,maxV6,continuousConversionMode,sps250);
  delay(sampligDelay);
  sensorVoltage[1] = ADCRead(ADC1, maxV6Val);
  sensorVoltage[5] = ADCRead(ADC2, maxV6Val);


  configureADC(ADC1,ch2,maxV6,continuousConversionMode,sps250);
  configureADC(ADC2,ch2,maxV6,continuousConversionMode,sps250);
  delay(sampligDelay);
  sensorVoltage[2] = ADCRead(ADC1, maxV6Val);
  sensorVoltage[6] = ADCRead(ADC2, maxV6Val);


  configureADC(ADC1,ch3,maxV6,continuousConversionMode,sps250);
  configureADC(ADC2,ch3,maxV6,continuousConversionMode,sps250);
  delay(sampligDelay);
  sensorVoltage[3] = ADCRead(ADC1, maxV6Val);
  sensorVoltage[7] = ADCRead(ADC2, maxV6Val);

  insertVoltage2Press();
}


void insertVoltage2Press()
{
  for (size_t i = 0; i < sensorsNum; i++)
  {
    memcpy(&(pressureSensors[i].mesures[1]),&(pressureSensors[i].mesures[0]),sizeof(float)*(moveMeanSize-1));
    pressureSensors[i].mesures[0] = sensorVoltage[i]*pressureSensors[i].m + pressureSensors[i].b;
    float aux = 0;
    for (size_t k = 0; k < moveMeanSize; k++)
    {
      aux += pressureSensors[i].mesures[k];
    }
    pressureSensors[i].measuresMean = aux/moveMeanSize;
    
  }
  
}
