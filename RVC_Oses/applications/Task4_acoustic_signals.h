/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-19     Omar       the first version
 */
#ifndef APPLICATIONS_TASK4_ACOUSTIC_SIGNALS_H
#define APPLICATIONS_TASK4_ACOUSTIC_SIGNALS_H

/* EVENT MASKS DEFINITION*/
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)

int thread_acoustic_signals(void);

#endif
