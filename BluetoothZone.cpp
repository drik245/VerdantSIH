#include "BluetoothZone.h"
#include "Config.h" // Include config to get remote calibration values
#include <BLEDevice.h>
// BLE Service and Characteristic UUIDs. These MUST match the Nano's code.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// This UUID now points to a characteristic holding a RAW INTEGER value
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
// This variable now stores the RAW value from the remote sensor
int remote_raw_value = -1;
// --- CONVERSION LOGIC FOR REMOTE SENSOR ---
// This logic is now executed on the ESP32
const float silty_clay_loam_swcc_bt[][2] = {
{0.50, 1}, {0.46, 10}, {0.42, 30}, {0.38, 80}, {0.35, 200}, {0.31, 500}, {0.25, 1000}, {0.22, 1500}
};
float getVWC_remote(int rawValue) {
float constrainedRaw = constrain(rawValue, SOIL_RAW_SATURATED_REMOTE, SOIL_RAW_DRY_REMOTE);
return map(constrainedRaw, SOIL_RAW_DRY_REMOTE, SOIL_RAW_SATURATED_REMOTE, 0, SOIL_MAX_VWC * 100.0) / 100.0;
}
float getKPaFromVWC_remote(float vwc) {
int tableSize = sizeof(silty_clay_loam_swcc_bt) / sizeof(silty_clay_loam_swcc_bt[0]);
if (vwc >= silty_clay_loam_swcc_bt[0][0]) return silty_clay_loam_swcc_bt[0][1];
if (vwc <= silty_clay_loam_swcc_bt[tableSize - 1][0]) return silty_clay_loam_swcc_bt[tableSize - 1][1];
for (int i = 0; i < tableSize - 1; i++) {
if (vwc <= silty_clay_loam_swcc_bt[i][0] && vwc > silty_clay_loam_swcc_bt[i+1][0]) {
float v1 = silty_clay_loam_swcc_bt[i][0], k1 = silty_clay_loam_swcc_bt[i][1];
float v2 = silty_clay_loam_swcc_bt[i+1][0], k2 = silty_clay_loam_swcc_bt[i+1][1];
return k1 + ((vwc - v1) * (k2 - k1)) / (v2 - v1);
}
}
return -1;
}
// --- END OF CONVERSION LOGIC ---
// Callback function that gets called when the server notifies of a change
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
// We expect an integer. Let's safely copy the bytes into an int variable.
if (length >= sizeof(int)) {
memcpy(&remote_raw_value, pData, sizeof(int));
}
}
// (The rest of the BLE connection functions are the same as before...)
bool connectToServer(BLEAddress pAddress) {
BLEClient* pClient = BLEDevice::createClient();
pClient->connect(pAddress);
BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
if (pRemoteService == nullptr) { return false; }
pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
if (pRemoteCharacteristic == nullptr) { return false; }
if(pRemoteCharacteristic->canNotify())
pRemoteCharacteristic->registerForNotify(notifyCallback);
return true;
}
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
void onResult(BLEAdvertisedDevice advertisedDevice) {
if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
advertisedDevice.getScan()->stop();
if (connectToServer(advertisedDevice.getAddress())) {
Serial.println("We are now connected to the BLE Server.");
connected = true;
}
}
}
};
void setupBluetooth() {
BLEDevice::init("ESP32_Irrigation_Hub");
BLEScan* pBLEScan = BLEDevice::getScan();
pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
pBLEScan->setActiveScan(true);
pBLEScan->start(30, false);
}
void handleBluetoothConnection() { /* Can be used for future reconnect logic */ }
// This public function now does the full conversion before returning the final value
float getRemoteKPa() {
if (remote_raw_value == -1) {
return -1.0; // Return -1 if we haven't received any data yet
}
// Perform the conversion on the fly
float vwc = getVWC_remote(remote_raw_value);
return getKPaFromVWC_remote(vwc);
}