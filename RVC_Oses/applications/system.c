
#include <rtthread.h>

#include "system.h"
#include "structures.h"

/** initialize global variables **/
void initSystem(void){

    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;
}
