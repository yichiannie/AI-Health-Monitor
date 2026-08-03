#ifndef DISPLAY_H
#define DISPLAY_H


#include "common/HealthData.h"


bool initDisplay();

void updateDisplay(const HealthData &data);


#endif