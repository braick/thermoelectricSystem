#include "tasks.h"
#include <Arduino.h>
void intializeTasks1()
{
  extern TaskHandle_t  canBusComTaskHandle;
  xTaskCreatePinnedToCore(
  canBusTask, 
  "tarea de comunicacion CAN ", 
  100000, 
  NULL, 
  1, 
  &canBusComTaskHandle, 
  1 
  );
  
  extern TaskHandle_t  controlTaskHandle;
  xTaskCreatePinnedToCore(
  controlTaskFuntion, 
  "tarea de control del sistema ", 
  2000, 
  NULL, 
  1, 
  &controlTaskHandle, 
  1 
  );
  
}

void intializeTasks2()
{
  
  extern TaskHandle_t  samplingCallsTaskHandle;
  xTaskCreatePinnedToCore(
  samplingCallsTask, 
  "tarea de peticiones periodicas a can ", 
  10000, 
  NULL, 
  1, 
  &samplingCallsTaskHandle, 
  1 
  );

}