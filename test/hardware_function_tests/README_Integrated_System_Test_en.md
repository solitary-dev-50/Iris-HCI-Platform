# MOSS Integrated System Test Documentation

## Test Objective

Verify the **multi-hardware coordination capability** of the MOSS Smart Education Module. This is a complete system integration test, including:
- Sound detection and state switching
- LED lighting effects linkage
- Servo steering control
- OLED status display
- Multi-module parallel operation

**Test Level**: System Integration Test (Phase 6)

## Test File

- `test/test_integrated_system.cpp.bak` - Integrated linkage test program (I2S digital microphone version)

## Hardware Requirements

### Required Hardware List:

| Module | Model/Spec | Pin Configuration | Quantity |
|--------|-----------|-------------------|----------|
| Main Controller | ESP32-S3 | - | 1 |
| OLED Display | SSD1306 (128x64) | GPIO1/2 (I2C) | 1 |
| LED Ring (Camera) | WS2812_4020 | GPIO48 (Serial) | 2 |
| LED Ring (Body) | WS2812B-2020 | GPIO48 (Serial) | 3 |
| Horizontal Servo | Standard Servo | GPIO4 | 1 |
| Vertical Servo | Standard Servo | GPIO5 | 1 |
| Microphone | SPH0645 (I2S) | GPIO7/12/13 | 2 |
| Speaker Module | MAX98357A (I2S) | GPIO9/10/11 | 1 |

### Complete Pin Connection Table:

#### OLED Display (I2C)
| OLED Pin | ESP32-S3 Pin | Description |
|----------|--------------|-------------|
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SDA | GPIO 2 | I2C Data |
| SCL | GPIO 1 | I2C Clock |

#### LED Ring (WS2812B Serial)
| LED | ESP32-S3 Pin | Description |
|-----|--------------|-------------|
| All LEDs (5) | GPIO 48 | Data line (Serial) |
| VCC | 5V | Power |
| GND | GND | Ground |

**LED Index Assignment**:
- Index 0-1: Camera LED (Pupil)
- Index 2-4: Body LED (Status ring)

#### Servos
| Servo | ESP32-S3 Pin | Description |
|-------|--------------|-------------|
| Horizontal Servo | GPIO 4 | PWM Control |
| Vertical Servo | GPIO 5 | PWM Control |
| VCC | 5V | Power (Independent supply) |
| GND | GND | Ground |

#### I2S Microphone (SPH0645)
| Microphone Pin | ESP32-S3 Pin | Description |
|----------------|--------------|-------------|
| BCLK | GPIO 13 | Serial Clock (Shared) |
| LRCL (WS) | GPIO 7 | Word Select (Shared) |
| DOUT | GPIO 12 | Data Line (Shared) |
| Left Mic SEL | GND | Left Channel |
| Right Mic SEL | 3.3V | Right Channel |
| 3V | 3.3V | Power |
| GND | GND | Ground |

#### Speaker Module (MAX98357A)
| Speaker Pin | ESP32-S3 Pin | Description |
|-------------|--------------|-------------|
| LRC | GPIO 9 | Left-Right Clock |
| BCLK | GPIO 10 | Bit Clock |
| DIN | GPIO 11 | Data Input |
| VIN | 5V | Power |
| GND | GND | Ground |

---

## System Architecture

### State Machine Design

```
┌─────────────┐
│ STATE_IDLE  │ Idle State
│ (Blue Breathing) │
└──────┬──────┘
       │ Sound Detected
       ↓
┌─────────────┐
│STATE_LISTEN │ Listening State
│ (Green Solid) │
└──────┬──────┘
       │ Volume > Threshold
       ↓
┌─────────────┐
│STATE_ACTIVE │ Active State
│ (Orange Bright) │ ← Servo turns to sound source
└──────┬──────┘
       │ 3 seconds no sound
       ↓
┌─────────────┐
│STATE_SPEAK  │ Speaking State
│ (Purple Flashing) │
└──────┬──────┘
       │ Complete
       ↓
    Return to Idle
```

### Module Linkage Logic

```
Sound Detection → LED Color Change → Servo Steering → OLED Update
  (10ms)            (20ms)             (500ms)          (50ms)
```

---

## Test Content

### 1. Idle Mode (STATE_IDLE)

**Trigger Condition**: System startup or long time no sound

**Hardware Behavior**:
- **Body LED**: Blue breathing light effect (Index 2-4)
- **Pupil LED**: Dark red (Index 0-1)
- **Servo**: Micro-movement (Simulating breathing)
- **OLED**: Display "IDLE" status
- **Microphone**: Continuous monitoring

**Verification Points**:
- ✅ LED breathing effect smooth
- ✅ Servo micro-movement natural
- ✅ OLED display correct
- ✅ Microphone real-time detection

---

### 2. Listening Mode (STATE_LISTENING)

**Trigger Condition**: Sound detected (Volume > 50)

**Hardware Behavior**:
- **Body LED**: Green solid
- **Pupil LED**: Dark red
- **Servo**: Stop micro-movement
- **OLED**: Display "LISTENING" and volume bar
- **Microphone**: Real-time volume display

**Verification Points**:
- ✅ LED color switch timely
- ✅ Volume bar updates in real-time
- ✅ Servo stops micro-movement
- ✅ State switch smooth

---

### 3. Active Mode (STATE_ACTIVE)

**Trigger Condition**: Volume peak > 100 (Fixed threshold)

**Hardware Behavior**:
- **Body LED**: Orange bright
- **Pupil LED**: Bright red (Focus)
- **Servo**: Turn to sound source direction
- **OLED**: Display "ACTIVE!" and volume peak
- **Sound Localization**: Calculate left-right channel time difference

**Verification Points**:
- ✅ LED immediately turns orange
- ✅ Servo steering accurate (Based on TDOA)
- ✅ OLED displays volume peak
- ✅ Pupil LED turns bright red

**Sound Localization Algorithm**:
```cpp
// Based on TDOA (Time Difference) to calculate azimuth
Time Difference = (Left Channel Peak Time - Right Channel Peak Time)
Distance Difference = Time Difference × Sound Speed
Angle = arcsin(Distance Difference / Microphone Spacing)
```

---

### 4. Speaking Mode (STATE_SPEAKING)

**Trigger Condition**: Manual trigger or API response

**Hardware Behavior**:
- **Body LED**: Purple flashing
- **Pupil LED**: Bright red
- **Servo**: Maintain current position
- **OLED**: Display "SPEAKING"
- **Speaker**: Play TTS audio

**Verification Points**:
- ✅ LED purple flashing
- ✅ Audio playback normal
- ✅ Servo remains stable

---

## Serial Commands

Test program supports the following serial commands:

| Command | Function | Description |
|---------|----------|-------------|
| `a` | Simulate sound trigger | Switch to active state |
| `l` | Simulate left sound source | Servo turns to 60 degrees |
| `r` | Simulate right sound source | Servo turns to 120 degrees |
| `i` | Return to idle | Switch to idle state |
| `s` | Speaking mode | Switch to speaking state |

---

## Running Tests

### Preparation:

1. **Hardware Connection**:
   - Connect all hardware according to pin connection table
   - Confirm servo independent power supply (5V, at least 1A)
   - Confirm LED power sufficient

2. **Software Preparation**:
   - Confirm all dependency libraries installed
   - Backup current main.cpp

### Run Steps:

```cmd
# 1. Rename file (remove .bak suffix)
ren test\test_integrated_system.cpp.bak test_integrated_system.cpp

# 2. Temporarily replace main.cpp
ren src\main.cpp main.cpp.bak
copy test\test_integrated_system.cpp src\main.cpp

# 3. Compile and upload
pio run -t upload

# 4. Open serial monitor
pio device monitor

# 5. Restore after testing
del src\main.cpp
ren src\main.cpp.bak main.cpp
```

### Expected Output (Serial):

```
=================================
   MOSS Integrated Linkage Test
=================================

Initializing...
[✓] OLED display initialized successfully
[✓] LED ring initialized successfully
[✓] Servo initialized successfully
[✓] I2S microphone initialized successfully
[✓] Speaker initialized successfully

System Ready!

Status: IDLE
Volume: 0
Peak: 0

--- Sound Detected ---
Status: LISTENING
Volume: 65

--- Volume Peak Triggered ---
Status: ACTIVE
Peak: 125
Steering: 75 degrees

--- 3 seconds no sound ---
Status: IDLE
```

---

## Performance Metrics

According to test report (Phase 6 testing):

| Metric | Target Value | Measured Value | Status |
|--------|--------------|----------------|--------|
| Main Loop Frequency | ≥100Hz | ~100Hz | ✅ |
| Sound Detection Delay | <100ms | ~50ms | ✅ |
| Servo Response Delay | <100ms | ~80ms | ✅ |
| LED Refresh Rate | ≥30fps | ~50fps | ✅ |
| Display Refresh | ≥10fps | ~20fps | ✅ |
| State Switch Delay | <200ms | ~150ms | ✅ |

---

## Test Scenarios

### Scenario 1: Sound Trigger Test

**Steps**:
1. System starts, enters idle state (Blue breathing)
2. Clap or speak
3. Observe LED turns green (Listening)
4. Clap loudly
5. Observe LED turns orange, servo turns

**Expected Results**:
- ✅ State switch smooth
- ✅ LED color correct
- ✅ Servo steering timely

### Scenario 2: Sound Localization Test

**Steps**:
1. Clap on left side
2. Observe if servo turns left (<90 degrees)
3. Clap on right side
4. Observe if servo turns right (>90 degrees)

**Expected Results**:
- ✅ Servo steering consistent with sound source position
- ✅ Steering angle reasonable (30-150 degrees)

### Scenario 3: Serial Command Test

**Steps**:
1. Send command `a` - Simulate sound trigger
2. Send command `l` - Simulate left sound source
3. Send command `r` - Simulate right sound source
4. Send command `i` - Return to idle

**Expected Results**:
- ✅ All commands respond normally
- ✅ State switch correct
- ✅ Servo steering accurate

### Scenario 4: Long-term Stability Test

**Steps**:
1. Let system run continuously for 2 hours
2. Periodically trigger sound
3. Observe for abnormalities

**Expected Results**:
- ✅ No memory leak
- ✅ No state deadlock
- ✅ Response always timely

---

## Technical Details

### I2S Microphone Configuration (SPH0645)

```cpp
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_MSB,  // MSB left-aligned
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};
```

**Key Configuration**:
- **Format**: MSB left-aligned (SPH0645 specific)
- **Sample Rate**: 16kHz
- **Bit Depth**: 32-bit (18-bit effective)
- **Channel**: Stereo (Left-right microphones)

### Sound Localization Algorithm

```cpp
// 1. Find peak positions in left-right channels
int leftPeakIndex = findPeakIndex(leftChannel);
int rightPeakIndex = findPeakIndex(rightChannel);

// 2. Calculate time difference (sample count)
int timeDiff = leftPeakIndex - rightPeakIndex;

// 3. Convert to distance difference
float distanceDiff = (timeDiff / SAMPLE_RATE) * SOUND_SPEED;

// 4. Calculate angle
float angle = asin(distanceDiff / MIC_DISTANCE) * 180 / PI;

// 5. Convert to servo angle (90 degrees as center)
int servoAngle = 90 + angle;
```

### LED Color Definitions

```cpp
// Body LED (Index 2-4)
COLOR_BOOT      = RGB(128, 0, 128)   // Purple (Boot)
COLOR_IDLE      = RGB(0, 50, 100)    // Blue (Idle)
COLOR_LISTENING = RGB(0, 255, 0)     // Green (Listening)
COLOR_ACTIVE    = RGB(255, 100, 0)   // Orange (Active)
COLOR_SPEAKING  = RGB(255, 0, 255)   // Purple (Speaking)

// Pupil LED (Index 0-1)
COLOR_EYE_OFF   = RGB(0, 0, 0)       // Off
COLOR_EYE_DIM   = RGB(50, 0, 0)      // Dark red (Idle)
COLOR_EYE_FOCUS = RGB(255, 0, 0)     // Bright red (Focus)
```

---

## Troubleshooting

### Issue 1: Microphone no sound
**Possible Causes**:
- Incorrect I2S configuration
- Microphone SEL pin not configured
- Wiring error

**Solutions**:
1. Confirm using MSB left-aligned format
2. Confirm left microphone SEL to GND, right microphone SEL to 3.3V
3. Check GPIO7/12/13 connections

### Issue 2: Servo not steering
**Possible Causes**:
- Insufficient servo power
- Abnormal PWM signal
- Angle calculation error

**Solutions**:
1. Use independent 5V power supply (at least 1A)
2. Check GPIO4/5 connections
3. Use serial commands `l`/`r` to test

### Issue 3: LED not lit or wrong color
**Possible Causes**:
- GPIO48 connection error
- LED serial order error
- Insufficient power

**Solutions**:
1. Confirm GPIO48 connected to first LED's DIN
2. Confirm 5 LEDs connected in series order
3. Use 5V power supply (at least 500mA)

### Issue 4: Abnormal state switching
**Possible Causes**:
- Improper volume threshold setting
- Microphone sensitivity issue
- State machine logic error

**Solutions**:
1. Adjust TRIGGER_THRESHOLD value
2. Use serial commands for manual testing
3. View volume values in serial output

---

## Test Results

### Test Status: ✅ Passed

According to test report (Phase 6 testing):

| Test Scenario | Status | Description |
|---------------|--------|-------------|
| Idle Mode | ✅ Passed | LED breathing, servo micro-movement normal |
| Listening Mode | ✅ Passed | Sound detection, LED color change normal |
| Active Mode | ✅ Passed | Sound localization, servo steering normal |
| Auto Demo | ✅ Passed | State loop, switching smooth |
| Stability Test | ✅ Passed | 2 hours no failure |

### Linkage Timing Verification:
```
Sound Detection → LED Color Change → Servo Steering → Display Update
   (10ms)            (20ms)             (500ms)          (50ms)
```
✅ All module response delays within acceptable range

---

## Related Documentation

- **Test Report**: `测试报告.md` - Phase 6 integrated linkage test
- **Pin Configuration**: `include/pin_config.h` - Complete pin definitions
- **Design Document**: `.kiro/specs/moss-education-module/design.md` - System architecture design

---

## Dependencies

```ini
[env:esp32-s3-devkitc-1]
lib_deps = 
    olikraus/U8g2@^2.35.9           # OLED display
    adafruit/Adafruit NeoPixel@^1.12.0  # LED control
    madhephaestus/ESP32Servo@^3.0.5     # Servo control
```

---

## Notes

1. **Microphone Version**: This program is adapted for SPH0645 I2S microphone (MSB left-aligned format)
2. **Test Completed**: Verified on actual hardware
3. **Performance Optimization**: Main loop frequency ~100Hz, meets real-time requirements
4. **Extensibility**: Can serve as foundation for complete system, add cloud API and state machine

---

**End of Document**
