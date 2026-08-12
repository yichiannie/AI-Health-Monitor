#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include "network/MQTTManager.h"
#include "config/Config.h"
#include "config/Secrets.h"


static WiFiClientSecure wifiClient;

static PubSubClient mqttClient(wifiClient);


bool initMQTT()
{
    wifiClient.setInsecure();

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setKeepAlive(15);
    mqttClient.setBufferSize(512);

    return true;
}


bool connectMQTT()
{
    Serial.println("Connecting to MQTT...");

    if(mqttClient.connect(
        "ai_health_monitor_yichi_01",
        MQTT_USERNAME,
        MQTT_PASSWORD
    ))
    {
        Serial.println("MQTT connected!");
        return true;
    }

    Serial.print("MQTT connection failed, state: ");
    Serial.println(mqttClient.state());

    return false;
}


bool publishHealthData(const HealthData &data)
{
        if(!mqttClient.connected())
    {
        Serial.print("MQTT disconnected! State: ");
        Serial.println(mqttClient.state());

        return false;
    }

    JsonDocument doc;

    doc["heartRate"] = data.heartRate;
    doc["spo2"] = data.spo2;

    if(data.signalQuality == SIGNAL_NO_FINGER)
        doc["signalQuality"] = "NO_FINGER";
    else if(data.signalQuality == SIGNAL_MOVING)
        doc["signalQuality"] = "MOVING";
    else if(data.signalQuality == SIGNAL_GOOD)
        doc["signalQuality"] = "GOOD";

    if(data.measureState == WAITING)
        doc["measureState"] = "WAITING";
    else if(data.measureState == MEASURING)
        doc["measureState"] = "MEASURING";
    else if(data.measureState == DATA_READY)
        doc["measureState"] = "DATA_READY";

    doc["timestamp"] = data.timestamp;

    char payload[256];

    serializeJson(doc, payload);

    if(mqttClient.publish(MQTT_TOPIC, payload))
    {
        Serial.println("MQTT publish OK");
        Serial.println(payload);
        return true;
    }
    else
    {
        Serial.println("MQTT publish FAILED");
        return false;
    }
}

void mqttLoop()
{
    if (mqttClient.connected())
    {
        mqttClient.loop();

        Serial.println("MQTT alive");
    }
}

bool isMQTTConnected()
{
    return mqttClient.connected();
}

