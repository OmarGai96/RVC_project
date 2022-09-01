/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-01     Omar       the first version
 */

#include <rtthread.h>
#include "Task2_3_synch.h"

#define THREAD_PRIORITY      9
#define THREAD_TIMESLICE     5

/* Event control block */
extern struct rt_event event;  //declaration inside main.c

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

        //USEFUL ONLY FOR DEBUG/////////
        if(batteryStatus == DISCHARGE){
                    return;
        }else{
                updateResources();
        }
        ////////////////////////////////

    }
}

int thread3_check_resources(void){

    rt_thread_init(&thread3,"thread3",
                   thread3_entry,
                   RT_NULL, &thread3_stack[0],
                   sizeof(thread3_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);

    return 0;
}

ALIGN(RT_ALIGN_SIZE)
static char thread4_stack[1024];
static struct rt_thread thread4;

/* Thread 4 Entry ACOUSTIC SIGNAL */
static void thread4_entry(void *param)
{
    rt_uint32_t e;  //to read event
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

int thread4_acoustic_signals(void){

    rt_thread_init(&thread4,"thread4",
                   thread4_entry,
                   RT_NULL, &thread4_stack[0],
                   sizeof(thread4_stack),
                   THREAD_PRIORITY-1, THREAD_TIMESLICE);


    return 0;
}

void startThreads(void){
    rt_thread_startup(&thread4);
    rt_thread_startup(&thread3);
}



