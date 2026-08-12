#include "algorithm/Algorithm.h"
#include <Arduino.h>


#define FILTER_SIZE 10

#define MOTION_THRESHOLD_PERCENT 5.0f

#define MOTION_COUNT_THRESHOLD 2

#define REQUIRED_BEATS 3

static uint32_t irBuffer[FILTER_SIZE];
static int bufferIndex = 0;

static uint32_t previousFilteredIR = 0;

static bool rising = false;

static unsigned long lastBeatTime = 0;

static float currentHeartRate = 0;

static uint32_t previousIR = 0;

static int motionCount = 0;

static int validBeatCount = 0;

static bool measurementReady = false;


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


static bool detectMotion(uint32_t ir)
{
    if(previousIR == 0)
    {
        previousIR = ir;
        return false;
    }

    float changePercent =
        abs((float)ir - (float)previousIR)
        / (float)previousIR
        * 100.0f;

    previousIR = ir;

    if(changePercent > MOTION_THRESHOLD_PERCENT)
    {
        motionCount++;
    }
    else
    {
        motionCount = 0;
    }

    if(motionCount >= MOTION_COUNT_THRESHOLD)
    {
        return true;
    }

    return false;
}

HealthData process(RawSensorData &data)
{
    HealthData result;

    result.timestamp = millis();


    // --------------------------------
    // 1. Finger detection
    // --------------------------------

    bool fingerDetected = (data.ir > 10000);

    if(!fingerDetected)
    {
        validBeatCount = 0;
        measurementReady = false;

        result.signalQuality = SIGNAL_NO_FINGER;
        result.measureState = WAITING;

        result.heartRate = 0;
        result.spo2 = 0;

        return result;
    }


    // --------------------------------
    // 2. Filter signal
    // --------------------------------

    uint32_t filteredIR = filterIR(data.ir);


    // --------------------------------
    // 3. Motion detection
    // --------------------------------

    bool moving = detectMotion(data.ir);

    if(moving)
    {
        measurementReady = false;
        validBeatCount = 0;

        result.signalQuality = SIGNAL_MOVING;
        result.measureState = MEASURING;

        result.heartRate = 0;
        result.spo2 = 0;

        Serial.println("Signal: MOVING");

        return result;
    }


    // --------------------------------
    // 4. Good signal
    // --------------------------------

    result.signalQuality = SIGNAL_GOOD;


    // --------------------------------
    // 5. Beat detection
    // --------------------------------

    if(detectBeat(filteredIR, data.timestamp))
    {
        validBeatCount++;

        Serial.print("Valid beats: ");
        Serial.println(validBeatCount);


        if(validBeatCount >= REQUIRED_BEATS)
        {
            measurementReady = true;
        }
    }


    // --------------------------------
    // 6. Measurement state
    // --------------------------------

    if(measurementReady)
    {
        result.measureState = DATA_READY;
    }
    else
    {
        result.measureState = MEASURING;
    }


    result.heartRate = currentHeartRate;
    result.spo2 = 0;


    return result;
}