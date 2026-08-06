#include "algorithm/Algorithm.h"
#include <Arduino.h>

#define BUFFER_SIZE 100

static uint32_t irBuffer[BUFFER_SIZE];

static int bufferIndex = 0;

HealthData process(RawSensorData data)
{
    HealthData result;


    result.timestamp = millis();


    if(!data.valid)
    {
        result.state = NO_FINGER;

        result.heartRate = 0;
        result.spo2 = 0;
    }
    else
    {
        result.state = MEASURING;

        result.heartRate = 0;
        result.spo2 = 0;
    }


    return result;
}