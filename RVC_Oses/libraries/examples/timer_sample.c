//DYNAMIC TIMERS EXAMPLE

#include <rtthread.h>

#define TIMER_1_PERIOD 10

static rt_timer_t timer1;  //periodic timer
static rt_timer_t timer2;  //one shot timer
static int cnt = 0;

/* Timer 1 Timeout Function */
static void timeout1(void *parameter)
{
    rt_kprintf("periodic timer is timeout %d\n", cnt);
    cnt++;
}
//this function is called every 10 OS ticks

/* Timer 2 Timeout Function */
static void timeout2(void *parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}
//this function is called after 30 OS ticks, only once

int timer_sample(void)
{
    /* Create Timer 1 Periodic Timer */
    timer1 = rt_timer_create("timer1", timeout1,
                                RT_NULL, TIMER_1_PERIOD,
                                RT_TIMER_FLAG_PERIODIC);

    /* Start Timer 1*/
    if (timer1 != RT_NULL) rt_timer_start(timer1);

    /* Create Timer 2 One Shot Timer */
    timer2 = rt_timer_create("timer2", timeout2,
                             RT_NULL,  30,
                             RT_TIMER_FLAG_ONE_SHOT);

    /* Start Timer 2 */
    if (timer2 != RT_NULL) rt_timer_start(timer2);
    return 0;
}
