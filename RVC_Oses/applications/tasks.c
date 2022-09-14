
#include <rtthread.h>
#include "tasks.h"
#include "structures.h"


/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{
    int sensor[] = {0,0,0,1,0,0,0,1,0,0,1,0,0,0,0};
    int count = 0;

    // infinite loop
    while (1)
    {
        // waits for the periodic activation from the timer
        if (rt_event_recv(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            // simulate the sensor, if finds an obstacle send an event
            if (count<14 && sensor[count] == 1)
            {
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND1);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND2);
            }
            count++;
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
            rt_kprintf("Turn rigth and start again.\n");
        }
    }
}
