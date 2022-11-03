#include "control.h"

extern SystemSensors systemSensorsCAN;
extern QueueHandle_t systemSensorsQueue;

uint8_t systemControlSM = valsOn;
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
        switch (systemControlSM)
        {
        case allOFF:
        {
            digitalWrite(valve1RelayGPIO, HIGH);
            digitalWrite(valve2RelayGPIO, HIGH);
            digitalWrite(alarmRelayGPIO, HIGH);
            break;
        }
        case valsOn:
        {
            digitalWrite(valve1RelayGPIO, LOW);
            digitalWrite(valve2RelayGPIO, LOW);
            digitalWrite(alarmRelayGPIO, HIGH);
            break;
        }
        case val1Off:
        {
            digitalWrite(valve1RelayGPIO, HIGH);
            digitalWrite(valve2RelayGPIO, LOW);
            digitalWrite(alarmRelayGPIO, LOW);
            break;
        }
        case val2Off:
        {
            digitalWrite(valve1RelayGPIO, LOW);
            digitalWrite(valve2RelayGPIO, HIGH);
            digitalWrite(alarmRelayGPIO, LOW);
            break;
        }
        case valsOff:
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
            break;
        }
        }   
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }
    
}