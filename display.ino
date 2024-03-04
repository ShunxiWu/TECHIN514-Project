#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
const int buttonPin = D8;
int buttonState = 0;   
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Axi Wifi Display";
const char* password = "18301030091";

const char* serverNameAttl = "http://192.168.5.1/data";

unsigned long previousMillis = 0;
const long interval = 1000; 
String attitude;

String httpGETRequest(const char* serverName); 

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.display();
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    if(currentMillis - previousMillis >= interval) {
      // Check WiFi connection status
      if(WiFi.status() == WL_CONNECTED) { 
        attitude = httpGETRequest(serverNameAttl);
        Serial.println(attitude);  
        
        // 分离字符串并将其转换为浮点数
        float imu1_pitch = attitude.substring(0, attitude.indexOf(',')).toFloat();
        attitude.remove(0, attitude.indexOf(',') + 1);
        float imu1_roll = attitude.substring(0, attitude.indexOf(',')).toFloat();
        attitude.remove(0, attitude.indexOf(',') + 1);
        float imu2_pitch = attitude.substring(0, attitude.indexOf(',')).toFloat();
        attitude.remove(0, attitude.indexOf(',') + 1);
        float imu2_roll = attitude.toFloat();
        
        // 在OLED上显示四个数字
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.print("IMU1 Pitch: ");
        display.println(imu1_pitch, 2); // 保留两位小数
        display.print("IMU1 Roll: ");
        display.println(imu1_roll, 2);
        display.print("IMU2 Pitch: ");
        display.println(imu2_pitch, 2);
        display.print("IMU2 Roll: ");
        display.println(imu2_roll, 2);
        display.display();
        
        previousMillis = currentMillis;
      }
    } else {
      Serial.println("WiFi Disconnected");
    }
    delay(1000); // 每1秒发送一次请求
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
