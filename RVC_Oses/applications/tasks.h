
#ifndef APPLICATIONS_TASKS_H_
#define APPLICATIONS_TASKS_H_

#include "system.h"


//************************************** DEBUG SECTION *********************************************************

/*see rtconfig.h file, user define section*/

// ************************************** DEFINITIONS **********************************************************




#ifdef BACKGROUND_SCHEDULING
#define OBSTACLE_CONTROL_PERIOD        200
#define MOVEMENT_STOP_PERIOD           100  //lowest period
#define MOVEMENT_CONTROL_PERIOD        250
#define CHECK_RESOURCES_PERIOD         300
#define ACOUSTIC_SIGNALS_PERIOD        0
#define BRUSHES_SPEED_PERIOD           300

#else
#define OBSTACLE_CONTROL_PRIORITY      2 //T1
#define MOVEMENT_STOP_PRIORITY         1
#define MOVEMENT_CONTROL_PRIORITY      3 //T2
#define CHECK_RESOURCES_PRIORITY       4 //T3
#define ACOUSTIC_SIGNALS_PRIORITY      6 //T4
#define BRUSHES_SPEED_PRIORITY         5 //T5
#define TSYSTEM_PRIORITY               10
#endif

#define THREAD_TIMESLICE               5

#define PROXIMITY_SENSOR                "proximity_sensor"
#define ENGINE                          "engine"
#define BATTERY                         "battery"
#define GARBAGE_BAG                     "garbage_bag"

#define ACUSTIC_SIGNAL_PIN_NUMBER       40
#define BRUSHES_SPEED_PIN_NUMBER        36
#define BRUSHES_POWER_PIN_NUMBER        37

#define MAP_SIDE                        10

enum directions {UP, DOWN, LEFT, RIGHT, RETURN};

//*********************************** TIME DURATIONS **********************************************************

#define PERIOD_TASK1    20 //200 ms, Task1 starts periodically
#define PERIOD_TASK2    50 //500 ms, Task2 starts periodically
#define PERIOD_TASK3    25 //250 ms, Task3 starts periodically
//TASK 4 is aperiodic
#define PERIOD_TASK5    25 //250 ms, Task5 starts periodically

#define TICK_DELAY_T1 4 //40 ms, is the duration of Task1 as ticks
#define TICK_DELAY_T2 4 //40 ms, is the duration of Task2 as ticks
#define TICK_DELAY_T3 2 //30 ms, is the duration of Task3 as ticks
#define TICK_DELAY_T4 2 //20 ms, is the duration of Task4 as ticks
#define TICK_DELAY_T5 2 //20 ms, is the duration of Task5 as ticks

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
char brushes_speed_stack[1024];
struct rt_thread brushes_speed;

ALIGN(RT_ALIGN_SIZE)
char Tsystem_stack[1024];
struct rt_thread Tsystem;


// *********************************** DEVICES **************************************************************


rt_device_t proximity_sensor;
rt_device_t engine;
rt_device_t battery;
rt_device_t garbage_bag;


// ************************************ FUNCTIONS **************************************************************

void mock_devices_init();
void obstacle_control_entry(void *param);
void movement_stop_entry(void *param);
void movement_control_entry(void *param);
void check_resources_entry(void *param);
void acoustic_signals_entry(void *param);
void brushes_speed_entry(void *param);
void Tsystem_task_entry(void *param);


#endif /* APPLICATIONS_TASKS_H_ */
