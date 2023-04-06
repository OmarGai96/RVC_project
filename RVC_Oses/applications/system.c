
#include <rtthread.h>
#include "system.h"
#include "structures.h"
#include "tasks.h"

/** initialize global variables **/
rt_err_t initSystem(void){

    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;
    rt_err_t result;

    // initializing System Task
    result = rt_thread_init(&Tsystem,
                           "TSystem",
                           Tsystem_task_entry,
                           RT_NULL,
                           &Tsystem_stack[0],
                           sizeof(Tsystem_stack),
                           TSYSTEM_PRIORITY,
                           THREAD_TIMESLICE);
    if (result != RT_EOK){
#ifdef DEBUG
        rt_kprintf("init thread TSystem failed.\n");
#endif
        return RT_ERROR;
    }

    result = rt_thread_startup(&Tsystem);
    if (result != RT_EOK){
#ifdef DEBUG
        rt_kprintf("startup of thread TSystem failed.\n");
#endif
        return RT_ERROR;
    }
    reset_end_flag(&Tsystem);
    return RT_EOK;
}

void Tsystem_task_entry(void *param){

    int curr_time = 0;

    rt_signal_install(SIGUSR2, turnOffSystem);
    rt_signal_unmask(SIGUSR2);

    set_task_started(&Tsystem, 10);

    while(1){
            if(turnOffFlag==1){
                if(closeAllTasks()==RT_EOK){
                    set_task_as_preemptable(&Tsystem, TSYSTEM_PRIORITY);
                    break;
                }
            }else{
                curr_time=rt_tick_get_millisecond();
                if((curr_time%900) == 0 && startingTime != 0){
                    rt_mutex_take(&battMutex, RT_WAITING_FOREVER);
                    batteryStatus-=DEC_VALUE;
                    printf("\n");
                    rt_mutex_release(&battMutex);
                }


            }

    }

     rt_kprintf("\n---------------System is TURNED OFF--------------------\n");

}
