
//inicializacion de las tareas
void intializeTasks1();
void intializeTasks2();

//tarea de comunicaciones
void communication (void* parameters);

//tarea de comunicacion CAN
void canBusTask (void* parameters);

//tarea de comandos periodicos a CAN
void samplingCallsTask (void* parameters);

//tarea de control del sistema
void controlTaskFuntion (void* parameters);

