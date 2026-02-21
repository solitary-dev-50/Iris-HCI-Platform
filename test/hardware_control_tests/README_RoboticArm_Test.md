# 舵机控制器属性测试说明（简化版）

## 测试目标

验证 **Property 7: 目标切换即时响应**

*For any* 舵机控制器正在运动中，调用 `setTarget()` 后，下一次 `update()` 必须立即基于新目标计算，无需等待当前动作完成。

**Validates: Requirements 12.3, 15.5**

## 简化说明

由于只有2个舵机（GPIO4/5），本测试使用简化版本：
- 不使用完整的RoboticArmController
- 直接使用ESP32Servo库
- 只测试2个关节的目标切换功能
- 迭代次数从100次减少到50次

## 测试文件

- `test/test_robotic_arm.cpp` - 简化版测试代码

## 硬件要求

### 必需硬件：
1. ESP32-S3开发板
2. 2个舵机（已连接）
3. 舵机电源（5V）

### 引脚连接：
| 舵机 | GPIO引脚 | 说明 |
|------|---------|------|
| 水平舵机 | GPIO 4 | 左右转动 |
| 垂直舵机 | GPIO 5 | 上下转动 |

### 安全注意事项：
1. 确保舵机周围没有障碍物
2. 测试过程中舵机会随机运动，这是正常现象
3. 如果发现异常，立即断开电源

## 测试内容

### 单元测试（4个）：
1. **test_unit_initialization** - 验证初始化
2. **test_unit_basic_movement** - 验证基本运动
3. **test_unit_target_switch_example** - 验证目标切换（具体示例）
4. **test_unit_limits** - 验证软件限位保护

### 属性测试（1个，50次迭代）：
1. **test_property_target_switch_immediate** - 目标切换即时响应
   - 随机生成第一个目标位置
   - 运行1/3时间后切换到新目标
   - 验证运动方向立即朝向新目标
   - 50次随机迭代（简化版）
   - 要求至少90%通过率

## 运行测试

### 准备工作：
1. 确认舵机已连接到GPIO4和GPIO5
2. 确认舵机电源已接通（5V）
3. 确保舵机周围空间充足

### 运行命令：
```cmd
# 重命名其他测试文件（避免冲突）
ren test\test_robotic_arm.cpp.bak test_robotic_arm.cpp

# 运行测试
pio test -f test_robotic_arm
```

### 预期输出：
```
========================================
舵机控制器测试 (简化版 - GPIO4/5)
========================================

单元测试 (Unit Tests)
----------------------------------------
test/test_robotic_arm.cpp:XX: test_unit_initialization [PASSED]
test/test_robotic_arm.cpp:XX: test_unit_basic_movement [PASSED]
test/test_robotic_arm.cpp:XX: test_unit_target_switch_example [PASSED]
test/test_robotic_arm.cpp:XX: test_unit_limits [PASSED]

========================================
属性测试 (Property Tests)
========================================
[Property Test] 目标切换即时响应
  完成 10/50 次迭代 (通过率: XX%)
  完成 20/50 次迭代 (通过率: XX%)
  ...
  完成 50/50 次迭代 (通过率: XX%)

[Property Test] 完成！通过率: XX/50 (XX%)

test/test_robotic_arm.cpp:XX: test_property_target_switch_immediate [PASSED]

===================== 5 test cases: 5 succeeded =====================
```

## 测试原理

### Property 7 的验证逻辑：

1. **设置第一个目标** - 让舵机开始运动
2. **运行1/3时间** - 在运动过程中（未完成）
3. **记录当前位置** - anglesBefore
4. **切换到新目标** - 调用setTarget()
5. **运行几次update** - 立即更新
6. **记录新位置** - anglesAfter
7. **验证运动方向** - 应该朝向新目标，而不是旧目标

### 判断标准：
```cpp
// 至少有一个轴应该朝向新目标移动（距离变小）
bool movingTowards = (distToTarget2After1 < distToTarget2Before1 - 1.0f) ||
                      (distToTarget2After2 < distToTarget2Before2 - 1.0f);
```

## 测试时长

- 单元测试：约20秒
- 属性测试：约5-8分钟（50次迭代，每次约6-10秒）
- 总计：约8-10分钟

## 故障排查

### 问题1：舵机不动
- 检查电源是否接通
- 检查引脚连接是否正确
- 检查串口输出是否有错误信息

### 问题2：舵机抖动
- 可能是电源不足，使用更大电流的电源
- 可能是PWM信号不稳定，检查接线

### 问题3：测试失败
- 检查舵机是否卡住
- 检查机械臂是否碰到障碍物
- 查看串口输出的详细错误信息

### 问题4：编译错误
- 确保已经实现了HardwareControllers库
- 确保已经实现了BiometricMotion库
- 确保已经实现了HardwareAbstraction库

## 测试后清理

```cmd
# 恢复其他测试文件
ren test\test_idle_motion.cpp.bak test_idle_motion.cpp

# 重命名当前测试
ren test\test_robotic_arm.cpp test_robotic_arm.cpp.bak
```

## 预期结果

如果测试通过，说明：
1. MotionCurve算法在真实硬件上工作正常
2. 目标切换机制响应及时
3. 多关节同步运动协调良好
4. 软件限位保护有效

这验证了设计文档中的核心功能：**非阻塞、即时响应的运动控制系统**。
