
#ifndef APPLICATIONS_STRUCTURES_H_
#define APPLICATIONS_STRUCTURES_H_


// ************************************** DEFINITIONS **********************************************************


#define EVENT_FLAG1 (1 << 1)


// ************************************ STRUCTURES *************************************************************


/* event used by the timer to activate obstacle control task */
struct rt_timer timer_obstacle_control;
/* timer used to activate periodically the obstacle control task */
struct rt_event event_obstacle;


// ************************************ FUNCTIONS **************************************************************


/* function executed by the timer that activates obstacle control periodically when it reaches the timeout */
void timeout_obstacle_control(void* parameter);



#endif /* APPLICATIONS_STRUCTURES_H_ */
