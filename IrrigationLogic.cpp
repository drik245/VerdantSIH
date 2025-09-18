#include "IrrigationLogic.h"
#include "Config.h"
#include "CropDatabase.h"
#include "SensorManager.h"
#include "BluetoothZone.h"
#include <Arduino.h>
bool isZone1Watering = false;
bool isZone2Watering = false;
unsigned long zone1StartTime = 0;
unsigned long zone2StartTime = 0;
void setupIrrigation() {
pinMode(SOLENOID_VALVE_1_PIN, OUTPUT);
pinMode(SOLENOID_VALVE_2_PIN, OUTPUT);
digitalWrite(SOLENOID_VALVE_1_PIN, LOW); // Off
digitalWrite(SOLENOID_VALVE_2_PIN, LOW); // Off
Serial.println("Irrigation system initialized.");
}
void checkAndRunIrrigation() {
const SensorData& sensors = getSensorData();
float remoteKPa = getRemoteKPa();

// --- Check if we need to STOP watering (timer based) ---
if (isZone1Watering && (millis() - zone1StartTime > IRRIGATION_DURATION_MS)) {
    digitalWrite(SOLENOID_VALVE_1_PIN, LOW);
    isZone1Watering = false;
    Serial.println("Zone 1 irrigation complete.");
}
if (isZone2Watering && (millis() - zone2StartTime > IRRIGATION_DURATION_MS)) {
    digitalWrite(SOLENOID_VALVE_2_PIN, LOW);
    isZone2Watering = false;
    Serial.println("Zone 2 irrigation complete.");
}

// --- Check if we need to START watering ---
bool isTempOk = (sensors.temperature >= activeCrop.minTemp && sensors.temperature <= activeCrop.maxTemp);

// Zone 1 Logic (Wired)
if (!isZone1Watering && sensors.local_kpa > activeCrop.trigger_kPa) {
    if (!isTempOk) {
        Serial.println("Zone 1: Soil is dry, but temperature is out of range. No irrigation.");
    } else if (sensors.isRaining) {
        Serial.println("Zone 1: Soil is dry, but it is raining. No irrigation.");
    } else {
        Serial.println(">>> Zone 1: Soil is dry. Starting irrigation! <<<");
        digitalWrite(SOLENOID_VALVE_1_PIN, HIGH);
        isZone1Watering = true;
        zone1StartTime = millis();
    }
}

// Zone 2 Logic (Wireless)
if (remoteKPa > 0 && !isZone2Watering && remoteKPa > activeCrop.trigger_kPa) {
    if (!isTempOk) {
        Serial.println("Zone 2: Soil is dry, but temperature is out of range. No irrigation.");
    } else if (sensors.isRaining) {
        Serial.println("Zone 2: Soil is dry, but it is raining. No irrigation.");
    } else {
        Serial.println(">>> Zone 2: Soil is dry. Starting irrigation! <<<");
        digitalWrite(SOLENOID_VALVE_2_PIN, HIGH);
        isZone2Watering = true;
        zone2StartTime = millis();
    }
}
}