
#include <rtthread.h>
#include <rtdevice.h>

#include "tasks.h"
#include "structures.h"


/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{
    int obstacle;
    rt_pin_mode(SENSOR_PIN_NUMBER, PIN_MODE_INPUT);

    // infinite loop
    while (1)
    {
        // waits for the periodic activation from the timer
        if (rt_event_recv(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            // check if there's an obstacle, if yes activates movements threads
            obstacle =  rt_pin_read(SENSOR_PIN_NUMBER);
            if (obstacle == 1)
            {
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND2);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND1);
            }
        }
    }
}


/* Entry for the task movement stop. */
void movement_stop_entry(void *param)
{
    // infinite loop
    while (1) {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND1,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            rt_kprintf("Engine stopped!\n");
        }
    }
}


/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    // infinite loop
    while (1) {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND2,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            /* Here I will insert code to decide where to go next */
            rt_kprintf("Turn rigth and start again.\n");
        }
    }
}
