
#include <rtthread.h>
#include "tasks.h"
#include "structures.h"
#include <stdio.h>


/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{
    int sensor[] = {0,0,0,1,0,0,0,1,0,0,1,0,0,0,0};
    int count = 0;

    // infinite loop
    while (1)
    {
        // waits for the periodic activation from the timer
        if (rt_event_recv(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            // simulate the sensor, if finds an obstacle send an event
            if (count<14 && sensor[count] == 1)
            {
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND1);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND2);
            }
            count++;
        }
    }
}


/* Entry for the task movement stop. */
void movement_stop_entry(void *param)
{
    // infinite loop
    while (1) {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND1,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            rt_kprintf("Engine stopped!\n");
        }
    }
}


/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    // infinite loop
    while (1) {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND2,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            rt_kprintf("Turn rigth and start again.\n");
        }
    }
}


/* Entry for the CHECK RESOURCES*/
void check_resources_entry(void *param){
    while(1){
        printf("\nTask3 CHECK resources\n\tBattery status %d\n", batteryStatus);
        printf("Started at time %d tick\n", rt_tick_get());

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
            //rt_event_send(&event_resources, EVENT_FLAG1);
            //rt_thread_mdelay(200);
        }

        if(garbageBagStatus == FULL){
            printf("Garbage bag is FULL\n");
            //rt_event_send(&event_resources, EVENT_FLAG2);
            //rt_thread_mdelay(200);
        }

        //USEFUL ONLY FOR DEBUG/////////
        if(batteryStatus == DISCHARGE){
                    return;
        }else{
                updateResources();
        }
        ////////////////////////////////
        rt_thread_mdelay(200);
    }
}


/* Entry for the ACOUSTIC SIGNAL */
void acoustic_signals_entry(void *param){

    rt_uint32_t e;  //to read event
    while(1){
        if (rt_event_recv(&event_resources,(EVENT_FLAG1 | EVENT_FLAG2),RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&e) == RT_EOK){
            if (e == 0x2){ //if EVENT 1 is set
                printf("\t\tLOW BATTERY SOUND\n");
            }else if (e == 0x4){//if EVENT 2 is set
                printf("\t\tGARBAGE BAG FULL SOUND\n");
            }
        }
    }

}


