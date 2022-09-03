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
#include "Tasks.h"

#define THREAD1_PRIORITY      9
#define THREAD2_PRIORITY      9
#define THREAD3_PRIORITY      9
#define THREAD4_PRIORITY      9
#define THREAD5_PRIORITY      9
#define THREAD_TIMESLICE      5

/* Event control block */
extern struct rt_event event;  //declaration inside main.c

/* Mailbox control block */
static struct rt_mailbox mb;
/* Memory pool for mails storage */
static char mb_pool[128];

static char mb_str1[] = "STOP_ENGINE";
static char mb_str2[] = "COME_BACK_HOME";
static char mb_str3[] = "TURN_ON_BRUSHES";


/* Tasks*/
static struct rt_thread thread1, thread2, thread3, thread4, thread5;
ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024],thread2_stack[1024],thread3_stack[1024],thread4_stack[512],thread5_stack[512];


/*
 * TASK 1 DECLARATION
 */

/* Thread 1 Entry OBSTACLES CONTROL */
static void thread1_entry(void *param)
{

    printf("Task1 obstacles control");
}

int thread1_obstacles_control(void){

    rt_thread_init(&thread1,"thread1",
                   thread1_entry,
                   RT_NULL, &thread1_stack[0],
                   sizeof(thread1_stack),
                   THREAD1_PRIORITY, THREAD_TIMESLICE);


    return 0;
}


/*
 * TASK 2 DECLARATION
 */

/* Thread 2 Entry MOVEMENTS CONTROL */
static void thread2_entry(void *param)
{

    printf("Task2 movement control");
}

int thread2_movements_control(void){

    rt_thread_init(&thread2,"thread2",
                   thread2_entry,
                   RT_NULL, &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD2_PRIORITY, THREAD_TIMESLICE);


    return 0;
}


/*
 * TASK 3 DECLARATION
 */

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
                   THREAD3_PRIORITY, THREAD_TIMESLICE);

    return 0;
}


/*
 * TASK 4 DECLARATION
 */

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
                   THREAD4_PRIORITY, THREAD_TIMESLICE);


    return 0;
}

/*
 * TASK 5 DECLARATION
 */

/* Thread 5 Entry BRUSHES SPEED */
static void thread5_entry(void *parameter)
{

    FILE* brushes_speed;
    FILE* brushes_power;

    char *str;

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

       //Reads from mail box in case the robot is coming back. In this case stop the brushes
           if (rt_mb_recv(&mb, (rt_uint32_t *)&str, RT_WAITING_NO) == RT_EOK){

                   if (str == mb_str2){
                       fprintf(brushes_power, "%d", STOP_BRUSHES);
                   }

                   rt_thread_mdelay(100);

                   /* Executing the mailbox object detachment */
                   rt_mb_detach(&mb);
               }
        //If the robot is starting on, turn on the brushes
           if (rt_mb_recv(&mb, (rt_uint32_t *)&str, RT_WAITING_NO) == RT_EOK){

                              if (str == mb_str3){
                                  fprintf(brushes_power, "%d", MEDIUM);
                              }

                              rt_thread_mdelay(100);

                              /* Executing the mailbox object detachment */
                              rt_mb_detach(&mb);
                          }

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


int thread5_brushes_speed(void)
{

        rt_err_t result;

        rt_thread_init(&thread5,
                           "brushes_speed",
                           thread5_entry,
                           RT_NULL,
                           &thread5_stack[0],
                           sizeof(thread5_stack),
                           THREAD5_PRIORITY, THREAD_TIMESLICE);

            rt_thread_startup(&thread5);


        /* Initialize a mailbox */
        result = rt_mb_init(&mb,
                            "mail_box",
                            &mb_pool[0],
                            sizeof(mb_pool) / 4,
                            RT_IPC_FLAG_FIFO);
        if (result != RT_EOK){
             rt_kprintf("init mailbox failed.\n");
             return -1;
        }



            return 0;
}


void startThreads(void){
    rt_thread_startup(&thread4);
    rt_thread_startup(&thread3);
    rt_thread_startup(&thread2);
    rt_thread_startup(&thread1);
    rt_thread_startup(&thread5);
}

