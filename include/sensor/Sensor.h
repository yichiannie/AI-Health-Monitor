#ifndef SENSOR_H
#define SENSOR_H


#include "common/HealthData.h"
#include "common/RawSensorData.h"

bool initSensor();

RawSensorData readSensor();

#endif