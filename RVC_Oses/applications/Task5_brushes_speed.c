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

#define THREAD_PRIORITY         10
#define THREAD_TIMESLICE        5
#define LOW 1
#define MEDIUM 2
#define HIGH 3

static void thread_entry(void *parameter)
{

    FILE* brushes_speed;
    FILE* brushes_power;

    int speed, power;
    int ret1;

    brushes_speed=fopen("/simulation/brushes_speed", "r");      //three level of speed can be received(LOW,MEDIUM, HIGH)
            if(brushes_speed==NULL){
                    printf("some ERROR OCCURED with brushes_speed FILE");
                    exit -1;
                }
    brushes_power=fopen("/simulation/brushes_power", "r+");//initialized to MEDIUM, three possible level like brushes speed
            if(brushes_power==NULL){
                                printf("some ERROR OCCURED with brushes_power FILE");
                                exit -1;
                }
   while(1){
       //TODO:
       //reads from mail box in case the robot is coming back. In this case stop the brashes
       //rt_err_t rt_mb_recv (rt_mailbox_t mb, rt_uint32_t* value, rt_int32_t timeout);
       ret1=fscanf(brushes_speed, "%d", &speed);
       if(ret1== EOF)
           break;
       fscanf(brushes_power, "%d", &power);   //it contains always one integer only, to notify the current power setting

       switch(speed){
           case LOW:
               if(power==LOW){
                   rewind(brushes_power);
                   fprintf(brushes_power, "%d", MEDIUM);
                   rewind(brushes_power);
               }else if(power==MEDIUM){
                   rewind(brushes_power);
                   fprintf(brushes_power, "%d",HIGH);
                   rewind(brushes_power);
               }
               break;
           case HIGH:
               if(power==HIGH){
                   rewind(brushes_power);
                   fprintf(brushes_power, "%d", MEDIUM);
                   rewind(brushes_power);
               }else if(power==MEDIUM){
                   rewind(brushes_power);
                   fprintf(brushes_power, "%d",LOW);
                   rewind(brushes_power);
               }
               break;
           default:
               break;
       }

       rt_thread_mdelay(250);
   }

   fclose(brushes_speed);
   fclose(brushes_power);
}

ALIGN(RT_ALIGN_SIZE)
static char thread_stack[512];
static struct rt_thread thread;

int thread_brushes(void)
{
        rt_thread_init(&thread,
                           "brushes_speed",
                           thread_entry,
                           RT_NULL,
                           &thread_stack[0],
                           sizeof(thread_stack),
                           THREAD_PRIORITY, THREAD_TIMESLICE);

            rt_thread_startup(&thread);

            return 0;
}
