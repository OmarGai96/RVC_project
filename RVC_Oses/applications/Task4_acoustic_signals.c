/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-19     Omar       the first version
 */

#include <rtthread.h>
#include "Task4_acoustic_signals.h"

#define THREAD_PRIORITY      9
#define THREAD_TIMESLICE     5

/* Event control block */
extern struct rt_event event;

ALIGN(RT_ALIGN_SIZE)
static char thread4_stack[1024];
static struct rt_thread thread4;

/* Thread 4 Entry ACOUSTIC SIGNAL */
static void thread4_entry(void *param)
{
    rt_uint32_t e;
    while(1){
        if (rt_event_recv(&event,(EVENT_FLAG1 | EVENT_FLAG2),RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&e) == RT_EOK){
            if (e == 0x2){ //if EVENT 1 is set
                printf("\t\tLOW BATTERY SOUND\n");
            }else if (e == 0x4){//if EVENT 2 is set
                printf("\t\tGARBAGE BAG FULL SOUND\n");
            }
        }
    }
}

int thread_acoustic_signals(void){

    rt_thread_init(&thread4,"thread4",
                   thread4_entry,
                   RT_NULL, &thread4_stack[0],
                   sizeof(thread4_stack),
                   THREAD_PRIORITY-1, THREAD_TIMESLICE);
    rt_thread_startup(&thread4);

    return 0;
}

