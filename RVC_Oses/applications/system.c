/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-01     Omar       the first version
 */

#include <rtthread.h>
#include "system.h"

void initSystem(void){
    batteryStatus = CHARGE;
    garbageBagStatus = EMPTY;
    return;
}

void updateResources(void){
    garbageBagStatus++;
    if(garbageBagStatus%2==0)
        batteryStatus--;
    return;
}
