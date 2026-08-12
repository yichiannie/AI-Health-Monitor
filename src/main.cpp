#include <Arduino.h>
#include <time.h>
#include <WiFi.h>

#include "sensor/Sensor.h"
#include "algorithm/Algorithm.h"
#include "display/Display.h"
#include "network/WiFiManager.h"
#include "config/Config.h"
#include "network/MQTTManager.h"


void setup()
{
    Serial.begin(115200);


    // Initialize Sensor
    if(initSensor())
    {
        Serial.println("Sensor OK");
    }
    else
    {
        Serial.println("Sensor initialization failed!");
    }


    // Initialize Display
    if(initDisplay())
    {
        Serial.println("Display OK");
    }
    else
    {
        Serial.println("Display initialization failed!");
    }


    initWiFi();

    if(initMQTT())
    {
        if(connectMQTT())
        {
            Serial.println("MQTT OK");
        }
        else
        {
            Serial.println("MQTT failed");
        }
    }

}




void loop()
{
    mqttLoop();


    // ================= Sensor =================

    RawSensorData rawData = readSensor();


    // ================= Algorithm =================

    HealthData healthData = process(rawData);


    // ================= OLED =================

    updateDisplay(healthData);


    // ================= MQTT reconnect =================

    static unsigned long lastReconnectAttempt = 0;


    if (!isMQTTConnected())
    {
        if (millis() - lastReconnectAttempt > 5000)
        {
            lastReconnectAttempt = millis();

            Serial.println("MQTT disconnected.");
            Serial.println("Attempting MQTT reconnect...");


            if(connectMQTT())
            {
                Serial.println("MQTT reconnect successful!");
            }
        }
    }


    // ================= MQTT Publish =================

    static unsigned long lastPublish = 0;


    if(millis() - lastPublish > 3000)
    {
        lastPublish = millis();

        publishHealthData(healthData);
    }


    delay(10);
}