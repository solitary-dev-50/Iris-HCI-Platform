# Test Directory

This directory contains unit tests, property-based tests, and hardware functional tests.

[中文版 (Chinese Version)](./README_zh.md)

## Folder Structure

```
tests/
├── README.md                          # This document (Test index - Chinese)
├── README_en.md                       # This document (Test index - English)
├── algorithm_tests/                   # Algorithm layer tests (Pure software, no hardware dependencies)
│   ├── test_motion_curve.cpp          # S-curve easing algorithm test
│   ├── test_idle_motion.cpp           # Perlin noise idle motion test
│   ├── README_MotionCurve_Test.md     # MotionCurve test documentation (Chinese)
│   ├── README_MotionCurve_Test_en.md  # MotionCurve test documentation (English)
│   ├── README_IdleMotion_Test.md      # IdleMotion test documentation (Chinese)
│   └── README_IdleMotion_Test_en.md   # IdleMotion test documentation (English)
├── hardware_control_tests/            # Hardware control layer tests (Requires actual hardware)
│   ├── test_robotic_arm.cpp           # Servo controller test
│   ├── README_RoboticArm_Test.md      # RoboticArm test documentation (Chinese)
│   └── README_RoboticArm_Test_en.md   # RoboticArm test documentation (English)
└── hardware_function_tests/           # Hardware functional tests (Complete functionality verification)
    ├── test_oled_display.cpp          # OLED display test
    ├── test_basic_blink.cpp           # Basic LED blink test
    ├── test_integrated_system.cpp     # Integrated system test
    ├── README_OLED_Display_Test.md    # OLED test documentation (Chinese)
    ├── README_OLED_Display_Test_en.md # OLED test documentation (English)
    ├── README_Basic_Blink_Test.md     # LED test documentation (Chinese)
    ├── README_Basic_Blink_Test_en.md  # LED test documentation (English)
    ├── README_Integrated_System_Test.md # Integrated test documentation (Chinese)
    └── README_Integrated_System_Test_en.md # Integrated test documentation (English)
```

### Folder Description

- **algorithm_tests/**: Pure software algorithm tests, no hardware dependencies, can run in any environment
- **hardware_control_tests/**: Hardware control layer tests, requires actual hardware devices
- **hardware_function_tests/**: Complete hardware functionality verification tests, requires full hardware configuration

## Test Framework

Uses Unity test framework (built into PlatformIO).

---

## Test File List

### Algorithm Layer Tests (Pure software, no hardware dependencies)

#### 1. MotionCurve Test
- **File:** `algorithm_tests/test_motion_curve.cpp`
- **Documentation:** `algorithm_tests/README_MotionCurve_Test_en.md`
- **Function:** Test S-curve easing algorithm
- **Test Content:**
  - 5 unit tests (boundary conditions, target switching, completion detection)
  - 3 property tests (boundary correctness, range constraints, monotonicity)
- **Test Results:** ✅ 8/8 passed (100 iterations/property)
- **Run Command:** `pio test -f algorithm_tests/test_motion_curve`

#### 2. IdleMotion Test
- **File:** `algorithm_tests/test_idle_motion.cpp`
- **Documentation:** `algorithm_tests/README_IdleMotion_Test_en.md`
- **Function:** Test Perlin noise idle motion algorithm
- **Test Content:**
  - 5 unit tests (basic functionality, amplitude constraints, parameter settings)
  - 4 property tests (continuity, amplitude constraints, smoothness, parameter stability)
- **Test Results:** ✅ 9/9 passed (100 iterations/property)
- **Run Command:** `pio test -f algorithm_tests/test_idle_motion`

---

### Hardware Control Layer Tests (Requires actual hardware)

#### 3. RoboticArmController Test
- **File:** `hardware_control_tests/test_robotic_arm.cpp`
- **Documentation:** `hardware_control_tests/README_RoboticArm_Test_en.md`
- **Function:** Test servo controller (GPIO4/5)
- **Test Content:**
  - 4 unit tests (initialization, pose setting, head turning, limits)
  - 1 property test (immediate target switch response, 50 iterations)
- **Test Results:** ✅ 6/6 passed
- **Hardware Requirements:** 2 servos (GPIO4 horizontal, GPIO5 vertical)
- **Run Command:** `pio test -f hardware_control_tests/test_robotic_arm`

---

### Hardware Functional Tests (Complete functionality verification)

#### 4. OLED Display Test
- **File:** `hardware_function_tests/test_oled_display.cpp`
- **Documentation:** `hardware_function_tests/README_OLED_Display_Test_en.md`
- **Function:** Test SSD1306 OLED display (128x64)
- **Test Content:** 11 test items
  1. Basic Chinese display
  2. System status interface
  3. Menu interface
  4. Mixed Chinese-English
  5. Progress bar animation
  6. Volume bar animation
  7. Emoji animation
  8. Scrolling text
  9. Clock display
  10. Dashboard
  11. Auto demo all
- **Test Results:** ✅ All passed
- **Hardware Requirements:** SSD1306 OLED (I2C, GPIO1/2)
- **Font:** WenQuanYi GB2312 (6763 common Chinese characters)
- **Run Command:** Upload then send commands via serial (1-0, a)

#### 5. Basic LED Blink Test
- **File:** `hardware_function_tests/test_basic_blink.cpp`
- **Documentation:** `hardware_function_tests/README_Basic_Blink_Test_en.md`
- **Function:** Hello World level basic test
- **Test Content:**
  - LED blinking (GPIO48)
  - Serial output
  - Basic hardware verification
- **Test Results:** ✅ Passed
- **Hardware Requirements:** LED (GPIO48)
- **Purpose:** Verify development environment and basic hardware

#### 6. Integrated System Test
- **File:** `hardware_function_tests/test_integrated_system.cpp`
- **Documentation:** `hardware_function_tests/README_Integrated_System_Test_en.md`
- **Function:** Multi-hardware coordination test (Phase 6 test)
- **Test Content:**
  - Speaker module (I2S audio output)
  - LED ring (5 WS2812 in series)
  - OLED display (I2C)
  - Servo control (2 servos)
  - Microphone array (I2S, pending hardware)
  - State machine (idle/listening/active)
- **Test Results:** ✅ Hardware linkage passed
- **Hardware Requirements:** Complete hardware configuration
- **Purpose:** Verify system-level integration

---

## Test Type Description

### 1. Unit Tests
Test the functional correctness of individual functions or classes.

**Naming Convention:** `test_<module_name>.cpp`

**Characteristics:**
- Test specific examples and boundary conditions
- Verify functional correctness
- Quick problem localization

### 2. Property-Based Testing
Uses hand-written generators for property testing, each test runs at least 100 iterations.

**Naming Convention:** `test_property_<feature_name>.cpp` (or included in unit test files)

**Characteristics:**
- Randomly generate input parameters
- Verify algorithm generality
- Broader coverage
- 100 iterations per property

### 3. Hardware Functional Tests
Verify actual hardware module functionality.

**Naming Convention:** `test_<hardware_name>.cpp`

**Characteristics:**
- Requires actual hardware
- Verify hardware connections and functionality
- May require manual observation for confirmation

---

## Running Tests

### Algorithm Layer Tests (No hardware)
```bash
# MotionCurve test
pio test -f algorithm_tests/test_motion_curve

# IdleMotion test
pio test -f algorithm_tests/test_idle_motion
```

### Hardware Control Layer Tests (Requires hardware)
```bash
# RoboticArmController test (requires 2 servos)
pio test -f hardware_control_tests/test_robotic_arm
```

### Hardware Functional Tests (Requires complete hardware)
```bash
# OLED display test
# 1. Modify platformio.ini, set src_filter to hardware_function_tests/test_oled_display.cpp
# 2. Upload to device
pio run -t upload

# 3. Open serial monitor
pio device monitor

# 4. Send test commands (1-0, a)
```

### Run tests on target device
```bash
pio test -e esp32-s3-devkitc-1
```

---

## Test Tags

Property tests must include tag comments in the format:
```cpp
/**
 * Feature: moss-education-module, Property 1: Real-time motion curve calculation correctness
 * Validates: Requirements 12.1, 12.2, 12.4
 */
```

---

## Test Progress

### Completed Tests
- ✅ Task 2.2, 2.3: MotionCurve test (8/8 passed)
- ✅ Task 2.5: IdleMotion test (9/9 passed)
- ✅ Task 3: Checkpoint - Verify algorithm library
- ✅ Task 5.2: RoboticArmController test (6/6 passed)
- ✅ OLED display test (11 items all passed)
- ✅ Basic LED test (passed)
- ✅ Integrated linkage test (passed)

### Pending Tests
- ⏸️ Task 2.7: SoundLocalization test (waiting for I2S microphone arrival)
- ⏸️ Task 5.4: LEDController property test
- ⏸️ Task 5.6: Controller parallel execution test
- ⏸️ Task 5.7: Non-blocking update test

---

## Test Statistics

| Test Type | Test Files | Test Cases | Pass Rate |
|-----------|-----------|-----------|----------|
| Algorithm Layer Tests | 2 | 17 | 100% |
| Hardware Control Layer Tests | 1 | 6 | 100% |
| Hardware Functional Tests | 3 | 28+ | 100% |
| **Total** | **6** | **51+** | **100%** |

---

## File Naming Convention

- `.cpp` - Currently active test file
- `.cpp.bak` - Completed test file (backup)
- `README_<TestName>_Test.md` - Test documentation (Chinese)
- `README_<TestName>_Test_en.md` - Test documentation (English)

---


