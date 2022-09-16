
#include "motorDriverModule.h"

bool rotation_sense = 0;//sentido de rotacion
MCP3208 myADC(10);

///////////////////////////////////////////////////
/////////PARAMETROS DEL LAZO DE CONTROL////////////
///////////////////////////////////////////////////

int getValvePosition()
{
    int value = 0;
    for (size_t i = 0; i < 4; i++)
    {
      value += myADC.analogRead(i);
    }
    return value/8;
}

//motor sensor parameters
long minSensorValue = 843;  //lectura del punto de maximo cierre de la valvula
long maxSensorValue = 4277; //lectura del punto de minimo cierre de la valvula

long minEqPtPWM = 60; //punto de equilibrio en el punto de cierre minimo
long maxEqPtPWM = 80; //punto de equilibrio en el punto de cierre maximo
long EqPointPWM = 50;
bool enableSlideCont = false;

//motor control parameters
long setPoint = 0;
long t0 = 0;
long t1 = 0;
long dt = 10;

long valvePosition0 = 0;
long valvePosition1 = 0;
long rawpos = 0;
float lpfCoef = 0.1;
long dp = 0;
long error = 0;

float P = 0.0;
float D = 0.0;
float I = 0.0;
float PIDr = 0.0;
long dutyPID = 0;

float kp = 45;//35
float kd = 2000;//4000
float ki = 0.2;//0.2

uint8_t dutyCycle = 0; //ciclo de trabajo del pwm
bool enablePIDCont = false;

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


void calibrateValve()
{

    analogWrite(PWM_pin, 110);
    delay(500);
    analogWrite(PWM_pin, 120);
    delay(500);
    analogWrite(PWM_pin, 130);
    delay(500);
    analogWrite(PWM_pin, 140);
    delay(500);
    analogWrite(PWM_pin, 150);
    delay(2000);
    
    float rawminSensorValue = 0;
    for (size_t i = 0; i < 100; i++)
    {
      rawminSensorValue += getValvePosition();
      delay(20);
    }
    
    minSensorValue = (long)(rawminSensorValue /100.0);
    minSensorValue = (long)((float)minSensorValue * 1.02);

    analogWrite(PWM_pin, 50);
    delay(500);
    analogWrite(PWM_pin, 20);
    delay(500);
    analogWrite(PWM_pin, 15);
    delay(500);
    analogWrite(PWM_pin, 10);
    delay(500);
    analogWrite(PWM_pin, 0);
    delay(2000);

    float rawmaxSensorValue = 0;
    for (size_t i = 0; i < 100; i++)
    {
      rawmaxSensorValue += getValvePosition();
      delay(20);
    }
    
    maxSensorValue = (long)(rawmaxSensorValue /100.0);
    
    //Serial.println(minSensorValue);
    //Serial.println(maxSensorValue);

}
void configureDriverOutputs()
{
    pinMode(PWM_pin, OUTPUT);
    pinMode(bridge_pin_1, OUTPUT);
    pinMode(bridge_pin_2, OUTPUT);
    
    digitalWrite(bridge_pin_1, rotation_sense);
    digitalWrite(bridge_pin_2, !rotation_sense);
}

void openValve()
{
    analogWrite(PWM_pin,0);
}

void monitorPos()
{
  long sensorValue =  getValvePosition();
  valvePosition1 = map((long)sensorValue,maxSensorValue,minSensorValue,0,5000);
}
/**
 * @brief Funcion de calculo de la accion de control
 * 
 * @param setPoin Consigna en tanto porciento con un decimal
 * @param sensorValue Valor de tension leido del sensor
 * @return uint8_t 
 */
long PWMcontrolActionCalc (int closeAngle, uint8_t controlState)
{   
    
    long sensorValue =  getValvePosition();
    //Sliding compenator
    EqPointPWM = map(sensorValue, minSensorValue, maxSensorValue, maxEqPtPWM, minEqPtPWM);
    //Serial.println(EqPointPWM);
    //analogWrite(PWM_pin,(uint8_t)EqPointPWM);
    //return (uint8_t)EqPointPWM;
    //PID compensator

    setPoint = map(closeAngle,0,100,0,5000);
    valvePosition0 = valvePosition1;
    valvePosition1 = map((long)sensorValue,maxSensorValue,minSensorValue,0,5000);
    dp = valvePosition1 - valvePosition0;
    error = setPoint - valvePosition1; 
    
    t0 = t1;
    t1 = millis();
    dt = t1 - t0;

    //Serial.println(dt);
    if (dt < 1 || dt > 15)  
    {
        dt = 10;
    }
    
    P =kp*(float)error;
    if (abs(error)>50)
    {
      I += ki*(error*(float)(dt));
    }
    I = constrain(I,-80000,80000);
    D = kd*((float)dp/(float)(dt));
    PIDr = P - D + I; 
    dutyPID = map((long)PIDr, -130000, 130000, -255, 255);
    dutyCycle = constrain(dutyPID + EqPointPWM,0,255);
    analogWrite(PWM_pin,dutyCycle);
    return sensorValue;
}