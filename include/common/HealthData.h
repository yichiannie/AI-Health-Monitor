#ifndef HEALTH_DATA_H
#define HEALTH_DATA_H


enum SensorState
{
    SENSOR_INIT,
    NO_FINGER,
    SIGNAL_WEAK,
    MEASURING,
    DATA_READY,
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