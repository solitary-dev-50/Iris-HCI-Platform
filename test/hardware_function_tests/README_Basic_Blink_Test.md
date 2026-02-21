# ESP32-S3 基础闪烁测试说明

## 测试目标

验证ESP32-S3开发板的基础功能，包括：
- 板载LED控制
- 串口通信
- Arduino框架运行
- 开发环境配置

这是一个**Hello World级别**的测试程序，用于快速验证硬件和开发环境是否正常工作。

## 测试文件

- `test/test_basic_blink.cpp.bak` - 基础LED闪烁测试代码（备份）

## 硬件要求

### 必需硬件：
1. ESP32-S3开发板
2. USB数据线（用于供电和串口通信）

### 无需外接硬件：
- 使用板载LED（GPIO2）
- 无需外接传感器或模块

## 测试内容

### 功能1：LED闪烁
- **GPIO**: GPIO2（板载LED）
- **模式**: 输出模式
- **闪烁频率**: 1Hz（亮1秒，灭1秒）

### 功能2：串口输出
- **波特率**: 115200
- **输出内容**:
  - 启动信息："ESP32-S3 Started!"
  - 测试标题："LED Blinking Test"
  - 状态信息："LED ON" / "LED OFF"

## 代码说明

### 完整代码：
```cpp
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
```

### 代码逻辑：

**setup() 函数**:
1. 设置GPIO2为输出模式（控制LED）
2. 初始化串口（115200波特率）
3. 等待2秒（确保串口稳定）
4. 打印启动信息

**loop() 函数**:
1. LED点亮 → 打印"LED ON" → 延迟1秒
2. LED熄灭 → 打印"LED OFF" → 延迟1秒
3. 循环执行

## 运行测试

### 方法1：作为主程序运行

1. **重命名文件**（去掉.bak后缀）:
```cmd
ren test\test_basic_blink.cpp.bak test_basic_blink.cpp
```

2. **临时替换main.cpp**:
```cmd
# 备份当前main.cpp
ren src\main.cpp main.cpp.bak

# 复制测试文件到src
copy test\test_basic_blink.cpp src\main.cpp
```

3. **编译上传**:
```cmd
pio run -t upload
```

4. **打开串口监控**:
```cmd
pio device monitor
```

5. **恢复main.cpp**:
```cmd
del src\main.cpp
ren src\main.cpp.bak main.cpp
```

### 方法2：使用PlatformIO测试框架

由于这是一个简单的功能测试（不是单元测试），建议使用方法1。

## 预期结果

### 硬件表现：
- **板载LED**（通常在ESP32-S3开发板上）应该以1Hz频率闪烁
- 亮1秒 → 灭1秒 → 循环

### 串口输出：
```
ESP32-S3 Started!
LED Blinking Test
LED ON
LED OFF
LED ON
LED OFF
LED ON
LED OFF
...
```

## 验证标准

### ✅ 测试通过条件：
1. LED能够正常闪烁（1Hz频率）
2. 串口能够正常输出信息
3. 程序能够稳定运行，无复位或崩溃
4. 串口输出与LED状态同步

### ❌ 测试失败情况：
1. LED不闪烁或闪烁不规律
2. 串口无输出或输出乱码
3. 程序频繁复位
4. LED状态与串口输出不一致

## 故障排查

### 问题1：LED不闪烁
**可能原因**:
- GPIO2不是板载LED引脚
- LED硬件故障

**解决方法**:
1. 查看开发板原理图，确认板载LED引脚
2. 尝试外接LED到GPIO2测试
3. 使用万用表测量GPIO2电压变化

### 问题2：串口无输出
**可能原因**:
- USB驱动未安装
- 串口波特率不匹配
- USB线只能充电不能传输数据

**解决方法**:
1. 安装CP2102或CH340驱动
2. 确认串口监控波特率为115200
3. 更换USB数据线

### 问题3：串口输出乱码
**可能原因**:
- 波特率设置错误
- USB CDC模式问题

**解决方法**:
1. 确认串口监控波特率为115200
2. 检查platformio.ini中的monitor_speed配置
3. 尝试按复位键重新启动

### 问题4：程序无法上传
**可能原因**:
- 串口被占用
- 开发板未进入下载模式

**解决方法**:
1. 关闭所有串口监控程序
2. 按住BOOT键，点击RESET键，进入下载模式
3. 检查USB连接

## 测试价值

### 这个简单的测试能验证：

1. **硬件正常**:
   - ESP32-S3芯片工作正常
   - GPIO输出功能正常
   - USB串口通信正常

2. **开发环境正确**:
   - PlatformIO配置正确
   - Arduino框架安装正确
   - 编译工具链正常
   - 上传工具正常

3. **基础功能**:
   - digitalWrite()函数正常
   - Serial.println()函数正常
   - delay()函数正常
   - setup()和loop()执行正常

### 适用场景：

- ✅ **新板测试** - 拿到新开发板后的第一个测试
- ✅ **环境验证** - 验证开发环境配置是否正确
- ✅ **故障排查** - 当复杂程序出问题时，用最简单的程序排查硬件
- ✅ **教学演示** - 作为ESP32-S3入门教程的第一个例子

## 扩展建议

### 可以在此基础上添加：

1. **多个LED控制**:
```cpp
pinMode(2, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
```

2. **不同闪烁模式**:
```cpp
// 快速闪烁
delay(100);

// SOS信号
// ... --- ...
```

3. **按键控制**:
```cpp
pinMode(0, INPUT_PULLUP);  // BOOT按键
if (digitalRead(0) == LOW) {
    // 按键按下
}
```

4. **PWM呼吸灯**:
```cpp
ledcSetup(0, 5000, 8);
ledcAttachPin(2, 0);
ledcWrite(0, brightness);
```

## 相关文档

- **测试报告**: `测试报告.md` - 完整的硬件测试记录
- **引脚配置**: `include/pin_config.h` - 完整引脚定义
- **项目文件列表**: `项目文件列表.md` - 项目结构说明

## 测试历史

### 测试状态：✅ 已通过

这是项目最早期的测试程序，用于验证：
- ESP32-S3开发板正常工作
- 开发环境配置正确
- 基础GPIO和串口功能正常

**测试时间**: 项目初期  
**测试结果**: 通过  
**当前状态**: 已归档为参考代码

## 备注

1. **文件后缀**: `.bak` 表示这是备份文件，不会被编译
2. **历史意义**: 这是项目的第一个测试程序，具有纪念意义
3. **保留价值**: 作为最简单的测试参考，用于快速验证硬件
4. **不建议修改**: 保持其简单性，作为基准测试

---

**文档结束**
