#include "algorithm/Algorithm.h"
#include <Arduino.h>


#define FILTER_SIZE 10

static uint32_t irBuffer[FILTER_SIZE];
static int bufferIndex = 0;

static uint32_t previousFilteredIR = 0;

static bool rising = false;

static unsigned long lastBeatTime = 0;

static float currentHeartRate = 0;


#define BPM_BUFFER_SIZE 5

static float bpmBuffer[BPM_BUFFER_SIZE];
static int bpmIndex = 0;



// Moving average filter
static uint32_t filterIR(uint32_t ir)
{
    irBuffer[bufferIndex] = ir;

    bufferIndex++;

    if(bufferIndex >= FILTER_SIZE)
        bufferIndex = 0;


    uint64_t sum = 0;

    for(int i=0;i<FILTER_SIZE;i++)
    {
        sum += irBuffer[i];
    }


    return sum / FILTER_SIZE;
}



// BPM smooth
static float smoothBPM(float bpm)
{
    bpmBuffer[bpmIndex] = bpm;

    bpmIndex++;

    if(bpmIndex >= BPM_BUFFER_SIZE)
        bpmIndex = 0;


    float sum = 0;

    int count = 0;

    for(int i=0;i<BPM_BUFFER_SIZE;i++)
    {
        if(bpmBuffer[i] > 0)
        {
            sum += bpmBuffer[i];
            count++;
        }
    }


    if(count == 0)
        return bpm;


    return sum / count;
}



static bool detectBeat(uint32_t filteredIR, unsigned long timestamp)
{

    bool beat = false;


    // Simple Peak Detection

    if(filteredIR > previousFilteredIR)
    {
        rising = true;
    }


    else if(filteredIR < previousFilteredIR)
    {

        if(rising)
        {

            unsigned long interval = timestamp - lastBeatTime;


            if(lastBeatTime != 0 &&
               interval > 300 &&
               interval < 2000)
            {
                currentHeartRate = 60000.0f / interval;

                currentHeartRate = smoothBPM(currentHeartRate);

                beat = true;

                Serial.print("Interval:");
                Serial.println(interval);

            }


            lastBeatTime = timestamp;

            rising = false;

        }

    }


    previousFilteredIR = filteredIR;


    return beat;

}



HealthData process(RawSensorData data)
{

    HealthData result;

    result.timestamp = millis();



    bool fingerDetected = (data.ir > 10000);

    if(!fingerDetected)
    {

        result.state = NO_FINGER;

        result.heartRate = 0;

        result.spo2 = 0;


        return result;

    }



    uint32_t filteredIR = filterIR(data.ir);


    if(detectBeat(filteredIR,data.timestamp))
    {

        Serial.print("Filtered IR:");
        Serial.print(filteredIR);


        Serial.print(" BPM:");
        Serial.println(currentHeartRate);


        result.state = DATA_READY;

    }

    else
    {

        result.state = MEASURING;

    }



    result.heartRate = currentHeartRate;

    result.spo2 = 0;


    return result;

}