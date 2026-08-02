#include <Arduino.h>

#include "common/HealthData.h"
#include "sensor/Sensor.h"


HealthData healthData;


void setup()
{
    Serial.begin(115200);


    initSensor();


    updateSensor(healthData);


    Serial.print("Heart Rate: ");
    Serial.println(healthData.heartRate);


    Serial.print("SpO2: ");
    Serial.println(healthData.spo2);
}


void loop()
{

}