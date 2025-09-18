#pragma once
#include <Arduino.h>

struct SensorData {
float temperature = 0;
float humidity = 0;
float local_kpa = 0;
float rainfall_mm = 0;
bool isRaining = false;
};
void setupSensors();
void updateSensorReadings();
const SensorData& getSensorData();