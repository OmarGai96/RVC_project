/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-11     fabio       the first version
 */

#include <stdlib.h>
#include <stdio.h>
#include <rthw.h>
#include <rtthread.h>

#include "Task1_obstacles_control.h"



#define STOP_ENGINE 0

#define THREAD_PRIORITY         1
#define THREAD_TIMESLICE        5

#define EVENT_FLAG1 (1 << 3)
#define EVENT_FLAG2 (1 << 4)



ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[512];
static struct rt_thread thread1;

static void thread1_entry(void *parameter)
{

    FILE* engine;

    engine=fopen("/simulation/engine", "w");
            if(engine==NULL){
                    printf("some ERROR OCCURED with engine FILE");
                    exit -1;
                }
    //wait interrupt

}


ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[512];
static struct rt_thread thread2;

/* Entry Function for Thread 2 */
static void thread2_entry(void *parameter)
{
    FILE* f1, *f2;
    int val1, val2;
    int ret1, ret2;

    f1=fopen("/simulation/proximity_sensor", "r");
    if(f1==NULL){
        printf("FILE proximity_sensor NOT FOUND");
        exit -1;
    }

    f2=fopen("/simulation/down_sensor", "r");
    if(f2==NULL){
            printf("FILE down_sensor NOT FOUND");
            exit -1;
        }



    while (1)
    {
        ret1=fscanf(f1, "%d", &val1);
        ret2=fscanf(f2, "%d", &val2);
          /* signal to thread1 in case of one of the obstacles found */

        if(ret1 == EOF || ret2 == EOF){
            break;
        }

        if(val1 || val2){
            //TODO: interrupt to thread1(enigine)
            //rt_err_t rt_mb_send (rt_mailbox_t mb, rt_uint32_t value);//mail to movements_control and brushes_speed
        }

        rt_thread_mdelay(500);

    }

    fclose(f1);
    fclose(f2);
}



/* Thread creation */
int thread_creation(void)
{
    /* Create thread 1, Name is obstacles_control_software，Entry is thread1_entry */
    rt_thread_init(&thread1, "obstacles_control_software",
                            thread1_entry, RT_NULL,
                            thread1_stack, sizeof(thread1_stack),
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    rt_thread_startup(&thread1);



    /* Create thread 2, Name is obstacles_control_hardware，Entry is thread1_entry */
    rt_thread_init(&thread2, "obstacles_control_hardware",
                                    thread2_entry, RT_NULL,
                                    thread2_stack, sizeof(thread2_stack),
                                    THREAD_PRIORITY, THREAD_TIMESLICE);

    rt_thread_startup(&thread2);

    return 0;
}

