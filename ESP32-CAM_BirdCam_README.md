
# 🐦 ESP32-CAM Bird Cam Project

A low-power, motion-activated camera system to monitor and identify birds using an ESP32-CAM, PIR sensor, and environmental sensing. Images are captured on motion and uploaded to a local AI server for species recognition.

---

## 📦 Hardware Overview

| Component                      | Purpose                                 |
|-------------------------------|-----------------------------------------|
| ESP32-CAM                     | Main controller, camera + Wi-Fi         |
| OV2640 Camera Module          | Captures bird images                    |
| MH-SR602 PIR Sensor           | Detects motion (wakes ESP32 from sleep) |
| DHT22 (AM2302) Sensor         | Temperature and humidity monitoring     |
| LiPo Battery (3.7V, 1800mAh)  | Power source                            |
| TP4056 Module                 | LiPo charging circuit                   |
| LM2596 Buck Converter         | Regulates 3.7–4.2V → 5V for ESP32-CAM   |
| Resistor Divider              | Measures battery voltage via ADC        |

---

## 🔌 GPIO Assignments

| Peripheral     | GPIO     | Notes                              |
|----------------|----------|------------------------------------|
| PIR Sensor     | GPIO 12  | External wake-up from deep sleep   |
| DHT22 Sensor   | GPIO 13  | 1-wire communication               |
| Battery ADC    | GPIO 14  | ADC1_CH6; use voltage divider      |
| Flash LED      | GPIO 4   | Built-in flash light (optional)    |
| Camera         | Fixed    | Uses GPIO 0, 2, 4, 5, 18, 19, etc. |
| UART TX/RX     | GPIO 1/3 | Used for programming               |

---

## 🔧 ASCII Schematic

```
                         +-----------------------------+
                         |       ESP32-CAM             |
                         |                             |
    PIR OUT ─────────────┤ GPIO12 (Wakeup Interrupt)   |
                         │                             |
    DHT22 DATA ──────────┤ GPIO13                      |
                         │                             |
    BAT DIV OUT ─────────┤ GPIO14 (ADC1_CH6)           |
                         │                             |
                         │                             |
      USB-TX <───────────┤ GPIO3 (RX)                  |
      USB-RX ───────────>┤ GPIO1 (TX)                  |
                         │                             |
           5V IN ───────>│ 5V (from LM2596)            |
                         │                             |
                         +-----------------------------+

      LiPo Battery (3.7V)
         │
         ├─> TP4056 (charging)
         │
         └─> LM2596 (regulated to 5V) ────> ESP32-CAM VCC

```

**Voltage Divider for ADC (Battery Sensing):**
```
     LiPo (3.7–4.2V)
          │
         [R1] 100kΩ
          │────> GPIO14 (ADC)
         [R2] 100kΩ
          │
         GND

  Vout = Vin * R2 / (R1 + R2)
       = Vin * 0.5  →  max ~2.1V
```

---

## 🧠 Design Summary

- ESP32-CAM has **just enough GPIOs** for:
  - PIR interrupt
  - DHT22 sensor
  - Battery ADC input
- **Camera pins are fixed and not available for repurposing**
- Battery level is measured via **voltage divider to ADC1**
- System wakes from **deep sleep via PIR**
- Data (image + sensor values) is sent to **home server over Wi-Fi**
- No SD card used — **images sent directly**

---

## 🔋 Power Considerations

- Use **deep sleep** to reduce idle current to µA range
- Wake-up via **GPIO12 (PIR rising/falling edge)**
- Power everything from **single LiPo + buck regulator**
- Disable Wi-Fi and camera during sleep for efficiency

---

## ✅ Next Steps

- Implement deep sleep + PIR wake-up
- Capture and buffer photo from OV2640
- Read DHT22 + battery voltage
- Send data to server (via HTTP POST or MQTT)
- Optional: mount in waterproof box and test outside

---

## 🧪 Testing & Debug Tips

- Use external USB-TTL adapter on **GPIO1/3** for programming
- Boot fails if GPIO0 is pulled low unintentionally
- Use logic level converters or avoid overvolting I/O
- Print debug info over serial during wake sequence

---
