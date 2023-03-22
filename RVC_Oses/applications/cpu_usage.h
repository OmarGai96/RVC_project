/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-06     Omar       the first version
 */
#ifndef APPLICATIONS_CPU_USAGE_H_
#define APPLICATIONS_CPU_USAGE_H_

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);
void cpu_usage_init();

#endif /* APPLICATIONS_CPU_USAGE_H_ */
