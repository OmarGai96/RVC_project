
#ifndef APPLICATIONS_STRUCTURES_H_
#define APPLICATIONS_STRUCTURES_H_


// ************************************** DEFINITIONS **********************************************************


#define EVENT_OBSTACLE_CONTROL_ACTIVATION   (1 << 1)
#define EVENT_MOVEMENT_CONTROL_ACTIVATION   (1 << 2)

#define EVENT_OBSTACLE_FOUND (1 << 1)


// ************************************ STRUCTURES *************************************************************


/* TIMER used to activate periodically the obstacle control task */
struct rt_timer timer_obstacle_control_activation;
/* TIMER used to activate periodically the map management task */
struct rt_timer timer_movement_control_activation;

/* EVENT used by the timer to activate tasks */
struct rt_event event_tasks_activation;
/* EVENT used to notify that there's an obstacle */
struct rt_event event_obstacle;


// ************************************ FUNCTIONS **************************************************************


/* function executed by the timer that activates obstacle control periodically when it reaches the timeout */
void timeout_obstacle_control(void* parameter);
/* function executed by the timer that activates movement control periodically when it reaches the timeout */
void timeout_movement_control(void* parameter);



#endif /* APPLICATIONS_STRUCTURES_H_ */
