#include <rtthread.h>

#define THREAD1_PRIORITY         25
#define THREAD1_STACK_SIZE       512
#define THREAD1_TIMESLICE        10  //number of ticks

#define THREAD2_PRIORITY         25
#define THREAD2_STACK_SIZE       512
#define THREAD2_TIMESLICE        5  //number of ticks

#define SLEEP_TIME 10   //number of ticks

static rt_thread_t tid1 = RT_NULL; //pointer to thread1
static rt_thread_t tid2 = RT_NULL; //pointer to thread2

/* Entry Function for Thread 1 */
static void task1(void *parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* Thread 1 runs with low priority and prints the count value all the time */
        rt_kprintf("thread1 count: %d\n", count ++);
        rt_thread_sleep(SLEEP_TIME);
    }
}



/* Entry Function for Thread 1 */
static void task2(void *parameter)
{
    rt_uint32_t count = 100;

    while (1)
    {
        /* Thread 1 runs with low priority and prints the count value all the time */
        rt_kprintf("thread2 count: %d\n", count --);
        rt_thread_sleep(SLEEP_TIME);
    }
}


/* Thread Sample */
int thread_sample2(void)
{
    /* Create thread 1, Name is thread1，Entry is task1 */
    tid1 = rt_thread_create("thread1",
                            task1, RT_NULL,
                            THREAD1_STACK_SIZE,
                            THREAD1_PRIORITY, THREAD1_TIMESLICE);

    /* Start this thread if you get the thread control block */


    /* Create thread 2, Name is thread2，Entry is task2 */
    tid2 = rt_thread_create("thread2",
                            task2, RT_NULL,
                            THREAD2_STACK_SIZE,
                            THREAD2_PRIORITY, THREAD2_TIMESLICE);
    if(tid1 != RT_NULL)
        rt_thread_startup(tid1);
    else {
        rt_kprintf("thread1 can't start\n");
    }
    if(tid2 != RT_NULL)
        rt_thread_startup(tid2);
    else {
        rt_kprintf("thread2 can't start\n");
        }
    return 0;
}
