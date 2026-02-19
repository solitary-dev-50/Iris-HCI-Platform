#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
// 使用Arduino兼容的旧版I2S API
#include <driver/i2s.h>

/**
 * MOSS 综合联动测试程序 - I2S数字麦克风版本
 * 
 * 硬件配置：
 * - 麦克风：SPH0645 I2S MEMS（左对齐格式）
 * - 左麦克风：SEL接GND（左声道）
 * - 右麦克风：SEL接3.3V（右声道）
 * - 共用引脚：SCK=GPIO13, WS=GPIO7, SD=GPIO12
 * - LED：GPIO48控制5个WS2812B（串联）
 */

// ========== OLED 配置 ==========
#define I2C_SDA 2
#define I2C_SCL 1

// 使用 U8g2 库（支持中文）
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

// ========== LED 配置 ==========
#define LED_PIN         48  // 所有LED共用一个引脚
#define LED_COUNT       5   // 总共5个LED（2个摄像头 + 3个机身）
#define LED_CAMERA_START 0  // 摄像头LED起始索引（瞳孔）
#define LED_CAMERA_COUNT 2  // 摄像头LED数量
#define LED_BODY_START   2  // 机身LED起始索引
#define LED_BODY_COUNT   3  // 机身LED数量

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// ========== 舵机配置 ==========
#define SERVO_PIN_HORIZONTAL  4
#define SERVO_PIN_VERTICAL    5

Servo servoH;
Servo servoV;

int angleH = 90;
int angleV = 90;

// ========== 麦克风配置 ==========
// I2S数字麦克风（INMP441或类似）- 立体声配置
#define I2S_PORT        I2S_NUM_0
#define I2S_SCK_PIN     13  // 串行时钟（两个麦克风共用）
#define I2S_WS_PIN      7   // 字选择（两个麦克风共用，区分左右声道）
#define I2S_SD_PIN      12  // 数据线（两个麦克风共用，分时复用）
#define SAMPLE_RATE     16000
#define BUFFER_SIZE     512

// 音频缓冲区
int32_t audioBuffer[BUFFER_SIZE * 2];  // 立体声需要双倍大小
size_t bytesRead = 0;

// 麦克风间距（单位：米）
const float MIC_DISTANCE = 0.07;  // 7cm
const float SOUND_SPEED = 343.0;  // 声速 m/s

// ========== 喇叭配置 ==========
#define SPK_LCK     9
#define SPK_BCK     10
#define SPK_DOUT    11

// ========== 系统状态 ==========
enum SystemState {
    STATE_IDLE,      // 待机
    STATE_LISTENING, // 监听中
    STATE_ACTIVE,    // 活跃（检测到声音）
    STATE_SPEAKING   // 说话中
};

SystemState currentState = STATE_IDLE;
unsigned long lastSoundTime = 0;
unsigned long stateStartTime = 0;

// 音量阈值（固定低阈值）
const float TRIGGER_THRESHOLD = 100;  // 固定阈值90
float currentVolume = 0;

// ========== 颜色定义 ==========
// 机身LED状态颜色（索引2-4）
uint32_t COLOR_BOOT = leds.Color(128, 0, 128);      // 紫色（启动）
uint32_t COLOR_IDLE = leds.Color(0, 50, 100);       // 蓝色（待机）
uint32_t COLOR_LISTENING = leds.Color(0, 255, 0);   // 绿色（监听）
uint32_t COLOR_ACTIVE = leds.Color(255, 100, 0);    // 橙色（活跃）
uint32_t COLOR_SPEAKING = leds.Color(255, 0, 255);  // 紫色（说话）

// 摄像头LED（瞳孔，索引0-1）颜色
uint32_t COLOR_EYE_OFF = leds.Color(0, 0, 0);       // 关闭
uint32_t COLOR_EYE_DIM = leds.Color(50, 0, 0);      // 暗红（待机/监听）
uint32_t COLOR_EYE_FOCUS = leds.Color(255, 0, 0);   // 亮红（聚焦/活跃）

// ========== 初始化函数 ==========

void setupDisplay() {
    Serial.println("[INIT] 初始化显示屏...");
    
    display.begin();
    display.clearBuffer();
    
    // 显示启动信息（使用英文避免编码问题）
    display.setFont(u8g2_font_ncenB14_tr);  // 英文字体
    display.setCursor(30, 30);
    display.print("MOSS");
    
    display.setFont(u8g2_font_ncenB08_tr);
    display.setCursor(20, 50);
    display.print("Starting...");
    
    display.sendBuffer();
    
    Serial.println("[INIT] ✓ 显示屏初始化成功");
}

void setupLEDs() {
    Serial.println("[INIT] 初始化LED（5个LED串联）...");
    
    leds.begin();
    leds.setBrightness(128);
    leds.show();
    
    Serial.println("[INIT] ✓ LED初始化成功（GPIO48控制5个LED）");
}

void setupServos() {
    Serial.println("[INIT] 初始化舵机...");
    
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    
    servoH.setPeriodHertz(50);
    servoH.attach(SERVO_PIN_HORIZONTAL, 500, 2500);
    
    servoV.setPeriodHertz(50);
    servoV.attach(SERVO_PIN_VERTICAL, 500, 2500);
    
    servoH.write(angleH);
    servoV.write(angleV);
    
    Serial.println("[INIT] ✓ 舵机初始化成功");
}

void setupMicrophone() {
    Serial.println("[INIT] 初始化I2S数字麦克风（立体声）...");
    Serial.println("[INFO] 麦克风型号：SPH0645（左对齐格式）");
    
    // 配置I2S - 使用MSB左对齐格式（SPH0645专用）
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,  // SPH0645输出32位
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  // 立体声
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,  // 关键：MSB左对齐格式
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 256,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };
    
    esp_err_t ret = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (ret != ESP_OK) {
        Serial.printf("[ERROR] I2S驱动安装失败: %d\n", ret);
        return;
    }
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK_PIN,      // GPIO13: 串行时钟
        .ws_io_num = I2S_WS_PIN,        // GPIO7:  字选择（左右声道）
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD_PIN       // GPIO12: 数据线（共用）
    };
    
    ret = i2s_set_pin(I2S_PORT, &pin_config);
    if (ret != ESP_OK) {
        Serial.printf("[ERROR] I2S引脚配置失败: %d\n", ret);
        return;
    }
    
    Serial.println("[INIT] ✓ I2S麦克风初始化成功");
    Serial.printf("[INFO] 采样率: %d Hz\n", SAMPLE_RATE);
    Serial.println("[INFO] 引脚: SCK=GPIO13, WS=GPIO7, SD=GPIO12");
    Serial.println("[INFO] 模式: I2S MSB左对齐（SPH0645专用）");
    Serial.println("[INFO] 声源定位: 立体声音量差异");
    Serial.println();
    Serial.println("[IMPORTANT] SPH0645接线：");
    Serial.println("  左麦克风：SEL接GND");
    Serial.println("  右麦克风：SEL接3.3V");
}

// ========== 功能函数 ==========

void setAllLEDs(uint32_t color) {
    for (int i = 0; i < LED_COUNT; i++) {
        leds.setPixelColor(i, color);
    }
    leds.show();
}

// 只设置摄像头LED（瞳孔，索引0-1）
void setEyeLEDs(uint32_t color) {
    for (int i = LED_CAMERA_START; i < LED_CAMERA_START + LED_CAMERA_COUNT; i++) {
        leds.setPixelColor(i, color);
    }
    leds.show();
}

// 只设置机身LED（状态灯环，索引2-4）
void setBodyLEDs(uint32_t color) {
    for (int i = LED_BODY_START; i < LED_BODY_START + LED_BODY_COUNT; i++) {
        leds.setPixelColor(i, color);
    }
    leds.show();
}

// 同时设置机身和瞳孔LED（避免两次show()调用）
void setBodyAndEyeLEDs(uint32_t bodyColor, uint32_t eyeColor) {
    for (int i = LED_BODY_START; i < LED_BODY_START + LED_BODY_COUNT; i++) {
        leds.setPixelColor(i, bodyColor);
    }
    for (int i = LED_CAMERA_START; i < LED_CAMERA_START + LED_CAMERA_COUNT; i++) {
        leds.setPixelColor(i, eyeColor);
    }
    leds.show();
}

// 瞳孔呼吸效果（模拟注意力集中）
void eyeBreathEffect() {
    static unsigned long lastUpdate = 0;
    static int brightness = 50;
    static int direction = 1;
    
    if (millis() - lastUpdate > 10) {
        brightness += direction * 5;
        
        if (brightness >= 255) {
            brightness = 255;
            direction = -1;
        } else if (brightness <= 50) {
            brightness = 50;
            direction = 1;
        }
        
        uint32_t color = leds.Color(brightness, 0, 0);
        setEyeLEDs(color);
        lastUpdate = millis();
    }
}

void updateDisplay(const char* status, float volume) {
    display.clearBuffer();
    
    // 标题（大字体）
    display.setFont(u8g2_font_ncenB14_tr);
    display.setCursor(30, 20);
    display.print("MOSS");
    
    // 状态
    display.setFont(u8g2_font_ncenB08_tr);
    display.setCursor(0, 35);
    display.print("State: ");
    display.print(status);
    
    // 音量
    display.setCursor(0, 50);
    display.print("Vol: ");
    display.print((int)volume);
    
    // 音量条
    int barWidth = (int)(volume / 2);
    barWidth = constrain(barWidth, 0, 128);
    display.drawBox(0, 54, barWidth, 10);
    
    display.sendBuffer();
}

float getVolume() {
    esp_err_t ret = i2s_read(I2S_PORT, audioBuffer, sizeof(audioBuffer), &bytesRead, pdMS_TO_TICKS(10));
    
    int32_t peak = 0;
    
    if (ret == ESP_OK && bytesRead > 0) {
        size_t samplesRead = bytesRead / sizeof(int32_t);
        for (size_t i = 0; i < samplesRead; i++) {
            // I2S 32位数据，取高16位
            int32_t sample = abs(audioBuffer[i] >> 16);
            if (sample > peak) {
                peak = sample;
            }
        }
    }
    
    return (float)peak;
}

// 立体声声源定位：通过左右声道音量差异判断方向
float getSoundDirection(float* leftVol, float* rightVol) {
    esp_err_t ret = i2s_read(I2S_PORT, audioBuffer, sizeof(audioBuffer), &bytesRead, pdMS_TO_TICKS(10));
    
    if (ret != ESP_OK || bytesRead == 0) {
        *leftVol = 0;
        *rightVol = 0;
        return 0;
    }
    
    size_t samplesRead = bytesRead / sizeof(int32_t);
    
    float leftPeak = 0;
    float rightPeak = 0;
    
    // I2S立体声数据格式：左, 右, 左, 右...
    for (size_t i = 0; i < samplesRead; i += 2) {
        // 左声道（偶数索引）
        int32_t leftSample = abs(audioBuffer[i] >> 16);  // 取高16位
        if (leftSample > leftPeak) {
            leftPeak = leftSample;
        }
        
        // 右声道（奇数索引）
        if (i + 1 < samplesRead) {
            int32_t rightSample = abs(audioBuffer[i + 1] >> 16);
            if (rightSample > rightPeak) {
                rightPeak = rightSample;
            }
        }
    }
    
    *leftVol = leftPeak;
    *rightVol = rightPeak;
    
    // 调试输出
    static unsigned long lastDebugPrint = 0;
    if (millis() - lastDebugPrint > 1000) {
        Serial.printf("[DEBUG] 左麦: %.0f, 右麦: %.0f, 差异: %.1f%%\n", 
                      leftPeak, rightPeak, 
                      (leftPeak + rightPeak > 0) ? (rightPeak - leftPeak) / (leftPeak + rightPeak) * 100 : 0);
        lastDebugPrint = millis();
    }
    
    float totalVolume = leftPeak + rightPeak;
    if (totalVolume < 100) {
        return 0;
    }
    
    // 计算音量差异比例
    float volumeDiff = (rightPeak - leftPeak) / totalVolume;
    
    // 转换为角度（-90到+90度）
    float angle = volumeDiff * 90.0;
    
    return angle;
}

void smoothMove(int targetH, int targetV, int delayMs = 10) {
    int stepsH = abs(targetH - angleH);
    int stepsV = abs(targetV - angleV);
    int maxSteps = max(stepsH, stepsV);
    
    if (maxSteps == 0) return;
    
    for (int step = 0; step <= maxSteps; step++) {
        int newH = angleH + (targetH - angleH) * step / maxSteps;
        int newV = angleV + (targetV - angleV) * step / maxSteps;
        
        servoH.write(newH);
        servoV.write(newV);
        
        delay(delayMs);
    }
    
    angleH = targetH;
    angleV = targetV;
}

// ========== 状态处理 ==========

void handleIdleState() {
    // 待机状态：机身LED蓝色呼吸 + 瞳孔暗红常亮 + 微动
    static unsigned long lastBreath = 0;
    static int brightness = 50;
    static int direction = 1;
    
    if (millis() - lastBreath > 20) {
        brightness += direction * 5;
        
        if (brightness >= 255) {
            brightness = 255;
            direction = -1;
        } else if (brightness <= 50) {
            brightness = 50;
            direction = 1;
        }
        
        // 机身LED：蓝色呼吸（按比例缩放RGB）
        uint32_t bodyColor = leds.Color(0, brightness * 50 / 255, brightness * 100 / 255);
        
        // 分别设置机身和瞳孔
        for (int i = LED_BODY_START; i < LED_BODY_START + LED_BODY_COUNT; i++) {
            leds.setPixelColor(i, bodyColor);
        }
        for (int i = LED_CAMERA_START; i < LED_CAMERA_START + LED_CAMERA_COUNT; i++) {
            leds.setPixelColor(i, COLOR_EYE_DIM);
        }
        
        leds.show();
        lastBreath = millis();
    }
    
    // 微动
    static unsigned long lastMove = 0;
    if (millis() - lastMove > 3000) {
        int randomH = angleH + random(-5, 6);
        int randomV = angleV + random(-3, 4);
        
        randomH = constrain(randomH, 80, 100);
        randomV = constrain(randomV, 85, 95);
        
        smoothMove(randomH, randomV, 20);
        lastMove = millis();
    }
    
    updateDisplay("IDLE", 0);
}

void handleListeningState() {
    // 监听状态：机身LED绿色常亮 + 瞳孔暗红
    static bool ledSet = false;
    if (!ledSet) {
        setBodyAndEyeLEDs(COLOR_LISTENING, COLOR_EYE_DIM);
        ledSet = true;
    }
    
    float volume = getVolume();
    currentVolume = volume;
    
    // 调试：显示左右声道音量
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 500) {  // 每500ms输出一次
        float leftVol, rightVol;
        getSoundDirection(&leftVol, &rightVol);
        
        // 强制输出，即使音量为0
        Serial.printf("[DEBUG] 音量: %.0f, 左: %.0f, 右: %.0f, 差异: %.1f%%\n", 
                     volume, leftVol, rightVol, 
                     (leftVol + rightVol > 0) ? (rightVol - leftVol) / (leftVol + rightVol) * 100 : 0);
        
        lastDebug = millis();
    }
    
    updateDisplay("LISTEN", volume);
    
    // 检测声音（固定阈值90，正常说话即可触发）
    if (volume > TRIGGER_THRESHOLD) {
        currentState = STATE_ACTIVE;
        stateStartTime = millis();
        lastSoundTime = millis();
        ledSet = false;  // 重置LED标志
        
        Serial.printf("[STATE] 检测到声音！峰值: %.0f (阈值: %.0f)\n", volume, TRIGGER_THRESHOLD);
    }
}

void handleActiveState() {
    // 活跃状态：机身LED橙色常亮 + 瞳孔呼吸（模拟注意力集中）
    static bool bodyLedSet = false;
    if (!bodyLedSet) {
        setBodyLEDs(COLOR_ACTIVE);
        bodyLedSet = true;
    }
    eyeBreathEffect();  // 瞳孔呼吸
    
    float volume = getVolume();
    updateDisplay("ACTIVE!", volume);
    
    // 【修改】持续检测声源方向，不只是转一次
    static unsigned long lastDirectionCheck = 0;
    if (millis() - lastDirectionCheck > 200) {  // 每200ms检测一次
        float leftVol, rightVol;
        float direction = getSoundDirection(&leftVol, &rightVol);
        
        // 持续输出调试信息
        Serial.printf("[ACTIVE] 左: %.0f, 右: %.0f, 差异: %.1f%%, 角度: %.1f°\n", 
                     leftVol, rightVol, 
                     (leftVol + rightVol > 0) ? (rightVol - leftVol) / (leftVol + rightVol) * 100 : 0,
                     direction);
        
        lastDirectionCheck = millis();
    }
    
    // 声源定位并转向（只在刚进入状态时转一次）
    static bool turned = false;
    if (!turned) {
        float leftVol, rightVol;
        float direction = getSoundDirection(&leftVol, &rightVol);
        
        Serial.printf("[LOCATE] 左声道: %.0f, 右声道: %.0f\n", leftVol, rightVol);
        Serial.printf("[LOCATE] 音量差异: %.1f%%, 计算角度: %.1f°\n", 
                     (rightVol - leftVol) / (leftVol + rightVol) * 100, direction);
        
        int targetH = 90 + (int)direction;
        targetH = constrain(targetH, 30, 150);
        int targetV = 90;
        
        smoothMove(targetH, targetV, 5);
        turned = true;
        
        Serial.printf("[LOCATE] 舵机转向: H=%d° (中心90°)\n", targetH);
    }
    
    // 持续检测声音（固定阈值）
    if (volume > TRIGGER_THRESHOLD) {
        lastSoundTime = millis();
    }
    
    // 3秒无声音，回到监听
    if (millis() - lastSoundTime > 3000) {
        currentState = STATE_LISTENING;
        turned = false;
        bodyLedSet = false;  // 重置LED标志
        smoothMove(90, 90, 10);
        Serial.println("[STATE] 回到监听状态");
    }
}

// ========== LED索引测试 ==========

void testLEDMapping() {
    Serial.println("\n[TEST] LED索引映射测试");
    Serial.println("[TEST] 逐个点亮LED，观察实际位置...\n");
    
    // 关闭所有LED
    setAllLEDs(leds.Color(0, 0, 0));
    delay(1000);
    
    // 逐个点亮，用不同颜色标识
    for (int i = 0; i < LED_COUNT; i++) {
        Serial.printf("[TEST] 点亮索引 %d - ", i);
        
        // 根据索引显示预期位置
        if (i < 2) {
            Serial.println("预期：瞳孔LED");
        } else {
            Serial.println("预期：机身LED");
        }
        
        // 关闭所有
        setAllLEDs(leds.Color(0, 0, 0));
        delay(200);
        
        // 只点亮当前索引（白色）
        leds.setPixelColor(i, leds.Color(255, 255, 255));
        leds.show();
        
        delay(2000);  // 停留2秒观察
    }
    
    // 关闭所有
    setAllLEDs(leds.Color(0, 0, 0));
    delay(500);
    
    Serial.println("\n[TEST] 请根据实际观察结果调整索引定义");
    Serial.println("[TEST] 按任意键继续...\n");
    
    // 等待用户输入
    while (!Serial.available()) {
        delay(100);
    }
    while (Serial.available()) {
        Serial.read();
    }
}

// ========== 演示模式 ==========

void demoMode() {
    Serial.println("[DEMO] 开始演示...");
    
    // 1. 待机状态
    Serial.println("[DEMO] 1. 待机状态（5秒）");
    currentState = STATE_IDLE;
    unsigned long start = millis();
    while (millis() - start < 5000) {
        handleIdleState();
        delay(10);
    }
    
    // 2. 监听状态
    Serial.println("[DEMO] 2. 监听状态（3秒）");
    currentState = STATE_LISTENING;
    start = millis();
    while (millis() - start < 3000) {
        handleListeningState();
        delay(10);
    }
    
    // 3. 活跃状态（模拟）
    Serial.println("[DEMO] 3. 活跃状态（转向）");
    currentState = STATE_ACTIVE;
    lastSoundTime = millis();
    start = millis();
    while (millis() - start < 5000) {
        handleActiveState();
        delay(10);
    }
    
    // 4. 回到待机
    Serial.println("[DEMO] 4. 回到待机");
    currentState = STATE_IDLE;
    smoothMove(90, 90, 10);
    
    Serial.println("[DEMO] ✓ 演示完成");
}

// ========== 主程序 ==========

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    for (int i = 0; i < 10; i++) {
        Serial.println();
        delay(100);
    }
    
    Serial.println("\n\n");
    Serial.println("=================================");
    Serial.println("   MOSS 综合联动测试");
    Serial.println("=================================");
    Serial.println();
    
    // 初始化所有模块
    setupDisplay();
    delay(500);
    
    setupLEDs();
    delay(500);
    
    setupServos();
    delay(500);
    
    setupMicrophone();
    delay(500);
    
    Serial.println();
    Serial.println("=================================");
    Serial.println("   初始化完成！");
    Serial.println("=================================");
    Serial.println();
    Serial.println("功能说明：");
    Serial.println("  - 待机：机身蓝色呼吸 + 瞳孔暗红 + 微动");
    Serial.println("  - 监听：机身绿色 + 瞳孔暗红");
    Serial.println("  - 活跃：机身橙色 + 瞳孔呼吸 + 声源定位转向");
    Serial.println("  - 触发阈值：90（峰值检测）");
    Serial.println("  - 声源定位：I2S立体声麦克风");
    Serial.println("  - LED分组：索引0-1=瞳孔, 索引2-4=机身");
    Serial.println();
    Serial.println("命令：");
    Serial.println("  1 - 进入监听模式");
    Serial.println("  0 - 回到待机模式");
    Serial.println("  h - 回到中心位置");
    Serial.println("  d - 演示模式（自动循环）");
    Serial.println("  t - LED索引映射测试");
    Serial.println("  a - 模拟声音触发（测试活跃状态）");
    Serial.println("  l - 模拟左侧声源（转向-30度）");
    Serial.println("  r - 模拟右侧声源（转向+30度）");
    Serial.println();
    
    // 启动动画：分别测试瞳孔和机身LED
    Serial.println("\n[TEST] LED分组测试...");
    
    // 测试瞳孔LED（红色）
    Serial.println("[TEST] 瞳孔LED（索引0-1）- 红色");
    setEyeLEDs(leds.Color(255, 0, 0));
    delay(500);
    
    // 测试机身LED（绿色）
    Serial.println("[TEST] 机身LED（索引2-4）- 绿色");
    setBodyLEDs(leds.Color(0, 255, 0));
    delay(500);
    
    // 测试全部LED（蓝色）
    Serial.println("[TEST] 全部LED - 蓝色");
    setAllLEDs(leds.Color(0, 0, 255));
    delay(500);
    
    // 关闭所有LED
    setAllLEDs(leds.Color(0, 0, 0));
    delay(200);
    
    Serial.println("[TEST] ✓ LED测试完成\n");
    
    currentState = STATE_IDLE;
    stateStartTime = millis();
}

void loop() {
    // 状态机
    switch (currentState) {
        case STATE_IDLE:
            handleIdleState();
            break;
            
        case STATE_LISTENING:
            handleListeningState();
            break;
            
        case STATE_ACTIVE:
            handleActiveState();
            break;
    }
    
    // 串口命令
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        
        switch (cmd) {
            case '1':
                Serial.println("\n[CMD] 进入监听模式");
                currentState = STATE_LISTENING;
                stateStartTime = millis();
                break;
                
            case '0':
                Serial.println("\n[CMD] 回到待机模式");
                currentState = STATE_IDLE;
                smoothMove(90, 90, 10);
                stateStartTime = millis();
                break;
                
            case 'h':
            case 'H':
                Serial.println("\n[CMD] 回到中心位置");
                smoothMove(90, 90, 10);
                break;
                
            case 'd':
            case 'D':
                Serial.println("\n[CMD] 演示模式");
                demoMode();
                break;
                
            case 't':
            case 'T':
                Serial.println("\n[CMD] LED索引映射测试");
                testLEDMapping();
                break;
                
            case 'a':
            case 'A':
                Serial.println("\n[CMD] 模拟声音触发");
                if (currentState == STATE_LISTENING) {
                    currentState = STATE_ACTIVE;
                    stateStartTime = millis();
                    lastSoundTime = millis();
                    Serial.println("[TEST] 进入活跃状态（模拟）");
                } else {
                    Serial.println("[TEST] 请先进入监听模式（按1）");
                }
                break;
                
            case 'l':
            case 'L':
                Serial.println("\n[CMD] 模拟左侧声源");
                if (currentState == STATE_LISTENING) {
                    currentState = STATE_ACTIVE;
                    stateStartTime = millis();
                    lastSoundTime = millis();
                    smoothMove(60, 90, 5);  // 转向左侧（-30度）
                    Serial.println("[TEST] 转向左侧60度");
                } else {
                    Serial.println("[TEST] 请先进入监听模式（按1）");
                }
                break;
                
            case 'r':
            case 'R':
                Serial.println("\n[CMD] 模拟右侧声源");
                if (currentState == STATE_LISTENING) {
                    currentState = STATE_ACTIVE;
                    stateStartTime = millis();
                    lastSoundTime = millis();
                    smoothMove(120, 90, 5);  // 转向右侧（+30度）
                    Serial.println("[TEST] 转向右侧120度");
                } else {
                    Serial.println("[TEST] 请先进入监听模式（按1）");
                }
                break;
        }
    }
    
    delay(10);
}
