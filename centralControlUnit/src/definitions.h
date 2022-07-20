//Lista de definiciones

//estados de la state machine
#define stop 0
#define calibrate 1
#define monitorin 2
#define positionControl 3

//tiempo de muestreo
#define Ts 10

//direcciones

#define CCUDir 0x01
#define valveCompDir1 0xA1
#define valveCompDir2 0xA2
#define pressureModDir 0xB1
#define tempModDir1 0xC1
#define tempModDir2 0xC2

//comandos modulo de la valvula

#define    stopValve                0
#define    calibrateValve           1
#define    monitorinCMD             2
#define    positionControlValve     3
#define    sendPositionValve        4
#define    modSetPtValve             5


////////////////////////////////

#define sendSensorsData 0x01

//respuestas 
#define ok 0xa0//ok
#define dataCorrupt 0xff//estructura de datos no coincide con el formato esperado
#define queueWriteError 0xfe//error de volcado de datos en las colas 
#define nothigToRead 0xfc// colas de datos vacias 
#define unknowCommand 0xfb//comando desonocido 
#define dataSizeExceded 0xfa//se excede el numero de datos que se puede enviar por red//255bytes
#define unknownFail 0xf9//error desconocido//


#define pressSensorNum 8
#define tempSensorNum 8

//comandos

//controlador de la valvula

#define modSetPtCMD 0x01
#define readPosition 0x02


//modulo de termopares

#define readTemperatures 0x01

// modulo de sensores de presionç

#define readPressures 0x01

struct SystemSensors
{
    float pressureArray[pressSensorNum];
    float tempArrayMod1[tempSensorNum];
    float tempArrayMod2[tempSensorNum];
    long valve1Pos;
    long valve2Pos;
};
