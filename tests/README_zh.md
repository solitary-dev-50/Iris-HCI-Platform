# Test Directory

此目录用于存放单元测试、属性测试和硬件功能测试。

[English Version](./README.md)

## 文件夹结构

```
tests/
├── README.md                          # 本文档（测试总索引）
├── algorithm_tests/                   # 算法层测试（纯软件，无硬件依赖）
│   ├── test_motion_curve.cpp          # S型曲线缓动算法测试
│   ├── test_idle_motion.cpp           # 柏林噪声待机微动测试
│   ├── README_MotionCurve_Test.md     # MotionCurve 测试文档（中文）
│   ├── README_MotionCurve_Test_en.md  # MotionCurve 测试文档（英文）
│   ├── README_IdleMotion_Test.md      # IdleMotion 测试文档（中文）
│   └── README_IdleMotion_Test_en.md   # IdleMotion 测试文档（英文）
├── hardware_control_tests/            # 硬件控制层测试（需要实际硬件）
│   ├── test_robotic_arm.cpp           # 舵机控制器测试
│   ├── README_RoboticArm_Test.md      # RoboticArm 测试文档（中文）
│   └── README_RoboticArm_Test_en.md   # RoboticArm 测试文档（英文）
└── hardware_function_tests/           # 硬件功能测试（完整功能验证）
    ├── test_oled_display.cpp          # OLED 显示屏测试
    ├── test_basic_blink.cpp           # 基础 LED 闪烁测试
    ├── test_integrated_system.cpp     # 综合联动系统测试
    ├── README_OLED_Display_Test.md    # OLED 测试文档（中文）
    ├── README_OLED_Display_Test_en.md # OLED 测试文档（英文）
    ├── README_Basic_Blink_Test.md     # LED 测试文档（中文）
    ├── README_Basic_Blink_Test_en.md  # LED 测试文档（英文）
    ├── README_Integrated_System_Test.md # 综合测试文档（中文）
    └── README_Integrated_System_Test_en.md # 综合测试文档（英文）
```

### 文件夹说明

- **algorithm_tests/**: 纯软件算法测试，不依赖硬件，可以在任何环境运行
- **hardware_control_tests/**: 硬件控制层测试，需要连接实际硬件设备
- **hardware_function_tests/**: 完整的硬件功能验证测试，需要完整硬件配置

## 测试框架

使用 Unity 测试框架（PlatformIO 内置）。

---

## 测试文件清单

### 算法层测试（纯软件，无硬件依赖）

#### 1. MotionCurve 测试
- **文件：** `algorithm_tests/test_motion_curve.cpp`
- **文档：** `algorithm_tests/README_MotionCurve_Test.md`
- **功能：** 测试 S 型曲线缓动算法
- **测试内容：**
  - 5 个单元测试（边界条件、目标切换、完成检测）
  - 3 个属性测试（边界正确性、范围约束、单调性）
- **测试结果：** ✅ 8/8 通过（100次迭代/属性）
- **运行命令：** `pio test -f test_motion_curve`

#### 2. IdleMotion 测试
- **文件：** `algorithm_tests/test_idle_motion.cpp`
- **文档：** `algorithm_tests/README_IdleMotion_Test.md`
- **功能：** 测试柏林噪声待机微动算法
- **测试内容：**
  - 5 个单元测试（基本功能、幅度约束、参数设置）
  - 4 个属性测试（连续性、幅度约束、平滑性、参数稳定性）
- **测试结果：** ✅ 9/9 通过（100次迭代/属性）
- **运行命令：** `pio test -f test_idle_motion`

---

### 硬件控制层测试（需要实际硬件）

#### 3. RoboticArmController 测试
- **文件：** `hardware_control_tests/test_robotic_arm.cpp`
- **文档：** `hardware_control_tests/README_RoboticArm_Test.md`
- **功能：** 测试舵机控制器（GPIO4/5）
- **测试内容：**
  - 4 个单元测试（初始化、姿态设置、转头、限位）
  - 1 个属性测试（目标切换即时响应，50次迭代）
- **测试结果：** ✅ 6/6 通过
- **硬件要求：** 2个舵机（GPIO4水平，GPIO5垂直）
- **运行命令：** `pio test -f test_robotic_arm`

---

### 硬件功能测试（完整功能验证）

#### 4. OLED 显示测试
- **文件：** `hardware_function_tests/test_oled_display.cpp`
- **文档：** `hardware_function_tests/README_OLED_Display_Test.md`
- **功能：** 测试 SSD1306 OLED 显示屏（128x64）
- **测试内容：** 11 个测试项
  1. 基础中文显示
  2. 系统状态界面
  3. 菜单界面
  4. 中英文混合
  5. 进度条动画
  6. 音量条动画
  7. 表情动画
  8. 滚动文字
  9. 时钟显示
  10. 仪表盘
  11. 自动演示全部
- **测试结果：** ✅ 全部通过
- **硬件要求：** SSD1306 OLED（I2C，GPIO1/2）
- **字体：** 文泉驿 GB2312（6763个常用汉字）
- **运行命令：** 上传后通过串口发送命令（1-0, a）

#### 5. 基础 LED 闪烁测试
- **文件：** `hardware_function_tests/test_basic_blink.cpp`
- **文档：** `hardware_function_tests/README_Basic_Blink_Test.md`
- **功能：** Hello World 级别的基础测试
- **测试内容：**
  - LED 闪烁（GPIO48）
  - 串口输出
  - 基础硬件验证
- **测试结果：** ✅ 通过
- **硬件要求：** LED（GPIO48）
- **用途：** 验证开发环境和基础硬件

#### 6. 综合联动系统测试
- **文件：** `hardware_function_tests/test_integrated_system.cpp`
- **文档：** `hardware_function_tests/README_Integrated_System_Test.md`
- **功能：** 多硬件协同测试（第六阶段测试）
- **测试内容：**
  - 喇叭模块（I2S音频输出）
  - LED灯环（5颗WS2812串联）
  - OLED显示屏（I2C）
  - 舵机控制（2个舵机）
  - 麦克风阵列（I2S，待硬件）
  - 状态机（待机/监听/活跃）
- **测试结果：** ✅ 硬件联动通过
- **硬件要求：** 完整硬件配置
- **用途：** 验证系统级集成

---

## 测试类型说明

### 1. 单元测试
测试单个函数或类的功能正确性。

**命名规范：** `test_<module_name>.cpp`

**特点：**
- 测试具体示例和边界条件
- 验证功能正确性
- 快速定位问题

### 2. 属性测试（Property-Based Testing）
使用手写生成器进行属性测试，每个测试至少运行100次迭代。

**命名规范：** `test_property_<feature_name>.cpp`（或包含在单元测试文件中）

**特点：**
- 随机生成输入参数
- 验证算法通用性
- 覆盖范围更广
- 每个属性100次迭代

### 3. 硬件功能测试
验证实际硬件模块的功能。

**命名规范：** `test_<hardware_name>.cpp`

**特点：**
- 需要实际硬件
- 验证硬件连接和功能
- 可能需要人工观察确认

---

## 运行测试

### 算法层测试（无硬件）
```bash
# MotionCurve 测试
pio test -f algorithm_tests/test_motion_curve

# IdleMotion 测试
pio test -f algorithm_tests/test_idle_motion
```

### 硬件控制层测试（需要硬件）
```bash
# RoboticArmController 测试（需要2个舵机）
pio test -f hardware_control_tests/test_robotic_arm
```

### 硬件功能测试（需要完整硬件）
```bash
# OLED 显示测试
# 1. 修改 platformio.ini，设置 src_filter 为 hardware_function_tests/test_oled_display.cpp
# 2. 上传到设备
pio run -t upload

# 3. 打开串口监视器
pio device monitor

# 4. 发送测试命令（1-0, a）
```

### 在目标设备上运行测试
```bash
pio test -e esp32-s3-devkitc-1
```

---

## 测试标签

属性测试必须包含标签注释，格式：
```cpp
/**
 * Feature: moss-education-module, Property 1: 运动曲线实时计算正确性
 * Validates: Requirements 12.1, 12.2, 12.4
 */
```

---

## 测试进度

### 已完成测试
- ✅ 任务 2.2, 2.3: MotionCurve 测试（8/8 通过）
- ✅ 任务 2.5: IdleMotion 测试（9/9 通过）
- ✅ 任务 3: Checkpoint - 验证算法库
- ✅ 任务 5.2: RoboticArmController 测试（6/6 通过）
- ✅ OLED 显示测试（11项全部通过）
- ✅ 基础 LED 测试（通过）
- ✅ 综合联动测试（通过）

### 待测试项目
- ⏸️ 任务 2.7: SoundLocalization 测试（待 I2S 麦克风到货）
- ⏸️ 任务 5.4: LEDController 属性测试
- ⏸️ 任务 5.6: 控制器并行执行测试
- ⏸️ 任务 5.7: 非阻塞更新测试

---

## 测试统计

| 测试类型 | 测试文件数 | 测试用例数 | 通过率 |
|---------|-----------|-----------|--------|
| 算法层测试 | 2 | 17 | 100% |
| 硬件控制层测试 | 1 | 6 | 100% |
| 硬件功能测试 | 3 | 28+ | 100% |
| **总计** | **6** | **51+** | **100%** |

---

## 文件命名规范

- `.cpp` - 当前活跃的测试文件
- `README_<TestName>_Test.md` - 测试文档

---

## 相关文档

- **测试报告：** `../测试报告.md`

