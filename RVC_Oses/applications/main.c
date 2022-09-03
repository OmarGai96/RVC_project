/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2019-10-19     xuzhuoyi     add stm32f429-st-disco bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "Tasks.h"
#include "system.h"

/* Event control block */
struct rt_event event;

int main(void)
{

    rt_err_t result;

    /* Initialize event object */
    result = rt_event_init(&event, "event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK){
        printf("init event failed.\n");
        return -1;
    }

    //SYSTEM INITIALIZATION
    initSystem();

    //THREAD CREATIONS
    thread3_check_resources();  //create Task3  //update resource is there
                                    //TODO: create a task that simulate the main functionalities
    thread4_acoustic_signals(); //create Task4

    //THREAD STARTUPS
    startThreads();
    //from here, task3 and 4 run
    return RT_EOK;
}


