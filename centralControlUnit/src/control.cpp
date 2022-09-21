#include "control.h"

uint8_t systemSMHT = 0, systemSMLT = 0;

TaskHandle_t controlTaskHandle = NULL;
void controlTaskFuntion (void* parameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 250;

    pinMode(valve1RelayGPIO, OUTPUT);
    pinMode(valve2RelayGPIO, OUTPUT);
    pinMode(alarmRelayGPIO, OUTPUT);
    pinMode(unconectedRelayGPIO, OUTPUT);

    digitalWrite(valve1RelayGPIO, HIGH);
    digitalWrite(valve2RelayGPIO, HIGH);
    digitalWrite(alarmRelayGPIO, HIGH);
    digitalWrite(unconectedRelayGPIO, HIGH);

    Serial.println("outuputs configured");
    for (;;)
    {   
        //Serial.print("control state:");
        //Serial.println(systemSMHT);
        switch (systemSMHT)
        {
        case 0:
        {
            digitalWrite(valve1RelayGPIO, HIGH);
            digitalWrite(valve2RelayGPIO, HIGH);
            digitalWrite(alarmRelayGPIO, HIGH);
            break;
        }
        case 1:
        {
            digitalWrite(valve1RelayGPIO, LOW);
            digitalWrite(valve2RelayGPIO, LOW);
            digitalWrite(alarmRelayGPIO, HIGH);
            break;
        }
        case 2:
        {
            digitalWrite(valve1RelayGPIO, HIGH);
            digitalWrite(valve2RelayGPIO, HIGH);
            digitalWrite(alarmRelayGPIO, LOW);
            break;
        }
        default:
        {
            digitalWrite(valve1RelayGPIO, HIGH);
            digitalWrite(valve2RelayGPIO, HIGH);
            digitalWrite(alarmRelayGPIO, HIGH);
            Serial.println("default state");
            break;
        }
        }
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
    
}