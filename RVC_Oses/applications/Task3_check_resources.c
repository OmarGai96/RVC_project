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
#include "Task3_check_resources.h"

#define THREAD_PRIORITY      9
#define THREAD_TIMESLICE     5

/* Event control block */
struct rt_event event;

ALIGN(RT_ALIGN_SIZE)
static char thread3_stack[1024];
static struct rt_thread thread3;

/* Thread 3 entry CHECK RESOURCES*/
static void thread3_entry(void *param)
{
    while(1){
        printf("\tBattery status %d\n", batteryStatus);
        if(batteryStatus <= CHARGE && batteryStatus > HALFCHARGE){
            printf("Battery is charged\n");
            //TODO: stampare su LCD
        }
        else if(batteryStatus <= HALFCHARGE && batteryStatus > DISCHARGE_THRESHOLD ){
            printf("Battery is half charged\n");
            //TODO: stampare su LCD
        }
        else if(batteryStatus <= DISCHARGE_THRESHOLD){
            printf("Battery low\n");
            rt_event_send(&event, EVENT_FLAG1);
            rt_thread_mdelay(200);
        }

        if(garbageBagStatus == FULL){
            printf("Garbage bag is FULL\n");
            rt_event_send(&event, EVENT_FLAG2);
            rt_thread_mdelay(200);
        }

        //USEFUL ONLY FOR DEBUG
        if(batteryStatus == DISCHARGE){
            return;
        }else{
            updateResources();
        }
    }
}


int thread_check_resources(void){
    rt_err_t result;

    /* Initialize event object */
    result = rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK){
        printf("init event failed.\n");
        return -1;
    }

    rt_thread_init(&thread3,"thread3",
                   thread3_entry,
                   RT_NULL, &thread3_stack[0],
                   sizeof(thread3_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread3);

    return 0;
}

void updateResources(void){
    garbageBagStatus++;
    if(garbageBagStatus%2==0)
        batteryStatus--;
    return;
}

void initSystem(void){
    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;
    return;
}
