
#include <rtthread.h>
#include "structures.h"



/* Timeout function for the timer that activates obstacle control periodically */
void timeout_obstacle_control(void* parameter)
{
    rt_kprintf("periodic timer is timeout\n");
    rt_event_send(&event_obstacle, EVENT_FLAG1);
}
