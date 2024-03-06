// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>
// #include <BLE2902.h>

// // BLE server and characteristic UUIDs
// #define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"
// #define CHARACTERISTIC_UUID "cba1d466-344c-4be3-ab3f-189f80dd7518"

// BLEClient*  pClient;
// BLEAddress* pServerAddress;
// BLERemoteCharacteristic* pCharacteristic;
// bool doConnect = false;
// bool connected = false;
// bool doScan = true;

// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//     void onResult(BLEAdvertisedDevice advertisedDevice) {
//       Serial.println("Found a new device");
//       if (advertisedDevice.getName() == "MPU6050_ESP32") {
//           Serial.println("Found your BLE server");
//           advertisedDevice.getScan()->stop();
//           pServerAddress = new BLEAddress(advertisedDevice.getAddress());
//           doConnect = true;
//           doScan = false;
//       }
//     }
// };

// class MyClientCallback : public BLEClientCallbacks {
//     void onConnect(BLEClient* pClient) {
//         connected = true;
//         Serial.println("Connected to BLE server");
//     }

//     void onDisconnect(BLEClient* pClient) {
//         connected = false;
//         Serial.println("Disconnected from BLE server");
//     }
// };

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Starting BLE client");

//   BLEDevice::init("ESP32");

//   BLEScan* pBLEScan = BLEDevice::getScan();
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setActiveScan(true);
//   pBLEScan->start(30);

//   while (doScan) {
//     delay(100);
//   }

//   pClient  = BLEDevice::createClient();
//   pClient->setClientCallbacks(new MyClientCallback());

//   if (doConnect == true) {
//     pClient->connect(*pServerAddress);
//     while (!connected) {
//       delay(100);
//     }
//     BLERemoteService* pRemoteService = pClient->getService(BLEUUID(SERVICE_UUID));
//     if (pRemoteService == nullptr) {
//       Serial.println("Failed to find service UUID");
//       return;
//     }
//     pCharacteristic = pRemoteService->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));
//     if (pCharacteristic == nullptr) {
//       Serial.println("Failed to find characteristic UUID");
//       return;
//     }
//     // You can perform other operations on the characteristic here
//   }
// }

// void loop() {
//   if (connected && pCharacteristic != nullptr) {
//     // Read value from characteristic
//     std::string value = pCharacteristic->readValue();

//     // Print the received value
//     Serial.print("Received value: ");
//     Serial.println(value.c_str());

//     // Parse the received value if necessary
//     // Your parsing code can go here
//   }

//   // Add a delay to avoid flooding the serial port
//   delay(1000);
// }
