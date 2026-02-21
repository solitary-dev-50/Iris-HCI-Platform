# Iris-HCI-Platform: The Multimodal Interaction Brain

> **An integrated HCI platform based on ESP32-S3 and BiometricMotion algorithms.**

[中文版 README (Chinese Version)](./README_zh.md)

---

## 🚀 Product Positioning
**Iris-HCI** is a highly integrated smart terminal mainboard designed for "instant feedback" and "humanized interaction". It integrates visual recognition, audio perception, and biometric motion algorithms - a core brain that can give machinery a sense of "life".

---

## 🛠 Core Features

### 1. Vision Perception
- **OV5640-AF**: 5MP autofocus camera module.
- **Capabilities**: Distance detection, gesture tracking, detail diagnosis (e.g., plant disease spots or precision operations).

### 2. Audio Perception
- **IM69D130**: Professional-grade digital microphone.
- **Capabilities**: Real-time environmental sound analysis, sound source localization, rhythm-synchronized motion.

### 3. Biometric Motion
- **BiometricMotion Algorithm Library**: 
  - **S-Curve**: Eliminates mechanical jerky movements, simulating muscle-like smooth motion.
  - **Perlin Noise**: Adds "breathing" micro-movements during idle states, making devices appear "alive".
- **Testing Excellence**: Unlike traditional testing, we use Property-Based Testing (100+ iterations per property) to ensure the mathematical correctness of our bio-inspired curves.

### 4. Industrial-Grade Stability
- Built on the foundation of **"Cornerstone Smart Agriculture"** codebase, featuring 5-stage startup self-check, fail-safe protection, and power isolation safeguards.
- **Quality Assurance**: 50+ test cases verified. Check our [Test Directory](./tests/README.md) for detailed reports.

---

## ⚡ Quick Start (Verification)

If you have **PlatformIO** installed, you can verify our core BiometricMotion algorithm with one command:

```bash
# Run the S-Curve mathematical correctness test (100+ iterations)
pio test -f algorithm_tests/test_motion_curve
```

This property-based test validates the mathematical correctness of our bio-inspired motion curves across 100+ randomly generated scenarios.

---

## 📂 Use Cases
- **Smart Interactive Devices (HCI)**: Achieve rhythm-aware, feedback-rich advanced tactile experiences.
- **Biometric Robots**: Serve as the vision and motion control center for robots.
- **Smart Diagnostic Terminals**: Integrate with APIs for image recognition and remote analysis.
- **Protocol & Ecosystem**: Fully compatible with the T-Code v0.3 standard. It integrates seamlessly with major haptic control frameworks such as Buttplug.io (Intiface), enabling low-latency synchronization with interactive media and remote haptic applications.

---

## 🌍 Protocol & Ecosystem

> **Designed for next-gen interactive immersion.**  
> Our firmware is **fully compatible with the T-Code protocol** and offers **seamless integration with the Buttplug.io (Intiface) ecosystem**.

---

## 📐 Technical Architecture Details

This hardware design follows the principles of "high reliability, low noise, multimodal interaction". Core circuit specifications:

### 1. Advanced Power Path Management
- **Multi-source Auto-switching**: Dual AO3401A (PMOS) logic circuit for seamless USB and Li-ion battery power switching.
- **Triple Protection**: 
  - **SS34 Schottky diode** for reverse current isolation.
  - **SRV05-4HTG-D** ESD protection chip on USB input.
  - Onboard self-recovery fuse for extreme condition safety.
- **High-efficiency Conversion**: 
  - **MT3608 DCDC**: Provides stable 5V power for servos and LED arrays, designed for 2A load capacity.
  - **AMS1117-3.3**: Delivers clean voltage for the core SoC.

### 2. High-fidelity Digital Audio Chain
- **Premium Sensor**: **IM69D130** high-performance PDM digital microphone.
- **Signal Conversion**: **PCM1840IRTWR** (quad-channel ADC/PDM to I2S) chip converts digital signals directly to ESP32-S3, avoiding analog interference for high SNR sampling.
- **Audio Output**: Integrated **NS4168** mono audio amplifier for efficient audio driving.

### 3. Professional Vision System
- **High-speed Interface**: 24-Pin FPC DVP interface connected to **OV5640-AF** (5MP autofocus).
- **Dedicated Power**: Specialized LDO (LDO-2V8 & LDO-1V5) for camera module to minimize image noise.

### 4. Expandability & Interaction
- **High Integration**: Dual LED array interfaces (WS2812B-2020 & 4020), 0.96" OLED interface, dual servo interfaces.
- **Debug Convenience**: Onboard **CH340C** serial converter chip supporting automatic download and hardware flow control.
- **Battery Monitoring**: Precision 100K/100K resistor voltage divider circuit for real-time Li-ion battery voltage monitoring.

---

## 📖 Developer's Words
I'm a 50-year-old developer. This terminal mainboard embodies six months of my dedication - the stability forged from the harsh environments of agricultural control, now condensed into this circuit board less than 10cm in size.

**The code is AI-assisted, but the logic, soul, and relentless pursuit of stability come from my reverence for the land and life as an old farmer.**

---

## 📜 Commercial Use

**Commercial Use**: This project is licensed under MIT for individual/educational use only. Any commercial production, distribution, or resale of the hardware/firmware requires a written license from the author.

**For detailed terms and conditions, please read our [End User License Agreement (EULA)](./EULA.md).**

---

## 🤝 Contact & Collaboration
- **GitHub Star**: If you appreciate this level of hardware integration, please star this project.
- **Development Collaboration**: Geeks and developers are welcome to discuss mainboard solution licensing and firmware customization.
- **Sponsorship**: [Support my development on Afdian](https://afdian.com/a/solitary-dev-50)