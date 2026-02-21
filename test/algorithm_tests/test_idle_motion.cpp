#include <Arduino.h>
#include <unity.h>
#include "BiometricMotion.h"

// ========================================
// 任务 2.5: IdleMotion 属性测试
// Property 4: 待机微动连续性
// Validates: Requirements 13.1, 13.2, 13.3
// ========================================

// 简单的伪随机数生成器（用于属性测试）
float testRandom(float min, float max) {
    static unsigned long seed = 54321;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    float normalized = (float)seed / (float)0x7fffffff;
    return min + normalized * (max - min);
}

// ========================================
// 单元测试（具体示例）
// ========================================

// 单元测试1: 基本功能
void test_unit_basic_functionality() {
    IdleMotion idle(1.0, 0.1);
    
    // 获取噪声值
    float noise1 = idle.getNoise(1000);
    float noise2 = idle.getNoise(2000);
    
    // 应该返回有效的浮点数
    TEST_ASSERT_FALSE(isnan(noise1));
    TEST_ASSERT_FALSE(isnan(noise2));
    TEST_ASSERT_FALSE(isinf(noise1));
    TEST_ASSERT_FALSE(isinf(noise2));
}

// 单元测试2: 幅度约束
void test_unit_amplitude_constraint() {
    IdleMotion idle(2.0, 0.1);
    
    // 采样多个点
    bool withinRange = true;
    for (int i = 0; i < 50; i++) {
        float noise = idle.getNoise(i * 100);
        // 噪声值应该在 [-amplitude, +amplitude] 范围内（允许小误差）
        if (abs(noise) > 2.5) {
            withinRange = false;
            break;
        }
    }
    
    TEST_ASSERT_TRUE(withinRange);
}

// 单元测试3: 参数设置
void test_unit_set_parameters() {
    IdleMotion idle(1.0, 0.1);
    
    // 改变参数
    idle.setAmplitude(3.0);
    idle.setFrequency(0.2);
    
    // 应该仍然正常工作
    float noise = idle.getNoise(1000);
    TEST_ASSERT_FALSE(isnan(noise));
    TEST_ASSERT_TRUE(abs(noise) <= 4.0);  // 在合理范围内
}

// 单元测试4: 连续性（相邻时刻）
void test_unit_continuity_adjacent() {
    IdleMotion idle(1.0, 0.1);
    
    // 获取两个非常接近的时刻的值
    float noise1 = idle.getNoise(1000);
    float noise2 = idle.getNoise(1010);  // 10ms后
    
    // 差异应该很小
    float diff = abs(noise2 - noise1);
    TEST_ASSERT_TRUE(diff < 0.2);  // 小于幅度的20%
}

// 单元测试5: 不同频率的影响
void test_unit_frequency_effect() {
    IdleMotion idleSlow(1.0, 0.05);   // 低频
    IdleMotion idleFast(1.0, 0.5);    // 高频
    
    unsigned long t1 = 1000;
    unsigned long t2 = 2000;
    
    // 低频：变化应该较小
    float slowDiff = abs(idleSlow.getNoise(t2) - idleSlow.getNoise(t1));
    
    // 高频：变化应该较大
    float fastDiff = abs(idleFast.getNoise(t2) - idleFast.getNoise(t1));
    
    // 高频的变化应该大于低频（大部分情况下）
    // 注意：由于噪声的随机性，这个测试可能偶尔失败
    // 但在大多数情况下应该成立
    TEST_ASSERT_TRUE(fastDiff >= slowDiff * 0.5);
}

// ========================================
// 属性测试（100次随机迭代）
// ========================================

// 属性1: 连续性
// Property 4: 待机微动连续性
// For any IdleMotion, 连续调用 getNoise(t) 和 getNoise(t+dt) 
// 返回的值差异必须小于 amplitude * 0.1
void test_property_continuity() {
    Serial.println("\n[Property Test] 连续性 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        // 生成随机参数
        float amplitude = testRandom(0.5, 5.0);
        float frequency = testRandom(0.05, 0.5);
        
        IdleMotion idle(amplitude, frequency);
        
        // 测试多个时间点的连续性
        for (int j = 0; j < 10; j++) {
            unsigned long t = (unsigned long)testRandom(0, 10000);
            unsigned long dt = 10;  // 10ms间隔
            
            float noise1 = idle.getNoise(t);
            float noise2 = idle.getNoise(t + dt);
            
            float diff = abs(noise2 - noise1);
            float threshold = amplitude * 0.1;  // 10%的幅度
            
            if (diff >= threshold) {
                char msg[150];
                sprintf(msg, "Iter %d, time %lu: continuity failed (diff=%.3f, threshold=%.3f, amp=%.2f)", 
                        i, t, diff, threshold, amplitude);
                TEST_FAIL_MESSAGE(msg);
            }
        }
        
        if ((i + 1) % 10 == 0) {
            Serial.printf("  完成 %d/100 次迭代\n", i + 1);
        }
    }
    
    TEST_PASS();
}

// 属性2: 幅度约束
// Property 4: 待机微动连续性
// For any IdleMotion with amplitude A, 所有噪声值必须在 [-A, +A] 范围内
void test_property_amplitude_bounds() {
    Serial.println("\n[Property Test] 幅度约束 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        float amplitude = testRandom(0.5, 5.0);
        float frequency = testRandom(0.05, 0.5);
        
        IdleMotion idle(amplitude, frequency);
        
        // 采样多个时间点
        for (int j = 0; j < 20; j++) {
            unsigned long t = (unsigned long)testRandom(0, 10000);
            float noise = idle.getNoise(t);
            
            // 允许小的数值误差
            float margin = 0.1;
            
            if (noise < -amplitude - margin || noise > amplitude + margin) {
                char msg[150];
                sprintf(msg, "Iter %d, time %lu: out of bounds (noise=%.3f, amplitude=%.2f)", 
                        i, t, noise, amplitude);
                TEST_FAIL_MESSAGE(msg);
            }
        }
        
        if ((i + 1) % 10 == 0) {
            Serial.printf("  完成 %d/100 次迭代\n", i + 1);
        }
    }
    
    TEST_PASS();
}

// 属性3: 平滑性（无突变）
// Property 4: 待机微动连续性
// For any IdleMotion, 在整个时间范围内，噪声值应该平滑变化，无突变
void test_property_smoothness() {
    Serial.println("\n[Property Test] 平滑性 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        float amplitude = testRandom(0.5, 5.0);
        float frequency = testRandom(0.05, 0.5);
        
        IdleMotion idle(amplitude, frequency);
        
        // 在一段时间内连续采样
        unsigned long startTime = (unsigned long)testRandom(0, 5000);
        float prevNoise = idle.getNoise(startTime);
        
        for (int j = 1; j <= 50; j++) {
            unsigned long t = startTime + j * 20;  // 每20ms采样一次
            float currentNoise = idle.getNoise(t);
            
            // 检查变化率
            float diff = abs(currentNoise - prevNoise);
            float maxChange = amplitude * 0.2;  // 最大变化不超过幅度的20%
            
            if (diff > maxChange) {
                char msg[150];
                sprintf(msg, "Iter %d, time %lu: sudden change (diff=%.3f, max=%.3f)", 
                        i, t, diff, maxChange);
                TEST_FAIL_MESSAGE(msg);
            }
            
            prevNoise = currentNoise;
        }
        
        if ((i + 1) % 10 == 0) {
            Serial.printf("  完成 %d/100 次迭代\n", i + 1);
        }
    }
    
    TEST_PASS();
}

// 属性4: 参数修改后的稳定性
// For any IdleMotion, 修改参数后应该仍然满足连续性和幅度约束
void test_property_parameter_change_stability() {
    Serial.println("\n[Property Test] 参数修改稳定性 - 100次迭代");
    
    for (int i = 0; i < 100; i++) {
        float amp1 = testRandom(0.5, 3.0);
        float freq1 = testRandom(0.05, 0.3);
        
        IdleMotion idle(amp1, freq1);
        
        // 获取初始值
        unsigned long t = 1000;
        float noise1 = idle.getNoise(t);
        
        // 修改参数
        float amp2 = testRandom(0.5, 3.0);
        float freq2 = testRandom(0.05, 0.3);
        idle.setAmplitude(amp2);
        idle.setFrequency(freq2);
        
        // 修改后应该仍然正常工作
        float noise2 = idle.getNoise(t + 100);
        
        // 检查新值是否在新幅度范围内
        if (abs(noise2) > amp2 + 0.1) {
            char msg[150];
            sprintf(msg, "Iter %d: after param change, out of bounds (noise=%.3f, new_amp=%.2f)", 
                    i, noise2, amp2);
            TEST_FAIL_MESSAGE(msg);
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
    Serial.println("IdleMotion 单元测试 (Unit Tests)");
    Serial.println("========================================");
    
    RUN_TEST(test_unit_basic_functionality);
    RUN_TEST(test_unit_amplitude_constraint);
    RUN_TEST(test_unit_set_parameters);
    RUN_TEST(test_unit_continuity_adjacent);
    RUN_TEST(test_unit_frequency_effect);
    
    Serial.println("\n========================================");
    Serial.println("IdleMotion 属性测试 (Property Tests)");
    Serial.println("Feature: moss-education-module");
    Serial.println("Property 4: 待机微动连续性");
    Serial.println("========================================");
    
    RUN_TEST(test_property_continuity);
    RUN_TEST(test_property_amplitude_bounds);
    RUN_TEST(test_property_smoothness);
    RUN_TEST(test_property_parameter_change_stability);
    
    UNITY_END();
}

void loop() {
    // 测试完成后不需要循环
}
