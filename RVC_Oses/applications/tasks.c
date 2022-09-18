
#include <rtthread.h>
#include <rtdevice.h>

#include "tasks.h"
#include "structures.h"


// EXTRA FUNCTIONS *********************************************************************************************


/* This function search for a new position where the robot can go, it is used in case we reached a
   bound of the room or if we have found an obstacle*/
int find_new_position(int *position, int map[MAP_SIDE][MAP_SIDE], enum directions *direction)
{
    if (*direction!=DOWN && position[0]>0 && map[ position[0]-1 ][ position[1] ]==0)
    {
        position[0]--;
        *direction = UP;
        return 0;
    }
    if (*direction!=UP && position[0]<MAP_SIDE-1 && map[ position[0]+1 ][ position[1] ]==0)
    {
        position[0]++;
        *direction = DOWN;
        return 0;
    }
   if (*direction!=RIGHT && position[1]>0 && map[ position[0] ][ position[1]-1 ]==0)
   {
       position[1]--;
       *direction = LEFT;
       return 0;
   }
    if (*direction!=LEFT && position[1]<MAP_SIDE-1 && map[ position[0] ][ position[1]+1 ]==0)
    {
       position[1]++;
       *direction = RIGHT;
       return 0;
    }
    return 1;
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
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND2);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND1);
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
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND1,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK)
        {
            // TODO: decide how to implement this in hw and modify correspondent driver
            rt_kprintf("Engine stopped!\n");
        }
    }
}


/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    // data structures used
    static int map[MAP_SIDE][MAP_SIDE];
    static int position[2];
    enum directions direction;
    int i,j, stuck;

    // initialization of data structures
    position[0] = 0;
    position[1] = 0;
    direction = DOWN;
    for (i=0;i<MAP_SIDE;i++)
        for (j=0;j<MAP_SIDE;j++)
            map[i][j] = 0;

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
                if (position[0]>0 && map[ position[0]-1 ][ position[1] ]==0) position[0]--;
                else stuck = find_new_position(position, map, &direction);
                break;
            case DOWN:
                if (position[0]<MAP_SIDE-1 && map[ position[0]+1 ][ position[1] ]==0) position[0]++;
                else stuck = find_new_position(position, map, &direction);
                break;
            case LEFT:
                if (position[1]>0 && map[ position[0] ][ position[1]-1 ]==0) position[1]--;
                else stuck = find_new_position(position, map, &direction);
                break;
            case RIGHT:
                if (position[1]<MAP_SIDE-1 && map[ position[0] ][ position[1]+1 ]==0) position[1]++;
                else stuck = find_new_position(position, map, &direction);
                break;
            }
            // TODO: implement what to do here, we could check if it's stuck because it's between obstacle
            //       or between cleaned tiles
            if (stuck == 1) rt_kprintf("The robot is stuck!!");
            rt_kprintf("Robot in position %d,%d\n", position[0], position[1]);
        }
    }
}

