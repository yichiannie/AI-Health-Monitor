#ifndef HEALTH_DATA_H
#define HEALTH_DATA_H


enum SignalQuality
{
    SIGNAL_NO_FINGER,
    SIGNAL_MOVING,
    SIGNAL_GOOD
};


enum MeasureState
{
    WAITING,
    MEASURING,
    DATA_READY
};



struct HealthData
{
    float heartRate;
    float spo2;

    SignalQuality signalQuality;
    MeasureState measureState;

    unsigned long timestamp;
};


#endif