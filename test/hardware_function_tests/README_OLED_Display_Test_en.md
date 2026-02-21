# OLED Display Functional Test Documentation

## Test Objective

Verify the complete functionality of the SSD1306 OLED display, including Chinese character display, graphics rendering, animation effects, etc.

## Test File

- `test/test_oled_display.cpp` - OLED display comprehensive test code

## Hardware Requirements

### Required Hardware:
1. ESP32-S3 development board
2. SSD1306 OLED display (128x64 pixels)
3. I2C connection wires

### Pin Connections:
| OLED Pin | ESP32-S3 Pin | Description |
|----------|--------------|-------------|
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SDA | GPIO 2 | I2C Data |
| SCL | GPIO 1 | I2C Clock |

### I2C Configuration:
- **I2C Address**: 0x3C
- **Communication Speed**: 400kHz (Fast mode)
- **Font Library**: u8g2_font_wqy14_t_gb2312 (WenQuanYi GB2312, includes 6763 common Chinese characters)

## Test Content

This test includes 10 independent test items, switched via serial commands:

### 1. Basic Chinese Display (Command: 1)
**Function**: Test basic Chinese character display capability

**Display Content**:
```
Hello World (你好世界)
MOSS Terminal
Chinese Test
Display Normal
```

**Verification Points**:
- Chinese characters display clearly
- No garbled characters
- Appropriate font size

---

### 2. System Status Interface (Command: 2)
**Function**: Simulate system status monitoring interface

**Display Content**:
```
========System Status========
Running:00:05:23
Memory:45%
Temperature:42°C
```

**Verification Points**:
- Title centered
- Separator line drawn correctly
- Status information aligned
- Mixed Chinese-English display

---

### 3. Menu Interface (Command: 3)
**Function**: Test menu selection interface

**Display Content**:
```
========Main Menu========
>1.Start Conversation
 2.System Settings
 3.About MOSS
```

**Verification Points**:
- Menu items clear
- Selection marker (>) displayed
- Clear hierarchical structure

---

### 4. Mixed Chinese-English (Command: 4)
**Function**: Test mixed Chinese-English display

**Display Content**:
```
Status:Idle
Volume:75%
Temperature:28°C
WiFi:Connected
```

**Verification Points**:
- Correct Chinese-English mixing
- Special symbols (%,°C) display normally
- Colon alignment

---

### 5. Progress Bar Animation (Command: 5)
**Function**: Test dynamic progress bar effect

**Display Content**:
```
========Loading========
[████████████████    ] 80%
```

**Verification Points**:
- Progress bar grows smoothly (0% → 100%)
- Percentage updates in real-time
- Animation smooth without flickering
- Refresh rate ~20fps

---

### 6. Volume Bar Animation (Command: 6)
**Function**: Test volume adjustment interface

**Display Content**:
```
========Volume Adjustment========
Volume: 75%
[███████████████     ]
```

**Verification Points**:
- Volume bar changes dynamically (0% → 100% → 0%)
- Loop playback
- Smooth visual effect

---

### 7. Emoji Animation (Command: 7)
**Function**: Test emoji animation

**Display Content**:
```
========Emoji Animation========
😊 → 😄 → 😁 → 😆
(Loop playback)
```

**Verification Points**:
- Emojis display correctly
- Smooth transitions
- Normal animation loop

---

### 8. Scrolling Text (Command: 8)
**Function**: Test text scrolling effect

**Display Content**:
```
MOSS Smart Education Terminal - Making Learning More Fun!
(Text scrolls from right to left)
```

**Verification Points**:
- Text scrolls smoothly
- Loop playback
- No frame skipping

---

### 9. Clock Display (Command: 9)
**Function**: Simulate clock interface

**Display Content**:
```
========Clock========
    12:34:56
  2025-02-19
```

**Verification Points**:
- Correct time format
- Clear digits
- Center alignment

---

### 10. Dashboard (Command: 0)
**Function**: Test dashboard graphics rendering

**Display Content**:
```
Arc dashboard
Pointer rotates dynamically
Scale marks
```

**Verification Points**:
- Arc drawn correctly
- Pointer rotates smoothly
- Clear scales

---

### 11. Auto Demo (Command: a)
**Function**: Automatically loop through all tests

**Process**:
1. Execute tests 1-10 in sequence
2. Each test displays for 3 seconds
3. Loop playback

**Verification Points**:
- Normal automatic switching
- No stuttering
- Stable loop

---

## Running Tests

### Preparation:
1. Connect OLED display to ESP32-S3
2. Confirm correct I2C connection (SDA→GPIO2, SCL→GPIO1)
3. Confirm display power is normal

### Run Command:
```cmd
# Rename other test files (avoid conflicts)
ren test\test_robotic_arm.cpp test_robotic_arm.cpp.bak

# Run test
pio test -f test_oled_display
```

### Or run as main program:
```cmd
# Upload to device
pio run -t upload

# Open serial monitor
pio device monitor
```

### Serial Commands:
After test starts, send the following commands via serial:
- `1` - Basic Chinese display
- `2` - System status interface
- `3` - Menu interface
- `4` - Mixed Chinese-English
- `5` - Progress bar animation
- `6` - Volume bar animation
- `7` - Emoji animation
- `8` - Scrolling text
- `9` - Clock display
- `0` - Dashboard
- `a` - Auto demo all

### Expected Output (Serial):
```
========================================
   OLED Complete Functional Test
========================================

Test Commands:
  1 - Basic Chinese display
  2 - System status interface
  3 - Menu interface
  4 - Mixed Chinese-English
  5 - Progress bar animation
  6 - Volume bar animation
  7 - Emoji animation
  8 - Scrolling text
  9 - Clock display
  0 - Dashboard
  a - Auto demo all

[Test 1] Basic Chinese display
```

---

## Technical Details

### U8g2 Library Configuration
```cpp
// Hardware I2C mode, no reset pin
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(
    U8G2_R0,           // Rotation: 0 degrees
    U8X8_PIN_NONE,     // No reset pin
    I2C_SCL,           // GPIO1
    I2C_SDA            // GPIO2
);
```

### Font Selection
```cpp
// WenQuanYi GB2312 font
display.setFont(u8g2_font_wqy14_t_gb2312);
```

**Font Features**:
- Includes 6763 common Chinese characters
- Supports GB2312 encoding
- Font size: 14 pixels
- Suitable for 128x64 display

### Chinese Display Method
```cpp
// Enable UTF-8 support
display.enableUTF8Print();

// Draw UTF-8 string
display.drawUTF8(x, y, "Chinese text");
```

### Animation Implementation Principle
```cpp
// Double buffering mechanism
display.clearBuffer();      // Clear buffer
display.drawXXX(...);        // Draw content to buffer
display.sendBuffer();        // Send to display at once
```

---

## Test Results

### Test Status: ✅ Passed

According to test report (Phase 3 testing):

| Test Item | Status | Description |
|-----------|--------|-------------|
| I2C Communication | ✅ Passed | Device address 0x3C normal |
| Chinese Display | ✅ Passed | Using wqy14 font, complete display |
| Graphics Rendering | ✅ Passed | Rectangles, circles, lines normal |
| Animation Effects | ✅ Passed | Smooth without flickering |
| Refresh Rate | ✅ Passed | ~20fps |

### Performance Metrics
- **Refresh Rate**: ~20fps (meets ≥10fps requirement)
- **Response Delay**: <50ms
- **Memory Usage**: ~8KB (display buffer)

---

## Troubleshooting

### Issue 1: Display shows nothing
**Possible Causes**:
- Incorrect I2C connection
- Power not connected
- I2C address mismatch

**Solutions**:
1. Check SDA/SCL connections
2. Confirm 3.3V power
3. Run I2C scan program to confirm address

### Issue 2: Chinese characters garbled
**Possible Causes**:
- Wrong font selection
- UTF-8 encoding issue

**Solutions**:
1. Confirm using `u8g2_font_wqy14_t_gb2312` font
2. Confirm `display.enableUTF8Print()` is called
3. Confirm source file encoding is UTF-8

### Issue 3: Display flickering
**Possible Causes**:
- Not using double buffering
- Refresh frequency too high

**Solutions**:
1. Use `clearBuffer()` and `sendBuffer()`
2. Add appropriate `delay()`

### Issue 4: Some Chinese characters display as boxes
**Possible Causes**:
- Font does not include that character

**Solutions**:
1. Use wqy14 font (includes 6763 common Chinese characters)
2. Avoid using rare characters

---

## Extended Features

### Features that can be added:
1. **Touch button support** - Switch interfaces via GPIO buttons
2. **Real-time data display** - Display sensor data
3. **QR code display** - Display WiFi configuration QR code
4. **Image display** - Display bitmap images
5. **Custom fonts** - Add more font choices

---

## Related Documentation

- **Test Report**: `测试报告.md` - Phase 3 testing
- **OLED Test Documentation**: `OLED中文显示测试说明.md` - Detailed test records
- **Pin Configuration**: `include/pin_config.h` - Complete pin definitions

---

## Dependencies

```ini
[env:esp32-s3-devkitc-1]
lib_deps = 
    olikraus/U8g2@^2.35.9
```

**U8g2 Library Features**:
- Supports multiple displays
- Rich font library
- Efficient graphics rendering
- Double buffering mechanism

---

## Notes

1. **Large font file**: wqy14 font is ~200KB, occupies significant Flash space
2. **Refresh rate limitation**: I2C communication speed limits refresh rate, complex animations may not be smooth enough
3. **Memory usage**: Full-screen buffer requires 1KB memory (128x64/8)
4. **Test completed**: This test has been verified on hardware and can serve as OLED functionality reference

---

**End of Document**
