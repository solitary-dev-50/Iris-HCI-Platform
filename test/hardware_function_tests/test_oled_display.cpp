#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// 硬件连接：SDA接GPIO2，SCL接GPIO1
#define I2C_SDA 2
#define I2C_SCL 1

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, I2C_SCL, I2C_SDA);

// ========== 测试函数 ==========

void test1_BasicChinese() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    display.drawUTF8(10, 14, "你好世界");
    display.drawUTF8(10, 30, "MOSS终端");
    display.drawUTF8(10, 46, "中文测试");
    display.drawUTF8(10, 62, "显示正常");
    
    display.sendBuffer();
}

void test2_SystemStatus() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    // 标题
    display.drawUTF8(30, 13, "系统状态");
    
    // 分隔线
    display.drawLine(0, 15, 128, 15);
    
    // 状态信息（恢复完整文字）
    display.drawUTF8(5, 31, "运行:00:05:23");
    display.drawUTF8(5, 47, "内存:45%");
    display.drawUTF8(5, 61, "温度:42°C");
    
    display.sendBuffer();
}

void test3_Menu() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    // 标题
    display.drawUTF8(35, 13, "主菜单");
    display.drawLine(0, 15, 128, 15);
    
    // 菜单项（恢复完整文字）
    display.drawUTF8(5, 31, ">1.开始对话");
    display.drawUTF8(5, 47, " 2.系统设置");
    display.drawUTF8(5, 61, " 3.关于MOSS");
    
    display.sendBuffer();
}

void test4_MixedText() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    display.drawUTF8(5, 13, "状态:待机中");
    display.drawUTF8(5, 29, "音量:75%");
    display.drawUTF8(5, 45, "温度:28°C");
    display.drawUTF8(5, 61, "WiFi:已连接");
    
    display.sendBuffer();
}

void test5_ProgressBar() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    display.drawUTF8(35, 13, "加载中");
    
    // 进度条动画
    for (int progress = 0; progress <= 100; progress += 5) {
        // 清除进度条区域
        display.setDrawColor(0);
        display.drawBox(0, 20, 128, 44);
        display.setDrawColor(1);
        
        // 绘制进度条边框
        display.drawFrame(10, 25, 108, 15);
        
        // 绘制进度
        int barWidth = (progress * 104) / 100;
        display.drawBox(12, 27, barWidth, 11);
        
        // 显示百分比
        char buf[10];
        sprintf(buf, "%d%%", progress);
        display.drawStr(55, 55, buf);
        
        display.sendBuffer();
        delay(50);
    }
    
    delay(500);
    
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    display.drawUTF8(30, 35, "加载完成");
    display.sendBuffer();
    delay(1000);
}

void test6_VolumeBar() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    display.drawUTF8(48, 13, "音量");
    
    // 音量条动画
    for (int vol = 0; vol <= 100; vol += 5) {
        // 清除音量条区域
        display.setDrawColor(0);
        display.drawBox(0, 15, 128, 49);
        display.setDrawColor(1);
        
        // 绘制音量条（竖条）
        int barHeight = (vol * 40) / 100;
        for (int i = 0; i < 10; i++) {
            int x = 10 + i * 11;
            int h = (barHeight * (i + 1)) / 10;
            display.drawBox(x, 54 - h, 8, h);
        }
        
        // 显示数值
        char buf[10];
        sprintf(buf, "%d%%", vol);
        display.drawStr(55, 61, buf);
        
        display.sendBuffer();
        delay(30);
    }
    
    delay(1000);
}

void test7_Animation() {
    // 简单的表情动画
    for (int i = 0; i < 3; i++) {
        // 睁眼
        display.clearBuffer();
        display.drawCircle(40, 32, 10);  // 左眼
        display.drawCircle(88, 32, 10);  // 右眼
        display.drawDisc(40, 32, 4);     // 左瞳孔
        display.drawDisc(88, 32, 4);     // 右瞳孔
        display.drawLine(30, 50, 98, 50); // 嘴巴
        display.sendBuffer();
        delay(500);
        
        // 闭眼
        display.clearBuffer();
        display.drawLine(30, 32, 50, 32); // 左眼
        display.drawLine(78, 32, 98, 32); // 右眼
        display.drawLine(30, 50, 98, 50); // 嘴巴
        display.sendBuffer();
        delay(200);
    }
}

void test8_ScrollText() {
    const char* text = "欢迎使用MOSS智能教育终端";
    
    display.setFont(u8g2_font_wqy14_t_gb2312);
    int textWidth = display.getUTF8Width(text);
    
    for (int x = 128; x > -textWidth; x -= 2) {
        display.clearBuffer();
        display.drawUTF8(x, 35, text);
        display.sendBuffer();
        delay(20);
    }
}

void test9_Clock() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    // 模拟时钟显示
    for (int sec = 0; sec < 10; sec++) {
        display.clearBuffer();
        
        display.drawUTF8(40, 13, "时钟");
        display.drawLine(0, 15, 128, 15);
        
        char timeStr[20];
        sprintf(timeStr, "00:00:%02d", sec);
        display.setFont(u8g2_font_logisoso16_tr);
        display.drawStr(25, 45, timeStr);
        
        display.sendBuffer();
        delay(1000);
    }
}

void test10_Dashboard() {
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    
    // 仪表盘样式
    display.drawUTF8(30, 13, "仪表盘");
    display.drawLine(0, 15, 128, 15);
    
    // CPU（恢复中文）
    display.drawUTF8(5, 31, "CPU:45%");
    display.drawBox(60, 22, 45, 8);
    
    // 内存
    display.drawUTF8(5, 47, "内存:67%");
    display.drawBox(60, 38, 67, 8);
    
    // 温度
    display.drawUTF8(5, 61, "温度:42°C");
    
    display.sendBuffer();
}

// ========== 主程序 ==========

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("   OLED完整功能测试");
    Serial.println("========================================\n");
    
    // 初始化
    display.begin();
    display.enableUTF8Print();
    
    // 启动画面
    display.clearBuffer();
    display.setFont(u8g2_font_wqy14_t_gb2312);
    display.drawUTF8(20, 25, "MOSS终端");
    display.drawUTF8(25, 45, "初始化中");
    display.sendBuffer();
    delay(1500);
    
    Serial.println("测试命令：");
    Serial.println("  1 - 基础中文显示");
    Serial.println("  2 - 系统状态界面");
    Serial.println("  3 - 菜单界面");
    Serial.println("  4 - 中英文混合");
    Serial.println("  5 - 进度条动画");
    Serial.println("  6 - 音量条动画");
    Serial.println("  7 - 表情动画");
    Serial.println("  8 - 滚动文字");
    Serial.println("  9 - 时钟显示");
    Serial.println("  0 - 仪表盘");
    Serial.println("  a - 自动演示全部\n");
    
    // 默认显示
    test1_BasicChinese();
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();
        
        switch (cmd) {
            case '1':
                Serial.println("\n[测试1] 基础中文显示");
                test1_BasicChinese();
                break;
                
            case '2':
                Serial.println("\n[测试2] 系统状态界面");
                test2_SystemStatus();
                break;
                
            case '3':
                Serial.println("\n[测试3] 菜单界面");
                test3_Menu();
                break;
                
            case '4':
                Serial.println("\n[测试4] 中英文混合");
                test4_MixedText();
                break;
                
            case '5':
                Serial.println("\n[测试5] 进度条动画");
                test5_ProgressBar();
                break;
                
            case '6':
                Serial.println("\n[测试6] 音量条动画");
                test6_VolumeBar();
                break;
                
            case '7':
                Serial.println("\n[测试7] 表情动画");
                test7_Animation();
                break;
                
            case '8':
                Serial.println("\n[测试8] 滚动文字");
                test8_ScrollText();
                break;
                
            case '9':
                Serial.println("\n[测试9] 时钟显示");
                test9_Clock();
                break;
                
            case '0':
                Serial.println("\n[测试10] 仪表盘");
                test10_Dashboard();
                break;
                
            case 'a':
            case 'A':
                Serial.println("\n[自动演示] 开始...");
                
                Serial.println("1/10 基础中文");
                test1_BasicChinese();
                delay(2000);
                
                Serial.println("2/10 系统状态");
                test2_SystemStatus();
                delay(2000);
                
                Serial.println("3/10 菜单界面");
                test3_Menu();
                delay(2000);
                
                Serial.println("4/10 中英文混合");
                test4_MixedText();
                delay(2000);
                
                Serial.println("5/10 进度条");
                test5_ProgressBar();
                
                Serial.println("6/10 音量条");
                test6_VolumeBar();
                
                Serial.println("7/10 表情动画");
                test7_Animation();
                delay(1000);
                
                Serial.println("8/10 滚动文字");
                test8_ScrollText();
                delay(1000);
                
                Serial.println("9/10 时钟显示");
                test9_Clock();
                
                Serial.println("10/10 仪表盘");
                test10_Dashboard();
                delay(2000);
                
                Serial.println("\n演示完成！");
                test1_BasicChinese();
                break;
        }
    }
}
