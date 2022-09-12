
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <tasks.h>

#include "system.h"
#include "tasks.h"


int main(void)
{

    // This function initializes global flags used by the application
    initSystem();

    //THREAD STARTUPS
    movement_threads_start();

    return RT_EOK;

}


