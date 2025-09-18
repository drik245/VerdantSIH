// FILE: CropDatabase.h (Corrected)

#pragma once
#include <Arduino.h>

struct CropProfile {
  const char* name;
  int minTemp;
  int maxTemp;
  int target_kPa;
  int trigger_kPa;
  float maxEC;
  float minPH;
  float maxPH;
  int minDLI;
  int maxDLI;
};

// --- DECLARATIONS ---

// Declare that this array exists somewhere else.
extern const CropProfile CROP_DATABASE[]; 
// Declare that this variable exists somewhere else.
extern CropProfile activeCrop; 
// Declare the size constant. This is safe to keep in the header.
extern const int CROP_DATABASE_SIZE;

// Declare the function prototype.
bool selectCropByName(String cropName);