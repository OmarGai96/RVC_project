/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-06     Omar       the first version
 */
#ifndef EVENT_SAMPLE_H
#define EVENT_SAMPLE_H


/* BATTERY STATUS THRESHOLDS*/
#define CHARGE 10
#define HALFCHARGE 5
#define DISCHARGE_THRESHOLD 2
#define DISCHARGE 0

/* GARBAGE BAG STATUS*/
#define EMPTY 0
#define FULL 10

/*SYSTEM RESOURCES DECLARATION*/
uint32_t batteryStatus;
uint32_t garbageBagStatus;

int event_sample(void);

#endif
