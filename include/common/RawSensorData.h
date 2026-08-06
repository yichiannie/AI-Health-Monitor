#ifndef RAW_SENSOR_DATA_H
#define RAW_SENSOR_DATA_H

#include <stdint.h>

struct RawSensorData
{
    uint32_t ir;
    uint32_t red;

    bool valid;

    unsigned long timestamp;
};

#endif