/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-19     Omar       the first version
 */
#ifndef APPLICATIONS_TASK3_CHECK_RESOURCES_H
#define APPLICATIONS_TASK3_CHECK_RESOURCES_H

/* BATTERY STATUS THRESHOLDS*/
#define CHARGE 10
#define HALFCHARGE 5
#define DISCHARGE_THRESHOLD 2
#define DISCHARGE 0

/* GARBAGE BAG STATUS*/
#define EMPTY 0
#define FULL 10

/* EVENT MASKS DEFINITION*/
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)

/*SYSTEM RESOURCES DECLARATION*/
uint32_t batteryStatus;
uint32_t garbageBagStatus;

int thread_check_resources(void);
void initSystem(void);

#endif
