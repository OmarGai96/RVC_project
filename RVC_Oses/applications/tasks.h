
#ifndef APPLICATIONS_TASKS_H_
#define APPLICATIONS_TASKS_H_

#include "system.h"


// ************************************** DEFINITIONS **********************************************************


#define OBSTACLE_CONTROL_PRIORITY      2
#define MOVEMENT_STOP_PRIORITY         1
#define MOVEMENT_CONTROL_PRIORITY      3
#define CHECK_RESOURCES_PRIORITY       4
#define ACOUSTIC_SIGNALS_PRIORITY      5
#define BRUSHES_SPEED_PRIORITY         5
#define THREAD_TIMESLICE               5

#define PROXIMITY_SENSOR_PIN_NUMBER     35
#define BRUSHES_SPEED_PIN_NUMBER        36
#define BRUSHES_POWER_PIN_NUMBER        37

#define MAP_SIDE                        10

enum directions {UP, DOWN, LEFT, RIGHT, RETURN};


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

ALIGN(RT_ALIGN_SIZE)
char check_resources_stack[1024];
struct rt_thread check_resources;

ALIGN(RT_ALIGN_SIZE)
char acoustic_signals_stack[1024];
struct rt_thread acoustic_signals;

ALIGN(RT_ALIGN_SIZE)
static char brushes_speed_stack[1024];
static struct rt_thread brushes_speed;


// ************************************ FUNCTIONS **************************************************************

void obstacle_control_entry(void *param);
void movement_stop_entry(void *param);
void movement_control_entry(void *param);
void check_resources_entry(void *param);
void acoustic_signals_entry(void *param);
void brushes_speed_entry(void *param);


#endif /* APPLICATIONS_TASKS_H_ */
