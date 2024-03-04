#include <Stepper.h>

const int stepsPerRevolution = 360;  // 电机每一圈的步数
const int minAngle = -20;  // 角度范围的最小值
const int maxAngle = 200;  // 角度范围的最大值

// 初始化步进电机，连接到引脚 D0, D1, D2, D3
Stepper myStepper(stepsPerRevolution, D0, D1, D2, D3);

int step0 = 0; // 用于存储前一步的步数

void setup() {
  randomSeed(analogRead(0));
  // 设置电机速度
  myStepper.setSpeed(100);  // 设置步进电机速度为100步/分钟

  // 将电机回到初始位置
  myStepper.step(-360); // 向相反方向移动360步
  myStepper.step(70);   // 再移动70步以到达初始位置

  // 初始化 step0 为 0
  step0 = 0;

  delay(1000); 
  // 启动串口通信，并设置波特率为9600
  Serial.begin(9600);
}

void loop() {
  // 生成随机角度
  int randomAngle = random(minAngle, maxAngle + 1);
  Serial.println(randomAngle);
  if (randomAngle > 180) {
    randomAngle = 200;
  } 
  if (randomAngle < 0) {
    randomAngle = -20;
  } 
  // 将角度转换为步数，并确保在有效范围内
  int step2 = map(randomAngle, minAngle, maxAngle, 0, stepsPerRevolution);
  
  // 旋转电机到指定位置
  myStepper.step(step2 - step0);  

  // 更新前一步的步数
  step0 = step2;

  delay(10000); // 延迟10秒，可以根据需要调整
}
