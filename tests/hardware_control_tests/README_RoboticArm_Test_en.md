# Servo Controller Property Test Documentation (Simplified Version)

## Test Objective

Verify **Property 7: Immediate Target Switch Response**

*For any* servo controller in motion, after calling `setTarget()`, the next `update()` must immediately calculate based on the new target without waiting for the current action to complete.

**Validates: Requirements 12.3, 15.5**

## Simplified Description

Since only 2 servos are available (GPIO4/5), this test uses a simplified version:
- Does not use complete RoboticArmController
- Directly uses ESP32Servo library
- Only tests target switching for 2 joints
- Iterations reduced from 100 to 50

## Test File

- `test/test_robotic_arm.cpp` - Simplified test code

## Hardware Requirements

### Required Hardware:
1. ESP32-S3 development board
2. 2 servos (already connected)
3. Servo power supply (5V)

### Pin Connections:
| Servo | GPIO Pin | Description |
|-------|----------|-------------|
| Horizontal Servo | GPIO 4 | Left-right rotation |
| Vertical Servo | GPIO 5 | Up-down rotation |

### Safety Precautions:
1. Ensure no obstacles around servos
2. Servos will move randomly during testing, this is normal
3. Disconnect power immediately if abnormalities occur

## Test Content

### Unit Tests (4 tests):
1. **test_unit_initialization** - Verify initialization
2. **test_unit_basic_movement** - Verify basic movement
3. **test_unit_target_switch_example** - Verify target switching (specific example)
4. **test_unit_limits** - Verify software limit protection

### Property Tests (1 test, 50 iterations):
1. **test_property_target_switch_immediate** - Immediate target switch response
   - Randomly generate first target position
   - Switch to new target after 1/3 time
   - Verify movement direction immediately towards new target
   - 50 random iterations (simplified version)
   - Requires at least 90% pass rate

## Running Tests

### Preparation:
1. Confirm servos are connected to GPIO4 and GPIO5
2. Confirm servo power is connected (5V)
3. Ensure sufficient space around servos

### Run Command:
```cmd
# Rename other test files (avoid conflicts)
ren test\test_robotic_arm.cpp.bak test_robotic_arm.cpp

# Run test
pio test -f test_robotic_arm
```

### Expected Output:
```
========================================
Servo Controller Test (Simplified - GPIO4/5)
========================================

Unit Tests
----------------------------------------
test/test_robotic_arm.cpp:XX: test_unit_initialization [PASSED]
test/test_robotic_arm.cpp:XX: test_unit_basic_movement [PASSED]
test/test_robotic_arm.cpp:XX: test_unit_target_switch_example [PASSED]
test/test_robotic_arm.cpp:XX: test_unit_limits [PASSED]

========================================
Property Tests
========================================
[Property Test] Immediate target switch response
  Completed 10/50 iterations (Pass rate: XX%)
  Completed 20/50 iterations (Pass rate: XX%)
  ...
  Completed 50/50 iterations (Pass rate: XX%)

[Property Test] Complete! Pass rate: XX/50 (XX%)

test/test_robotic_arm.cpp:XX: test_property_target_switch_immediate [PASSED]

===================== 5 test cases: 5 succeeded =====================
```

## Test Principles

### Property 7 Verification Logic:

1. **Set first target** - Start servo movement
2. **Run 1/3 time** - During motion (not complete)
3. **Record current position** - anglesBefore
4. **Switch to new target** - Call setTarget()
5. **Run several updates** - Immediate update
6. **Record new position** - anglesAfter
7. **Verify movement direction** - Should move towards new target, not old target

### Judgment Criteria:
```cpp
// At least one axis should move towards new target (distance decreases)
bool movingTowards = (distToTarget2After1 < distToTarget2Before1 - 1.0f) ||
                      (distToTarget2After2 < distToTarget2Before2 - 1.0f);
```

## Test Duration

- Unit tests: ~20 seconds
- Property tests: ~5-8 minutes (50 iterations, ~6-10 seconds each)
- Total: ~8-10 minutes

## Troubleshooting

### Issue 1: Servos not moving
- Check if power is connected
- Check if pin connections are correct
- Check serial output for error messages

### Issue 2: Servo jittering
- May be insufficient power, use higher current power supply
- May be unstable PWM signal, check wiring

### Issue 3: Test failure
- Check if servos are stuck
- Check if robotic arm hits obstacles
- View detailed error messages in serial output

### Issue 4: Compilation error
- Ensure HardwareControllers library is implemented
- Ensure BiometricMotion library is implemented
- Ensure HardwareAbstraction library is implemented

## Post-Test Cleanup

```cmd
# Restore other test files
ren test\test_idle_motion.cpp.bak test_idle_motion.cpp

# Rename current test
ren test\test_robotic_arm.cpp test_robotic_arm.cpp.bak
```

## Expected Results

If tests pass, it indicates:
1. MotionCurve algorithm works normally on real hardware
2. Target switching mechanism responds promptly
3. Multi-joint synchronized motion is well coordinated
4. Software limit protection is effective

This verifies the core functionality in the design document: **Non-blocking, immediately responsive motion control system**.
