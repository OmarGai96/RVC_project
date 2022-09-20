
#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>

#include "tasks.h"
#include "structures.h"


static int map[MAP_SIDE][MAP_SIDE];
static int position[2];
enum directions direction;


// EXTRA FUNCTIONS *********************************************************************************************


/* This function search for a new position where the robot can go, it is used in case we reached a
   bound of the room or if we have found an obstacle*/
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


/* Signal handler for map_management*/
void movement_control_obstacle_handler(int sig)
{
    // mark obstacle on the map
    map[ position[0] ][ position[1] ] = 1;
    // revert position to precious tile
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
    print_map();
}



// THREADS ENTRIES *********************************************************************************************



/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{
    int obstacle;
    rt_pin_mode(PROXIMITY_SENSOR_PIN_NUMBER, PIN_MODE_INPUT);

    // infinite loop
    while (1)
    {
        // waits for the periodic activation from the timer
        if (rt_event_recv(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            // check if there's an obstacle, if yes activates movements threads
            obstacle =  rt_pin_read(PROXIMITY_SENSOR_PIN_NUMBER);
            if (obstacle == 1)
            {
                rt_thread_kill(&movement_control, SIGUSR1);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND);
            }
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
            rt_kprintf("Engine stopped!\n");
        }
    }
}


// TODO: if stuck at the next obstacle found the map changes and start again, implement action
// TODO: gets stuck easily
/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    // data structures used
    int i,j, stuck;

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
            // if the previous tile is not an obstacle signal it as cleaned
            if (map[ position[0] ][ position[1] ] == 0)
                map[ position[0] ][ position[1] ] = 2;
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
            }
            if (stuck == 1) rt_kprintf("The robot is stuck!!");
            rt_kprintf("Robot in position %d,%d\n", position[0], position[1]);
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


