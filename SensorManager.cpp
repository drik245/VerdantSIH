#include "SensorManager.h"
#include "Config.h"
#include <DHT.h>
#include "HX711.h"
#include <Arduino.h>

// Sensor Objects
DHT dht(DHT_PIN, DHT22);
HX711 scale;
// Private global struct to hold the data
SensorData currentSensorData;
// SWCC table for your specific soil
const float silty_clay_loam_swcc[][2] = {
{0.50, 1}, {0.46, 10}, {0.42, 30}, {0.38, 80}, {0.35, 200}, {0.31, 500}, {0.25, 1000}, {0.22, 1500}
};
// This is the corrected function

float getVWC(int rawValue) {
  // Use the specific LOCAL calibration constants from Config.h
  float constrainedRaw = constrain(rawValue, SOIL_RAW_SATURATED_LOCAL, SOIL_RAW_DRY_LOCAL);
  return map(constrainedRaw, SOIL_RAW_DRY_LOCAL, SOIL_RAW_SATURATED_LOCAL, 0, SOIL_MAX_VWC * 100.0) / 100.0;
}
float getKPaFromVWC(float vwc) {
int tableSize = sizeof(silty_clay_loam_swcc) / sizeof(silty_clay_loam_swcc[0]);
if (vwc >= silty_clay_loam_swcc[0][0]) return silty_clay_loam_swcc[0][1];
if (vwc <= silty_clay_loam_swcc[tableSize - 1][0]) return silty_clay_loam_swcc[tableSize - 1][1];
for (int i = 0; i < tableSize - 1; i++) {
if (vwc <= silty_clay_loam_swcc[i][0] && vwc > silty_clay_loam_swcc[i+1][0]) {
float v1 = silty_clay_loam_swcc[i][0], k1 = silty_clay_loam_swcc[i][1];
float v2 = silty_clay_loam_swcc[i+1][0], k2 = silty_clay_loam_swcc[i+1][1];
return k1 + ((vwc - v1) * (k2 - k1)) / (v2 - v1);
}
}
return -1;
}
float convertGramsToMM(float grams) {
// 1 gram of water = 1 cm^3. 1 cm^3 / Area(cm^2) = depth in cm. cm * 10 = mm.
if (FUNNEL_AREA_CM2 > 0) {
return (grams / FUNNEL_AREA_CM2) * 10.0;
}
return 0;
}
void setupSensors() {
dht.begin();
scale.begin(HX711_DOUT_PIN, HX711_SCK_PIN);
scale.set_scale(HX711_CALIBRATION_FACTOR);
scale.tare(); // Zero the scale
Serial.println("Wired sensors initialized.");
}
void updateSensorReadings() {
currentSensorData.temperature = dht.readTemperature();
currentSensorData.humidity = dht.readHumidity();
if (isnan(currentSensorData.temperature) || isnan(currentSensorData.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
}

int rawSoilValue = analogRead(SOIL_MOISTURE_PIN_LOCAL);
float vwc = getVWC(rawSoilValue);
currentSensorData.local_kpa = getKPaFromVWC(vwc);

float rainGrams = scale.get_units(5); // Average of 5 readings
if (rainGrams > 0.1) { // Threshold for detecting rain
  currentSensorData.isRaining = true;
  currentSensorData.rainfall_mm = convertGramsToMM(rainGrams);
  // NOTE: For cumulative rainfall, you would add this value to a daily total
  // and only tare the scale once per day. For now, we tare it after reading.
  scale.tare(); 
} else {
  currentSensorData.isRaining = false;
  currentSensorData.rainfall_mm = 0;
}
}
const SensorData& getSensorData() {
return currentSensorData;
}