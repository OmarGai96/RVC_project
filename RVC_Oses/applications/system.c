
#include <rtthread.h>

#include "system.h"
#include "structures.h"


void initSystem(void)
{
    /* initialize global variables */
    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;
}
