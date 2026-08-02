#include <Arduino.h>

#include "sensor/Sensor.h"


bool initSensor()
{
    return true;
}


void updateSensor(HealthData &data)
{
    data.heartRate = 75;
    data.spo2 = 98;

    data.state = READY;

    data.timestamp = millis();
}