#ifndef SENSOR_H
#define SENSOR_H


#include "common/HealthData.h"


bool initSensor();

void updateSensor(HealthData &data);


#endif