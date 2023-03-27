#include <drivers/mock_devices.h>
/* Here we create the fake devices with fake drivers that we will use, in particular we have:
 * - proximity_sensor: sensor read to see if there are obstacle in front
 * - engine: the engine, we will send command to stop and start
 * - battery: we will read battery status ??
 * - garbage_bag: we will read garbage_bag status
 */

/* DRIVERS for proximity_sensor */
static int proximity_sensor_init(rt_device_t dev) {
    dev->ref_count++;
    return RT_EOK;
}
static int proximity_sensor_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size) {
    int i;
    char *buf;

    buf = (char*)buffer;
    srand( (unsigned int) rt_tick_get_millisecond());
    i = rand();

    if (i%30 == 0) {
        *buf = 'y';
    } else {
        *buf = 'n';
    }
    return RT_EOK;
}
const static struct rt_device_ops proximity_sensor_ops =
{
    proximity_sensor_init,
    RT_NULL,
    RT_NULL,
    proximity_sensor_read,
    RT_NULL,
    RT_NULL,
};


/* DRIVERS for engine */
static int engine_init(rt_device_t dev) {
    dev->ref_count++;
    return RT_EOK;
}
static int engine_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) {

    char *msg;
    static int stopped = 1;
    msg = (char*)buffer;

    if (strcmp(msg, "STOP") == 0) {
        stopped = 1;
        rt_kprintf("\n\tENGINE: stopped!\n");
    } else if (strcmp(msg, "START") == 0) {
        if(stopped==1)  rt_kprintf("\n\tENGINE: started!\n");
        stopped = 0;
    }

    return RT_EOK;
}
const static struct rt_device_ops engine_ops =
{
    engine_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    engine_write,
    RT_NULL,
};


/* DRIVERS for battery */
static int battery_value = 100;
static int battery_init(rt_device_t dev) {
    dev->ref_count++;
    return RT_EOK;
}
static int battery_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) {
    battery_value--;
    return RT_EOK;
}
static int battery_read(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) {
    *((int*)buffer) = battery_value;
    return RT_EOK;
}
const static struct rt_device_ops battery_ops =
{
    battery_init,
    RT_NULL,
    RT_NULL,
    battery_read,
    battery_write,
    RT_NULL,
};


/* DRIVERS for garbage_bag */
static int garbage_bag_value = 0;
static int garbage_bag_init(rt_device_t dev) {
    dev->ref_count++;
    return RT_EOK;
}
static int garbage_bag_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) {
    garbage_bag_value++;
    return RT_EOK;
}
static int garbage_bag_read(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) {
    *((int*)buffer) = garbage_bag_value;
    return RT_EOK;
}
const static struct rt_device_ops garbage_bag_ops =
{
    garbage_bag_init,
    RT_NULL,
    RT_NULL,
    garbage_bag_read,
    garbage_bag_write,
    RT_NULL,
};


/* DRIVERS for garbage_bag */
static int speaker_init(rt_device_t dev) {
    dev->ref_count++;
    return RT_EOK;
}
static int speaker_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) {

    rt_kprintf("\n\tSPEAKER: %s", (char*) buffer);

    return RT_EOK;
}
const static struct rt_device_ops speaker_ops =
{
    garbage_bag_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    speaker_write,
    RT_NULL,
};




void rt_mock_devices_init()
{

    /* creating and registering proximity sensor */
    proximity_sensor = rt_device_create(RT_Device_Class_Mock, SIZE);
    proximity_sensor->ops = &proximity_sensor_ops;
    rt_device_register(proximity_sensor, "proximity_sensor", RT_DEVICE_FLAG_RDWR);

    /* creating and registering engine */
    engine = rt_device_create(RT_Device_Class_Mock, SIZE);
    engine->ops = &engine_ops;
    rt_device_register(engine, "engine", RT_DEVICE_FLAG_RDWR);

    /* creating and registering engine */
    battery = rt_device_create(RT_Device_Class_Mock, SIZE);
    battery->ops = &battery_ops;
    rt_device_register(battery, "battery", RT_DEVICE_FLAG_RDWR);

    /* creating and registering engine */
    garbage_bag = rt_device_create(RT_Device_Class_Mock, SIZE);
    garbage_bag->ops = &garbage_bag_ops;
    rt_device_register(garbage_bag, "garbage_bag", RT_DEVICE_FLAG_RDWR);

    /* creating and registering speaker */
    speaker = rt_device_create(RT_Device_Class_Mock, SIZE);
    speaker->ops = &speaker_ops;
    rt_device_register(speaker, "speaker", RT_DEVICE_FLAG_RDWR);

}
