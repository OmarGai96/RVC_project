
#ifndef APPLICATIONS_TASKS_H_
#define APPLICATIONS_TASKS_H_

#include "system.h"

// ************************************** DEFINITIONS **********************************************************


#define OBSTACLE_CONTROL_PRIORITY      2
#define MOVEMENT_STOP_PRIORITY         1
#define MOVEMENT_CONTROL_PRIORITY      3
#define THREAD_TIMESLICE               5

#define SENSOR_PIN_NUMBER              35


// ************************************ STRUCTURES *************************************************************


ALIGN(RT_ALIGN_SIZE)
char obstacle_control_stack[1024];
struct rt_thread obstacle_control;

ALIGN(RT_ALIGN_SIZE)
char movement_stop_stack[1024];
struct rt_thread movement_stop;

ALIGN(RT_ALIGN_SIZE)
char movement_control_stack[1024];
struct rt_thread movement_control;


// ************************************ FUNCTIONS **************************************************************

void obstacle_control_entry(void *param);
void movement_stop_entry(void *param);
void movement_control_entry(void *param);


#endif /* APPLICATIONS_TASKS_H_ */
