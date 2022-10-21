//Lista de definiciones

//estados de la state machine

typedef enum
{
    stop,
    calibrate,
    monitorin,
    positionControl
}   valveControlSM;
/*
#define stop 0
#define calibrate 1
#define monitorin 2
#define positionControl 3
*/
//tiempo de muestreo
#define Ts 10

//direcciones

#define linkDir 0x00
#define CCUDir 0x01
#define valveCompDir1 0xA1
#define valveCompDir2 0xA2
#define pressureModDir 0xB1
#define tempModDir1 0xC1
#define tempModDir1 0xC2

//comandos del controlador


#define    stopCMD              0
#define    calibrateCMD         1
#define    monitorinCMD         2
#define    positionControlCMD   3
#define    sendPositionCMD      4
#define    modSetPtCMD          5
