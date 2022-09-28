
#include <rtthread.h>

#include "system.h"
#include "structures.h"


void initSystem(void)
{
    /* initialize global variables */
    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;
}


void updateResources(void){
    garbageBagStatus++;
    if(garbageBagStatus%10==0)
        batteryStatus--;
    return;
}
