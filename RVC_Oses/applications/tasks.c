
#include <stdio.h>
#include <stdlib.h>
#include <rthw.h>
#include <rtthread.h>
#include <tasks.h>

#define OBSTACLE_CONTROL_PRIORITY      2
#define MOVEMENT_CONTROL_PRIORITY      1
#define THREAD_TIMESLICE               5
#define EVENT_FLAG1 (1 << 1)


//********************************* STRUCTURES *****************************************************************

static struct rt_event event_obstacle;

static struct rt_timer timer_obstacle_control;

ALIGN(RT_ALIGN_SIZE)
static char obstacle_control_stack[1024];
static struct rt_thread obstacle_control;

ALIGN(RT_ALIGN_SIZE)
static char movement_control_stack[1024];
static struct rt_thread movement_control;


//******************************* ENTRIES AND TIMEOUT FUNCTIONS ************************************************


/* Entry for the task obstacle control */
static void obstacle_control_entry(void *param)
{
    while (1) {
        if (rt_event_recv(&event_obstacle, EVENT_FLAG1,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            rt_kprintf("Obstacles control\n");
        }
    }
}


/* Entry for the task movement control */
static void movement_control_entry(void *param)
{
    rt_kprintf("Movement control\n");
}


/* Timeout function for the timer */
static void timeout_obstacle_control(void* parameter)
{
    rt_kprintf("periodic timer is timeout\n");
    rt_event_send(&event_obstacle, EVENT_FLAG1);
}


//********************************** INITIALIZATION FUNCTION ***************************************************


int movement_threads_start(void)
{

    // initializing event
    rt_err_t result;
    result = rt_event_init(&event_obstacle, "event_obstacle", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init event failed.\n");
        return -1;
    }


    // initializing and starting obstacle_control thread
    rt_thread_init(&obstacle_control,
                   "obstacle_control",
                   obstacle_control_entry,
                   RT_NULL,
                   &obstacle_control_stack[0],
                   sizeof(obstacle_control_stack),
                   OBSTACLE_CONTROL_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&obstacle_control);

    // initializing and starting movement_control thread
    rt_thread_init(&movement_control,
                   "movement_control",
                   movement_control_entry,
                   RT_NULL,
                   &movement_control_stack[0],
                   sizeof(movement_control_stack),
                   MOVEMENT_CONTROL_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&movement_control);


    // initializing and starting the timer for obstacle_control
    rt_timer_init(&timer_obstacle_control, "timer_obstacle_control",
                    timeout_obstacle_control,
                    RT_NULL,
                    500,    // to achieve a period of 500ms we need to put 50
                    RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&timer_obstacle_control);


    return 0;
}


