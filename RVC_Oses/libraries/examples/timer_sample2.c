//STATIC TIMERS EXAMPLE

#include <rtthread.h>

static struct rt_timer timer1;
static struct rt_timer timer2;
static int cnt = 0;

/* Timer 1 Timeout Function */
static void timeout1(void* parameter)
{
    rt_kprintf("periodic timer is timeout, cnt = %d \n", cnt);
    /* Run for 10 times */
    if (cnt++>= 9)
    {
        rt_timer_stop(&timer1);
    }
}

/* Timer 2 Timeout Function */
static void timeout2(void* parameter)
{
    rt_kprintf("one shot timer is timeout\n");
}

int timer_static_sample(void)
{
    /* Initialize Timer */
    rt_timer_init(&timer1, "timer1",  /* Timer name is timer1 */
                    timeout1, /* Callback handler for timeout */
                    RT_NULL, /* Entry parameter of the timeout function */
                    10, /* Timing length in OS Tick, 10 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* Periodic timer */
    rt_timer_init(&timer2, "timer2",   /* Timer name is timer2 */
                  timeout2, /* Callback handler for timeout */
                  RT_NULL, /* Entry parameter of the timeout function */
                  30, /* Timing length is 30 OS Tick */
                  RT_TIMER_FLAG_ONE_SHOT); /* One-shot timer */

    /* Start Timer */
    rt_timer_start(&timer1);
    rt_timer_start(&timer2);
    return 0;
}
