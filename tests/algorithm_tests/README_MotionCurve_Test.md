# MotionCurve 测试说明

## 测试概述

本测试文件验证 `MotionCurve` 类的正确性，包括单元测试和属性测试。

## 测试任务

- **任务 2.2**: MotionCurve 属性测试
- **任务 2.3**: MotionCurve 单元测试

## 验证的需求

- **Requirements 12.1**: S型曲线控制所有运动
- **Requirements 12.2**: 实时计算，避免预生成大数组
- **Requirements 12.3**: 运动中可切换目标
- **Requirements 12.4**: 包含启动、加速、匀速、减速、停止五个阶段
- **Requirements 15.5**: 立即响应新指令

## 测试内容

### 单元测试（5个）

1. **test_unit_basic_boundaries**: 基本边界条件
   - 验证起点值（t=0）
   - 验证终点值（t=duration）
   - 验证超过终点后的值
   - 验证中点值在合理范围内

2. **test_unit_negative_motion**: 负向运动
   - 验证从大值到小值的运动
   - 确保负向运动也遵循边界条件

3. **test_unit_target_switch**: 目标切换即时响应
   - 验证运动中切换目标的能力
   - 确保新目标立即生效

4. **test_unit_is_complete**: 完成检测
   - 验证 `isComplete()` 方法的正确性
   - 测试不同时刻的完成状态

5. **test_unit_reset**: 重置功能
   - 验证 `reset()` 方法的正确性

### 属性测试（3个，每个100次迭代）

1. **test_property_boundaries**: 边界条件正确性
   - **Property 1**: 运动曲线实时计算正确性
   - 对于任意起点、终点和时长，验证边界值准确

2. **test_property_intermediate_range**: 中间值范围约束
   - **Property 1**: 运动曲线实时计算正确性
   - 对于任意参数，验证中间值在起点和终点之间

3. **test_property_monotonic**: 单调性
   - **Property 1**: 运动曲线实时计算正确性
   - 对于任意参数，验证曲线单调（无回退）

## 运行测试

### 方法1: PlatformIO 命令行

```bash
# 运行所有测试
pio test

# 运行特定测试
pio test -f test_motion_curve

# 查看详细输出
pio test -v
```

### 方法2: VSCode 界面

1. 打开 PlatformIO 侧边栏
2. 点击 "Test" 图标
3. 选择 "test_motion_curve"
4. 点击运行按钮

## 预期输出

```
========================================
MotionCurve 单元测试 (Unit Tests)
========================================
test_unit_basic_boundaries:PASS
test_unit_negative_motion:PASS
test_unit_target_switch:PASS
test_unit_is_complete:PASS
test_unit_reset:PASS

========================================
MotionCurve 属性测试 (Property Tests)
Feature: moss-education-module
Property 1: 运动曲线实时计算正确性
========================================

[Property Test] 边界条件 - 100次迭代
  完成 10/100 次迭代
  完成 20/100 次迭代
  ...
  完成 100/100 次迭代
test_property_boundaries:PASS

[Property Test] 中间值范围 - 100次迭代
  完成 10/100 次迭代
  ...
  完成 100/100 次迭代
test_property_intermediate_range:PASS

[Property Test] 单调性 - 100次迭代
  完成 10/100 次迭代
  ...
  完成 100/100 次迭代
test_property_monotonic:PASS

-----------------------
8 Tests 0 Failures 0 Ignored
OK
```

## 注意事项

1. 属性测试使用伪随机数生成器，确保测试可重复
2. 每个属性测试运行100次迭代，覆盖广泛的输入空间
3. 测试使用实际的 `millis()` 时间，因此需要在硬件上运行
4. 浮点数比较使用容差（0.1），避免精度问题

## 下一步

测试通过后，继续执行：
- **任务 2.5**: IdleMotion 属性测试
- **任务 3**: Checkpoint - 验证算法库
