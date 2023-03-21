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
    static int i=0;
    char *buf;

    buf = (char*)buffer;

    if (++i == 50) {
        i=0;
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
    /* insert function */
    rt_kprintf("\n\nTEST\n\n");
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

}
