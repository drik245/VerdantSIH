#include "CropDatabase.h" // Include the header to link this implementation to its declaration

// --- DEFINITIONS ---
// This is the one and only place where these variables and the database are actually created in memory.

// Define the actual database array. It is constant, so it's stored in flash.
const CropProfile CROP_DATABASE[] = {
    // Name, MinTemp, MaxTemp, target_kPa(Min), trigger_kPa(Max), maxEC, minPH, maxPH, minDLI, maxDLI
    {"Rice", 20, 35, 0, 5, 1.5, 5, 6.5, 12, 18},
    {"Maize", 18, 30, 20, 40, 1.8, 5.5, 7, 18, 30},
    {"Finger Millet", 18, 30, 30, 60, 2, 5, 6.5, 18, 30},
    {"Barley", 12, 20, 30, 50, 2, 6, 7.5, 18, 30},
    {"Wheat", 15, 22, 25, 40, 2, 6, 7, 18, 30},
    {"Black Gram (Urd)", 25, 35, 20, 40, 1.5, 6, 7.5, 18, 30},
    {"Pea", 13, 20, 15, 30, 1.5, 6, 7.5, 18, 30},
    {"Soybean", 20, 30, 20, 40, 1.5, 6, 7, 18, 30},
    {"Mustard", 15, 25, 20, 40, 1.8, 6, 7.5, 18, 30},
    {"Potato", 15, 20, 15, 25, 1.5, 5, 6.5, 18, 30},
    {"Sweet Potato", 20, 30, 30, 50, 2, 5.5, 6.5, 18, 30},
    {"Large Cardamom", 15, 25, 10, 25, 1.2, 5, 6.5, 6, 12},
    {"Chilly", 20, 30, 15, 30, 2.5, 5.5, 6.8, 18, 30},
    {"Turmeric", 20, 30, 15, 25, 1.8, 5.5, 7.5, 18, 30},
    {"Coriander", 15, 25, 10, 20, 1.5, 6, 7, 12, 18},
    {"Ginger", 25, 30, 15, 25, 1.5, 5.5, 6.5, 12, 18},
    {"Mandarin Orange", 20, 30, 25, 40, 1.8, 5.5, 6.5, 18, 30},
    {"Banana", 22, 30, 10, 20, 1, 5.5, 6.5, 18, 30},
    {"Papaya", 21, 33, 15, 30, 1.5, 5.5, 6.5, 18, 30},
    {"Litchi", 20, 25, 25, 40, 1.5, 5.5, 7, 18, 30},
    {"Passion Fruit", 18, 25, 20, 35, 1.5, 5.5, 7, 18, 30},
    {"Guava", 15, 30, 30, 50, 1.8, 5.5, 7, 18, 30},
    {"Jackfruit", 22, 32, 20, 40, 2, 5.5, 6.5, 18, 30},
    {"Broccoli", 15, 20, 15, 25, 2, 6, 6.8, 18, 30},
    {"Iskus (Chayote)", 15, 25, 15, 30, 1.8, 5.5, 6.5, 18, 30},
    {"Tomato", 18, 25, 20, 35, 2.5, 5.5, 6.8, 18, 30},
    {"Cauliflower", 15, 20, 15, 25, 2, 6, 6.8, 18, 30},
    {"Onion", 13, 25, 15, 50, 1.5, 6, 6.8, 18, 30},
    {"Pumpkin", 20, 30, 20, 40, 1.8, 5.5, 6.8, 18, 30},
    {"Tree Tomato", 15, 22, 20, 35, 1.8, 5.5, 6.8, 18, 30},
    {"Brinjal", 22, 30, 20, 35, 2, 5.5, 6.8, 18, 30},
    {"Carrot", 15, 20, 15, 25, 1.2, 5.5, 6.8, 18, 30},
    {"Radish", 10, 20, 15, 25, 1.5, 5.5, 6.8, 18, 30},
    {"Cabbage", 15, 20, 15, 25, 2, 6, 6.8, 18, 30},
    {"Anthurium", 16, 25, 10, 20, 0.8, 5.5, 6.5, 6, 12},
    {"Cymbidium Orchid", 10, 26, 10, 20, 0.8, 5.5, 6.5, 8, 16},
    {"Rose", 18, 24, 20, 40, 1.5, 5.5, 6.5, 18, 30}
};

// Define the size constant.
const int CROP_DATABASE_SIZE = sizeof(CROP_DATABASE) / sizeof(CROP_DATABASE[0]);

// Define the actual variable that will hold the active crop data. This is stored in RAM.
CropProfile activeCrop;

// Define the actual function body here.
bool selectCropByName(String cropName) {
  for (int i = 0; i < CROP_DATABASE_SIZE; i++) {
    if (cropName.equalsIgnoreCase(CROP_DATABASE[i].name)) {
      activeCrop = CROP_DATABASE[i];
      Serial.println("----------------------------------------");
      Serial.println("Active Crop Profile Updated!");
      Serial.println("Crop: " + String(activeCrop.name));
      Serial.printf("Trigger kPa: %d | Target kPa: %d\n", activeCrop.trigger_kPa, activeCrop.target_kPa);
      Serial.printf("Temp Range: %dC - %dC\n", activeCrop.minTemp, activeCrop.maxTemp);
      Serial.printf("DLI Range: %d - %d\n", activeCrop.minDLI, activeCrop.maxDLI);
      Serial.println("----------------------------------------");
      return true;
    }
  }
  Serial.println("Warning: Crop '" + cropName + "' not found in database.");
  return false;
}