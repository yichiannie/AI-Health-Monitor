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

    // 基础排查：如果串口依然没有任何输出，解开下面这行的注释，看看 sensor raw 是否在正常刷新
    // Serial.print("IR: "); Serial.print(raw.ir); Serial.print(" State: "); Serial.println(health.state);

    delay(10);
}