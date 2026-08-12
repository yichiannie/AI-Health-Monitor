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
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);


    // --------------------------------
    // Title
    // --------------------------------

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("AI HEALTH MONITOR");


    // --------------------------------
    // Heart Rate
    // --------------------------------

    display.setTextSize(1);
    display.setCursor(0, 16);
    display.println("HEART RATE");


    display.setTextSize(2);
    display.setCursor(0, 27);

    if(data.heartRate > 0)
    {
        display.print(data.heartRate, 0);
        display.println(" BPM");
    }
    else
    {
        display.println("-- BPM");
    }


    // --------------------------------
    // Signal Quality
    // --------------------------------

    display.setTextSize(1);
    display.setCursor(0, 48);

    display.print("Signal: ");

    if(data.signalQuality == SIGNAL_NO_FINGER)
    {
        display.println("NO FINGER");
    }
    else if(data.signalQuality == SIGNAL_MOVING)
    {
        display.println("MOVING");
    }
    else if(data.signalQuality == SIGNAL_GOOD)
    {
        display.println("GOOD");
    }


    // --------------------------------
    // Measurement State
    // --------------------------------

    display.setCursor(0, 57);

    display.print("Status: ");

    if(data.measureState == WAITING)
    {
        display.println("WAITING");
    }
    else if(data.measureState == MEASURING)
    {
        display.println("MEASURING");
    }
    else if(data.measureState == DATA_READY)
    {
        display.println("READY");
    }


    display.display();
}