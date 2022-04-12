/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-06     Omar       the first version
 */

#include <rtthread.h>
#include "event_sample.h"

#define THREAD_PRIORITY      9
#define THREAD_TIMESLICE     5

/* EVENT MASKS DEFINITION*/
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)

/* Event control block */
static struct rt_event event;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;

/* Thread 1 entry CHECK RESOURCES*/
static void thread1_entry(void *param)
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


ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

/* Thread 2 Entry ACOUSTIC SIGNAL */
static void thread2_entry(void *param)
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

int event_sample(void)
{
    rt_err_t result;

    /* Initialize event object */
    result = rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK){
        printf("init event failed.\n");
        return -1;
    }

    rt_thread_init(&thread1,"thread1",
                   thread1_entry,
                   RT_NULL, &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread1);

    rt_thread_init(&thread2,"thread2",
                   thread2_entry,
                   RT_NULL, &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY-1, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);

    return 0;
}

void updateResources(void){
    garbageBagStatus++;
    if(garbageBagStatus%2==0)
        batteryStatus--;
    return;
}

