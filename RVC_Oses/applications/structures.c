
#include <rtthread.h>
#include "structures.h"



/* Timeout function for the timer that activates obstacle control periodically */
void timeout_obstacle_control(void* parameter)
{
    rt_event_send(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION);
}


// TODO: not sure if using the same event object is good
/* Timeout function for the timer that activates map management periodically */
void timeout_map_management(void* parameter)
{
    rt_event_send(&event_tasks_activation, EVENT_MAP_MANAGEMENT_ACTIVATION);
}
