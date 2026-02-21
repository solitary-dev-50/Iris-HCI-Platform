#include <Arduino.h>
#include <unity.h>
#include "BiometricMotion.h"

// ========================================
// 任务 2.2 & 2.3: MotionCurve 测试
// Property 1: 运动曲线实时计算正确性
// Validates: Requirements 12.1, 12.2, 12.4
// ========================================

// 简单的伪随机数生成器（用于属性测试）
float testRandom(float min, float max) {
    static unsigned long seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    float normalized = (float)seed / (float)0x7fffffff;
    return min + normalized * (max - min);
}

// ========================================
// 单元测试（具体示例）
// ========================================

// 单元测试1: 基本边界条件
void test_unit_basic_boundaries() {
    MotionCurve curve;
    
    unsigned long startTime = millis();
    curve.setTarget(0, 100, 1000);
    
    // 起点（刚开始）
    float start = curve.computeNext(startTime);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0, start);
    
    // 终点（1秒后）
    float end = curve.computeNext(startTime + 1000);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 100, end);
    
    // 超过终点
    float after = curve.computeNext(startTime + 1500);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 100, after);
    
    // 中点应该在范围内
    float mid = curve.computeNext(startTime + 500);
    TEST_ASSERT_TRUE(mid > 0 && mid < 100);
}

// 单元测试2: 负向运动
void test_unit_negative_motion() {
    MotionCurve curve;
    
    unsigned long startTime = millis();
    curve.setTarget(100, 0, 1000);
    
    // 起点
    float start = curve.computeNext(startTime);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 100, start);
    
    // 终点
    float end = curve.computeNext(startTime + 1000);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0, end);
    
    // 中点应该在范围内
    float mid = curve.computeNext(startTime + 500);
    TEST_ASSERT_TRUE(mid > 0 && mid < 100);
}

// 单元测试3: 目标切换即时响应
// Validates: Requirements 12.3, 15.5
void test_unit_target_switch() {
    MotionCurve curve;
    
    // 第一个目标
    unsigned long startTime = millis();
    curve.setTarget(0, 100, 1000);
    
    // 运动到一半的值
    float midValue = curve.computeNext(startTime + 500);
    
    // 立即切换新目标（从当前位置到新目标）
    unsigned long switchTime = millis();
    curve.setTarget(midValue, 50, 500);
    
    // 下一次计算应该基于新目标
    float afterSwitch = curve.computeNext(switchTime + 10);
    
    // 验证：新值应该在 midValue 和 50 之间
    float minVal = min(midValue, 50.0f);
    float maxVal = max(midValue, 50.0f);
    
    TEST_ASSERT_TRUE(afterSwitch >= minVal - 1.0f);
    TEST_ASSERT_TRUE(afterSwitch <= maxVal + 1.0f);
}

// 单元测试4: isComplete 检测
void test_unit_is_complete() {
    MotionCurve curve;
    
    unsigned long startTime = millis();
    curve.setTarget(0, 100, 1000);
    
    // 开始时未完成
    TEST_ASSERT_FALSE(curve.isComplete(startTime));
    
    // 中途未完成
    TEST_ASSERT_FALSE(curve.isComplete(startTime + 500));
    
    // 结束时完成
    TEST_ASSERT_TRUE(curve.isComplete(startTime + 1000));
    
    // 超过结束时间仍然完成
    TEST_ASSERT_TRUE(curve.isComplete(startTime + 1500));
}

// 单元测试5: reset 功能
void test_unit_reset() {
    MotionCurve curve;
    
    unsigned long startTime = millis();
    curve.setTarget(0, 100, 1000);
    
    // 运动到一半
    TEST_ASSERT_FALSE(curve.isComplete(startTime + 500));
    
    // 重置
    curve.reset();
    
    // 重置后应该完成
    TEST_ASSERT_TRUE(curve.isComplete(millis()));
}

// ========================================
// 属性测试（100次随机迭代）
// ========================================

// 属性1: 边界条件正确性
// For any MotionCurve, 起点和终点的值必须准确
void test_property_boundaries() {
    Serial.println("\n[Property Test] 边界条件 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        MotionCurve curve;
        
        // 生成随机参数
        float startPos = testRandom(-180, 180);
        float targetPos = testRandom(-180, 180);
        unsigned long duration = (unsigned long)testRandom(100, 2000);
        
        unsigned long startTime = millis();
        curve.setTarget(startPos, targetPos, duration);
        
        // 测试起点
        float resultStart = curve.computeNext(startTime);
        if (abs(resultStart - startPos) > 0.1f) {
            char msg[100];
            sprintf(msg, "Iter %d: Start boundary failed (expected %.2f, got %.2f)", 
                    i, startPos, resultStart);
            TEST_FAIL_MESSAGE(msg);
        }
        
        // 测试终点
        float resultEnd = curve.computeNext(startTime + duration);
        if (abs(resultEnd - targetPos) > 0.1f) {
            char msg[100];
            sprintf(msg, "Iter %d: End boundary failed (expected %.2f, got %.2f)", 
                    i, targetPos, resultEnd);
            TEST_FAIL_MESSAGE(msg);
        }
        
        // 每10次迭代打印一次进度
        if ((i + 1) % 10 == 0) {
            Serial.printf("  完成 %d/100 次迭代\n", i + 1);
        }
    }
    
    TEST_PASS();
}

// 属性2: 中间值范围约束
// For any MotionCurve, 中间值必须在起点和终点之间
void test_property_intermediate_range() {
    Serial.println("\n[Property Test] 中间值范围 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        MotionCurve curve;
        
        float startPos = testRandom(-180, 180);
        float targetPos = testRandom(-180, 180);
        unsigned long duration = (unsigned long)testRandom(500, 2000);
        
        float minPos = min(startPos, targetPos);
        float maxPos = max(startPos, targetPos);
        
        unsigned long startTime = millis();
        curve.setTarget(startPos, targetPos, duration);
        
        // 测试多个中间时刻
        for (int j = 1; j < 10; j++) {
            unsigned long testTime = startTime + (duration * j / 10);
            float result = curve.computeNext(testTime);
            
            if (result < minPos - 0.1f || result > maxPos + 0.1f) {
                char msg[150];
                sprintf(msg, "Iter %d, time %d/10: value %.2f out of range [%.2f, %.2f]", 
                        i, j, result, minPos, maxPos);
                TEST_FAIL_MESSAGE(msg);
            }
        }
        
        if ((i + 1) % 10 == 0) {
            Serial.printf("  完成 %d/100 次迭代\n", i + 1);
        }
    }
    
    TEST_PASS();
}

// 属性3: 单调性
// For any MotionCurve, 曲线必须单调（无回退）
void test_property_monotonic() {
    Serial.println("\n[Property Test] 单调性 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        MotionCurve curve;
        
        float startPos = testRandom(-180, 180);
        float targetPos = testRandom(-180, 180);
        unsigned long duration = (unsigned long)testRandom(500, 2000);
        
        unsigned long startTime = millis();
        curve.setTarget(startPos, targetPos, duration);
        
        float prevValue = startPos;
        bool isIncreasing = (targetPos > startPos);
        
        // 测试整个运动过程（每10ms采样一次）
        for (unsigned long t = 0; t <= duration; t += 10) {
            float currentValue = curve.computeNext(startTime + t);
            
            bool monotonic = isIncreasing ? 
                (currentValue >= prevValue - 0.1f) : 
                (currentValue <= prevValue + 0.1f);
            
            if (!monotonic) {
                char msg[150];
                sprintf(msg, "Iter %d, time %lu: monotonic failed (prev=%.2f, curr=%.2f, %s)", 
                        i, t, prevValue, currentValue, 
                        isIncreasing ? "increasing" : "decreasing");
                TEST_FAIL_MESSAGE(msg);
            }
            
            prevValue = currentValue;
        }
        
        if ((i + 1) % 10 == 0) {
            Serial.printf("  完成 %d/100 次迭代\n", i + 1);
        }
    }
    
    TEST_PASS();
}

// ========================================
// 测试运行器
// ========================================

void setup() {
    delay(2000);  // 等待串口稳定
    
    UNITY_BEGIN();
    
    Serial.println("\n========================================");
    Serial.println("MotionCurve 单元测试 (Unit Tests)");
    Serial.println("========================================");
    
    RUN_TEST(test_unit_basic_boundaries);
    RUN_TEST(test_unit_negative_motion);
    RUN_TEST(test_unit_target_switch);
    RUN_TEST(test_unit_is_complete);
    RUN_TEST(test_unit_reset);
    
    Serial.println("\n========================================");
    Serial.println("MotionCurve 属性测试 (Property Tests)");
    Serial.println("Feature: moss-education-module");
    Serial.println("Property 1: 运动曲线实时计算正确性");
    Serial.println("========================================");
    
    RUN_TEST(test_property_boundaries);
    RUN_TEST(test_property_intermediate_range);
    RUN_TEST(test_property_monotonic);
    
    UNITY_END();
}

void loop() {
    // 测试完成后不需要循环
}
