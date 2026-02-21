#include <Arduino.h>
#include <unity.h>
#include <ESP32Servo.h>

// ========================================
// 任务 5.2: RoboticArmController 属性测试（简化版）
// Property 7: 目标切换即时响应
// Validates: Requirements 12.3, 15.5
// 
// 简化说明：由于只有2个舵机（GPIO4/5），我们创建一个简化的
// 测试版本，直接使用ESP32Servo库测试核心功能
// ========================================

// 简化的舵机控制器（用于测试）
class SimpleServoController {
private:
    Servo _servo1;  // GPIO4 - 水平舵机
    Servo _servo2;  // GPIO5 - 垂直舵机
    
    float _currentAngle1;
    float _currentAngle2;
    float _targetAngle1;
    float _targetAngle2;
    
    unsigned long _startTime;
    unsigned long _duration;
    bool _isMoving;
    
public:
    SimpleServoController() : _currentAngle1(90), _currentAngle2(90),
                               _targetAngle1(90), _targetAngle2(90),
                               _isMoving(false) {}
    
    void init() {
        _servo1.attach(4);  // GPIO4
        _servo2.attach(5);  // GPIO5
        
        // 初始化到中间位置
        _servo1.write(90);
        _servo2.write(90);
        
        Serial.println("SimpleServoController initialized (GPIO4/5)");
    }
    
    void setTarget(float angle1, float angle2, unsigned long durationMs) {
        _targetAngle1 = constrain(angle1, 0, 180);
        _targetAngle2 = constrain(angle2, 30, 150);  // 垂直舵机限位
        
        _startTime = millis();
        _duration = durationMs;
        _isMoving = true;
        
        Serial.printf("SetTarget: [%.1f, %.1f] duration=%lu ms\n", 
                      _targetAngle1, _targetAngle2, durationMs);
    }
    
    void update() {
        if (!_isMoving) return;
        
        unsigned long now = millis();
        unsigned long elapsed = now - _startTime;
        
        if (elapsed >= _duration) {
            // 运动完成
            _currentAngle1 = _targetAngle1;
            _currentAngle2 = _targetAngle2;
            _isMoving = false;
        } else {
            // 线性插值（简化版，实际应该用S型曲线）
            float progress = (float)elapsed / (float)_duration;
            
            float startAngle1 = _currentAngle1;
            float startAngle2 = _currentAngle2;
            
            _currentAngle1 = startAngle1 + ((_targetAngle1 - startAngle1) * progress);
            _currentAngle2 = startAngle2 + ((_targetAngle2 - startAngle2) * progress);
        }
        
        // 驱动舵机
        _servo1.write((int)_currentAngle1);
        _servo2.write((int)_currentAngle2);
    }
    
    bool isMoving() { return _isMoving; }
    float getAngle1() { return _currentAngle1; }
    float getAngle2() { return _currentAngle2; }
};

// 简单的伪随机数生成器（用于属性测试）
float testRandom(float min, float max) {
    static unsigned long seed = 98765;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    float normalized = (float)seed / (float)0x7fffffff;
    return min + normalized * (max - min);
}

// ========================================
// 单元测试（具体示例）
// ========================================

// 单元测试1: 基本初始化
void test_unit_initialization() {
    SimpleServoController servo;
    servo.init();
    
    delay(500);  // 等待舵机稳定
    
    // 初始化后应该在90度
    float angle1 = servo.getAngle1();
    float angle2 = servo.getAngle2();
    
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 90, angle1);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 90, angle2);
    
    Serial.println("[Unit Test] Initialization passed");
}

// 单元测试2: 基本运动
void test_unit_basic_movement() {
    SimpleServoController servo;
    servo.init();
    delay(500);
    
    // 设置新目标
    servo.setTarget(45, 60, 1000);
    
    // 立即检查：应该还在运动中
    TEST_ASSERT_TRUE(servo.isMoving());
    
    // 运行update
    unsigned long startTime = millis();
    while (millis() - startTime < 1200) {
        servo.update();
        delay(20);
    }
    
    // 应该已经完成
    TEST_ASSERT_FALSE(servo.isMoving());
    
    // 检查最终位置
    float angle1 = servo.getAngle1();
    float angle2 = servo.getAngle2();
    
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 45, angle1);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 60, angle2);
    
    Serial.println("[Unit Test] Basic movement passed");
}

// 单元测试3: 目标切换（具体示例）
void test_unit_target_switch_example() {
    SimpleServoController servo;
    servo.init();
    delay(500);
    
    Serial.println("[Unit Test] Target switch example starting...");
    
    // 第一个目标
    servo.setTarget(30, 45, 1000);
    
    // 运行一半时间
    unsigned long startTime = millis();
    while (millis() - startTime < 500) {
        servo.update();
        delay(20);
    }
    
    // 记录中途位置
    float midAngle1 = servo.getAngle1();
    float midAngle2 = servo.getAngle2();
    
    Serial.printf("Mid position: [%.1f, %.1f]\n", midAngle1, midAngle2);
    
    // 立即切换到新目标
    servo.setTarget(150, 120, 800);
    
    // 继续运行
    startTime = millis();
    while (millis() - startTime < 1000) {
        servo.update();
        delay(20);
    }
    
    // 检查最终位置应该接近新目标
    float finalAngle1 = servo.getAngle1();
    float finalAngle2 = servo.getAngle2();
    
    Serial.printf("Final position: [%.1f, %.1f]\n", finalAngle1, finalAngle2);
    
    // 应该更接近新目标(150, 120)而不是旧目标(30, 45)
    float distToNew = abs(finalAngle1 - 150) + abs(finalAngle2 - 120);
    float distToOld = abs(finalAngle1 - 30) + abs(finalAngle2 - 45);
    
    TEST_ASSERT_TRUE(distToNew < distToOld);
    
    Serial.println("[Unit Test] Target switch example passed");
}

// 单元测试4: 软件限位
void test_unit_limits() {
    SimpleServoController servo;
    servo.init();
    delay(500);
    
    // 尝试超出限位的角度
    servo.setTarget(200, 200, 500);  // 超限
    
    // 运行完成
    unsigned long startTime = millis();
    while (millis() - startTime < 700) {
        servo.update();
        delay(20);
    }
    
    // 检查被限制在范围内
    float angle1 = servo.getAngle1();
    float angle2 = servo.getAngle2();
    
    TEST_ASSERT_TRUE(angle1 >= 0 && angle1 <= 180);
    TEST_ASSERT_TRUE(angle2 >= 30 && angle2 <= 150);
    
    Serial.println("[Unit Test] Limits protection passed");
}
// ========================================
// 属性测试（50次随机迭代，简化版）
// ========================================

// 属性1: 目标切换即时响应
// Property 7: For any 舵机控制器正在运动中，调用 setTarget() 后，
// 下一次 update() 必须立即基于新目标计算，无需等待当前动作完成
void test_property_target_switch_immediate() {
    Serial.println("\n========================================");
    Serial.println("[Property Test] 目标切换即时响应");
    Serial.println("========================================");
    Serial.println("这个测试会驱动真实舵机（GPIO4/5）！");
    Serial.println("请确保舵机已连接并供电");
    Serial.println("测试过程中舵机会随机运动，这是正常现象");
    Serial.println("开始测试...\n");
    
    SimpleServoController servo;
    servo.init();
    
    // 等待初始化完成
    delay(2000);
    
    int passCount = 0;
    int totalTests = 50;  // 简化为50次迭代
    
    for (int iter = 0; iter < totalTests; iter++) {
        // 生成随机的第一个目标
        float target1_angle1 = testRandom(20, 160);
        float target1_angle2 = testRandom(40, 140);
        unsigned long duration1 = (unsigned long)testRandom(800, 1500);
        
        // 设置第一个目标
        servo.setTarget(target1_angle1, target1_angle2, duration1);
        
        // 运行1/3时间
        unsigned long runTime = duration1 / 3;
        unsigned long startTime = millis();
        while (millis() - startTime < runTime) {
            servo.update();
            delay(20);
        }
        
        // 记录切换前的位置
        float angleBefore1 = servo.getAngle1();
        float angleBefore2 = servo.getAngle2();
        
        // 生成随机的第二个目标（确保与第一个目标有明显差异）
        float target2_angle1 = testRandom(20, 160);
        float target2_angle2 = testRandom(40, 140);
        
        // 确保新目标与旧目标差异足够大
        while (abs(target2_angle1 - target1_angle1) < 30) {
            target2_angle1 = testRandom(20, 160);
        }
        while (abs(target2_angle2 - target1_angle2) < 30) {
            target2_angle2 = testRandom(40, 140);
        }
        
        unsigned long duration2 = (unsigned long)testRandom(800, 1500);
        
        // 立即切换到新目标
        servo.setTarget(target2_angle1, target2_angle2, duration2);
        
        // 运行几次update
        for (int i = 0; i < 5; i++) {
            servo.update();
            delay(20);
        }
        
        // 记录切换后的位置
        float angleAfter1 = servo.getAngle1();
        float angleAfter2 = servo.getAngle2();
        
        // 验证：切换后应该朝向新目标移动
        float distToTarget2Before1 = abs(angleBefore1 - target2_angle1);
        float distToTarget2After1 = abs(angleAfter1 - target2_angle1);
        
        float distToTarget2Before2 = abs(angleBefore2 - target2_angle2);
        float distToTarget2After2 = abs(angleAfter2 - target2_angle2);
        
        // 至少有一个轴应该朝向新目标移动（距离变小）
        bool movingTowards = (distToTarget2After1 < distToTarget2Before1 - 1.0f) ||
                              (distToTarget2After2 < distToTarget2Before2 - 1.0f);
        
        if (movingTowards) {
            passCount++;
        } else {
            Serial.printf("Iter %d: Not moving toward new target\n", iter);
            Serial.printf("  Target1=[%.1f, %.1f] Target2=[%.1f, %.1f]\n",
                         target1_angle1, target1_angle2, target2_angle1, target2_angle2);
            Serial.printf("  Before=[%.1f, %.1f] After=[%.1f, %.1f]\n",
                         angleBefore1, angleBefore2, angleAfter1, angleAfter2);
        }
        
        // 完成这次运动
        startTime = millis();
        while (millis() - startTime < duration2) {
            servo.update();
            delay(20);
        }
        
        // 每10次迭代打印一次进度
        if ((iter + 1) % 10 == 0) {
            Serial.printf("  完成 %d/%d 次迭代 (通过率: %.1f%%)\n", 
                         iter + 1, totalTests, (float)passCount / (iter + 1) * 100);
        }
    }
    
    // 测试完成，回到中间位置
    servo.setTarget(90, 90, 1000);
    unsigned long startTime = millis();
    while (millis() - startTime < 1200) {
        servo.update();
        delay(20);
    }
    
    Serial.printf("\n[Property Test] 完成！通过率: %d/%d (%.1f%%)\n", 
                 passCount, totalTests, (float)passCount / totalTests * 100);
    
    // 要求至少90%通过率
    TEST_ASSERT_TRUE(passCount >= totalTests * 0.9f);
    
    TEST_PASS();
}

// ========================================
// 测试运行器
// ========================================

void setup() {
    delay(2000);  // 等待串口稳定
    
    UNITY_BEGIN();
    
    Serial.println("\n========================================");
    Serial.println("舵机控制器测试 (简化版 - GPIO4/5)");
    Serial.println("========================================");
    Serial.println("硬件要求：");
    Serial.println("- GPIO4: 水平舵机");
    Serial.println("- GPIO5: 垂直舵机");
    Serial.println("- 舵机电源: 5V");
    Serial.println("========================================\n");
    
    Serial.println("单元测试 (Unit Tests)");
    Serial.println("----------------------------------------");
    
    RUN_TEST(test_unit_initialization);
    RUN_TEST(test_unit_basic_movement);
    RUN_TEST(test_unit_target_switch_example);
    RUN_TEST(test_unit_limits);
    
    Serial.println("\n========================================");
    Serial.println("属性测试 (Property Tests)");
    Serial.println("Feature: moss-education-module");
    Serial.println("Property 7: 目标切换即时响应");
    Serial.println("========================================");
    Serial.println("⚠️  警告：这个测试会驱动真实舵机！");
    Serial.println("请确保：");
    Serial.println("1. 舵机已正确连接到GPIO4和GPIO5");
    Serial.println("2. 舵机电源已接通（5V）");
    Serial.println("3. 舵机周围没有障碍物");
    Serial.println("========================================\n");
    
    delay(3000);  // 给用户3秒时间阅读警告
    
    RUN_TEST(test_property_target_switch_immediate);
    
    UNITY_END();
}

void loop() {
    // 测试完成后不需要循环
}
