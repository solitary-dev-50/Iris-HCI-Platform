# ESP32-S3 Basic Blink Test Documentation

## Test Objective

Verify the basic functionality of the ESP32-S3 development board, including:
- Onboard LED control
- Serial communication
- Arduino framework operation
- Development environment configuration

This is a **Hello World level** test program for quickly verifying that hardware and development environment are working properly.

## Test File

- `test/test_basic_blink.cpp.bak` - Basic LED blink test code (backup)

## Hardware Requirements

### Required Hardware:
1. ESP32-S3 development board
2. USB data cable (for power and serial communication)

### No External Hardware Needed:
- Uses onboard LED (GPIO2)
- No external sensors or modules required

## Test Content

### Function 1: LED Blinking
- **GPIO**: GPIO2 (onboard LED)
- **Mode**: Output mode
- **Blink Frequency**: 1Hz (on 1 second, off 1 second)

### Function 2: Serial Output
- **Baud Rate**: 115200
- **Output Content**:
  - Startup message: "ESP32-S3 Started!"
  - Test title: "LED Blinking Test"
  - Status messages: "LED ON" / "LED OFF"

## Code Description

### Complete Code:
```cpp
#include <Arduino.h>

void setup() {
    // Simple blink test
    pinMode(2, OUTPUT);  // Onboard LED
    
    // Initialize serial
    Serial.begin(115200);
    delay(2000);
    
    // Send test information
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

### Code Logic:

**setup() Function**:
1. Set GPIO2 as output mode (control LED)
2. Initialize serial (115200 baud rate)
3. Wait 2 seconds (ensure serial stability)
4. Print startup information

**loop() Function**:
1. LED on → Print "LED ON" → Delay 1 second
2. LED off → Print "LED OFF" → Delay 1 second
3. Loop execution

## Running Tests

### Method 1: Run as Main Program

1. **Rename file** (remove .bak suffix):
```cmd
ren test\test_basic_blink.cpp.bak test_basic_blink.cpp
```

2. **Temporarily replace main.cpp**:
```cmd
# Backup current main.cpp
ren src\main.cpp main.cpp.bak

# Copy test file to src
copy test\test_basic_blink.cpp src\main.cpp
```

3. **Compile and upload**:
```cmd
pio run -t upload
```

4. **Open serial monitor**:
```cmd
pio device monitor
```

5. **Restore main.cpp**:
```cmd
del src\main.cpp
ren src\main.cpp.bak main.cpp
```

### Method 2: Using PlatformIO Test Framework

Since this is a simple functional test (not a unit test), Method 1 is recommended.

## Expected Results

### Hardware Behavior:
- **Onboard LED** (usually on ESP32-S3 development board) should blink at 1Hz frequency
- On 1 second → Off 1 second → Loop

### Serial Output:
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

## Verification Criteria

### ✅ Test Pass Conditions:
1. LED can blink normally (1Hz frequency)
2. Serial can output information normally
3. Program runs stably without reset or crash
4. Serial output synchronized with LED status

### ❌ Test Failure Situations:
1. LED does not blink or blinks irregularly
2. No serial output or garbled output
3. Program resets frequently
4. LED status inconsistent with serial output

## Troubleshooting

### Issue 1: LED not blinking
**Possible Causes**:
- GPIO2 is not the onboard LED pin
- LED hardware failure

**Solutions**:
1. Check development board schematic to confirm onboard LED pin
2. Try external LED on GPIO2 for testing
3. Use multimeter to measure GPIO2 voltage changes

### Issue 2: No serial output
**Possible Causes**:
- USB driver not installed
- Serial baud rate mismatch
- USB cable can only charge, not transfer data

**Solutions**:
1. Install CP2102 or CH340 driver
2. Confirm serial monitor baud rate is 115200
3. Replace USB data cable

### Issue 3: Serial output garbled
**Possible Causes**:
- Incorrect baud rate setting
- USB CDC mode issue

**Solutions**:
1. Confirm serial monitor baud rate is 115200
2. Check monitor_speed configuration in platformio.ini
3. Try pressing reset button to restart

### Issue 4: Cannot upload program
**Possible Causes**:
- Serial port occupied
- Development board not in download mode

**Solutions**:
1. Close all serial monitor programs
2. Hold BOOT button, click RESET button to enter download mode
3. Check USB connection

## Test Value

### This simple test can verify:

1. **Hardware Normal**:
   - ESP32-S3 chip working normally
   - GPIO output function normal
   - USB serial communication normal

2. **Development Environment Correct**:
   - PlatformIO configuration correct
   - Arduino framework installed correctly
   - Compilation toolchain normal
   - Upload tool normal

3. **Basic Functions**:
   - digitalWrite() function normal
   - Serial.println() function normal
   - delay() function normal
   - setup() and loop() execution normal

### Applicable Scenarios:

- ✅ **New Board Testing** - First test after getting new development board
- ✅ **Environment Verification** - Verify development environment configuration is correct
- ✅ **Troubleshooting** - Use simplest program to troubleshoot hardware when complex programs have issues
- ✅ **Teaching Demo** - First example for ESP32-S3 introductory tutorial

## Extension Suggestions

### Can add on this basis:

1. **Multiple LED Control**:
```cpp
pinMode(2, OUTPUT);
pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
```

2. **Different Blink Patterns**:
```cpp
// Fast blink
delay(100);

// SOS signal
// ... --- ...
```

3. **Button Control**:
```cpp
pinMode(0, INPUT_PULLUP);  // BOOT button
if (digitalRead(0) == LOW) {
    // Button pressed
}
```

4. **PWM Breathing Light**:
```cpp
ledcSetup(0, 5000, 8);
ledcAttachPin(2, 0);
ledcWrite(0, brightness);
```

## Related Documentation

- **Test Report**: `测试报告.md` - Complete hardware test records
- **Pin Configuration**: `include/pin_config.h` - Complete pin definitions
- **Project File List**: `项目文件列表.md` - Project structure description

## Test History

### Test Status: ✅ Passed

This is the earliest test program in the project, used to verify:
- ESP32-S3 development board works normally
- Development environment configured correctly
- Basic GPIO and serial functions normal

**Test Time**: Early project stage  
**Test Result**: Passed  
**Current Status**: Archived as reference code

## Notes

1. **File Suffix**: `.bak` indicates this is a backup file, will not be compiled
2. **Historical Significance**: This is the first test program of the project, has commemorative value
3. **Retention Value**: As the simplest test reference, used for quick hardware verification
4. **Not Recommended to Modify**: Keep its simplicity as a baseline test

---

**End of Document**
