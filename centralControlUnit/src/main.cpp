#include <Arduino.h>
#include "tasks.h"
#include "control.h"


void setup() {
  pinMode(valve1RelayGPIO, OUTPUT);
  pinMode(valve2RelayGPIO, OUTPUT);
  pinMode(alarmRelayGPIO, OUTPUT);
  pinMode(unconectedRelayGPIO, OUTPUT);
  
  digitalWrite(valve1RelayGPIO, HIGH);
  digitalWrite(valve2RelayGPIO, HIGH);
  digitalWrite(alarmRelayGPIO, HIGH);
  digitalWrite(unconectedRelayGPIO, HIGH);
  Serial.begin(115200);
  //inciciar las tareas
  Serial.println("Starting...");
  delay(5000);
  intializeTasks1();
  
}

void loop() 
{
  Serial.println("Void loop");
  //suspender tarea del loop de arduino
  vTaskSuspend(NULL);
}