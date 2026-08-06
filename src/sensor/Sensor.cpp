#include <Arduino.h>
#include "sensor/Sensor.h"

#include <Wire.h>
#include "MAX30105.h"


#define SDA_PIN 26
#define SCL_PIN 25


MAX30105 particleSensor;

bool initSensor()
{
    Wire.begin(SDA_PIN, SCL_PIN);


    if(!particleSensor.begin(Wire, I2C_SPEED_FAST))
    {
        return false;
    }


    particleSensor.setup();


    return true;
}

RawSensorData readSensor()
{
    RawSensorData data;
    
    data.timestamp = millis();

    data.ir = particleSensor.getIR();

    data.red = particleSensor.getRed();


    data.valid = (data.ir > 50000);


    return data;
}