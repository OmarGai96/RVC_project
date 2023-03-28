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
#define CHARGE 100
#define HALFCHARGE 50
#define DISCHARGE_THRESHOLD 10
#define DISCHARGE 5
#define TOTALLY_DISCHARGE 0

/* GARBAGE BAG STATUS*/
#define EMPTY 0
#define FULL 20

/* MOVEMENTS SIGNALS*/
#define STOP_ENGINE 0

#define STOP_BRUSHES 0
#define LOW 1
#define MEDIUM 2
#define HIGH 3

/*SYSTEM RESOURCES DECLARATION*/
uint32_t batteryStatus;
uint32_t garbageBagStatus;

/** This function initializes the batteryStatus and garbageBagStatus (global) variables
 * @param: none
 * @return: none
 *
**/
void initSystem(void);


#endif /* APPLICATIONS_SYSTEM_H_ */
