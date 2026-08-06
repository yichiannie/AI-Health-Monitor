#include <Arduino.h>

#include "sensor/Sensor.h"
#include "algorithm/Algorithm.h"


void setup()
{
    Serial.begin(115200);


    if(initSensor())
    {
        Serial.println("Sensor OK");
    }
}


void loop()
{
    RawSensorData raw = readSensor();


    HealthData health = process(raw);


    Serial.print("State: ");
    Serial.println(health.state);


    Serial.print("HR: ");
    Serial.println(health.heartRate);


    Serial.print("SpO2: ");
    Serial.println(health.spo2);


    delay(100);
}