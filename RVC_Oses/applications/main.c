
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "system.h"
#include "tasks.h"
#include "structures.h"


int main(void)
{

    rt_err_t result;

    // This function initializes global flags used by the application
    initSystem();

    // initialize the timer management system
    rt_system_timer_init();


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
                    500,
                    RT_TIMER_FLAG_PERIODIC);


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


// *************************************** STARTING *************************************************************


    // starting the timers
    rt_timer_start (&timer_obstacle_control_activation);
    // starting the threads
    rt_thread_startup(&obstacle_control);
    rt_thread_startup(&movement_stop);
    rt_thread_startup(&movement_control);


    return RT_EOK;

}


