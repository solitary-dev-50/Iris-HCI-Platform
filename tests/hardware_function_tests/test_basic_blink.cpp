#include <Arduino.h>

void setup() {
    // 简单的闪烁测试
    pinMode(2, OUTPUT);  // 板载LED
    
    // 初始化串口
    Serial.begin(115200);
    delay(2000);
    
    // 发送测试信息
    Serial.println("ESP32-S3 Started!");
    Serial.println("LED Blinking Test");
}

void loop() {
    digitalWrite(2, HIGH);
    Serial.println("LED ON");
    delay(1000);
    
    digitalWrite(2, LOW);
    Serial.println("LED OFF");
    delay(1000);
}
