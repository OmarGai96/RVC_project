/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-03     Omar       the first version
 */


#include <stdio.h>
#include <stdlib.h>
#include <rthw.h>
#include <rtthread.h>
#include <tasks.h>

#define OBSTACLE_CONTROL_PRIORITY      2
#define MOVEMENT_CONTROL_PRIORITY      1
#define THREAD_TIMESLICE               5


//********************************* STRUCTURES *****************************************************************


ALIGN(RT_ALIGN_SIZE)
static char obstacle_control_stack[1024];
static struct rt_thread obstacle_control;

ALIGN(RT_ALIGN_SIZE)
static char movement_control_stack[1024];
static struct rt_thread movement_control;


//******************************* ENTRIES FUNCTIONS ************************************************************


/* Entry for the task obstacle control */
static void obstacle_control_entry(void *param)
{
    rt_kprintf("Obstacles control\n");
}


/* Entry for the task  */
static void movement_control_entry(void *param)
{
    rt_kprintf("Movement control\n");
}


//********************************** INITIALIZATION FUNCTION ***************************************************


int movement_threads_start(void)
{

    // initializing and starting obstacle_control thread
    rt_thread_init(&obstacle_control,
                   "obstacle_control",
                   obstacle_control_entry,
                   RT_NULL,
                   &obstacle_control_stack[0],
                   sizeof(obstacle_control_stack),
                   OBSTACLE_CONTROL_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&obstacle_control);

    // initializing and starting movement_control thread
    rt_thread_init(&movement_control,
                   "movement_control",
                   movement_control_entry,
                   RT_NULL,
                   &movement_control_stack[0],
                   sizeof(movement_control_stack),
                   MOVEMENT_CONTROL_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&movement_control);

    return 0;
}


