# IdleMotion 测试说明

## 测试概述

本测试文件验证 `IdleMotion` 类的正确性，包括单元测试和属性测试。

## 测试任务

- **任务 2.5**: IdleMotion 属性测试

## 验证的需求

- **Requirements 13.1**: 待机状态持续产生微小随机波动
- **Requirements 13.2**: 使用柏林噪声算法生成自然波动
- **Requirements 13.3**: 叠加到控制信号，产生轻微浮动和摆动
- **Requirements 13.4**: 波动幅度不超过2度，频率不超过0.1Hz

## 测试内容

### 单元测试（5个）

1. **test_unit_basic_functionality**: 基本功能
   - 验证 getNoise() 返回有效的浮点数
   - 确保无 NaN 或 Inf 值

2. **test_unit_amplitude_constraint**: 幅度约束
   - 验证噪声值在 [-amplitude, +amplitude] 范围内
   - 采样50个点进行验证

3. **test_unit_set_parameters**: 参数设置
   - 验证 setAmplitude() 和 setFrequency() 功能
   - 确保参数修改后仍正常工作

4. **test_unit_continuity_adjacent**: 连续性（相邻时刻）
   - 验证相邻时刻（10ms间隔）的值变化很小
   - 差异应小于幅度的20%

5. **test_unit_frequency_effect**: 频率影响
   - 验证高频率产生更快的变化
   - 对比低频和高频的变化率

### 属性测试（4个，每个100次迭代）

1. **test_property_continuity**: 连续性
   - **Property 4**: 待机微动连续性
   - 对于任意参数，连续时刻的值差异 < amplitude * 0.1

2. **test_property_amplitude_bounds**: 幅度约束
   - **Property 4**: 待机微动连续性
   - 对于任意参数，所有噪声值在 [-amplitude, +amplitude] 范围内

3. **test_property_smoothness**: 平滑性
   - **Property 4**: 待机微动连续性
   - 对于任意参数，连续采样无突变（变化 < amplitude * 0.2）

4. **test_property_parameter_change_stability**: 参数修改稳定性
   - **Property 4**: 待机微动连续性
   - 修改参数后仍满足连续性和幅度约束

## 运行测试

### 方法1: PlatformIO 命令行

```bash
# 运行 IdleMotion 测试
pio test -f test_idle_motion

# 查看详细输出
pio test -f test_idle_motion -v
```

### 方法2: VSCode 界面

1. 打开 PlatformIO 侧边栏
2. 点击 "Test" 图标
3. 选择 "test_idle_motion"
4. 点击运行按钮

## 预期输出

```
========================================
IdleMotion 单元测试 (Unit Tests)
========================================
test_unit_basic_functionality:PASS
test_unit_amplitude_constraint:PASS
test_unit_set_parameters:PASS
test_unit_continuity_adjacent:PASS
test_unit_frequency_effect:PASS

========================================
IdleMotion 属性测试 (Property Tests)
Feature: moss-education-module
Property 4: 待机微动连续性
========================================

[Property Test] 连续性 - 100次迭代
  完成 10/100 次迭代
  完成 20/100 次迭代
  ...
  完成 100/100 次迭代
test_property_continuity:PASS

[Property Test] 幅度约束 - 100次迭代
  完成 10/100 次迭代
  ...
  完成 100/100 次迭代
test_property_amplitude_bounds:PASS

[Property Test] 平滑性 - 100次迭代
  完成 10/100 次迭代
  ...
  完成 100/100 次迭代
test_property_smoothness:PASS

[Property Test] 参数修改稳定性 - 100次迭代
  完成 10/100 次迭代
  ...
  完成 100/100 次迭代
test_property_parameter_change_stability:PASS

-----------------------
9 Tests 0 Failures 0 Ignored
OK
```

## 测试原理

### 柏林噪声特性

IdleMotion 使用1D柏林噪声算法，具有以下特性：
- **连续性**: 相邻时刻的值平滑过渡
- **自然性**: 类似自然界的随机波动
- **可控性**: 通过幅度和频率参数控制

### 连续性验证

连续性是待机微动的核心特性，确保运动看起来自然：
- 相邻时刻（10ms）的差异 < 10% 幅度
- 连续采样（20ms间隔）的变化 < 20% 幅度
- 无突变或跳跃

### 幅度约束

幅度约束确保微动不会过大：
- 所有噪声值在 [-amplitude, +amplitude] 范围内
- 符合 Requirements 13.4（不超过2度）

## 注意事项

1. 属性测试使用伪随机数生成器，确保测试可重复
2. 每个属性测试运行100次迭代，覆盖广泛的参数空间
3. 连续性测试使用10ms和20ms间隔，模拟实际使用场景
4. 浮点数比较使用容差，避免精度问题

## 下一步

测试通过后，继续执行：
- **任务 2.7**: SoundLocalization 属性测试（需要等待I2S麦克风到货）
- **任务 3**: Checkpoint - 验证算法库
