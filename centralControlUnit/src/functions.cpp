#include "dataStructures.h"
#include "Arduino.h"

float meanOfFloatArrray(float *floatArray, uint16_t size)
{
  float auxVarMean = 0;
  //Serial.println(size);
  for (size_t k = 0; k < size; k++)
  {
    //Serial.println(auxVarMean);
    auxVarMean += floatArray[k];
  }
  //Serial.println(auxVarMean);
  return auxVarMean/(float)size;
  
}

void pushBackReadValue (float newValue, sensorDataStruct *dataStruct)
{
    float auxArray[moveMeanSizeFilter] = {0.0};
    memcpy(&auxArray[1],&(*dataStruct).value,(moveMeanSizeFilter-1)*sizeof(float));
    auxArray[0] = newValue;
    memcpy((*dataStruct).value,auxArray,moveMeanSizeFilter*sizeof(float));
}