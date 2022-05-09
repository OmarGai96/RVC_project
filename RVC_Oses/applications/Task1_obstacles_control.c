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


/* Mailbox control block */
static struct rt_mailbox mb;
/* Memory pool for mails storage */
static char mb_pool[128];

static char mb_str1[] = "STOP_ENGINE";



void thread1_signal_handler(int sig)
{
    FILE* engine;

    engine=fopen("/simulation/engine", "w");

           if(engine==NULL){
              printf("some ERROR OCCURED with engine FILE");
              exit -1;
           }

    fprinf(engine, "%d", STOP_ENGINE);

    return;
}


ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[512];
static struct rt_thread thread1;

/* Entry Function for Thread 2 */
static void thread1_entry(void *parameter)
{
    FILE* f1, *f2;
    int val1, val2;
    int ret1, ret2;

    //in case of obstacle it sends SIGUSR1 signal
    //#define SIGUSR1 10 (POSIX). Action: exit
    rt_signal_install(SIGUSR1, thread1_signal_handler);
    rt_signal_unmask(SIGUSR1);

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


        if(ret1 == EOF || ret2 == EOF){
            break;
        }

        if(val1 || val2){
            //send the signal to thread1_signal_handler
            rt_thread_kill(tid1, SIGUSR1);
            //mail to movements_control and brushes_speed
            rt_mb_send(&mb, (rt_uint32_t)&mb_str1);
        }

        rt_thread_mdelay(500);

    }

    fclose(f1);
    fclose(f2);
}



/* Thread creation */
int thread_creation(void)
{
    rt_err_t result;

    /* Create thread 1, Name is obstacles_control，Entry is thread1_entry */
    rt_thread_init(&thread1, "obstacles_control",
                            thread1_entry, RT_NULL,
                            thread1_stack, sizeof(thread1_stack),
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    rt_thread_startup(&thread1);

    /* Initialize a mailbox */
       result = rt_mb_init(&mb,
                           "mail_box",                      /* Name is mbt */
                           &mb_pool[0],                /* The memory pool used by the mailbox is mb_pool */
                           sizeof(mb_pool) / 4,        /* The number of messages in the mailbox because a message occupies 4 bytes */
                           RT_IPC_FLAG_FIFO);          /* Thread waiting in FIFO approach */
       if (result != RT_EOK)
       {
           rt_kprintf("init mailbox failed.\n");
           return -1;
       }


    return 0;
}

