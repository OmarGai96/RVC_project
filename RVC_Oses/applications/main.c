
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdio.h>

#include "system.h"
#include "tasks.h"
#include "structures.h"

//#define BENCHMARKING


static void hook_of_scheduler(struct rt_thread* from, struct rt_thread* to)
{
    rt_kprintf("from: %s -->  to: %s at %d ms\n", from->name , to->name, rt_tick_get()*10);
}


int main(void){

    rt_err_t result;

    // This function initializes global flags used by the application
    initSystem();

    // initialize the timer management system
    rt_system_timer_init();

    // set the scheduler hook
    // rt_scheduler_sethook(hook_of_scheduler);


// ***************************************** STRUCTURES *********************************************************


    // initializing EVENT that activates obstacle_control task periodically
    result = rt_event_init(&event_tasks_activation, "event_tasks_activation", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Initialization of tasks activation event failed.\n");
        return -1;
    }
    // initializing EVENT that notifies there's an obstacle
    result = rt_event_init(&event_obstacle, "event_obstacle", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("Initialization of obstacle notification event failed.\n");
        return -1;
    }


    // initializing the TIMER for obstacle_control
    rt_timer_init(&timer_obstacle_control_activation, "timer_obstacle_control_activation",
                    timeout_obstacle_control,
                    RT_NULL,
                    50, // every 500ms control for an obstacle
                    RT_TIMER_FLAG_PERIODIC);
    // initializing the TIMER for movement_control
    rt_timer_init(&timer_movement_control_activation, "timer_movement_control_activation",
                  timeout_movement_control,
                  RT_NULL,
                  500,
                  RT_TIMER_FLAG_PERIODIC);
    // initializing the TIMER for check resources
    rt_timer_init(&timer_check_resources_activation, "timer_check_resources_activation",
                  timeout_check_resources,
                  RT_NULL,
                  5000,
                  RT_TIMER_FLAG_PERIODIC);

    // Initializing event object for resources
        result = rt_event_init(&event_resources, "event_resources", RT_IPC_FLAG_FIFO);
        if (result != RT_EOK){
            printf("Initialization of resources event failed.\n");
            return -1;
        }


    // Initializing a mailbox for communication between T3 and T2
        result = rt_mb_init(&mb2_3,
                            "mb_res",
                            &mb_pool[0],
                            sizeof(mb_pool) / 4,
                            RT_IPC_FLAG_FIFO);
        if (result != RT_EOK)
        {
            rt_kprintf("init mailbox failed.\n");
            return -1;
        }


// ****************************************** THREADS ***********************************************************


    // initializing obstacle_control thread
    rt_thread_init(&obstacle_control,
                   "obstacle_control",
                   obstacle_control_entry,
                   RT_NULL,
                   &obstacle_control_stack[0],
                   sizeof(obstacle_control_stack),
                   OBSTACLE_CONTROL_PRIORITY, THREAD_TIMESLICE);

    // initializing movement_stop thread
    rt_thread_init(&movement_stop,
                   "movement_stop",
                   movement_stop_entry,
                   RT_NULL,
                   &movement_stop_stack[0],
                   sizeof(movement_stop_stack),
                   MOVEMENT_STOP_PRIORITY, THREAD_TIMESLICE);

    // initializing movement_control thread
    rt_thread_init(&movement_control,
                   "movement_control",
                   movement_control_entry,
                   RT_NULL,
                   &movement_control_stack[0],
                   sizeof(movement_control_stack),
                   MOVEMENT_CONTROL_PRIORITY, THREAD_TIMESLICE);

    // initializing check_resources thread
    rt_thread_init(&check_resources,
                   "check_resources",
                   check_resources_entry,
                   RT_NULL,
                   &check_resources_stack[0],
                   sizeof(check_resources_stack),
                   CHECK_RESOURCES_PRIORITY, THREAD_TIMESLICE);

    // initializing acoustic_signal thread
    rt_thread_init(&acoustic_signals,
                   "acoustic_signal",
                   acoustic_signals_entry,
                   RT_NULL,
                   &acoustic_signals_stack[0],
                   sizeof(acoustic_signals_stack),
                   ACOUSTIC_SIGNALS_PRIORITY, THREAD_TIMESLICE);



// *************************************** STARTING *************************************************************


    // starting the timers
    rt_timer_start (&timer_obstacle_control_activation);
    rt_timer_start (&timer_movement_control_activation);
    rt_timer_start (&timer_check_resources_activation);

    // starting the threads
    rt_thread_startup(&obstacle_control);
    rt_thread_startup(&movement_stop);
    rt_thread_startup(&movement_control);
    rt_thread_startup(&check_resources);
    rt_thread_startup(&acoustic_signals);

    return 0;
}
