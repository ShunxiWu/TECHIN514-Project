#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <wushunxi-project-1_inferencing.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#include <Stepper.h>

const int stepsPerRevolution = 360;  // 电机每一圈的步数
// const int minAngle = -20;  // 角度范围的最小值
// const int maxAngle = 200;  // 角度范围的最大值

// 初始化步进电机，连接到引脚 D0, D1, D2, D3
Stepper myStepper(stepsPerRevolution, D0, D1, D2, D3);

int step0 = 0; // 用于存储前一步的步数
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
// 设置用于推断的模型输入特征数组
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
size_t feature_ix = 0;


// 辅助函数：将 raw feature 数据拷贝到 out_ptr
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void print_inference_result(ei_impulse_result_t result);

void setup() {
    Serial.begin(115200);
    Serial.println("Connected to WiFi.");
    myStepper.setSpeed(100);  // 设置步进电机速度为100步/分钟

    // 将电机回到初始位置
    myStepper.step(-360); // 向相反方向移动360步
    myStepper.step(70);   // 再移动70步以到达初始位置

    // 初始化 step0 为 0
    step0 = 0;

    Serial.println("Serial port initialized.");
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.println("Button pin mode set.");
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    Serial.println("Display initialized.");
    delay(2000);
    display.clearDisplay();
    display.display();
    Serial.println("Display cleared and updated.");
    
    WiFi.begin(ssid, password);
    Serial.println("Attempting to connect to WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    delay(1000); 
}

void loop() {
    unsigned long currentMillis = millis();
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        if(currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            
            if(WiFi.status() == WL_CONNECTED) { 
                attitude = httpGETRequest(serverNameAttl);
                Serial.println("HTTP request sent.");  
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
                
                // 设置四个输入特征
                features[0] = imu1_pitch;
                features[1] = imu1_roll;
                features[2] = imu2_pitch;
                features[3] = imu2_roll;
                
                ei_impulse_result_t result;
                signal_t signal;
                
                // create signal from features frame
                numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
                
                // run classifier
                EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
                ei_printf("run_classifier returned: %d\n", res);
                
                if (res == 0) {
                    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
                              result.timing.dsp, result.timing.classification, result.timing.anomaly);
                    // print the predictions
                    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
                        ei_printf("%s:\t%.5f\n", result.classification[ix].label, result.classification[ix].value);
                        display.print("Predicted Degree: ");
                        display.println(); // 添加这一行来强制换行                        
                        display.setTextSize(2); // 设置字体大小为两倍
                        display.println(result.classification[ix].value, 2);
                        // 将浮点数值转换为整数
                        int motorPosition = int(result.classification[ix].value);
                        
                        // 将整数值限制在电机接受的范围内
                        if (motorPosition > 180) {
                            motorPosition = 200; // 如果超过最大角度范围，将其设为最大值
                        } 
                        if (motorPosition < 0) {
                            motorPosition = -20; // 如果小于最小角度范围，将其设为最小值
                        } 
                        
                        // 将角度转换为步数，并确保在有效范围内
                        int step2 = map(motorPosition, -20, 200, 0, stepsPerRevolution);

                        
                        // 旋转电机到指定位置
                        myStepper.step(step2 - step0);  

                        // 更新前一步的步数
                        step0 = step2;
                    }
                    
                #if EI_CLASSIFIER_HAS_ANOMALY == 1
                    ei_printf("anomaly:\t%.3f\n", result.anomaly);
                #endif
                }
                
                display.display(); // 更新 OLED 显示
                Serial.println("Display updated.");
                
                // 重置特征索引
                feature_ix = 0;
            } else {
                Serial.println("WiFi Disconnected");
            }
        }
    }
    // delay(1000); // 延迟一秒后继续下一次推断
}

void ei_printf(const char *format, ...) {
    static char print_buf[1024] = { 0 };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0) {
        Serial.write(print_buf);
    }
}

String httpGETRequest(const char* serverName) {
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, serverName);
    Serial.println("HTTP request started.");
    
    int httpResponseCode = http.GET();
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
    
    String payload = "--"; 
    
    if (httpResponseCode > 0) {
        payload = http.getString();
        Serial.println("HTTP request successful.");
    } else {
        Serial.println("HTTP request failed.");
    }
    
    http.end();
    
    return payload;
}
