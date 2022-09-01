/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-01     Omar       the first version
 */
#ifndef APPLICATIONS_TASK2_3_SYNCH_H_
#define APPLICATIONS_TASK2_3_SYNCH_H_

#include "system.h"

/* EVENT MASKS DEFINITION*/
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)

int thread3_check_resources(void);   //Task3
int thread4_acoustic_signals(void);  //Task4

void startThreads(void);

#endif /* APPLICATIONS_TASK2_3_SYNCH_H_ */
