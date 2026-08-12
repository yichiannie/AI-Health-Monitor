#include <Arduino.h>
#include <WiFi.h>

#include "network/WiFiManager.h"
#include "config/Secrets.h"


bool initWiFi()
{
    Serial.println("Connecting to WiFi...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long startTime = millis();

    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");

        if(millis() - startTime > 15000)
        {
            Serial.println();
            Serial.println("WiFi connection failed!");

            return false;
        }
    }

    Serial.println();
    Serial.println("WiFi connected!");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return true;
}


bool isWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}