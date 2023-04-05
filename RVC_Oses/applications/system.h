/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-01     Omar       the first version
 */
#ifndef APPLICATIONS_SYSTEM_H_
#define APPLICATIONS_SYSTEM_H_

/* BATTERY STATUS THRESHOLDS*/
#ifdef FAST_SIMULATION_BATTERY
#define CHARGE 50
#define HALFCHARGE 20
#define DISCHARGE_THRESHOLD 10
#define DISCHARGE 2
#define TOTALLY_DISCHARGE 0
#else
#define CHARGE 100
#define HALFCHARGE 50
#define DISCHARGE_THRESHOLD 20
#define DISCHARGE 10
#define TOTALLY_DISCHARGE 0
#endif


/* GARBAGE BAG STATUS*/
#ifdef FAST_SIMULATION_GARBAGE
#define EMPTY 0
#define FULL 15
#else
#define EMPTY 0
#define FULL 30
#endif
/* MOVEMENTS SIGNALS*/
#define STOP_ENGINE 0

#define STOP_BRUSHES 0
#define LOW 1
#define MEDIUM 2
#define HIGH 3

/*SYSTEM RESOURCES DECLARATION*/
uint32_t batteryStatus;
uint32_t garbageBagStatus;

extern int turnOffFlag;
extern int startingTime;

/** This function initializes the batteryStatus and garbageBagStatus (global) variables
 * @param: none
 * @return: RT_EOK if everything is OK, RT_ERROR otherwise
 *
**/
rt_err_t initSystem(void);

ALIGN(RT_ALIGN_SIZE)
char Tsystem_stack[1024];
struct rt_thread Tsystem;
void Tsystem_task_entry(void *param);

#endif /* APPLICATIONS_SYSTEM_H_ */
