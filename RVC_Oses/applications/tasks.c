
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#include "tasks.h"
#include "structures.h"


static int map[MAP_SIDE][MAP_SIDE];
static int position[2];
enum directions direction;

static char mb_str1[] = "Garbage bag full";
static char mb_str2[] = "Battery discharge";
static char mb_str3[] = "Battery totally discharged";
static char mb_str4[] = "Come back home";
static char mb_str5[] = "Turn on brushes";

extern startingTime;


// EXTRA FUNCTIONS *********************************************************************************************


/* This function search for a new position where the robot can go, it is used in case we reached a
   bound of the room or if we have found an obstacle */
int find_new_position()
{
    if (direction!=DOWN && position[0]>0 && map[ position[0]-1 ][ position[1] ]!=1)
    {
        position[0]--;
        direction = UP;
        return 0;
    }
    if (direction!=UP && position[0]<MAP_SIDE-1 && map[ position[0]+1 ][ position[1] ]!=1)
    {
        position[0]++;
        direction = DOWN;
        return 0;
    }
   if (direction!=RIGHT && position[1]>0 && map[ position[0] ][ position[1]-1 ]!=1)
   {
       position[1]--;
       direction = LEFT;
       return 0;
   }
    if (direction!=LEFT && position[1]<MAP_SIDE-1 && map[ position[0] ][ position[1]+1 ]!=1)
    {
       position[1]++;
       direction = RIGHT;
       return 0;
    }
    return 1;
}


void print_map() {
    int i,j;
    rt_kprintf("\n");
    for (i=0;i<MAP_SIDE;i++) {
        for(j=0;j<MAP_SIDE;j++)
            rt_kprintf("%d ", map[i][j]);
        rt_kprintf("\n");
    }
    rt_kprintf("\n\n");
}


/* Signal handler for map_management, if an obstacle is found go back to previous tile*/
void movement_control_obstacle_handler(int sig)
{
    // mark obstacle on the map
    map[ position[0] ][ position[1] ] = 1;
    // revert position to previous tile
    switch (direction)
    {
    case UP:
        position[0]++;
        break;
    case DOWN:
        position[0]--;
        break;
    case LEFT:
        position[1]++;
        break;
    case RIGHT:
        position[1]--;
        break;
    }

#ifdef DEBUG_2
    print_map();
#endif

}



// THREADS ENTRIES *********************************************************************************************



/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{
    char obstacle;
    proximity_sensor = rt_device_find("proximity_sensor");
    rt_device_init(proximity_sensor);

    // infinite loop
    while (1)
    {
        // waits for the periodic activation from the timer
        if (rt_event_recv(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {

#ifdef BENCHMARKING
        printf("\nTask1:\t Started at time %d tick, %d ms\n", rt_tick_get(), rt_tick_get_millisecond()-startingTime);
#endif

            // check if there's an obstacle, if yes activates movements threads
            rt_device_read(proximity_sensor, 0, &obstacle, 1);
            if (obstacle == 'y')
            {
                rt_thread_kill(&movement_control, SIGUSR1);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND);

            }

#ifdef BENCHMARKING
        printf("\t\tStop at time %d tick\n", rt_tick_get());
#endif

        }
    }
}


/* Entry for the task movement stop. */
void movement_stop_entry(void *param)
{
    // infinite loop
    while (1) {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {

            // TODO: decide how to implement this in hw and modify correspondent driver
            //WE CAN USE JUST A SPECIFIC PIN SET TO ZERO, and using a global variable to remember it, if necessary.
            //Otherwise modify or create a driver that remember the last status of the engine

#ifdef DEBUG_2
            rt_kprintf("\tEngine stopped!\n");
#endif

        }
    }
}


/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    // data structures used
    int i,j, stuck;
    char *str;          //to read mail contents

    // initialization of data structures
    position[0] = 0;
    position[1] = 0;
    direction = DOWN;
    for (i=0;i<MAP_SIDE;i++)
        for (j=0;j<MAP_SIDE;j++)
            map[i][j] = 0;

    // installing the signal
    rt_signal_install(SIGUSR1, movement_control_obstacle_handler);
    rt_signal_unmask(SIGUSR1);

    // infinite loop
    while (1)
    {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_tasks_activation, EVENT_MOVEMENT_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {

#ifdef BENCHMARKING
       printf("\nTask2:\t Started at time %d tick, %d ms\n", rt_tick_get(), rt_tick_get_millisecond()-startingTime);
#endif

            //mailbox receive
            if (rt_mb_recv(&mb2_3, (rt_uint32_t *)&str, 0) == RT_EOK){

#ifdef DEBUG_1
                rt_kprintf("\tget a mail from mailbox, the content: %s\n", str);
#endif
                direction = RETURN;
                rt_signal_mask(SIGUSR1);
            }

            // if the previous tile is not an obstacle signal it as cleaned
            if (map[ position[0] ][ position[1] ] == 0) {
                map[ position[0] ][ position[1] ] = 2;
                garbageBagStatus++;
            }
            // decide where to go next
            switch (direction)
            {
            case UP:
                if (position[0]>0 && map[ position[0]-1 ][ position[1] ]!=1) position[0]--;
                else stuck = find_new_position();
                break;
            case DOWN:
                if (position[0]<MAP_SIDE-1 && map[ position[0]+1 ][ position[1] ]!=1) position[0]++;
                else stuck = find_new_position();
                break;
            case LEFT:
                if (position[1]>0 && map[ position[0] ][ position[1]-1 ]!=1) position[1]--;
                else stuck = find_new_position();
                break;
            case RIGHT:
                if (position[1]<MAP_SIDE-1 && map[ position[0] ][ position[1]+1 ]!=1) position[1]++;
                else stuck = find_new_position();
                break;
            case RETURN:
                if (position[0]>0)
                    position[0]--;
                else if (position[1]>0)
                    position[1]--;
                break;
            }
            // if the function couldn't find where to go the robot is stuck, we end the thread
            if (stuck == 1) {

#ifdef DEBUG_2
                rt_kprintf("\tThe robot is stuck!!");
#endif

                break;
            }
            // if the robot is back at starting station after receiving command to return we end the thread
            if (direction==RETURN && position[0]==0 && position[1]==0) {

#ifdef DEBUG_2
                rt_kprintf("\tThe robot is back at charging station!\n");
#endif

                break;
            }

#ifdef DEBUG_2
            rt_kprintf("\n\tRobot in position %d,%d\n", position[0], position[1]);
#endif

#ifdef BENCHMARKING
        printf("\t\tStop at time %d tick\n", rt_tick_get());
#endif

        }
    }
}


/* Entry for the CHECK RESOURCES*/
void check_resources_entry(void *param){

    // used for debug
    uint32_t previousBatteryStatus = CHARGE;

    while(1){

        // waiting the periodic activation
        if (rt_event_recv(&event_tasks_activation, EVENT_CHECK_RESOURCES_ACTIVATION,
                                  RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                                  RT_WAITING_FOREVER, RT_NULL) == RT_EOK){

#ifdef BENCHMARKING
        printf("\nTask3:\t Started at time %d tick, %d ms\n", rt_tick_get(), rt_tick_get_millisecond()-startingTime);
#endif

#ifdef DEBUG_2
        if(batteryStatus!=previousBatteryStatus && batteryStatus%5==0){
            previousBatteryStatus = batteryStatus;
            printf("\n\tBattery status %d %% \n", batteryStatus);
        }

#endif
        /**Check BATTERY status**/
        if(batteryStatus <= DISCHARGE+1) {
            rt_mb_send(&mb2_3, (rt_uint32_t)&mb_str3);      //notify task 2 BATTERY is completely LOW
        }else if(batteryStatus <= DISCHARGE_THRESHOLD) {
            rt_event_send(&event_resources, EVENT_FLAG1);   //notify task 4
            rt_mb_send(&mb2_3, (rt_uint32_t)&mb_str2);      //notify task 2

#ifdef DEBUG_2
            printf("\tBattery LOW\n\t\tMail sent %s\n", mb_str2);
#endif

        }

        /**Check GARBAGE BAG status **/

        if(garbageBagStatus == FULL){
            rt_event_send(&event_resources, EVENT_FLAG2);   //notify task 4
            rt_mb_send(&mb2_3, (rt_uint32_t)&mb_str1);      //notify task 2

#ifdef DEBUG_2
            printf("\tGarbage bag FULL\n\t\tMail sent %s\n", mb_str1);
#endif

        }

        batteryStatus--;

#ifdef BENCHMARKING
        printf("\t\tStop at time %d tick\n", rt_tick_get());
#endif

      }
    }
}


/* Entry for the ACOUSTIC SIGNAL */
void acoustic_signals_entry(void *param){

    rt_uint32_t e;  //to read event
    while(1){
        if (rt_event_recv(&event_tasks_activation, EVENT_ACOUSTIC_SIGNALS_ACTIVATION,
                                  RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                                  RT_WAITING_FOREVER, RT_NULL) == RT_EOK){
#ifdef BENCHMARKING
        printf("\nTask4:\t Started at time %d tick, %d ms\n", rt_tick_get(), rt_tick_get_millisecond()-startingTime);
#endif
        if (rt_event_recv(&event_resources,(EVENT_FLAG1 | EVENT_FLAG2),RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&e) == RT_EOK){
            if (e == 0x2){

                // EVENT_FLAG_1 is set
#ifdef DEBUG_2
                printf("\t\tLOW BATTERY ALARM\n");
#endif

            }else if (e == 0x4){

                // EVENT_FLAG_2 is set
#ifdef DEBUG_2
                printf("\t\tGARBAGE BAG FULL ALARM\n");
#endif

            }

         }

#ifdef BENCHMARKING
        printf("\t\tStop at time %d tick\n", rt_tick_get());
#endif

        }
    }

}


/* Entry for the BRUSHES SPEED */
void brushes_speed_entry(void *param)
{
    /* 0 0 MEDIUM
     * 0 1 HIGH
     * 1 0 LOW
     * 1 1 STOP
     */
    int brushes_speed[2], brushes_power[2];
    char *str;

    while(1){
        if (rt_event_recv(&event_tasks_activation, EVENT_BRUSHES_SPEED_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK){

#ifdef BENCHMARKING
        printf("\nTask5:\t Started at time %d tick, %d ms\n", rt_tick_get(), rt_tick_get_millisecond()-startingTime);
#endif

            rt_pin_mode(BRUSHES_SPEED_PIN_NUMBER, PIN_MODE_INPUT);
            rt_pin_mode(BRUSHES_POWER_PIN_NUMBER, PIN_MODE_OUTPUT);

            //If the robot is starting on, turn on the brushes
            if (rt_mb_recv(&mb2_5, (rt_uint32_t *)&str, RT_WAITING_NO) == RT_EOK){
                // TODO:WHO SENDS AT TURN ON OF THE ROBOT?

                if (!rt_strcmp(str,mb_str5)){
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, MEDIUM);
                    brushes_power[0]=0;
                    brushes_power[1]=0;
                }

                /* Executing the mailbox object detachment */
                rt_mb_detach(&mb2_5);
            }

            //Reads from mail box in case the robot is coming back. In this case stop the brushes
            if (rt_mb_recv(&mb2_5, (rt_uint32_t *)&str, RT_WAITING_NO) == RT_EOK){
                //TODO: WHO SAYS TO COME BACK?

                if (!rt_strcmp(str,mb_str4)){
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, STOP_BRUSHES);
                    brushes_power[0]=1;
                    brushes_power[1]=1;
                }

                /* Executing the mailbox object detachment */
                rt_mb_detach(&mb2_5);
            }

            brushes_speed[0] =  rt_pin_read(BRUSHES_SPEED_PIN_NUMBER);
            brushes_speed[1] =  rt_pin_read(BRUSHES_SPEED_PIN_NUMBER);

            rt_pin_mode(BRUSHES_POWER_PIN_NUMBER, PIN_MODE_INPUT);
            brushes_power[0] =  rt_pin_read(BRUSHES_POWER_PIN_NUMBER);
            brushes_speed[1] =  rt_pin_read(BRUSHES_POWER_PIN_NUMBER);

            rt_pin_mode(BRUSHES_POWER_PIN_NUMBER, PIN_MODE_OUTPUT);

            if(brushes_speed[0] == 1 && brushes_speed[1] == 0){
                if(brushes_power[0] == 1 && brushes_power[1] == 0){
                    brushes_power[0]=0;
                    brushes_power[1]=0;
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, MEDIUM);
                }else if(brushes_power[0] == 0 && brushes_power[1] == 0){
                    brushes_power[0]=0;
                    brushes_power[1]=1;
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, HIGH);
                }
            }else if(brushes_speed[0] == 0 && brushes_speed[1] == 1){
                if(brushes_power[0] == 0 && brushes_power[1] == 1){
                    brushes_power[0]=0;
                    brushes_power[1]=0;
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, MEDIUM);
                }else if(brushes_power[0] == 0 && brushes_power[1] == 0){
                    brushes_power[0]=1;
                    brushes_power[1]=0;
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, LOW);
                }
            }

#ifdef BENCHMARKING
        printf("\t\tStop at time %d tick\n", rt_tick_get());
#endif

        }
    }
}


