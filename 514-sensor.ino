#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

const char* ssid = "Axi Wifi";
const char* password = "2022270124";

float ax_offset = 0, ay_offset = 0;
float rad2deg = 57.29578;
float roll, pitch;
float rawpitch, rawroll; // 新添加的变量

Adafruit_MPU6050 mpu;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  server.on("/accelerometer", HTTP_GET, [](AsyncWebServerRequest *request) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    String response = String(accel.acceleration.x) + "," + String(accel.acceleration.y) + "," + String(accel.acceleration.z);
    request->send(200, "text/plain", response);
  });

  server.on("/gyroscope", HTTP_GET, [](AsyncWebServerRequest *request) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    String response = String(gyro.gyro.x) + "," + String(gyro.gyro.y) + "," + String(gyro.gyro.z);
    request->send(200, "text/plain", response);
  });

  server.on("/attitude", HTTP_GET, [](AsyncWebServerRequest *request) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    // Calculate pitch and roll angles
    rawpitch = atan2(accel.acceleration.y, sqrt(accel.acceleration.x * accel.acceleration.x + accel.acceleration.z * accel.acceleration.z));
    rawroll = atan2(-accel.acceleration.x, accel.acceleration.z);

    // Convert radians to degrees
    pitch = rawpitch * rad2deg;
    roll = rawroll * rad2deg;

    String response = String(pitch) + "," + String(roll);
    request->send(200, "text/plain", response);
  });

  // Calibration
  for (int i = 0; i < 2000; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    ax_offset += a.acceleration.x;
    ay_offset += a.acceleration.y;
  }
  ax_offset /= 2000;
  ay_offset /= 2000;

  server.begin();
}

void loop() {
  // Do any other tasks if needed
  delay(100); // Adjust delay as needed

  // Serve MPU6050 data via HTTP server
  if (mpu.getMotionInterruptStatus()) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    // Calculate pitch and roll angles
    rawpitch = atan2(accel.acceleration.y, sqrt(accel.acceleration.x * accel.acceleration.x + accel.acceleration.z * accel.acceleration.z));
    rawroll = atan2(-accel.acceleration.x, accel.acceleration.z);

    // Convert radians to degrees
    pitch = rawpitch * rad2deg;
    roll = rawroll * rad2deg;

    // Update sensor data for accelerometer and gyroscope
    float accelX = accel.acceleration.x;
    float accelY = accel.acceleration.y;
    float accelZ = accel.acceleration.z;

    float gyroX = gyro.gyro.x;
    float gyroY = gyro.gyro.y;
    float gyroZ = gyro.gyro.z;
  }
}
