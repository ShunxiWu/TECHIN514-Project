#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Axi Wifi";
const char* password = "2022270124";

const char* serverNameAccl = "http://192.168.4.1/accelerometer";
const char* serverNameGyro = "http://192.168.4.1/gyroscope";
const char* serverNameAttl = "http://192.168.4.1/attitude";

const char* ssid2 = "Axi Wifi Display";
const char* password2 = "18301030091";

float ax_offset = 0, ay_offset = 0;
float rad2deg = 57.29578;
float roll, pitch;
float rawpitch, rawroll; // 新添加的变量

String accelerometer;
String gyroscope;
String attitude;
String selfaccelerometer;
String selfgyroscope;
String selfattitude;

unsigned long previousMillis = 0;
const long interval = 1000; 

Adafruit_MPU6050 mpu;
AsyncWebServer server(80); // 将server定义为全局变量

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  IPAddress local_ip1(192, 168, 5, 1);   // 设备2的SoftAP IP地址
  IPAddress gateway1(192, 168, 5, 1);    // 网关地址
  IPAddress subnet1(255, 255, 255, 0);   // 子网掩码

  WiFi.softAPConfig(local_ip1, gateway1, subnet1);
  WiFi.softAP(ssid2, password2);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (!mpu.begin()) {
    while (1);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
  String response = attitude + "," + selfattitude;
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
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    if(WiFi.status() == WL_CONNECTED) { 
      accelerometer = httpGETRequest(serverNameAccl);
      gyroscope = httpGETRequest(serverNameGyro);
      attitude = httpGETRequest(serverNameAttl);

      sensors_event_t accel, gyro, temp;
      mpu.getEvent(&accel, &gyro, &temp);
      selfaccelerometer = String(accel.acceleration.x) + "," + String(accel.acceleration.y) + "," + String(accel.acceleration.z);
      selfgyroscope = String(gyro.gyro.x) + "," + String(gyro.gyro.y) + "," + String(gyro.gyro.z);
      rawpitch = atan2(accel.acceleration.y, sqrt(accel.acceleration.x * accel.acceleration.x + accel.acceleration.z * accel.acceleration.z));
      rawroll = atan2(-accel.acceleration.x, accel.acceleration.z);
      pitch = rawpitch * rad2deg;
      roll = rawroll * rad2deg;
      selfattitude =  String(pitch) + "," + String(roll);
      
      // Update sensor data for accelerometer and gyroscope
      float accelX = accel.acceleration.x;
      float accelY = accel.acceleration.y;
      float accelZ = accel.acceleration.z;

      float gyroX = gyro.gyro.x;
      float gyroY = gyro.gyro.y;
      float gyroZ = gyro.gyro.z;

      // Serial.println(accelerometer+"," + gyroscope+ "," + attitude+ "," + selfaccelerometer+ "," + selfgyroscope + ","+ selfattitude );  
      Serial.println( attitude + "," + selfattitude );        
      previousMillis = currentMillis;

      // String dataToSend = accelerometer + "," + gyroscope + "," + attitude + "," + selfaccelerometer + "," + selfgyroscope + "," + selfattitude;
      String dataToSend =attitude + "," + selfattitude;
    } else {
      Serial.println("WiFi Disconnected");
    }

    delay(1000);
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode > 0) {
    payload = http.getString();
  }

  http.end();

  return payload;
}
