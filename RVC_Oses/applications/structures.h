
#ifndef APPLICATIONS_STRUCTURES_H_
#define APPLICATIONS_STRUCTURES_H_


// ************************************** DEFINITIONS **********************************************************


#define EVENT_OBSTACLE_CONTROL_ACTIVATION (1 << 1)
#define EVENT_OBSTACLE_FOUND1 (1 << 1)
#define EVENT_OBSTACLE_FOUND2 (1 << 2)
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)


// ************************************ STRUCTURES *************************************************************


/* TIMER used to activate periodically the obstacle control task */
struct rt_timer timer_obstacle_control_activation;

/* EVENT used by the timer to activate obstacle control task */
struct rt_event event_tasks_activation;
/* EVENT used to notify that there's an obstacle */
struct rt_event event_obstacle;
/* EVENT used to notify that there's an obstacle */
struct rt_event event_resources;


// ************************************ FUNCTIONS **************************************************************


/* function executed by the timer that activates obstacle control periodically when it reaches the timeout */
void timeout_obstacle_control(void* parameter);



#endif /* APPLICATIONS_STRUCTURES_H_ */
