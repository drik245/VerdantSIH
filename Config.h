#pragma once
#include <Arduino.h>
// --- PIN DEFINITIONS ---
#define DHT_PIN 4
#define SOIL_MOISTURE_PIN_LOCAL 34
#define HX711_DOUT_PIN 2
#define HX711_SCK_PIN 15
#define SOLENOID_VALVE_1_PIN 26
#define SOLENOID_VALVE_2_PIN 27
// --- CALIBRATION CONSTANTS ---
// ** ACTION REQUIRED: Calibrate all these values **
// 1. LOCAL (Wired) Soil Sensor Calibration
const int SOIL_RAW_DRY_LOCAL = 3200;
const int SOIL_RAW_SATURATED_LOCAL = 1150;
// 2. REMOTE (Wireless) Soil Sensor Calibration
const int SOIL_RAW_DRY_REMOTE = 3150;      // This sensor will have slightly different values
const int SOIL_RAW_SATURATED_REMOTE = 1200; // Calibrate it separately!
// 3. SOIL TYPE CONSTANT (Applies to both zones if soil is the same)
const float SOIL_MAX_VWC = 0.45;     // Max Volumetric Water Content for Silty Clay Loam
// 4. Load Cell / Rain Gauge Calibration
const float HX711_CALIBRATION_FACTOR = -430.0;
const float FUNNEL_AREA_CM2 = 78.5;
// 5. Irrigation Logic
const unsigned long IRRIGATION_DURATION_MS = 60000;