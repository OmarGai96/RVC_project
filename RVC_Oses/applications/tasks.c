
#include <rtthread.h>
#include "tasks.h"
#include "structures.h"


/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{
    while (1) {
        if (rt_event_recv(&event_obstacle, EVENT_FLAG1,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            rt_kprintf ( " Obstacle control \n " );
        }
    }
}


/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    rt_kprintf("Movement control\n");
}
