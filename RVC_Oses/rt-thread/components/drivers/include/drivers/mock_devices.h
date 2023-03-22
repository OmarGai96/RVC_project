#ifndef LIBRARIES_MOCK_DEVICES_FAKE_DEVICE_H_
#define LIBRARIES_FAKE_DEVICE_FAKE_DEVICE_H_


#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>


#define SIZE 1024


rt_device_t proximity_sensor;
rt_device_t engine;


void rt_mock_devices_init();


#endif /* LIBRARIES_MOCK_DEVICES_FAKE_DEVICE_H_ */
