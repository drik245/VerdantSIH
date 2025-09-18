#include "Config.h"
#include "CropDatabase.h"
#include "SensorManager.h"
#include "BluetoothZone.h"
#include "IrrigationLogic.h"
#include <Arduino.h>

unsigned long lastSensorRead = 0;
unsigned long lastStatusPrint = 0;
unsigned long lastLogicCheck = 0;
void setup() {
Serial.begin(115200);
Serial.println("\n--- Smart Irrigation System Booting Up ---");


setupSensors();
setupBluetooth(); // This will start the initial scan
setupIrrigation();

selectCropByName("Tomato"); // Default crop
}
void loop() {
handleBluetoothConnection(); // Manages BLE state


if (millis() - lastSensorRead > 10000) {
    updateSensorReadings();
    lastSensorRead = millis();
}

if (millis() - lastLogicCheck > 15000) {
    checkAndRunIrrigation();
    lastLogicCheck = millis();
}

if (millis() - lastStatusPrint > 30000) {
    const SensorData& sensors = getSensorData();
    Serial.println("\n--- STATUS REPORT ---");
    Serial.printf("Temperature: %.1fC | Humidity: %.1f%%\n", sensors.temperature, sensors.humidity);
    Serial.printf("Zone 1 (Wired) Soil Tension: %.1f kPa\n", sensors.local_kpa);
    Serial.printf("Zone 2 (Wireless) Soil Tension: %.1f kPa\n", getRemoteKPa());
    Serial.printf("Rainfall Detected: %s (%.2f mm)\n", sensors.isRaining ? "YES" : "NO", sensors.rainfall_mm);
    Serial.println("---------------------\n");
    lastStatusPrint = millis();
}
}