#include <Arduino.h>

#include "display/Display.h"
#include "config/Config.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &SPI,
    OLED_DC_PIN,
    OLED_RESET_PIN,
    OLED_CS_PIN
);

bool initDisplay()
{
    SPI.begin(
        OLED_CLK_PIN,
        -1,
        OLED_MOSI_PIN,
        OLED_CS_PIN
    );

    if (!display.begin(SSD1306_SWITCHCAPVCC))
    {
        return false;
    }

    display.clearDisplay();

    display.setTextSize(1);

    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);

    display.println("AI Health Monitor");
    display.println("Initializing...");

    display.display();

    return true;
}


void updateDisplay(const HealthData &data)
{
    Serial.println("------ HEALTH ------");

    Serial.print("Heart Rate: ");
    Serial.println(data.heartRate);

    Serial.print("SpO2: ");
    Serial.println(data.spo2);

    Serial.println("--------------------");
}