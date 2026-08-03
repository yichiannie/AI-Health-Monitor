#include <Arduino.h>

#include "common/HealthData.h"
#include "sensor/Sensor.h"
#include "display/Display.h"

HealthData healthData;


void setup()
{
    Serial.begin(115200);


    initSensor();

    initDisplay();

    updateSensor(healthData);

    updateDisplay(healthData);
}


void loop()
{

}