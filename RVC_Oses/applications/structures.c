
#include <rtthread.h>
#include "structures.h"



/* Timeout function for the timer that activates obstacle control periodically */
void timeout_obstacle_control(void* parameter)
{
    rt_event_send(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION);
}


/* Timeout function for the timer that activates map management periodically */
void timeout_movement_control(void* parameter)
{
    rt_event_send(&event_tasks_activation, EVENT_MOVEMENT_CONTROL_ACTIVATION);
}

/* Timeout function for the timer that activates check resources periodically */
void timeout_check_resources(void* parameter)
{
    rt_event_send(&event_tasks_activation, EVENT_CHECK_RESOURCES_ACTIVATION);
}

/* Timeout function for the timer that activates brushes speed periodically */
void timeout_brushes_speed(void* parameter)
{
    rt_event_send(&event_tasks_activation, EVENT_BRUSHES_SPEED_ACTIVATION);
}



