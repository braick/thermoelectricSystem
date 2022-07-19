#include <Arduino.h>
#include "tasks.h"



void setup() {
  Serial.begin(115200);
  //inciciar las tareas
  intializeTasks1();
  
}

void loop() 
{
  Serial.println("Void loop");
  //suspender tarea del loop de arduino
  vTaskSuspend(NULL);
}