
#include <rtthread.h>

#include "system.h"
#include "structures.h"
#include "tasks.h"

/** initialize global variables **/
void initSystem(void){

    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;

    // initializing System Task
        rt_thread_init(&Tsystem,
                       "TSystem",
                       Tsystem_task_entry,
                       RT_NULL,
                       &Tsystem_stack[0],
                       sizeof(Tsystem_stack),
                       TSYSTEM_PRIORITY,
                       THREAD_TIMESLICE);

        rt_thread_startup(&Tsystem);
        reset_end_flag(&Tsystem);
}


