
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


/* Function that initialize the device such that they are globally accessible by tasks */
void mock_devices_init() {

    proximity_sensor = rt_device_find(PROXIMITY_SENSOR);
    rt_device_init(proximity_sensor);

    engine = rt_device_find(ENGINE);
    rt_device_init(engine);

    battery = rt_device_find(BATTERY);
    rt_device_init(battery);

    garbage_bag = rt_device_find(GARBAGE_BAG);
    rt_device_init(garbage_bag);

    speaker = rt_device_find(SPEAKER);
    rt_device_init(speaker);

}


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
    rt_kprintf("\n");
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

#ifdef DEB_DISPLAY
    print_map();
#endif

}

/* Function to turn off the system (detach all the tasks)
 * usecases: - battery is totally LOW
 *           - robot is in re-charging station (after cleaning)
 *
 */
void turnOffSystem(void){

#ifdef DEB_INTERNAL
    printf("\t SIGNAL received: TURN OFF THE SYSTEM\n");
#endif

    if (rt_thread_detach(&obstacle_control) == RT_EOK){
#ifdef DEB_INTERNAL
        printf("\n\tTask1 detached correctly");
#endif
    }else{
#ifdef DEB_INTERNAL
        printf("\n\tTask1 detach error");
#endif
    }

    if (rt_thread_detach(&movement_stop) == RT_EOK){
#ifdef DEB_INTERNAL
        printf("\n\tTaskS detached correctly");
#endif
    }else{
#ifdef DEB_INTERNAL
        printf("\n\tTaskS detach error");
#endif
    }

    if (rt_thread_detach(&movement_control) == RT_EOK){
#ifdef DEB_INTERNAL
        printf("\n\tTask2 detached correctly");
#endif
    }else{
#ifdef DEB_INTERNAL
        printf("\n\tTask2 detach error");
#endif
    }

    if (rt_thread_detach(&check_resources) == RT_EOK){
#ifdef DEB_INTERNAL
        printf("\n\tTask3 detached correctly");
#endif
    }else{
#ifdef DEB_INTERNAL
        printf("\n\tTask3 detach error");
#endif
    }

    if (rt_thread_detach(&acoustic_signals) == RT_EOK){
#ifdef DEB_INTERNAL
        printf("\n\tTask4 detached correctly");
#endif
    }else{
#ifdef DEB_INTERNAL
        printf("\n\tTask4 detach error");
#endif
    }

    if (rt_thread_detach(&brushes_speed) == RT_EOK){
#ifdef DEB_INTERNAL
        printf("\n\tTask5 detached correctly");
#endif
    }else{
#ifdef DEB_INTERNAL
        printf("\n\tTask5 detach error");
#endif
    }
}


// THREADS ENTRIES *********************************************************************************************



/* Entry for the task obstacle control */
void obstacle_control_entry(void *param)
{

    int tick_start, tick_end, time_start, time_end;
    char obstacle;

    // infinite loop
    while (1){
        // waits for the periodic activation from the timer
        if (rt_event_recv(&event_tasks_activation, EVENT_OBSTACLE_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK){

            tick_start=rt_tick_get();
            set_tick_count(&obstacle_control, TICK_DELAY_T1);  //set Task1 minimum duration
            time_start=tick_start*10-startingTime;
            reset_end_flag(&obstacle_control);

#ifdef BENCHMARK_TIME
            printf("\n\t\tTASK_1:\t Started at time %d ms\n", time_start);
#endif

            // check if there's an obstacle, if yes activates movements threads
            rt_device_read(proximity_sensor, 0, &obstacle, 1);
            if (obstacle == 'y')
            {
                rt_thread_kill(&movement_control, SIGUSR1);
                rt_event_send(&event_obstacle, EVENT_OBSTACLE_FOUND);

            }

            while(get_tick_count(&obstacle_control)!=0){ }

            tick_end = rt_tick_get();
            time_end= tick_end*10-startingTime;


#ifdef BENCHMARK_TIME

        printf("\t\tTASK_1: Stop at time %d ms\tDeadline was: %d ms\n", time_end, (PERIOD_TASK1)*10+time_start);
        printf("\t\tTASK_1: TOTAL EXECUTION TIME: %d ticks (%d ms)\n", tick_end-tick_start,time_end-time_start);
#endif
        }
        set_end_flag(&obstacle_control);  //last instruction of current thread is pass
    }
}


/* Entry for the task movement stop. */
void movement_stop_entry(void *param)
{
    int stop_size = 5;
    char *stop = "STOP";

    // infinite loop
    while (1) {
        // waits for the aperiodic event that signals an obstacle have been found
        if (rt_event_recv(&event_obstacle, EVENT_OBSTACLE_FOUND,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK){

            rt_device_write(engine, 0, stop, stop_size);

        }
    }
}


/* Entry for the task movement control */
void movement_control_entry(void *param)
{
    int tick_start, tick_end, time_start, time_end;
    int start_size = 6;
    char *start = "START";

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
    while (1){
        // waits for the timer activation
        if (rt_event_recv(&event_tasks_activation, EVENT_MOVEMENT_CONTROL_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK){

            tick_start=rt_tick_get();
            set_tick_count(&movement_control, TICK_DELAY_T2); //set Task2 minimum duration
            time_start=tick_start*10-startingTime;
            reset_end_flag(&movement_control);


#ifdef BENCHMARK_TIME
            printf("\n\t\tTASK_2:\t Started at time %d ms\n", time_start);
#endif
            //mailbox receive
            if (rt_mb_recv(&mb2_3, (rt_uint32_t *)&str, 0) == RT_EOK){

#ifdef DEB_INTERNAL
                rt_kprintf("\tget a mail from mailbox, the content: %s\n", str);
#endif
                direction = RETURN;
                rt_signal_mask(SIGUSR1);
            }

            // RESOURCES UPDATE: we put it here to simulate the depletion of
            // decrease battery
            rt_device_write(battery, 0, NULL, 0);
            // if the previous tile is not an obstacle signal it as cleaned
            if (map[ position[0] ][ position[1] ] == 0) {
                map[ position[0] ][ position[1] ] = 2;
                rt_device_write(garbage_bag, 0, NULL, 0);
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

#ifdef DEB_DISPLAY
                rt_kprintf("\tThe robot is stuck!!");
#endif

                break;
            }
            // if the robot is back at starting station after receiving command to return we end the thread
            if (direction==RETURN && position[0]==0 && position[1]==0) {

#ifdef DEB_DISPLAY
                rt_kprintf("\tThe robot is back at charging station!\n");
#endif

                rt_mb_send(&mb2_5, (rt_uint32_t)&mb_str4);      //notify task 5 with an email

                break;
            }

            rt_device_write(engine, 0, start, start_size);

#ifdef DEB_DISPLAY
            rt_kprintf("\n\tRobot in position %d,%d\n", position[0], position[1]);
#endif

            while(get_tick_count(&movement_control)!=0){ }

            tick_end = rt_tick_get();
            time_end= tick_end*10-startingTime;

#ifdef BENCHMARK_TIME
        printf("\t\tTASK_2: Stop at time %d ms\tDeadline was: %d ms\n", time_end, (PERIOD_TASK2)*10+time_start);
        printf("\t\tTASK_2: TOTAL EXECUTION TIME: %d (%d ms)\n", tick_end-tick_start,time_end-time_start);
#endif
        //WCET THE MAXIMUM AMONG THE DIFFERENT EXECUTIONS
        }
        set_end_flag(&movement_control);
    }
}


/* Entry for the CHECK RESOURCES*/
void check_resources_entry(void *param){

    int tick_start, tick_end, time_start, time_end;
    int battery_value;
    int garbage_bag_value;


    while(1){

        // waiting the periodic activation
        if (rt_event_recv(&event_tasks_activation, EVENT_CHECK_RESOURCES_ACTIVATION,
                                  RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                                  RT_WAITING_FOREVER, RT_NULL) == RT_EOK) {

            tick_start=rt_tick_get();
            set_tick_count(&check_resources, TICK_DELAY_T3);
            time_start=tick_start*10-startingTime;
            reset_end_flag(&check_resources);


#ifdef BENCHMARK_TIME
        printf("\n\t\tTASK_3:\t Started at time %d ms\n", time_start);
#endif


#ifdef DEB_DISPLAY
            if(battery_value%10 == 0) printf("\n\tBATTERY: %d\%\n", battery_value); //ITA: la batteria è completamente scarica
#endif

        /**Check BATTERY status**/
        if(batteryStatus <= TOTALLY_DISCHARGE){

#ifdef DEB_DISPLAY
                printf("\t\tBATTERY TOTALLY LOW --> TURN OFF THE SYSTEM\n\n"); //ITA: la batteria è completamente scarica
#endif

#ifdef DEB_INTERNAL
            printf("\t\tBATTERY TOTALLY LOW --> sending signal to TURN OFF THE SYSTEM \n\n"); //ITA: la batteria è completamente scarica
#endif
            rt_thread_kill(&Tsystem, SIGUSR2); //notify TSystem to TURN OFF the system

        }else if(batteryStatus <= DISCHARGE && batteryStatus > TOTALLY_DISCHARGE) {
            //rt_mb_send(&mb2_3, (rt_uint32_t)&mb_str3);      //notify task 2 BATTERY is LOW

#ifdef DEB_DISPLAY
                printf("\t\tBATTERY is LOW\n\n"); //ITA: la batteria è scarica
#endif

        }else if(batteryStatus <= DISCHARGE_THRESHOLD && batteryStatus > DISCHARGE) {
            rt_event_send(&event_resources, EVENT_FLAG1);   //notify task 4 with an event
            //rt_mb_send(&mb2_3, (rt_uint32_t)&mb_str2);      //notify task 2 with an email

#ifdef DEB_DISPLAY
            printf("\t\tBATTERY is running LOW\n\n");  //ITA: la batteria si sta scaricando
#endif
#ifdef DEB_INTERNAL
            printf("\t\tMail sent %s\t\t because battery LOW\n", mb_str2);
#endif
        }

        /**Check GARBAGE BAG status **/

            rt_device_read(garbage_bag, 0, &garbage_bag_value, sizeof(int));
            if(garbage_bag_value == FULL){
                rt_event_send(&event_resources, EVENT_FLAG2);   //notify task 4 with an event
                rt_mb_send(&mb2_3, (rt_uint32_t)&mb_str1);      //notify task 2 with an email

#ifdef DEB_DISPLAY
                printf("\n\tGarbage bag FULL\n\n");
#endif
#ifdef DEB_INTERNAL
                printf("\t\tMail sent %s\t\t because Garbage bag is FULL\n", mb_str1);
#endif
            }

            while(get_tick_count(&check_resources)!=0){ }

            tick_end = rt_tick_get();
            time_end= tick_end*10-startingTime;

#ifdef BENCHMARK_TIME
            printf("\t\tTASK_3: Stop at time %d ms\tDeadline was: %d ms\n", time_end, (PERIOD_TASK3)*10+time_start);
            printf("\t\tTASK_3: TOTAL EXECUTION TIME: %d (%d ms)\n", tick_end-tick_start,time_end-time_start);
#endif
        }
        set_end_flag(&check_resources);
    }
}


/* Entry for the ACOUSTIC SIGNAL */
void acoustic_signals_entry(void *param){

    int tick_start, tick_end, time_start, time_end;
    rt_uint32_t e;  //to read event

    char low_battery[100] = "LOW BATTERY ALARM";
    char garbage_bag_full[100] = "GARBAGE BAG FULL";

    while(1){
        if (rt_event_recv(&event_resources,(EVENT_FLAG1 | EVENT_FLAG2),RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&e) == RT_EOK){

            startingTime = rt_tick_get_millisecond()-startingTime;
            tick_start = rt_tick_get();
            set_tick_count(&acoustic_signals, TICK_DELAY_T4);
            time_start=tick_start*10-startingTime;
            reset_end_flag(&acoustic_signals);


#ifdef BENCHMARK_TIME
        printf("\n\t\tTASK_4:\t Started at time %d ms\n", time_start);
#endif
            if (e == 0x2){
             // EVENT_FLAG_1 is set
                rt_device_write(speaker, 0, low_battery, 100);
                rt_kprintf("ACUSTIC SI è ATTIVATO");

            }else if (e == 0x4){
                // EVENT_FLAG_2 is set
                rt_device_write(speaker, 0, garbage_bag_full, 100);
                rt_kprintf("ACUSTIC SI è ATTIVATO");
            }

            while(get_tick_count(&acoustic_signals)!=0){}

            tick_end = rt_tick_get();
            time_end= tick_end*10-startingTime;

#ifdef BENCHMARK_TIME
        printf("\t\tTASK_4: Stop at time %d ms\t APERIODIC task\n", time_end);
        printf("\t\tTASK_4: TOTAL EXECUTION TIME: %d (%d ms)\n", tick_end-tick_start,time_end-time_start);
#endif
        }
        set_end_flag(&acoustic_signals);
    }
}


/* Entry for the BRUSHES SPEED */
void brushes_speed_entry(void *param)
{
    int tick_start, tick_end, time_start, time_end;

    /* 0 0 MEDIUM
     * 0 1 HIGH
     * 1 0 LOW
     * 1 1 STOP
     */
    int brushes_speed_v[2], brushes_power[2];
    char *str;

    while(1){
        if (rt_event_recv(&event_tasks_activation, EVENT_BRUSHES_SPEED_ACTIVATION,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER, RT_NULL) == RT_EOK){
            tick_start=rt_tick_get();
            set_tick_count(&brushes_speed, TICK_DELAY_T5);
            time_start=tick_start*10-startingTime;
            reset_end_flag(&brushes_speed);

#ifdef BENCHMARK_TIME
            printf("\n\t\tTASK_5:\t Started at time %d ms\n", time_start);
#endif

            rt_pin_mode(BRUSHES_SPEED_PIN_NUMBER, PIN_MODE_INPUT);
            rt_pin_mode(BRUSHES_POWER_PIN_NUMBER, PIN_MODE_OUTPUT);

//            printf("\n----------------Task5--------------------\n");
/*
            //If the robot is starting on, turn on the brushes
            if (rt_mb_recv(&mb2_5, (rt_uint32_t *)&str, RT_WAITING_NO) == RT_EOK){
                // TODO:WHO SENDS AT TURN ON OF THE ROBOT? -> Tsystem

                if (rt_strcmp(str,mb_str5)==0){
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, MEDIUM);
                    brushes_power[0]=0;
                    brushes_power[1]=0;
                }

                // Executing the mailbox object detachment
                rt_mb_detach(&mb2_5);
            }
*/

            //Reads from mail box in case the robot is coming back. In this case stop the brushes
            if (rt_mb_recv(&mb2_5, (rt_uint32_t *)&str, RT_WAITING_NO) == RT_EOK){
                //TODO: WHO SAYS TO COME BACK? -> t3

                if (rt_strcmp(str,mb_str4)==0){
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, STOP_BRUSHES);
                    brushes_power[0]=1;
                    brushes_power[1]=1;
                }

                /* Executing the mailbox object detachment */
                rt_mb_detach(&mb2_5);
            }

            brushes_speed_v[0] =  rt_pin_read(BRUSHES_SPEED_PIN_NUMBER);
            brushes_speed_v[1] =  rt_pin_read(BRUSHES_SPEED_PIN_NUMBER);

            rt_pin_mode(BRUSHES_POWER_PIN_NUMBER, PIN_MODE_INPUT);
            brushes_power[0] =  rt_pin_read(BRUSHES_POWER_PIN_NUMBER);
            brushes_speed_v[1] =  rt_pin_read(BRUSHES_POWER_PIN_NUMBER);

            rt_pin_mode(BRUSHES_POWER_PIN_NUMBER, PIN_MODE_OUTPUT);

            if(brushes_speed_v[0] == 1 && brushes_speed_v[1] == 0){
                if(brushes_power[0] == 1 && brushes_power[1] == 0){
                    brushes_power[0]=0;
                    brushes_power[1]=0;
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, MEDIUM);
                }else if(brushes_power[0] == 0 && brushes_power[1] == 0){
                    brushes_power[0]=0;
                    brushes_power[1]=1;
                    rt_pin_write(BRUSHES_POWER_PIN_NUMBER, HIGH);
                }
            }else if(brushes_speed_v[0] == 0 && brushes_speed_v[1] == 1){
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

            while(get_tick_count(&brushes_speed)!=0){ }

            tick_end = rt_tick_get();
            time_end= tick_end*10-startingTime;

#ifdef BENCHMARK_TIME
        printf("\t\tTASK_5: Stop at time %d ms\tDeadline was: %d ms\n", time_end, (PERIOD_TASK5)*10+time_start);
        printf("\t\tTASK_5: TOTAL EXECUTION TIME: %d (%d ms)\n", tick_end-tick_start,time_end-time_start);
#endif
        }
        set_end_flag(&brushes_speed);
    }
}

void Tsystem_task_entry(void *param){
    int ticks;

    rt_signal_install(SIGUSR2, turnOffSystem);
    rt_signal_unmask(SIGUSR2);

    while(1){
            ticks=rt_tick_get_millisecond()-startingTime;
            if(ticks%350 == 0 && startingTime != 0){
                batteryStatus--;
#ifdef DEB_INTERNAL
                printf("batteryStatus %d%%, decremented at time %d ms]\n", batteryStatus, ticks);
#endif
            }
            //printf("Tsystem\n");


    }
}

