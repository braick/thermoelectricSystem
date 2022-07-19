
//inicializacion de las tareas
void intializeTasks1();
void intializeTasks2();

//tarea de comunicaciones
void communication (void* parameters);

//tarea de comunicacion CAN
void canBusTask (void* parameters);

//tarea de comandos periodicos a CAN
void samplingCallsTask (void* parameters);

//tarea de control de la posicion de la valvula
//void valvePosControl(void *parameters);

//tarea de lectura de los sensores de presion
//void readDAQ (void* parameters);

//tarea de lectura de los sensores de presion
//void readThermocouples (void* parameters);

