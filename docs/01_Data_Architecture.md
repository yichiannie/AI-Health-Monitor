# Data Architecture

The system uses a centralized HealthData structure
for communication between FreeRTOS tasks.

## Design Principle

Single Writer Principle:

- Sensor Task updates HealthData
- Other tasks only read the data

## Current Data

- Heart Rate
- SpO2
- Sensor State
- Timestamp