#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "common/HealthData.h"

bool initMQTT();

bool connectMQTT();

bool publishHealthData(const HealthData &data);

void mqttLoop();

bool isMQTTConnected();

#endif