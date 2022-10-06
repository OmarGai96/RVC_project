
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdio.h>

#include "system.h"
#include "tasks.h"
#include "structures.h"
#include "cpu_usage.h"

int startingTime =0;

static void hook_of_scheduler(struct rt_thread* from, struct rt_thread* to)
{
    rt_kprintf("from: %s -->  to: %s at %d ms\n", from->name , to->name, rt_tick_get()*10);
}


int main(void){

    rt_err_t result;

    // This function initializes global flags used by the application
    initSystem();

    // initialize the timer management system
    //rt_system_timer_init();

    // set the scheduler hook
    // rt_scheduler_sethook(hook_of_scheduler);


// ***************************************** STRUCTURES *********************************************************


    // initializing EVENT that activates obstacle_control task periodically
    result = rt_event_init(&event_tasks_activation, "event_tasks_activation", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
#ifdef DEBUG
        rt_kprintf("Initialization of tasks activation event failed.\n");
#endif
        return -1;
    }
    // initializing EVENT that notifies there's an obstacle
    result = rt_event_init(&event_obstacle, "event_obstacle", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
#ifdef DEBUG
        rt_kprintf("Initialization of obstacle notification event failed.\n");
#endif
        return -1;
    }

    // Initializing EVENT object for resources
    result = rt_event_init(&event_resources, "event_resources", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK){
#ifdef DEBUG
        printf("Initialization of resources event failed.\n");
#endif
        return -1;
    }


    // initializing the TIMER for obstacle_control
    rt_timer_init(&timer_obstacle_control_activation, "timer_obstacle_control_activation",
                    timeout_obstacle_control,
                    RT_NULL,
                    PERIOD_TASK1, // every 200ms control for an obstacle
                    RT_TIMER_FLAG_PERIODIC);
    // initializing the TIMER for movement_control
    rt_timer_init(&timer_movement_control_activation, "timer_movement_control_activation",
                  timeout_movement_control,
                  RT_NULL,
                  PERIOD_TASK2, //every 500 ms
                  RT_TIMER_FLAG_PERIODIC);
    // initializing the TIMER for check resources
    rt_timer_init(&timer_check_resources_activation, "timer_check_resources_activation",
                  timeout_check_resources,
                  RT_NULL,
                  PERIOD_TASK3, //every 250 ms
                  RT_TIMER_FLAG_PERIODIC);

    // initializing the TIMER for acoustic signals
    rt_timer_init(&timer_acoustic_signals_activation, "timer_acoustic_signals_activation",
                  timeout_acoustic_signals,
                  RT_NULL,
                  PERIOD_TASK4, // every 500 ms
                  RT_TIMER_FLAG_PERIODIC);

    // initializing the TIMER for brushes_speed
    rt_timer_init(&timer_brushes_speed_activation, "timer_brushes_speed_activation",
                  timeout_brushes_speed,
                  RT_NULL,
                  PERIOD_TASK5,   //every 250 ms
                  RT_TIMER_FLAG_PERIODIC);


    // Initializing a mailbox for communication between T3 and T2
        result = rt_mb_init(&mb2_3,
                            "mb_res",
                            &mb_pool[0],
                            sizeof(mb_pool) / 4,
                            RT_IPC_FLAG_FIFO);
        if (result != RT_EOK)
        {
#ifdef DEBUG
            rt_kprintf("init mailbox failed.\n");
#endif
            return -1;
        }


    // Initializing a mailbox for communication between T5 and T2
        result = rt_mb_init(&mb2_5,
                            "mb_brushes",
                            &mb_pool2_5[0],
                            sizeof(mb_pool2_5) / 4,
                            RT_IPC_FLAG_FIFO);
        if (result != RT_EOK){
#ifdef DEBUG
             rt_kprintf("init mailbox failed.\n");
#endif
             return -1;
        }


// ****************************************** THREADS ***********************************************************


    // initializing obstacle_control thread
    rt_thread_init(&obstacle_control,
                   "Task1",
                   obstacle_control_entry,
                   RT_NULL,
                   &obstacle_control_stack[0],
                   sizeof(obstacle_control_stack),
                   OBSTACLE_CONTROL_PRIORITY, THREAD_TIMESLICE);

    // initializing movement_stop thread
    rt_thread_init(&movement_stop,
                   "TaskS",
                   movement_stop_entry,
                   RT_NULL,
                   &movement_stop_stack[0],
                   sizeof(movement_stop_stack),
                   MOVEMENT_STOP_PRIORITY, THREAD_TIMESLICE);

    // initializing movement_control thread
    rt_thread_init(&movement_control,
                   "Task2",
                   movement_control_entry,
                   RT_NULL,
                   &movement_control_stack[0],
                   sizeof(movement_control_stack),
                   MOVEMENT_CONTROL_PRIORITY, THREAD_TIMESLICE);

    // initializing check_resources thread
    rt_thread_init(&check_resources,
                   "Task3",
                   check_resources_entry,
                   RT_NULL,
                   &check_resources_stack[0],
                   sizeof(check_resources_stack),
                   CHECK_RESOURCES_PRIORITY, THREAD_TIMESLICE);

    // initializing acoustic_signals thread
    rt_thread_init(&acoustic_signals,
                   "Task4",
                   acoustic_signals_entry,
                   RT_NULL,
                   &acoustic_signals_stack[0],
                   sizeof(acoustic_signals_stack),
                   ACOUSTIC_SIGNALS_PRIORITY, THREAD_TIMESLICE);

    // initializing brushes speed thread
    rt_thread_init(&brushes_speed,
                   "Task5",
                   brushes_speed_entry,
                   RT_NULL,
                   &brushes_speed_stack[0],
                   sizeof(brushes_speed_stack),
                   BRUSHES_SPEED_PRIORITY, THREAD_TIMESLICE);




// *************************************** STARTING *************************************************************

    // initialize the timer management system
    rt_system_timer_init();

    // starting the threads
    rt_thread_startup(&obstacle_control);
    rt_thread_startup(&movement_stop);
    rt_thread_startup(&movement_control);
    rt_thread_startup(&check_resources);
    rt_thread_startup(&acoustic_signals);
    rt_thread_startup(&brushes_speed);

    startingTime = rt_tick_get_millisecond();  //set the current starting time before launching our timers

    // starting the timers
    rt_timer_start (&timer_obstacle_control_activation);
    rt_timer_start (&timer_movement_control_activation);
    rt_timer_start (&timer_check_resources_activation);
    rt_timer_start (&timer_acoustic_signals_activation);
    rt_timer_start (&timer_brushes_speed_activation);



    return 0;
}

