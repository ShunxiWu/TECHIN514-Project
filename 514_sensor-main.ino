#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

const char* ssid = "Axi Wifi";
const char* password = "2022270124";

const char* serverNameAccl = "http://192.168.4.1/accelerometer";
const char* serverNameGyro = "http://192.168.4.1/gyroscope";
const char* serverNameAttl = "http://192.168.4.1/attitude";

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

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  // Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    // Serial.println("Connecting to WiFi...");
  }
  
  // Serial.println("Connected to WiFi");
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());

  // Initialize MPU6050
  if (!mpu.begin()) {
    // Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  // Set up motion detection
  // mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  // mpu.setMotionDetectionThreshold(1);
  // mpu.setMotionDetectionDuration(20);
  // mpu.setInterruptPinLatch(true);
  // mpu.setInterruptPinPolarity(true);
  // mpu.setMotionInterrupt(true);
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    // Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED) { 
      accelerometer = httpGETRequest(serverNameAccl);
      gyroscope = httpGETRequest(serverNameGyro);
      attitude = httpGETRequest(serverNameAttl); // 修改为selfAttitude
      Serial.print(getTimeStamp());
      // Serial.println(" - Accelerometer data: " + accelerometer + " Gyroscope data: " + gyroscope + "Attitude data: " + Attitude);

      // Read MPU6050 data
      sensors_event_t accel, gyro, temp;
      mpu.getEvent(&accel, &gyro, &temp);
      selfaccelerometer = String(accel.acceleration.x) + "," + String(accel.acceleration.y) + "," + String(accel.acceleration.z);
      selfgyroscope = String(gyro.gyro.x) + "," + String(gyro.gyro.y) + "," + String(gyro.gyro.z);
      rawpitch = atan2(accel.acceleration.y, sqrt(accel.acceleration.x * accel.acceleration.x + accel.acceleration.z * accel.acceleration.z));
      rawroll = atan2(-accel.acceleration.x, accel.acceleration.z);

      // Convert radians to degrees
      pitch = rawpitch * rad2deg;
      roll = rawroll * rad2deg;
      selfattitude =  String(pitch) + "," + String(roll); // 修改poll为roll

      // Serial.print(getTimeStamp());
      // Serial.println(" -Self Accelerometer data: " + selfAccelerometer + "Self Gyroscope data: " + selfGyroscope + "Self Attitude data:" + selfAttitude);    
      Serial. println(accelerometer+"," + gyroscope+ "," + attitude+ "," + selfaccelerometer+ "," + selfgyroscope + ","+ selfattitude );  
      previousMillis = currentMillis;
    } else {
      Serial.println("WiFi Disconnected");
    }
    delay(1000); // 每5秒发送一次请求
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode > 0) {
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    // Serial.print("Error code: ");
    // Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

String getTimeStamp() {
  unsigned long milliseconds = millis();
  unsigned long seconds = milliseconds / 1000;
  milliseconds = milliseconds % 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  unsigned long hours = minutes / 60;
  minutes = minutes % 60;

  String timeStamp = "";
  timeStamp += hours < 10 ? "0" + String(hours) : String(hours);
  timeStamp += ":";
  timeStamp += minutes < 10 ? "0" + String(minutes) : String(minutes);
  timeStamp += ":";
  timeStamp += seconds < 10 ? "0" + String(seconds) : String(seconds);
  timeStamp += ".";
  timeStamp += milliseconds < 10 ? "00" + String(milliseconds) : (milliseconds < 100 ? "0" + String(milliseconds) : String(milliseconds));

  return timeStamp;
}
