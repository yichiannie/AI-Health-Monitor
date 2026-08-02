#ifndef HEALTH_DATA_H
#define HEALTH_DATA_H


enum SensorState
{
    WAITING_FOR_FINGER,
    CALCULATING,
    READY,
    SENSOR_ERROR
};


struct HealthData
{
    float heartRate;
    float spo2;

    SensorState state;

    unsigned long timestamp;
};


#endif